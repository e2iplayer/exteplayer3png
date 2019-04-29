/*
 * Subtitle output to one registered client.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <stdlib.h>
#include <png.h>

extern int SaveRGBAImage(const char *filename, unsigned char *data, int width, int height)
{
    if (width <= 0 || height <= 0 || data == NULL || filename == NULL) return -1;

    int ret = -1;
    int x = 0;
    int y = 0;

    png_bytep *row_pointers = NULL;

    FILE *fp = fopen(filename, "wb");
    if(!fp) goto faile0;

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) goto faile1;

    png_infop info = png_create_info_struct(png);
    if (!info) goto faile2;
    if (setjmp(png_jmpbuf(png))) goto faile2;

    png_init_io(png, fp);

    // Output is 8bit depth, RGBA format., PNG_COLOR_TYPE_RGBA, PNG_COLOR_TYPE_PALETTE
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);

    for (y = 0; y < height; y++) row_pointers[y] = &(data[y * width * 4]);

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    free(row_pointers);

    ret = 0;

faile2:
    png_destroy_write_struct(&png,  &info);
 
faile1:
    fclose(fp);
    if (ret) remove(filename);

faile0:
    return ret;
}