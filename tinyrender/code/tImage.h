#pragma once

#include <cstring>

namespace TR
{
	struct Color
	{
		unsigned char rgba[4];
		unsigned char bytespp;	//bytes per pixel

		Color() : bytespp(1)
		{
			memset(&rgba, 0, sizeof(rgba));
		}

		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) :bytespp(4)
		{
			rgba[0] = r;
			rgba[1] = g;
			rgba[2] = b;
			rgba[3] = a;
		}

		Color(unsigned char v) :bytespp(1) 
		{
			memset(&rgba, 0, sizeof(rgba));
			rgba[0] = v;
		}

		Color(unsigned char* p, unsigned char bpp) : bytespp(bpp)
		{
			memcpy(&rgba[0], p, bpp > 4 ? 4:bpp);
			if (bpp < 4) memset(&rgba[bpp], 0, 4 - bpp);
		}

		unsigned char operator [] (int i)
		{
			return rgba[i];
		}

		Color operator*(float intensity)
		{
			Color ret = *this;
			intensity = intensity > 1.f ? 1.f : intensity < 0.f ? 0.f : intensity;
			for (int i = 0; i < 4; ++i)
			{
				ret.rgba[i] = (unsigned char)(intensity * rgba[i]);
			}
			return ret;
		}
	};

	class Image
	{
	public:
		enum Format { GRAYSCALE = 1, RGB = 3, RGBA = 4 };

		Image();
		Image(int w, int h, int bpp);
		~Image();

		void Reset();
		bool ReadFile(const char* filename);
		bool WriteTGAFile(const char* filename, bool rle = true);

		//image operations
		bool FlipHorizontally();
		bool FlipVertically();
		bool Scale(int w, int h);
		
		Color Get(int x, int y) const
		{
			return Color(mData + (y * mWidth+x)*mBytespp, mBytespp);
		}

		void Set(int x, int y, const Color& c)
		{
			memcpy(mData + (y * mWidth+x)*mBytespp, c.rgba, mBytespp);
		}

		int GetWidth() const { return mWidth; }
		int GetHeight() const { return mHeight; }
		int GetBpp() const { return mBytespp; }
		unsigned char* GetBuffer() const { return mData; }

	private:
		int mWidth;
		int mHeight;
		int mBytespp;
		unsigned char* mData;
	};
}
