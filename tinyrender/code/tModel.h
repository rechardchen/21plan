#pragma once
#include "tMath.h"
#include "tImage.h"
#include <vector>

namespace TR
{
	class Model
	{
	public:
		Model(const char* filename);
		~Model();

		size_t nverts() const { return mVertices.size(); }
		size_t nfaces() const { return mFaces.size(); }
		Vec3f normal(size_t iface, size_t nthvert) const;
		Vec3f normal(Vec2f uv)const;
		Vec3f Vert(size_t i)const { return mVertices[i]; }
		Vec3f Vert(size_t iface, size_t nthvert)const;
		Vec2f uv(size_t iface, size_t nthvert)const;

		Color diffuse(Vec2f uv) const;
		float specular(Vec2f uv) const;
		std::vector<int> face(int idx) const;

	private:
		std::vector<Vec3f> mVertices;
		std::vector<Vec3f> mNorms;
		std::vector<std::vector<Vec3i>> mFaces;
		std::vector<Vec2f> mUvs;
		
		Image mDiffuseMap;
		Image mSpecularMap;
		Image mNormalMap;

	private:
		static void loadTexture(const char* filename, const char* suffix, Image& image);
	};
}
