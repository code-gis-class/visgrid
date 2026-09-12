// Laura Toma
//
// add your name here if you modify the file

#include "pixel_buffer.h"
#include "grid.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <time.h>



static const u8 max_rgb_value = 0xffu;

int main(int argc, char** argv) {

  //user shoud specify raster name as argv[1]
  if(argc!= 2) {
    printf("usage: %s [raster.asc]\n", argv[0]); 
    exit(1); 
  } 
  printf("reading raster %s\n", argv[1]);
  FILE* infile = fopen(argv[1], "r");
  if (!infile) {
    printf("cannot open file %s\n", argv[1]);
    exit(1); 
  } 
  Grid* grid = grid_read(infile); 
  grid_write_header(stdout, grid);   //sanity check  

 
  //create a bmp of same size as the raster
  const PixelBuffer pb = init_pixel_buffer(grid->ncols, grid->nrows);
  
  //write values in pb
  float color[3]; 
  for (u16 y=0; y < pb.height; y++) {
    for (u16 x=0; x< pb.width; x++) {

      float h = grid_get(grid, y, x); //get height of this pixel 
      if (grid_is_nodata(grid, y, x)) {
	color[0] = 0; color[1] = 0; color[2] = 1; //blue if nodata 
      } else {
	//interpolate the color
	// hin is set to {0,0,0} and hmax is set to {1,1,1}
	float c = (h - grid->min_value)/ (grid->max_value - grid->min_value);
	color[0] = c; 
	color[1] = c; 
	color[2] = c; 
      }
      
      //bring it from [0,1] to [0, 255]
      u16 rgb_color[3] = {
	(u16) (color[0] * max_rgb_value),
	(u16) (color[1] * max_rgb_value),
	(u16) (color[2] * max_rgb_value)}; 
      
      write_pixel_to_buffer(&pb, x, y, rgb_color[0], rgb_color[1], rgb_color[2]);
    }
  } 
  
  printf("saving raster %s to out.bmp\n", argv[1]); 
  save_pixel_buffer_to_file(&pb, "out.bmp");

  //cleanup
  deinit_pixel_buffer(&pb);
  grid_free(grid); 
} 
