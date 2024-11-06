C Program to add filters to bmp images

Filters:

    ASCII Art, -a, display image as ASCII art in terminal and outputs as .txt. file
    Grayscale, -g, outputs new bmp file
    Reflect, -r, outputs new bmp file
    Box Blur, -b, outputs new bmp file
    Pixelize, -p, outputs new bmp file
    Sobel Edge Detection, -e, outputs new bmp file

Instructions:

    To compile:
        edit makefile to use your c compiler (clang by default)
        make filter
    The command to run consists of:
        ./filter
        the operator for the filter you want to apply listed above (-a, -g, -r, -b, -p, or -e)
        the path to the bmp image you want to apply filter to
        the path to send the resulting bmp image with the filter applied
        EXAMPLE: ./filter -p images/tower.bmp images/out.bmp

IMAGES:

    I included sample free bmp files that can be used to showcase the filters
    There are 4 regular images and each one has a regular version and a vertically-flipped version
    The vertically flipped ones are for the ASCII art filter, which display text vertically-flipped. So use ASCII filter on flipped images and all other filters on normal images.

NOTE For best results displaying terminal ASCII art, go to your terminal's preferences and set the font size to small size like 6
