#include "tImage.h"
#include "tCommon.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

namespace TR
{

	Image::Image() : mWidth(0), mHeight(0), mBytespp(0), mData(nullptr)
	{

	}

	void Image::Reset()
	{
		mWidth = mHeight = mBytespp = 0;
		if (mData)
		{
			TFree(mData);
			mData = nullptr;
		}
	}

	Image::~Image()
	{
		Reset();
	}

	bool Image::ReadFile(const char* filename)
	{
		Reset();

		int width, height, bpp;
		if (unsigned char* data = stbi_load(filename, &width, &height, &bpp, 0))
		{
			mData = data;
			mWidth = width;
			mHeight = height;
			mBytespp = bpp;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Image::WriteTGAFile(const char* filename, bool rle /*= true*/)
	{
		if (!mData)
		{
			return false;
		}

		stbi_write_tga_with_rle = rle ? 1 : 0;
		return stbi_write_tga(filename, mWidth, mHeight, mBytespp, mData) != 0;
	}

	bool Image::FlipHorizontally()
	{
		if (!mData)
		{
			return false;
		}

		const int HALF = mWidth >> 1;
		for (int x = 0; x < HALF; ++x)
		{
			for (int y = 0; y < mHeight; ++y)
			{
				Color c1 = Get(x, y);
				Color c2 = Get(mWidth - x - 1, y);
				Set(x, y, c2);
				Set(mWidth - x - 1, y, c1);
			}
		}
		return true;
	}

	bool Image::FlipVertically()
	{
		if (!mData)
		{
			return false;
		}

		const int LineBytes = mBytespp * mWidth;
		const int HALF = mHeight >> 1;
		unsigned char* buffer = (unsigned char*)TMalloc(LineBytes);
		for (int y = 0; y < HALF; ++y)
		{
			unsigned char* line = mData + y * LineBytes;
			unsigned char* rline = mData + (mHeight - y - 1)*LineBytes;
			memcpy(buffer, line, LineBytes);
			memcpy(line, rline, LineBytes);
			memcpy(rline, buffer, LineBytes);
		}
		TFree(buffer);
		return true;
	}

	bool Image::Scale(int w, int h)
	{
		if (w <= 0 || h <= 0 || !mData)
		{
			return false;
		}

		unsigned char* newData = (unsigned char*)TMalloc(w*h*mBytespp);
		int result = stbir_resize_uint8(mData, mWidth, mHeight, 0, newData, w, h, 0, mBytespp);
		if (result != 0)
		{
			TFree(mData);
			mData = newData;
			mWidth = w;
			mHeight = h;
			return true;
		}
		else
		{
			TFree(newData);
			return false;
		}
	}

	Image::Image(int w, int h, int bpp) : mWidth(w), mHeight(h), mBytespp(bpp)
	{
		const int BYTES = mWidth * mHeight*mBytespp;
		mData = new unsigned char[BYTES];
		memset(mData, 0, BYTES);
	}

}