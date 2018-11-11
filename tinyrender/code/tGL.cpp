#include "tGL.h"
#include "tImage.h"
#include <limits>
#include <algorithm>

namespace TR
{
	Vec3f Barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P)
	{
		Vec3f u(B[0] - A[0], C[0] - A[0], A[0] - P[0]);
		Vec3f v(B[1] - A[1], C[1] - A[1], A[1] - P[1]);
		Vec3f w = cross(u, v);

		if (abs(w[2]) > Epsilon)
		{
			float b = w[0] / w[2], c = w[1] / w[2];
			return Vec3f(1 - c - b, b, c);
		}
		return Vec3f(-1, 1, 1); //throw away by rasterizer
	}

	TR::Mat4 Viewport(int x, int y, int w, int h)
	{
		Mat4 ret = Mat4::identity();
		float halfW = w / 2.f;
		float halfH = h / 2.f;
		ret[0][0] = halfW;
		ret[0][3] = x + halfW;
		ret[1][1] = halfH;
		ret[1][3] = y + halfH;
		ret[2][2] = .5f;
		ret[2][3] = .5f;

		return ret;
	}


	TR::Mat4 Frustum(float l, float r, float t, float b, float n, float f)
	{
		Mat4 mat = Mat4::identity();

		mat[0][0] = 2 * n / (r - l);
		mat[0][2] = (r + l) / (r - l);
		mat[1][1] = 2 * n / (t - b);
		mat[1][2] = (t + b) / (t - b);
		mat[2][2] = -(f + n) / (f - n);
		mat[2][3] = -2 * f*n / (f - n);
		mat[3][2] = -1.f;
		mat[3][3] = 0.f;

		return mat;
	}

	TR::Mat4 LookAt(Vec3f eye, Vec3f center, Vec3f up)
	{
		auto backward = normalize(eye - center);
		auto right = normalize(cross(up, backward));
		up = normalize(cross(backward, right));

		Mat3 R;
		R[0] = right;
		R[1] = up;
		R[2] = backward;

		Vec3f _Rc = -(R * eye); //why -R*eye not work??

		Mat4 m = Mat4::identity();
		m[0] = Vec4f(R[0][0], R[0][1], R[0][2], _Rc[0]);
		m[1] = Vec4f(R[1][0], R[1][1], R[1][2], _Rc[1]);
		m[2] = Vec4f(R[2][0], R[2][1], R[2][2], _Rc[2]);
		return m;

		//Vec3f z = normalize((eye - center));
		//Vec3f x = normalize(cross(up, z));
		//Vec3f y = (cross(z, x));
		//Mat4 Minv = Mat4::identity();
		//Mat4 Tr = Mat4::identity();
		//for (int i = 0; i<3; i++) {
		//	Minv[0][i] = x[i];
		//	Minv[1][i] = y[i];
		//	Minv[2][i] = z[i];
		//	Tr[i][3] = -eye[i];
		//}
		//return Minv * Tr;

		//Vec3f f = normalize((center - eye));
		//Vec3f u = normalize(up);
		//Vec3f s = normalize(cross(f, u));
		//u = cross(s, f);

		//Mat4 ModelView;
		//ModelView[0][0] = s.x;
		//ModelView[0][1] = s.y;
		//ModelView[0][2] = s.z;

		//ModelView[1][0] = u.x;
		//ModelView[1][1] = u.y;
		//ModelView[1][2] = u.z;

		//ModelView[2][0] = -f.x;
		//ModelView[2][1] = -f.y;
		//ModelView[2][2] = -f.z;

		//ModelView[3][0] = 0.f;
		//ModelView[3][1] = 0.f;
		//ModelView[3][2] = 0.f;

		//ModelView[0][3] = -(s[0] * eye[0] + s[1] * eye[1] + s[2] * eye[2]);
		//ModelView[1][3] = -(u[0] * eye[0] + u[1] * eye[1] + u[2] * eye[2]);
		//ModelView[2][3] = f[0] * eye[0] + f[1] * eye[1] + f[2] * eye[2];
		//ModelView[3][3] = 1.f;
		//return ModelView;
	}

	void Triangle(const Mat<4, 3, float>& clipc, IShader& shader, class Image& colorBuffer, float* zbuffer, const Mat4& viewport)
	{
		auto pts = (viewport * clipc).transpose();

		Mat<3, 2, float> pts2;
		pts2[0] = proj<2>(pts[0]) / pts[0][3];
		pts2[1] = proj<2>(pts[1]) / pts[1][3];
		pts2[2] = proj<2>(pts[2]) / pts[2][3];

		Vec2i bbMax(std::numeric_limits<int>::min(), std::numeric_limits<int>::min());
		Vec2i bbMin(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
		for (int i = 0; i < 3; ++i)
		{
			bbMin.x = std::max<int>(0, std::min<int>(bbMin.x, (int)pts2[i].x));
			bbMin.y = std::max<int>(0, std::min<int>(bbMin.y, (int)pts2[i].y));
			bbMax.x = std::min<int>(colorBuffer.GetWidth() - 1, std::max<int>(bbMax.x, (int)pts2[i].x));
			bbMax.y = std::min<int>(colorBuffer.GetHeight() - 1, std::max<int>(bbMax.y, (int)pts2[i].y));
		}

		for (int i = bbMin.x; i <= bbMax.x; ++i)
		{
			for (int j = bbMin.y; j <= bbMax.y; ++j)
			{
				Vec2f p(i + 0.5f, j + 0.5f);
				Vec3f bc = Barycentric(pts2[0], pts2[1], pts2[2], p);
				if (bc.x < 0.f || bc.y < 0.f || bc.z < 0.f)
				{
					continue;
				}

				Vec3f bc_clip(bc[0] / clipc[3][0], bc[1] / clipc[3][1], bc[2] / clipc[3][2]);
				float z = 1.f / (bc_clip.x + bc_clip.y + bc_clip.z);
				bc_clip *= z;

				float depth = -z;
				float* depthPtr = &zbuffer[j*colorBuffer.GetWidth() + i];
				if (depth > *depthPtr)
				{
					continue;
				}

				Color color;
				if (shader.Fragment(bc_clip, color)) //check discard
				{
					*depthPtr = depth;
					colorBuffer.Set(i, j, color);
				}
			}
		}
	}
}