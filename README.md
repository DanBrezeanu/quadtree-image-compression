
## Quadtree image compression algorithm

The algorithm compresses PPM images using a quadtree representation.
Options for running:
- compression
- decompression
- mirror
- merge


### Compiling and running options

    make build

#### Syntax:

    ./quadtree [-c threshold | -d | -m type | -o] input_file1 [input_file2] [output_file]
    
> -c compress with given threshold
>
> -d decompress
>
> -m mirror image
>
>> type
>>>  h (horizontally)
>>>
>>>  v (vertically)
>
> -o merge images
>> input_file2 needs to be given for this option


__ALL IMAGES MUST BE P6 PPM FILES__

__Image headers must NOT contain comments__
### Examples

| Original files   | Compressed Image (t = 80) | Mirrored Vertically | Mirrored Horizontally | Merged |
|------------|-------------|-------------|-------------|-------------|
| ![alt text](https://raw.githubusercontent.com/DanBrezeanu/quadtree-image-compression/master/screenshots/original_sailboat.PNG) | ![alt text](https://raw.githubusercontent.com/DanBrezeanu/quadtree-image-compression/master/screenshots/decompressed_sailboat_80.PNG) | ![alt text](https://raw.githubusercontent.com/DanBrezeanu/quadtree-image-compression/master/screenshots/mirror_v_sailboat.PNG) | ![alt text](https://raw.githubusercontent.com/DanBrezeanu/quadtree-image-compression/master/screenshots/mirror_h_sailboat.PNG) | ![alt text](https://raw.githubusercontent.com/DanBrezeanu/quadtree-image-compression/master/screenshots/merge_lenna_sailboat.PNG) | 
|  ![alt text](https://raw.githubusercontent.com/DanBrezeanu/quadtree-image-compression/master/screenshots/original_lenna.PNG) | ![alt text](https://raw.githubusercontent.com/DanBrezeanu/quadtree-image-compression/master/screenshots/decompressed_lenna_80.PNG) | ![alt text](https://raw.githubusercontent.com/DanBrezeanu/quadtree-image-compression/master/screenshots/mirror_v_lenna.PNG) | ![alt text](https://raw.githubusercontent.com/DanBrezeanu/quadtree-image-compression/master/screenshots/mirror_h_lenna.PNG) | ![alt text](https://raw.githubusercontent.com/DanBrezeanu/quadtree-image-compression/master/screenshots/merge_lenna_sailboat.PNG) |
