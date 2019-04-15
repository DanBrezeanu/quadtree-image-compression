// Copyright 2017 Brezeanu Dan-Eugen
#include "pixel_map.h"

int parseInt(char *str){
  int result = 0;

  for(int i = 0; i < strlen(str); ++i)
      result = result*10 + str[i] - '0';

  return result;
}

unsigned char* read(int *Height, int *Width, char* fileName){

  int height = 0, width = 0, colorRange = 0;
  int cheight = 0, cwidth = 0, crange = 0;
  int lSize, bytesToSkip = 0;
  char c;
  unsigned char *input;

  FILE *fs = fopen(fileName,"r");

  fscanf(fs,"%c %c", &c, &c);
  fscanf(fs,"%d %d %d", &height, &width, &colorRange);

  cheight = height; cwidth = width; crange = colorRange;

  while(cheight || cwidth || crange){ //calculez numarul de bytes din antet
      if(cheight){
          cheight/=10;
          bytesToSkip++;
          continue;
      }

      if(cwidth){
          cwidth/=10;
          bytesToSkip++;
          continue;
      }

      crange/=10;
      bytesToSkip++;
  }

  bytesToSkip += 6; // trei \n, un spatiu si P6
  fclose(fs);
  fs = fopen(fileName,"rb");
  fseek (fs , 0 , SEEK_END);
  lSize = ftell (fs); //dimensiunea fisierului
  rewind (fs);

  fseek(fs, bytesToSkip, SEEK_SET); //sare peste antet
  input =  malloc (sizeof(unsigned char)*(lSize));
  fread(input,1,lSize-bytesToSkip,fs); //citeste tot continutul binar

  fclose(fs);
  (*Height) = height; (*Width) = width;
  return input;

}

RGB **buildPixelMap(unsigned char *input, const int WIDTH, const int HEIGHT){
    RGB **pixelMap = calloc(HEIGHT + 1, sizeof(RGB*));
    int k = 0; //iterator pentru input

    for(int i = 0; i < HEIGHT; ++i)
      pixelMap[i] = calloc(WIDTH + 1, sizeof(RGB));

    for(int i = 0; i < HEIGHT; ++i)
      for(int j = 0; j < WIDTH; ++j){ //adauga cate 3 valori RGB in matrice
        pixelMap[i][j].red   = input[k++];
        pixelMap[i][j].green = input[k++];
        pixelMap[i][j].blue  = input[k++];
      }

      return pixelMap;
}

long long getSubMatrixScore(RGB **pixelMap, QuadtreeNode **v, int currentNode, int startX, int startY, int area){
  int height = sqrt(area);
  long long medR = 0, medG = 0, medB = 0;
  long long mean = 0, divisor;

  //valoare medie pe submatrice
  for(int i = startX; i < startX + height; ++i)
    for(int j = startY; j < startY + height; ++j){
        medR =(long long)medR + pixelMap[i][j].red;
        medG =(long long)medG + pixelMap[i][j].green;
        medB =(long long)medB + pixelMap[i][j].blue;
    }

  medR = (long long)medR / area;
  medG = (long long)medG / area;
  medB = (long long)medB / area;

  (*v)[currentNode].red   = medR;  // adauga valoarea medie in nodul curent
  (*v)[currentNode].green = medG;
  (*v)[currentNode].blue  = medB;

  // calculeaza deimpartitul formulei pentru scorul de similitudine
  for(int i = startX; i < startX + height; ++i)
    for(int j = startY; j < startY + height; ++j){
        mean = (long long)mean +
               (medR - pixelMap[i][j].red) * (medR - pixelMap[i][j].red) +
               (medG - pixelMap[i][j].green) * (medG - pixelMap[i][j].green) +
               (medB - pixelMap[i][j].blue) * (medB - pixelMap[i][j].blue);
    }

    divisor = (long long)3 * area;
    mean = (long long)mean / divisor;
    return mean; //scorul de similitudine
}

QuadtreeNode* buildTree(RGB **pixelMap, uint32_t *nLeaves, uint32_t *nBlocks, int area, int threshold){

    QuadtreeNode *v = calloc(area*2 , sizeof(QuadtreeNode));
    int *startX = calloc(2*area, sizeof(int)); //coordonatele X a subamtricelor
    int *startY = calloc(2*area, sizeof(int)); //coordonatele Y a subamtricelor
    int height, g;
    int currentNode, lastNodeIndexed = 0, lastNodeParsed = -1;
    v[0].area = area;

    //ultimul nod adaugat in arbore > ultimul nod procesat
    while(lastNodeIndexed > lastNodeParsed){
        currentNode = ++lastNodeParsed;
        height = sqrt(v[currentNode].area);
        g = getSubMatrixScore(pixelMap, &v, currentNode, startX[currentNode], startY[currentNode], v[currentNode].area);

        if(g > threshold){ //daca se imparte in 4
            v[currentNode].top_left = ++lastNodeIndexed;
            v[lastNodeIndexed].area = v[currentNode].area / 4;
            startX[lastNodeIndexed] = startX[currentNode];
            startY[lastNodeIndexed] = startY[currentNode];

            v[currentNode].top_right = ++lastNodeIndexed;
            v[lastNodeIndexed].area = v[currentNode].area / 4;
            startX[lastNodeIndexed] = startX[currentNode];
            startY[lastNodeIndexed] = startY[currentNode] + height/2;

            v[currentNode].bottom_right = ++lastNodeIndexed;
            v[lastNodeIndexed].area = v[currentNode].area / 4;
            startX[lastNodeIndexed] = startX[currentNode] + height/2;
            startY[lastNodeIndexed] = startY[currentNode] + height/2;

            v[currentNode].bottom_left = ++lastNodeIndexed;
            startX[lastNodeIndexed] = startX[currentNode] + height/2;
            startY[lastNodeIndexed] = startY[currentNode];
            v[lastNodeIndexed].area = v[currentNode].area / 4;
        }
        else{ //daca nu se imparte
            (*nLeaves)++;
            v[currentNode].top_left = -1;
            v[currentNode].top_right = -1;
            v[currentNode].bottom_right = -1;
            v[currentNode].bottom_left = -1;

        }

    }
    (*nBlocks) = lastNodeIndexed + 1;
    free(startX); free(startY);
    return v;
}

QuadtreeNode* mirrorImageVertically(RGB **pixelMap, uint32_t *nBlocks, uint32_t *nLeaves,  int area, int threshold){
  QuadtreeNode *v = calloc(area , sizeof(QuadtreeNode));
  int *startX = calloc(area, sizeof(int)); //coordonatele X ale submatricelor
  int *startY = calloc(area, sizeof(int)); //coordonatele Y ale subamtricelor
  int height, g;
  int currentNode, lastNodeIndexed = 0, lastNodeParsed = -1;
  v[0].area = area;

  //ultimul nod adaugat in arbore > ultimul nod procesat
  while(lastNodeIndexed > lastNodeParsed){
      currentNode = ++lastNodeParsed;
      height = sqrt(v[currentNode].area);
      g = getSubMatrixScore(pixelMap, &v, currentNode, startX[currentNode], startY[currentNode], v[currentNode].area);

        if(g > threshold){ //daca se imparte in 4
          v[currentNode].bottom_left = ++lastNodeIndexed;
          v[lastNodeIndexed].area = v[currentNode].area / 4;
          startX[lastNodeIndexed] = startX[currentNode];
          startY[lastNodeIndexed] = startY[currentNode];

          v[currentNode].bottom_right = ++lastNodeIndexed;
          v[lastNodeIndexed].area = v[currentNode].area / 4;
          startX[lastNodeIndexed] = startX[currentNode];
          startY[lastNodeIndexed] = startY[currentNode] + height/2;

          v[currentNode].top_right = ++lastNodeIndexed;
          v[lastNodeIndexed].area = v[currentNode].area / 4;
          startX[lastNodeIndexed] = startX[currentNode] + height/2;
          startY[lastNodeIndexed] = startY[currentNode] + height/2;

          v[currentNode].top_left = ++lastNodeIndexed;
          startX[lastNodeIndexed] = startX[currentNode] + height/2;
          startY[lastNodeIndexed] = startY[currentNode];
          v[lastNodeIndexed].area = v[currentNode].area / 4;
      }
      else{ //daca nu se imparte
          (*nLeaves)++;
          v[currentNode].top_left = -1;
          v[currentNode].top_right = -1;
          v[currentNode].bottom_right = -1;
          v[currentNode].bottom_left = -1;

      }

  }
  (*nBlocks) = lastNodeIndexed + 1;
  free(startX); free(startY);
  return v;
}

QuadtreeNode* mirrorImageHorizontally(RGB **pixelMap, uint32_t *nBlocks, uint32_t *nLeaves,  int area, int threshold){
  QuadtreeNode *v = calloc(area , sizeof(QuadtreeNode));
  int *startX = calloc(area, sizeof(int)); //coordontale X ale submatricelor
  int *startY = calloc(area, sizeof(int)); //coordontale Y ale subamtricelor
  int height, g;
  int currentNode, lastNodeIndexed = 0, lastNodeParsed = -1;
  v[0].area = area;

  //ultimul nod adaugat in arbore > ultimul nod procesat
  while(lastNodeIndexed > lastNodeParsed){
      currentNode = ++lastNodeParsed;
      height = sqrt(v[currentNode].area);
      g = getSubMatrixScore(pixelMap, &v, currentNode, startX[currentNode], startY[currentNode], v[currentNode].area);


      if(g > threshold){
          v[currentNode].top_right = ++lastNodeIndexed;
          v[lastNodeIndexed].area = v[currentNode].area / 4;
          startX[lastNodeIndexed] = startX[currentNode];
          startY[lastNodeIndexed] = startY[currentNode];

          v[currentNode].top_left = ++lastNodeIndexed;
          v[lastNodeIndexed].area = v[currentNode].area / 4;
          startX[lastNodeIndexed] = startX[currentNode];
          startY[lastNodeIndexed] = startY[currentNode] + height/2;

          v[currentNode].bottom_left = ++lastNodeIndexed;
          v[lastNodeIndexed].area = v[currentNode].area / 4;
          startX[lastNodeIndexed] = startX[currentNode] + height/2;
          startY[lastNodeIndexed] = startY[currentNode] + height/2;

          v[currentNode].bottom_right = ++lastNodeIndexed;
          startX[lastNodeIndexed] = startX[currentNode] + height/2;
          startY[lastNodeIndexed] = startY[currentNode];
          v[lastNodeIndexed].area = v[currentNode].area / 4; //daca se imparte in 4
      }
      else{
          (*nLeaves)++;
          v[currentNode].top_left = -1;
          v[currentNode].top_right = -1;
          v[currentNode].bottom_right = -1;
          v[currentNode].bottom_left = -1;

      } //daca nu se imparte

  }
  (*nBlocks) = lastNodeIndexed + 1;
  free(startX); free(startY);
  return v;
}

int isLeaf(QuadtreeNode *v,int index){
    if(v[index].top_left == -1)
      return 1;

    return 0;
}

void parseTree(RGB ***pixelMap, QuadtreeNode *v, int index, int x, int y, int area){
    int length = sqrt(area);
    int r,g,b;

    if(!isLeaf(v,index)){ //continuam recurenta
      parseTree(pixelMap,v, v[index].top_left, x, y, area/4);
      parseTree(pixelMap,v, v[index].top_right, x, y + length/2, area/4);
      parseTree(pixelMap,v, v[index].bottom_right, x + length/2, y + length/2, area/4);
      parseTree(pixelMap,v, v[index].bottom_left, x + length/2, y, area/4);
    }
    else{ //adaugam culoarea medie a submatricei in imagine
      r = v[index].red; g = v[index].green; b = v[index].blue;
      for(int i = x; i < x + length; ++i)
         for(int j = y; j < y + length; ++j){
           (*pixelMap)[i][j].red = r;
           (*pixelMap)[i][j].green = g;
           (*pixelMap)[i][j].blue = b;
         }
    }

}

void decompressImage(char *fileNameIN, char *fileNameOUT){

    FILE *fi = fopen(fileNameIN,"rb");
    FILE *fo = fopen(fileNameOUT,"w");

    uint32_t nBlocks, nLeaves;

    //citeste din fisierul comprimat
    fread(&nLeaves, sizeof(uint32_t), 1, fi);
    fread(&nBlocks, sizeof(uint32_t), 1, fi);
    QuadtreeNode *v = calloc(nBlocks*2 + 1, sizeof(QuadtreeNode));

    for(int i = 0; i < nBlocks; ++i)
        fread(&v[i], sizeof(QuadtreeNode), 1, fi);

    fclose(fi);

    int length = sqrt(v[0].area);
    RGB **pixelMap = calloc(length + 1, sizeof(RGB*));

    for(int i = 0; i < length; ++i)
      pixelMap[i] = calloc(length + 1, sizeof(RGB));

    fprintf(fo,"P6\n%d %d\n255\n", length, length);
    fclose(fo);
    fo = fopen(fileNameOUT,"ab");

    //construieste imaginea din arbore
    parseTree(&pixelMap, v, 0, 0, 0, v[0].area);

    length = sqrt(v[0].area);
    for(int i = 0; i < length; ++i)
      for(int j = 0; j < length; ++j)
        fprintf(fo,"%c%c%c", pixelMap[i][j].red, pixelMap[i][j].green, pixelMap[i][j].blue);

    for(int i = 0; i < length; ++i)
      free(pixelMap[i]);
    free(pixelMap);
    free(v);

    fclose(fo);
}

void decompressMirroredImage(QuadtreeNode *v, uint32_t nLeaves, uint32_t nBlocks, char *fileNameOUT){

    FILE *fo = fopen(fileNameOUT,"w");

    int length = sqrt(v[0].area);
    RGB **pixelMap = calloc(length + 1, sizeof(RGB*));

    for(int i = 0; i < length; ++i)
      pixelMap[i] = calloc(length + 1, sizeof(RGB));

    fprintf(fo,"P6\n%d %d\n255\n", length, length);
    fclose(fo);
    fo = fopen(fileNameOUT,"ab");

    //construieste imaginea din arbore
    parseTree(&pixelMap, v, 0, 0, 0, v[0].area);

    length = sqrt(v[0].area);
    for(int i = 0; i < length; ++i)
      for(int j = 0; j < length; ++j)
        fprintf(fo,"%c%c%c", pixelMap[i][j].red, pixelMap[i][j].green, pixelMap[i][j].blue);

    for(int i = 0; i < length; ++i)
      free(pixelMap[i]);
    free(pixelMap);

    fclose(fo);
}

RGB** decompressImageForMerge(QuadtreeNode *v, int length){
    RGB **pixelMap = calloc(length, sizeof(RGB*));

    for(int i = 0; i < length; ++i)
      pixelMap[i] = calloc(length + 1, sizeof(RGB));

    //construieste imaginea din arbore
    parseTree(&pixelMap, v, 0, 0, 0, v[0].area);

    return pixelMap;
}

RGB** mergeImages(QuadtreeNode *v1, QuadtreeNode *v2, int length){
  RGB **pixelMapMerged = calloc(length, sizeof(RGB*));
  RGB **pixelMap1, **pixelMap2;

  for(int i = 0; i < length; ++i)
      pixelMapMerged[i] = calloc(length, sizeof(RGB));

  //construieste matricele de pixeli
  pixelMap1 = decompressImageForMerge(v1, length);
  pixelMap2 = decompressImageForMerge(v2, length);

  //face medie aritmetica a valorilor din matrice
  for(int i = 0; i < length; ++i)
      for(int j = 0; j < length; ++j){
        pixelMapMerged[i][j].red = (pixelMap1[i][j].red + pixelMap2[i][j].red)/2;
        pixelMapMerged[i][j].green = (pixelMap1[i][j].green + pixelMap2[i][j].green)/2;
        pixelMapMerged[i][j].blue = (pixelMap1[i][j].blue + pixelMap2[i][j].blue)/2;
      }

 for(int i = 0; i < length; ++i){
        free(pixelMap1[i]);
        free(pixelMap2[i]);
  }
  free(pixelMap1); free(pixelMap2);

  return pixelMapMerged;
}
