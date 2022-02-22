#include "common.h"

void Filter2D(
		const char           coeffs[FILTER_V_SIZE][FILTER_H_SIZE],
		float		         factor,
		short                bias,
		unsigned short       width,
		unsigned short       height,
		unsigned short       stride,
		const unsigned char *src,
		unsigned char       *dst)
{
	// Generate local storage for conv output
	unsigned char inter_tmp[MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT];

	// printf("-------- SW Convolution Output--------\n");
	// Convolution Process
    for(int y=0; y<height; ++y)
    {
        for(int x=0; x<width; ++x)
        {
        	// Apply 2D filter to the pixel window
			int sum = 0;
			for(int row=0; row<FILTER_V_SIZE; row++)
			{
				for(int col=0; col<FILTER_H_SIZE; col++)
				{
					unsigned char pixel;
					int xoffset = (x+col-(FILTER_H_SIZE/2));
					int yoffset = (y+row-(FILTER_V_SIZE/2));
					// Deal with boundary conditions : clamp pixels to 0 when outside of image 
					if ( (xoffset<0) || (xoffset>=width) || (yoffset<0) || (yoffset>=height) ) {
						pixel = 0;
					} else {
						pixel = src[yoffset*stride+xoffset];
						// printf("pixel: %d\n", pixel);
					}
					// printf("coeff: %d\n", coeffs[row][col]);
					sum += pixel*coeffs[row][col];
				}
			}
			// printf("sum: %d\n", sum);
			
        	// Normalize and saturate result - ReLU
			unsigned char outpix = MIN(MAX((int(factor * sum)+bias), 0), 255);
			// printf("outpix: %d\n", outpix);

			// Write output
           	inter_tmp[y*stride+x] = outpix;
			// printf(" ---- \n");
            // printf("%3d ", outpix);
            // if(x==width-1) {
            //     printf("\n");
            // }
        }
    }

	// printf("-------- SW Maxpooling Output--------\n");
	// Maxpooling Process
	for(int y=0; y<height; y=y+2)
    {
        for(int x=0; x<width; x=x+2)
        {
        	// Apply maximum pooling filter
			int maxNum = 0;
			for(int row=0; row<FILTER_Pool_V_SIZE; row++)
			{
				for(int col=0; col<FILTER_Pool_V_SIZE; col++)
				{
                    unsigned char pixel;
                    int xoffset = x + col;
                    int yoffset = (y+row)*stride;
                    pixel = inter_tmp[yoffset + xoffset];
                    if (pixel >= maxNum) {
                        maxNum = pixel;
                    }
				}
			}
        	// Normalize and saturate result
			unsigned char outpix = MIN(MAX(int(maxNum), 0), 255);
            int x_out = x/FILTER_Pool_V_SIZE;
            int y_out = y/FILTER_Pool_V_SIZE;
			// Write output
           	dst[y_out*stride/2+x_out] = outpix;
			// printf("dst[y_out*stride/2+x_out]: %5d \n", dst[y_out*stride/2+x_out]);
			// printf("%3d ", maxNum);
            // if(x==width-2) {
            //     printf("\n");
            // }
        }
    }

}




