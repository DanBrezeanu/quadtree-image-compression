// Copyright 2017 Brezeanu Dan-Eugen
#ifndef PMAPH
#define PMAPH
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define packed __attribute__((packed))

typedef struct RGB{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
}RGB;

typedef struct QuadtreeNode{
    unsigned char blue, green, red;
    uint32_t area;
    int32_t top_left, top_right;
    int32_t bottom_left, bottom_right;
}packed QuadtreeNode;

int            parseInt(char *str); // stringToInt
int            isLeaf(QuadtreeNode *v,int index);

unsigned char* read(int *Height, int *Width, char* fileName);


RGB**          buildPixelMap(unsigned char *input, const int WIDTH, const int HEIGHT);
QuadtreeNode*  buildTree(RGB **pixelMap, uint32_t *nLeaves, uint32_t *nBlocks,
                         int area, int threshold); //quadTree cu threshold-ul dat

void           decompressImage(char *fileNameIN, char *fileNameOUT);
void           decompressMirroredImage(QuadtreeNode *v, uint32_t nLeaves,
                                       uint32_t nBlocks, char *fileNameOUT);
RGB**          decompressImageForMerge(QuadtreeNode *v, int length);

void           parseTree(RGB ***pixelMap, QuadtreeNode *v, int index, int x, int y, int area);
long long      getSubMatrixScore(RGB **pixelMap, QuadtreeNode **v, int currentNode,
                                 int startX, int startY, int area);

QuadtreeNode*  mirrorImageVertically(RGB **pixelMap, uint32_t *nBlocks,
                                     uint32_t *nLeaves,  int area, int threshold);
QuadtreeNode*  mirrorImageHorizontally(RGB **pixelMap, uint32_t *nBlocks,
                                       uint32_t *nLeaves,  int area, int threshold);

RGB**          mergeImages(QuadtreeNode *v1, QuadtreeNode *v2, int length);


#endif
