#include "tImage.h"
using namespace TR;

int main(int argc, char* argv[])
{
	Image img;
	if (img.ReadFile(argv[1]))
	{
		img.FlipVertically();
		img.WriteTGAFile("output.tga");
	}

}