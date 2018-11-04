#include "tImage.h"
#include "tGL.h"
#include "tCommon.h"
#include "tModel.h"
#include "limits"
using namespace TR;

Model* model = nullptr;
Mat4 projMatrix, viewMatrix;

struct SimpleShader : public IShader
{
	virtual Vec4f Vert(int nface, int nthvert) override;
	virtual bool Fragment(Vec3f bar, struct Color& color) override;

	Mat<2, 3, float> varying_uv;
	Mat<4, 3, float> varying_tri;
};

TR::Vec4f SimpleShader::Vert(int nface, int nthvert)
{
	Vec4f gl_Vertex = projMatrix * viewMatrix * embed<4>(model->Vert(nface, nthvert), 1.f);
	varying_uv.setCol(nthvert, model->uv(nface, nthvert));
	varying_tri.setCol(nthvert, gl_Vertex);
	return gl_Vertex;
}

bool SimpleShader::Fragment(Vec3f bar, struct Color& color)
{
	Vec2f uv = varying_uv * bar;
	color = model->diffuse(uv);
	return true;
}

int main(int argc, char* argv[])
{
	const int WIDTH = 960;
	const int HEIGHT = 640;

	Image img(WIDTH, HEIGHT, Image::RGB);
	float *zbuffer = (float*)TMalloc(sizeof(float)*WIDTH*HEIGHT);
	for (int i = 0; i < WIDTH*HEIGHT; ++i)
	{
		zbuffer[i] = std::numeric_limits<float>::max();
	}

	model = new Model(argv[1]);
	viewMatrix = LookAt(Vec3f(1, 1, 3), Vec3f(0, 0, 0), Vec3f(0, 1, 0));
	projMatrix = Frustum(-1, 1, 1, -1, 1, 100);

	Mat4 viewPort = Viewport(0, 0, WIDTH, HEIGHT);
	SimpleShader s;
	for (int f = 0; f < model->nfaces(); ++f)
	{
		s.Vert(f, 0);
		s.Vert(f, 1);
		s.Vert(f, 2);

		Triangle(s.varying_tri, s, img, zbuffer, viewPort);
	}

	img.FlipVertically();
	img.WriteTGAFile("output.tga");
}