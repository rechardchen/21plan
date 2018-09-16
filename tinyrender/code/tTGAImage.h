#pragma once
#include <cstring>

namespace TR
{
	struct TGAColor
	{
		unsigned char bgra[4];
		unsigned char bytespp;	//bytes per pixel

		TGAColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) :bytespp(4)
		{
			bgra[0] = b;
			bgra[1] = g;
			bgra[2] = r;
			bgra[3] = a;
		}

		TGAColor(unsigned char v) :bytespp(1) 
		{
			memset(&bgra, 0, sizeof(bgra));
			bgra[0] = v;
		}

		TGAColor(unsigned char* p, unsigned char bpp) : bytespp(bpp)
		{
			memcpy(&bgra[0], p, bpp);
			if (bpp < 4) memset(&bgra[bpp], 0, 4 - bpp);
		}

		unsigned char operator [] (int i)
		{
			return bgra[i];
		}

		TGAColor operator*(float intensity)
		{
			TGAColor ret = *this;
			intensity = intensity > 1.f ? 1.f : intensity < 0.f ? 0.f : intensity;
			for (int i = 0; i < 4; ++i)
			{
				ret.bgra[i] = intensity * bgra[i];
			}
			return ret;
		}
	};

	class TGAImage
	{
	public:
		enum Format { GRAYSCALE = 1, RGB = 3, RGBA = 4 };

		TGAImage();
		TGAImage(int w, int h, int bpp);
		~TGAImage();

		bool ReadTGAFile(const char* filename);
		bool WriteTGAFile(const char* filename, bool rle = true);

		//image operations
		bool FlipHorizontally();
		bool FlipVertically();
		bool Scale(int w, int h);
		
		TGAColor Get(int x, int y) const;
		void Set(int x, int y, const TGAColor& c);

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
