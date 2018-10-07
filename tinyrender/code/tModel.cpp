#include "tModel.h"
#include <fstream>
#include <string>
#include <sstream>

namespace TR
{
	Model::Model(const char* filename)
	{
		std::ifstream fin(filename, std::ios::in);
		if (fin.fail())
		{
			return;
		}
		std::string line, trashStr;
		while (!fin.eof())
		{
			std::getline(fin, line);
			std::istringstream iss(line.c_str());

			char trash;
			
			if (line.compare(0, 2, "v ") == 0)
			{
				iss >> trashStr;
				Vec3f v;
				iss >> v[0] >> v[1] >> v[2];
				mVertices.push_back(v);
			}
			else if (line.compare(0, 3, "vn ") == 0)
			{
				iss >> trashStr;
				Vec3f n;
				iss >> n[0] >> n[1] >> n[2];
				mNorms.push_back(n);
			}
			else if (line.compare(0, 3, "vt ") == 0)
			{
				iss >> trashStr;
				Vec2f uv;
				iss >> uv[0] >> uv[1];
				mUvs.push_back(uv);
			}
			else if (line.compare(0, 2, "f ") == 0)
			{
				iss >> trash;
				std::vector<Vec3i> f;
				Vec3i tmp;
				while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2])
				{
					//wavefront obj indices start at 1
					tmp[0] -= 1;
					tmp[1] -= 1;
					tmp[2] -= 1;
					f.push_back(tmp);
				}

				mFaces.push_back(f);
			}
		}
		loadTexture(filename, "_diffuse.tga", mDiffuseMap);
		loadTexture(filename, "_nm_tangent.tga", mNormalMap);
		loadTexture(filename, "_spec.tga", mSpecularMap);
	}

	Vec3f Model::normal(size_t iface, size_t nthvert) const
	{
		return mNorms[mFaces[iface][nthvert][2]];
	}

	Vec3f Model::normal(Vec2f uvf) const
	{
		if (mNormalMap.GetWidth() && mNormalMap.GetHeight())
		{
			//TODO: define sampler
			Vec2i uv(uvf[0]*mNormalMap.GetWidth(), uvf[1]*mNormalMap.GetHeight());
			uv.x = clamp<int>(uv.x, 0, mNormalMap.GetWidth() - 1);
			uv.y = clamp<int>(uv.y, 0, mNormalMap.GetHeight() - 1);
			Color c = mNormalMap.Get(uv[0], uv[1]);
			Vec3f n(c[0], c[1], c[2]);
			n = n / 255.f*2.f - 1.f;
			return n;
		}
		return Vec3f(0.f, 0.f, 1.f);
	}

	Vec3f Model::Vert(size_t iface, size_t nthvert) const
	{
		return mVertices[mFaces[iface][nthvert][0]];
	}

	Vec2f Model::uv(size_t iface, size_t nthvert) const
	{
		return mUvs[mFaces[iface][nthvert][1]];
	}

	Color Model::diffuse(Vec2f uvf) const
	{
		if (mDiffuseMap.GetWidth() && mDiffuseMap.GetHeight())
		{
			Vec2i uv(uvf[0] * mDiffuseMap.GetWidth(), uvf[1] * mDiffuseMap.GetHeight());
			uv.x = clamp(uv.x, 0, mDiffuseMap.GetWidth() - 1);
			uv.y = clamp(uv.y, 0, mDiffuseMap.GetHeight() - 1);
			return mDiffuseMap.Get(uv.x, uv.y);
		}
		return Color(255, 255, 255, 255);
	}

	float Model::specular(Vec2f uvf) const
	{
		if (mSpecularMap.GetWidth() && mSpecularMap.GetHeight())
		{
			Vec2i uv(uvf[0] * mSpecularMap.GetWidth(), uvf[1] * mSpecularMap.GetHeight());
			uv.x = clamp(uv.x, 0, mSpecularMap.GetWidth() - 1);
			uv.y = clamp(uv.y, 0, mSpecularMap.GetHeight() - 1);
			return mSpecularMap.Get(uv.x, uv.y)[0] / 1.f;
		}
		return 2.f;
	}

	std::vector<int> Model::face(int idx) const
	{
		std::vector<int> ret;
		for (auto it = mFaces[idx].begin(); it != mFaces[idx].end(); ++it)
		{
			ret.push_back((*it)[0]);
		}
		return ret;
	}

	void Model::loadTexture(const char* filename, const char* suffix, Image& img)
	{
		//1. get prefix
		std::string prefix = filename;
		auto dot = prefix.rfind('.');
		if (dot != std::string::npos)
		{
			prefix = prefix.substr(0, dot);
		}
		std::string imagePath = prefix + suffix;
		if (img.ReadFile(imagePath.c_str()))
		{
			img.FlipVertically();
		}
	}
}