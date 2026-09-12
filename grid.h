// Laura Toma
//
// add you name if you modify this file 

#ifndef __grid_h
#define __grid_h

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>


typedef struct _grid {
  unsigned int     ncols;
  unsigned int     nrows;
  float* data;
  
  float   xllcorner;
  float   yllcorner;
  float   cellsize;
  float   nodata_value;

  //min and max values in grid.data
  float   min_value;
  float   max_value;
} Grid;


typedef struct gridpoint {
    unsigned int r, c;
} GridPoint;


// return true if point (r,c) is inside the grid and false otherwise 
static inline int inside_grid(const Grid* grid, int r, int c) {
  assert(grid); 
  return r >= 0 && r < grid->nrows && c >= 0 && c < grid->ncols;
}




// return grid[r][c]
static inline float grid_get(const Grid* grid, int r, int c) {
  assert(grid); 
  return grid->data[r * grid->ncols + c];
}

// set grid[r][c] to val  and update grid.min and grid.max
static inline void grid_set(Grid* grid, int r, int c, const float val) {

  assert(grid && grid->data);
  assert(inside_grid(grid, r, c)); //point must be inside the grid 
  
  grid->data[r * grid->ncols + c] = val;
  if (val != grid->nodata_value) {
    grid->min_value = fmin(val, grid->min_value);
    grid->max_value = fmax(val, grid->max_value);
  }
}

// return  true if grid[r][c]==nodata and false otherwise 
static inline bool  grid_is_nodata(Grid* grid, int r, int c) {
  assert(grid); 
  return grid->nodata_value == grid_get(grid, r, c);
}

// set (r,c) to be nodata 
static inline void  grid_set_nodata(Grid* grid, int r, int c) {
  grid_set(grid, r, c, grid->nodata_value);
  //grid->data[r * grid->ncols + c] = grid->nodata_value; 
} 


// read grid from file and return it 
Grid* grid_read(FILE* in_file);

// pre: grid is a valid grid populated with data
// create and initialize a new grid based on grid, copying its header
// and allocating its data
Grid* grid_init_from(Grid* grid);


// write grid header to file
// pre: out_file and grid must be valid pointers 
void grid_write_header(FILE* out_file, Grid* grid);


// write the grid to file in asrascii format
void  grid_write(FILE* out_file, Grid* grid);

// free a grid and its associated malloc-ed data
void  grid_free(Grid* grid);

#endif
