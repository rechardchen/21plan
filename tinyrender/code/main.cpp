#include "tImage.h"
#include "tGL.h"
#include "tCommon.h"
#include "tModel.h"
#include <limits>
#include <algorithm>
using namespace TR;

Vec3f light_dir(1, 1, 1);
Model* model = nullptr;
Mat4 projMatrix, viewMatrix;
Mat4 MVPMatrix;
Mat4 modelView;
Mat4 modelViewInvT;

struct GouraudShader : public IShader
{
	Vec3f varying_intensity;

	virtual Vec4f Vert(int nface, int nthvert) override
	{
		float intensity = std::max(0.f, light_dir*model->normal(nface, nthvert));
		varying_intensity[nthvert] = intensity;
		Vec4f gl_Vertex = projMatrix * viewMatrix*embed<4>(model->Vert(nface, nthvert), 1.f);
		varying_tri.setCol(nthvert, gl_Vertex);
		return gl_Vertex;
	}

	virtual bool Fragment(Vec3f bar, Color& color) override
	{
		float intensity = varying_intensity * bar;
		color = Color(255, 255, 255)*intensity;
		return true;
	}
};

struct SimpleShader : public IShader
{
	Mat<2, 3, float> varying_uv;
	Mat<3, 3, float> varying_nrm;
	Mat<3, 3, float> local_tri;

	virtual Vec4f Vert(int nface, int nthvert) override;
	virtual bool Fragment(Vec3f bar, struct Color& color) override;
};

TR::Vec4f SimpleShader::Vert(int nface, int nthvert)
{
	Vec4f gl_Vertex = MVPMatrix * embed<4>(model->Vert(nface, nthvert), 1.f);
	varying_uv.setCol(nthvert, model->uv(nface, nthvert));
	varying_tri.setCol(nthvert, gl_Vertex);
	varying_nrm.setCol(nthvert, model->normal(nface, nthvert));

	local_tri[nthvert] = model->Vert(nface, nthvert); //row as vertex local position

	return gl_Vertex;
}

bool SimpleShader::Fragment(Vec3f bar, struct Color& color)
{
	Vec2f uv = varying_uv * bar;
	Vec3f bn = normalize(varying_nrm * bar);

	Mat3 A;
	A[2] = bn;
	A[0] = local_tri[1] - local_tri[0];
	A[1] = local_tri[2] - local_tri[0];

	Mat3 AI = A.inv();
	Vec3f i = normalize(AI * Vec3f(varying_uv[0][1] - varying_uv[0][0], varying_uv[0][2] - varying_uv[0][0], 0.f));
	Vec3f j = normalize(AI * Vec3f(varying_uv[1][1] - varying_uv[1][0], varying_uv[1][2] - varying_uv[1][0], 0.f));

	Mat3 B;
	B.setCol(0, i);
	B.setCol(1, j);
	B.setCol(2, bn);
	
	Vec3f n = B * model->normal(uv);
	n = proj<3>(modelViewInvT*embed<4>(n, 0.f));
	n = normalize(n);
	float diffuse = std::max(0.f, n*light_dir);
	Vec3f r = 2.f*(n*light_dir)*n - light_dir;
	float specular = powf(std::max(0.f, r.z), model->specular(uv));

	Color c = model->diffuse(uv);

	for (int i = 0; i < 3; ++i)
	{
		color[i] = std::min<int>(5 + c[i] * (diffuse + .6f*specular), 255);
	}

	return true;
}

int main(int argc, char* argv[])
{
	const int WIDTH = 1600;
	const int HEIGHT = 1600;

	Image img(WIDTH, HEIGHT, Image::RGB);
	float *zbuffer = (float*)TMalloc(sizeof(float)*WIDTH*HEIGHT);
	for (int i = 0; i < WIDTH*HEIGHT; ++i)
	{
		zbuffer[i] = std::numeric_limits<float>::max();
	}

	light_dir = normalize(light_dir);
	viewMatrix = LookAt(Vec3f(1, 1, 3), Vec3f(0, 0, 0), Vec3f(0, 1, 0));
	projMatrix = Frustum(-1, 1, 1, -1, 3, 10);
	MVPMatrix = projMatrix * viewMatrix;
	modelView = viewMatrix;
	modelViewInvT = viewMatrix.inv_transpose();
	Mat4 viewPort = Viewport(0, 0, WIDTH, HEIGHT);

	
	SimpleShader s;
	for (int i = 1; i < argc; ++i)
	{
		model = new Model(argv[i]);

		for (int f = 0; f < model->nfaces(); ++f)
		{
			s.Vert(f, 0);
			s.Vert(f, 1);
			s.Vert(f, 2);

			Triangle(s.varying_tri, s, img, zbuffer, viewPort);
		}

		delete model;
	}

	img.FlipVertically();
	img.WriteTGAFile("output.tga");
}