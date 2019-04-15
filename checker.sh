#!/bin/bash

mkdir Compression50Out 2>/dev/null
mkdir Compression100Out 2>/dev/null
mkdir Decompression50Out 2>/dev/null
mkdir Decompression100Out 2>/dev/null
mkdir Mirror 2>/dev/null
mkdir Mirror/MirrorVertically 2>/dev/null
mkdir Mirror/MirrorHorizontally 2>/dev/null
for (( i=0; i<=$1; i++));
do
  echo "Test $i:"
  ./quadtree -c 50 Images/test$i.ppm Compression50Out/test$i.bin 2> /dev/null && echo -e "\tCompression t=50  OK" || echo -e "\tCompression t=50  FAIL"
  ./quadtree -d Compression50Out/test$i.bin Decompression50Out/test$i.ppm 2>/dev/null && echo -e "\tDecompression t=50  OK" || echo -e "\tDecompression t=50 FAIL"
  ./quadtree -c 100 Images/test$i.ppm Compression50Out/test$i.bin 2> /dev/null && echo -e "\tCompression t=100  OK" || echo -e "\tCompression t=100  FAIL"
  ./quadtree -d Compression50Out/test$i.bin Decompression50Out/test$i.ppm 2>/dev/null && echo -e "\tDecompression t=100  OK" || echo -e "\tDecompression t=100 FAIL"
  ./quadtree -m v 10 Images/test$i.ppm Mirror/MirrorVertically/test$i.ppm 2> /dev/null && echo -e "\tMirror Vertically  OK" || echo -e "\tMirror Vertically  FAIL"
  ./quadtree -m h 10 Images/test$i.ppm Mirror/MirrorHorizontally/test$i.ppm 2> /dev/null && echo -e "\tMirror Horizontally  OK" || echo -e "\tMirror Horizontally  FAIL"
done
