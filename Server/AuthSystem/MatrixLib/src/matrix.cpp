//====================================================
//
// matrix.c / Generate Matrix Card 
//
// Copyright by The9 Corp. 2007.2
//
//====================================================

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../include/SHA1.h"
#include "../include/random.h"
#include "../include/main.h"

//typedef unsigned char matrix_seed[SEED_SIZE];

static unsigned long _div(int size, unsigned long base, unsigned long *src, unsigned long *dst)
{
    unsigned long long div;
    unsigned long residue = 0;
    int i;

    for(i=0; i<size; ++i)
    {                               
        div = ((unsigned long long)residue << 32) + src[i];
        residue=div % base;        
        dst[i]=div / base;
    }

    return residue;
}

void matrix_gen(const matrix_seed seed, int matrix_size, int matrix[], const int matrix_base, const int matrix_data_size)
{
    int  i,j;
//    unsigned long data[matrix_data_size];
	unsigned long *data;
	data = (unsigned long *)(malloc(sizeof(unsigned long)*matrix_data_size));

    Random_Init(seed, SEED_SIZE);
    Random_Read(data, sizeof(unsigned long)*matrix_data_size);	//sizeof(data));

    //for(j = 0; j < matrix_data_size; j++)
    //{   
    //    printf("%lu\t", data[j]);
    //}
    //printf("\n");

    for (i = 0; i < matrix_size; ++i)
    {
        matrix[i] = _div(matrix_data_size, matrix_base, data, data);
    }

	free(data);
}

void PrintSeed(matrix_seed seed)
{
    int i;
    printf("Seed: ");
    for(i=0; i<16; i++)
    {
        printf("%d ", seed[i]);
    }
    printf("\n");
}

int genseed(char *mc_seed, matrix_seed seed)
{
    int i;
    char hex_num[5] = "0x00";
    for(i=0; i<16; i++)
    {
        hex_num[2] = mc_seed[i*2];
        hex_num[3] = mc_seed[i*2+1];
        seed[i] = strtol(hex_num, NULL, 16);
    }
    return 0;
}
