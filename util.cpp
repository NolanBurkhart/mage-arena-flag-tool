#include "color.h";
#include <png.h> 
#include <stdexcept>

png_bytep* load_image(const char* path, int& width, int& height) {
	FILE* image = fopen(path, "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) abort();
	png_infop info = png_create_info_struct(png);
	if (!info) abort();

	if (setjmp(png_jmpbuf(png))) abort();

	png_init_io(png, image);
	png_read_info(png, info);

	int width_t = png_get_image_width(png, info);
	if (width != 0 && width != width_t)
	throw std::runtime_error("Image not of expected width");
	else
	width = width_t;

	int height_t = png_get_image_height(png, info);
	if (height != 0 && height != height_t)
	throw std::runtime_error("Image not of expected height");
	else
	height = height_t;

	png_bytep* row_pointers = NULL;
	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for (int y = 0; y < height; y++) {
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
	}
	png_read_image(png, row_pointers);

	fclose(image);

	return row_pointers;
}