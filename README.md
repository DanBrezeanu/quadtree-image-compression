
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
