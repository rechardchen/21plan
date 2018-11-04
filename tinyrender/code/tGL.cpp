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

		return ret;
	}


	TR::Mat4 Frustum(float l, float r, float t, float b, float n, float f)
	{
		Mat4 mat = Mat4::identity();

		mat[0][0] = 2 * n / (r - l);
		mat[0][2] = (r + l) / (r - l);
		mat[1][2] = 2 * n / (t - b);
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
		up = cross(backward, right);

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
	}

	void Triangle(const Mat<4, 3, float>& clipc, IShader& shader, class Image& colorBuffer, float* zbuffer, const Mat4& viewport)
	{
		auto pts = (viewport * clipc).transpose();

		Mat<3, 2, float> pts2;
		pts2[0] = proj<2>(pts[0]) / pts[0][3];
		pts2[1] = proj<2>(pts[1]) / pts[1][3];
		pts2[2] = proj<2>(pts[2]) / pts[2][3];

		Vec2i bbMax(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
		Vec2i bbMin(std::numeric_limits<int>::min(), std::numeric_limits<int>::min());
		for (int i = 0; i < 3; ++i)
		{
			bbMin.x = std::max<int>(0, std::min<int>(bbMin.x, pts2[i].x));
			bbMin.y = std::max<int>(0, std::min<int>(bbMin.y, pts2[i].y));
			bbMax.x = std::min<int>(colorBuffer.GetWidth() - 1, std::max<int>(bbMax.x, pts2[i].x));
			bbMax.y = std::min<int>(colorBuffer.GetHeight() - 1, std::max<int>(bbMax.y, pts2[i].y));
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