#include "tCommon.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#define STBI_MALLOC(size) (TR::TMalloc(size))
#define STBI_FREE(ptr) (TR::TFree(ptr))
#define STBI_REALLOC(ptr,sz) (TR::TRealloc(ptr,sz))
#define STBIR_MALLOC(size,context) ((void)(context),TR::TMalloc(size))
#define STBIR_FREE(ptr,context) ((void)(context),TR::TFree(ptr))


#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

