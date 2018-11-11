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
	Vec3f varying_intensity;

	virtual Vec4f Vert(int nface, int nthvert) override;
	virtual bool Fragment(Vec3f bar, struct Color& color) override;
};

TR::Vec4f SimpleShader::Vert(int nface, int nthvert)
{
	Vec4f gl_Vertex = projMatrix * viewMatrix * embed<4>(model->Vert(nface, nthvert), 1.f);
	varying_uv.setCol(nthvert, model->uv(nface, nthvert));
	varying_tri.setCol(nthvert, gl_Vertex);
	varying_intensity[nthvert] = std::max(0.f, light_dir*model->normal(nface, nthvert));
	return gl_Vertex;
}

bool SimpleShader::Fragment(Vec3f bar, struct Color& color)
{
	float intensity = varying_intensity * bar;
	Vec2f uv = varying_uv * bar;
	color = model->diffuse(uv) * intensity;
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
		zbuffer[i] = -std::numeric_limits<float>::max();
	}

	viewMatrix = LookAt(Vec3f(0, 0, 3), Vec3f(0, 0, 0), Vec3f(0, 1, 0));
	projMatrix = Frustum(-1, 1, 1, -1, 3, 10);
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