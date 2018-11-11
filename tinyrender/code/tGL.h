#pragma once
#include "tMath.h"

namespace TR
{
	Mat4 Viewport(int x, int y, int w, int h);
	Mat4 Frustum(float l, float r, float t, float b, float n, float f);
	Mat4 LookAt(Vec3f eye, Vec3f center, Vec3f up);
	
	//shader interface
	struct IShader
	{
		virtual Vec4f Vert(int nface, int nthvert) = 0;
		virtual bool Fragment(Vec3f bar, struct Color& color) = 0;
		Mat<4, 3, float> varying_tri;
	};

	//rasterizer
	void Triangle(const Mat<4, 3, float>& clipc, IShader& shader, class Image& colorBuffer, float* zbuffer, const Mat4& viewport);
}