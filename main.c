// Copyright 2017 Brezeanu Dan-Eugen
#include "pixel_map.h"

int main(int argc, char **argv){
  unsigned char *input;
  int height, width, threshold;
  uint32_t nLeaves = 0, nBlocks = 0;
  RGB **pixelMap;
  QuadtreeNode *v;

  if(!strcmp(argv[1],"-c")){ //compresie
    input = read(&height, &width, argv[3]); //citirea pixelilor
    threshold = parseInt(argv[2]);

    pixelMap = buildPixelMap(input, width, height); //creeaza matricea
    free(input);

    FILE *f = fopen(argv[4],"wb");
    //creeaza arborele
    v = buildTree(pixelMap, &nLeaves, &nBlocks, height*width, threshold);

    //afisare
    fwrite(&nLeaves, sizeof(uint32_t), 1, f);
    fwrite(&nBlocks, sizeof(uint32_t), 1, f);
    for(int i = 0; i < nBlocks; ++i)
      fwrite(&v[i],sizeof(QuadtreeNode), 1, f);

    //eliberare de memorie
    for(int i = 0; i < height; ++i)
      free(pixelMap[i]);
    free(pixelMap);
    free(v);
    fclose(f);
  }

  if(!strcmp(argv[1], "-d")) //decompresie
    decompressImage(argv[2], argv[3]);

  if(!strcmp(argv[1], "-m")){ //oglindire
    input = read(&height, &width, argv[4]);
    threshold = parseInt(argv[3]);

    pixelMap = buildPixelMap(input, width, height);
    free(input);

    if(!strcmp(argv[2],"v")){ //oglindire verticala
     //construieste arborele
     v = mirrorImageVertically(pixelMap, &nBlocks, &nLeaves, width*height, threshold);
     //construieste matricea de pixeli
     decompressMirroredImage(v, nLeaves, nBlocks, argv[5]);

     //eliberare de memorie
     for(int i = 0; i <= height; ++i)
      free(pixelMap[i]);
     free(pixelMap);

     free(v);
    }

    if(!strcmp(argv[2],"h")){ //oglindire orizontala
     //construieste arborele
     v = mirrorImageHorizontally(pixelMap, &nBlocks, &nLeaves, width*height, threshold);
     //construieste matricea de pixeli
     decompressMirroredImage(v, nLeaves, nBlocks, argv[5]);

     //eliberare de memorie
     for(int i = 0; i <= height; ++i)
      free(pixelMap[i]);
     free(pixelMap);

     free(v);
   }
  }

  if(!strcmp(argv[1], "-o")){ //suprapunere
    RGB **pixelMap1, **pixelMap2;
    uint32_t nLeaves1, nLeaves2, nBlocks1, nBlocks2;
    QuadtreeNode *v1, *v2;

    threshold = parseInt(argv[2]);
    input = read(&height, &width, argv[3]); //citeste prima imagine
    //construieste matricea de pixeli pentru prima imagine
    pixelMap1 = buildPixelMap(input, width, height);
    //construieste arborele pentru prima imagine
    v1 = buildTree(pixelMap1, &nLeaves1, &nBlocks1, width*height, threshold);

    //eliberare de memorie
    for(int i = 0; i <= width; ++i)
      free(pixelMap1[i]);
    free(pixelMap1);

    free(input);
    unsigned char *input2;

    input2 = read(&height, &width, argv[4]); //citeste a doua imagine
    //construieste matricea de pixeli pentru a doua imagine
    pixelMap2 = buildPixelMap(input2, width, height);
      //construieste arborele pentru a doua imagine
    v2 = buildTree(pixelMap2, &nLeaves2, &nBlocks2, width*height, threshold);

    //eliberare de memorie
    for(int i = 0; i <= width; ++i)
      free(pixelMap2[i]);
    free(pixelMap2);

    free(input2);

    pixelMap = mergeImages(v1, v2, height); //realizeaza suprapunerea
    FILE *fo = fopen(argv[5], "w");

    //afisare
    fprintf(fo,"P6\n%d %d\n255\n",width, height);
    for(int i = 0; i < width; ++i)
      for(int j = 0; j < width; ++j)
        fprintf(fo,"%c%c%c", pixelMap[i][j].red, pixelMap[i][j].green, pixelMap[i][j].blue);

    //eliberare de memorie
    for(int i = 0; i < width; ++i)
      free(pixelMap[i]);
    free(pixelMap);
    free(v1); free(v2);
    fclose(fo);
  }

  return 0;
}
