/*
 ============================================================================
 Name        : general_lincoln.c
 Author      : LincolnHard
 Version     :
 Copyright   : free and open
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "utils.h"

void* pool_starting_address;
char* pool_current_address;
unsigned int pool_current_alloc_size;

void init_memory_pool_lincoln
	(
	void
	)
{
	posix_memalign(&pool_starting_address, 0x20, MEMORYPOOL_TOTAL_SIZE);
	if(pool_starting_address == NULL)
		{
		puts(" failed creating memory pool ");
		exit(1);
		}
	pool_current_address = (char*)pool_starting_address;
	pool_current_alloc_size = 0;
}

void* malloc_lincoln
	(
	size_t len
	)
{
	void* ptr = NULL;
	if(len <= 0)
		{
		len = 32;
		}
	pool_current_alloc_size += ((len + 0xF) & (~0xF));
	if(pool_current_alloc_size >= MEMORYPOOL_TOTAL_SIZE)
		{
		puts("failed allocating memory from pool anymore");
		free(pool_starting_address);
		exit(1);
		}
	ptr = pool_current_address;
	pool_current_address += ((len + 0xF) & (~0xF));
	memset(ptr, 0, len);
	return ptr;
}

void free_memory_pool_lincoln
	(
	void
	)
{
	free(pool_starting_address);
}

//the vector here stores the copy of object instead of address
//push operation here doesn't check whether size is touch boundary or not
//no vector free here, because vector are allocated by pool here. it should be freed with pool
void vector_init_lincoln
	(
	vector_lincoln* vec,
	size_t elem_size,
	size_t init_elem_num
	)
{
	vec->elem_size = elem_size;
	vec->num_alloc_elems = (init_elem_num != 0) ? init_elem_num : VECTOR_INIT_SIZE;
	vec->num_elems = 0;
	vec->beginning = malloc_lincoln(elem_size * vec->num_alloc_elems);
}

void vector_push_lincoln
	(
	vector_lincoln* vec,
	void* elem
	)
{
	//check space first
	if(vec->num_elems == vec->num_alloc_elems)
		{
		puts("failed push element into vector anymore");
		free(pool_starting_address);
		exit(1);
		}
	char* end_address = (char*)vec->beginning + vec->elem_size * vec->num_elems;
	memcpy(end_address, elem, vec->elem_size);
	vec->num_elems++;
}

void vector_pop_lincoln
	(
	vector_lincoln* vec,
	void* elem
	)
{
	char* latest_address = (char*)vec->beginning + vec->elem_size * (vec->num_elems - 1);
	memcpy(elem, latest_address, vec->elem_size);
	vec->num_elems--;
}

void vector_clear_lincoln
	(
	vector_lincoln* vec
	)
{
	vec->num_elems = 0;
	memset(vec->beginning, 0, vec->elem_size * vec->num_alloc_elems);
}

void vector_qsort_lincoln
	(
	vector_lincoln* vec,
	int (*compare_func)(const void*, const void*)
	)
{
	qsort(vec->beginning, vec->num_elems, vec->elem_size, compare_func);
}

void read_pgm
	(
	char* filename,
	unsigned char* src
	)
{
	const int IMG_TOTAL_PIXEL = IMG_WIDTH * IMG_HEIGHT;
	FILE* fp;
	int character;
	char linecontent[15];
	char* pch;
	fp = fopen(filename, "r");
	if(fp == NULL)
		{
		puts("no such image file");
		return;
		}
	character = fgetc(fp);
	if(character != 'P')
		{
		puts("not a PGM file");
		}
	character = fgetc(fp);
	if(character != '5')
		{
		puts("not in PGM raw format");
		}
	fseek(fp, 3, SEEK_SET);
	fgets(linecontent, 15, fp);
	pch = strtok(linecontent, " ");
	//the width check can be commented out if necessary
	if(atoi(pch) != IMG_WIDTH)
		{
		puts("width is invalid");
		}
	pch = strtok(NULL, " ");
	//the height check can be commented out if necessary
	if(atoi(pch) != IMG_HEIGHT)
		{
		puts("height is invalid");
		}
	//skip the line that record max gray value in PGM (no need)
	fgets(linecontent, 5, fp);
	//start to read data to buffer
	int i;
	for(i = 0; i < IMG_TOTAL_PIXEL; ++i)
		{
		character = fgetc(fp);
		src[i] = (unsigned char)character;
		}
	fclose(fp);
}

void write_pgm
	(
	char* filename,
	const unsigned char* src,
	imsize_u16_lincoln srcsize,
	rect_u16_lincoln* roi
	)
{
	char linecontent[15];
	FILE* fp;
	fp = fopen(filename, "w");
	fputs("P5", fp);
	fputc('\n', fp);
	if(roi == NULL)
		{
		//image width and height
		sprintf(linecontent, "%d %d", srcsize.width, srcsize.height);
		fputs(linecontent, fp);
		fputc('\n', fp);
		//max gray (note: hard code here)
		fputs("255", fp);
		fputc('\n', fp);
		int i;
		int totalpixel = srcsize.width * srcsize.height;
		for(i = 0; i < totalpixel; ++i)
			{
			fputc(src[i], fp);
			}
		}
	else
		{
		//image width and height
		sprintf(linecontent, "%d %d", roi->w, roi->h);
		fputs(linecontent, fp);
		fputc('\n', fp);
		//max gray (note: hard code here)
		fputs("255", fp);
		fputc('\n', fp);
		int i, j;
		for(j = 0; j < roi->h; ++j)
			{
			for(i = 0; i < roi->w; ++i)
				{
				fputc(src[roi->x + i + (roi->y + j) * srcsize.width], fp);
				}
			}
		}
	fclose(fp);
}

void draw_rectangle
	(
	unsigned char* src,
	rect_u16_lincoln* rectptr
	)
{
	memset(src + rectptr->x + rectptr->y * IMG_WIDTH, 255, rectptr->w);
	memset(src + rectptr->x + (rectptr->y + rectptr->h) * IMG_WIDTH, 255, rectptr->w);
}

unsigned int int_sqrt
	(
	unsigned int value
	)
{
	int i;
	unsigned int a = 0, b = 0, c = 0;
	unsigned int v = value;
	for(i = 0; i < 16; ++i)
		{
		c <<= 2;
		c += (v >> 30);
		v <<= 2;
		a <<= 1;
		b = (a << 1) | 1;
		if(c >= b)
			{
			c -= b;
			a++;
			}
		}
	return a;
}

unsigned short u16_round_lincoln
	(
	float fvalue
	)
{
	return (unsigned short)(fvalue + (fvalue > 0 ? 0.5 : -0.5));
}

//resizing source using "nearest neighbor" interpolation
void resize_interpolation
	(
	const unsigned char* src,
	const rect_u16_lincoln* srcroi,
	const imsize_u16_lincoln dstsize,
	unsigned char* dst
	)
{
	int dstW = (int)dstsize.width;
	int dstH = (int)dstsize.height;
	int srcW, srcH;
	const unsigned char* startaddress;
	if(srcroi == NULL)
		{
		srcW = IMG_WIDTH;
		srcH = IMG_HEIGHT;
		startaddress = src;
		}
	else
		{
		srcW = (int)srcroi->w;
		srcH = (int)srcroi->h;
		startaddress = src + srcroi->x + srcroi->y * IMG_WIDTH;
		}
	int i, j;
	int width_ratio = (int)((srcW << 16) / dstW) + 1;
	int height_ratio = (int)((srcH << 16) / dstH) + 1;
	unsigned char* dst_row_temp;
	const unsigned char* src_row_temp;
	for(j = 0; j < dstH; ++j)
		{
		dst_row_temp = dst + dstW * j;
		src_row_temp = startaddress + IMG_WIDTH * ((j * height_ratio) >> 16);
		for(i = 0; i < dstW; ++i)
			{
			int srcidx = (i * width_ratio) >> 16;
			dst_row_temp[i] = src_row_temp[srcidx];
			}
		}
}
