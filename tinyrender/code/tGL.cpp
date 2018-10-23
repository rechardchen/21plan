#include "tGL.h"

namespace TR
{

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

}