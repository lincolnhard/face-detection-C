/*
 ============================================================================
 Name        : general_lincoln.h
 Author      : LincolnHard
 Version     :
 Copyright   : free and open
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#define MEMORYPOOL_TOTAL_SIZE 0x900000 //9Mbytes
/* note the reserve size is for all the initialization(face detection is primer) */
/* the memory pool current alloc size should be reset to this number when infinite loop is applied */
#define MEMORYPOOL_RESERVE_SIZE 0x300000 //3Mbytes;
#define VECTOR_INIT_SIZE 512

typedef struct
	{
	float x;
	float y;
	}point_2f32_lincoln;

typedef struct
	{
	unsigned short width;
	unsigned short height;
	}imsize_u16_lincoln;

typedef struct
	{
	unsigned short x;
	unsigned short y;
	unsigned short w;
	unsigned short h;
	}rect_u16_lincoln;

typedef struct
	{
	void* beginning;
	size_t elem_size;
	size_t num_elems;
	size_t num_alloc_elems;
	}vector_lincoln;

void init_memory_pool_lincoln
	(
	void
	);

void* malloc_lincoln
	(
	size_t len
	);

void free_memory_pool_lincoln
	(
	void
	);

void vector_init_lincoln
	(
	vector_lincoln* vec,
	size_t elem_size,
	size_t init_elem_num
	);

void vector_push_lincoln
	(
	vector_lincoln* vec,
	void* elem
	);

void vector_pop_lincoln
	(
	vector_lincoln* vec,
	void* elem
	);

void vector_clear_lincoln
	(
	vector_lincoln* vec
	);

void vector_qsort_lincoln
	(
	vector_lincoln* vec,
	int (*compare_func)(const void*, const void*)
	);

void read_pgm
	(
	char* filename,
	unsigned char* src
	);

void write_pgm
	(
	char* filename,
	const unsigned char* src,
	imsize_u16_lincoln src_size,
	rect_u16_lincoln* roi
	);

void draw_rectangle
	(
	unsigned char* src,
	rect_u16_lincoln* rectptr
	);

unsigned int int_sqrt
	(
	unsigned int value
	);

unsigned short u16_round_lincoln
	(
	float fvalue
	);

void resize_interpolation
	(
	const unsigned char* src,
	const rect_u16_lincoln* srcroi,
	const imsize_u16_lincoln dstsize,
	unsigned char* dst
	);

#endif /* UTILS_H_ */
