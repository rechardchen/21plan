#pragma once
#include <stdlib.h>

namespace TR
{
	inline void TFree(void* ptr)
	{
		free(ptr);
	}

	inline void* TMalloc(size_t size)
	{
		return malloc(size);
	}

	inline void* TRealloc(void* p, size_t ns)
	{
		return realloc(p, ns);
	}
}