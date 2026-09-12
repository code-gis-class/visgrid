// Laura Toma
//
// add you name if you modify this file 

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits.h>

#include "grid.h"


// creates and returns an empty grid 
Grid* grid_init() {

  Grid* grid;
  grid = malloc(sizeof(Grid));
  assert(grid);
  grid->data = NULL;
  grid->nrows = grid->ncols = 0; //to be safe 
  
  grid->min_value = (float) INT_MAX;
  grid->max_value = (float) -INT_MAX;
  return grid;
}

//reads grid header from file 
//pre: grid must be allocated 
void grid_read_header(FILE* in_file, Grid* grid) {

  assert(grid); 
  assert(fscanf(in_file, "ncols %d\n",        &grid->ncols));
  assert(fscanf(in_file, "nrows %d\n",        &grid->nrows));
  assert(fscanf(in_file, "xllcorner %f\n",    &grid->xllcorner));
  assert(fscanf(in_file, "yllcorner %f\n",    &grid->yllcorner));
  assert(fscanf(in_file, "cellsize %f\n",     &grid->cellsize));
  assert(fscanf(in_file, "NODATA_value %f\n", &grid->nodata_value));
}


// allocate space for the grid->data
// pre: grif must exist and header must be populated
void grid_malloc_data(Grid* grid) {
  assert(grid); 
  if (grid->nrows * grid->ncols ==0) {
    printf("grid_malloc_data: something's wrong, ncols=%d, nrows=%d\n",
	   grid->ncols, grid->nrows); 
    return; //nothing to do
  }
  if (grid->data) {
    printf("grid_malloc_data: oops, something's wrong, data already exists\n"); 
    return; 
  } 
  if (!grid->data) {
    grid->data = malloc(grid->nrows * grid->ncols * sizeof(float*));
    assert(grid->data);
  }
}

// read grid data from file
// pre: grid must exist and header must be populated 
void grid_read_data(FILE* in_file, Grid* grid) {
  float val;
  for (int r = 0; r < grid->nrows; r++) {
    for (int c = 0; c < grid->ncols; c++) {
      assert(fscanf(in_file, "%f ", &val));
      grid_set(grid, r, c, val);
    }//for c
  }//for r
}


// read grid from file and return it 
Grid* grid_read(FILE* in_file) {

  assert(in_file); 
  Grid* grid = grid_init();
  assert(grid); 
  grid_read_header(in_file, grid);
  grid_malloc_data(grid);
  grid_read_data(in_file, grid);
  return grid;
}



// write grid header to file
// pre: out_file and grid must be valid pointers 
void grid_write_header(FILE* out_file, Grid* grid) {
  fprintf(out_file, "ncols %d\n",        grid->ncols);
  fprintf(out_file, "nrows %d\n",        grid->nrows);
  fprintf(out_file, "xllcorner %f\n",    grid->xllcorner);
  fprintf(out_file, "yllcorner %f\n",    grid->yllcorner);
  fprintf(out_file, "cellsize %f\n",     grid->cellsize);
  fprintf(out_file, "NODATA_value %f\n", grid->nodata_value);
}


// Write the grid to file in asrascii format
void grid_write(FILE* out_file, Grid* grid) {

  grid_write_header(out_file, grid);
  for (int r = 0; r < grid->nrows; r++) {
    for (int c = 0; c < grid->ncols; c++) {
      fprintf(out_file, "%f ", grid_get(grid, r, c));
    }
    fprintf(out_file, "\n");
  }
}



// pre: grid and new_grid exist
// copy header from grid to new_grid 
void grid_copy_header(Grid* grid, Grid* new_grid) {

  assert (grid && new_grid); 
  new_grid->ncols =        grid->ncols;
  new_grid->nrows =        grid->nrows;
  new_grid->xllcorner =    grid->xllcorner;
  new_grid->yllcorner =    grid->yllcorner;
  new_grid->cellsize =     grid->cellsize;
  new_grid->nodata_value = grid->nodata_value;
}


// pre: grid is a valid grid  populated  with data 
// create and initialize a new grid based on grid, copying its header and allocating its data 
Grid* grid_init_from(Grid* grid) {

  assert(grid); 
  Grid* new_grid = grid_init();
  grid_copy_header(grid, new_grid);
  grid_malloc_data(new_grid);
  return new_grid;
}


// Free a grid and its associated malloc-ed data
void grid_free(Grid* grid) {
  if (!grid)
    return; //nothing to free 
  if (grid->data)
    free(grid->data);
  free(grid);
}



