//
//  main.c
//  srt
//
//  Created by vector on 11/2/10.
//  Copyright (c) 2010 Brian F. Allen.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//main.c

#include "raymath.h"
#include "shaders.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <pthread.h>

...

void *threadFunction(void *tNum){
    Vec3 camera_pos;
    set( camera_pos, 0., 0., -4. );
    Vec3 camera_dir;
    set( camera_dir, 0., 0., 1. );
    const double camera_fov = 75.0 * (PI/180.0);
    Vec3 bg_color;
    set( bg_color, 0.8, 0.8, 1 );

    const double pixel_dx = tan( 0.5*camera_fov ) / ((double)width*0.5);
    const double pixel_dy = tan( 0.5*camera_fov ) / ((double)height*0.5);
    const double subsample_dx
        = halfSamples  ? pixel_dx / ((double)halfSamples*2.0)
                       : pixel_dx;
    const double subsample_dy
        = halfSamples ? pixel_dy / ((double)halfSamples*2.0)
                      : pixel_dy;

    for( int px=*(int *)tNum; px<width; px += nthreads )
    {
        const double x = pixel_dx * ((double)( px-(width/2) ));
        for( int py=0; py<height; ++py )
        {
            const double y = pixel_dy * ((double)( py-(height/2) ));
            Vec3 pixel_color;
            set( pixel_color, 0, 0, 0 );

            for( int xs=-halfSamples; xs<=halfSamples; ++xs )
            {
                for( int ys=-halfSamples; ys<=halfSamples; ++ys )
                {
                    double subx = x + ((double)xs)*subsample_dx;
                    double suby = y + ((double)ys)*subsample_dy;

                    /* construct the ray coming out of the camera, through
                     * the screen at (subx,suby)
                     */
                    ray_t pixel_ray;
                    copy( pixel_ray.org, camera_pos );
                    Vec3 pixel_target;
                    set( pixel_target, subx, suby, z );
                    sub( pixel_ray.dir, pixel_target, camera_pos );
                    norm( pixel_ray.dir, pixel_ray.dir );

                    Vec3 sample_color;
                    copy( sample_color, bg_color );
                    /* trace the ray from the camera that
                     * passes through this pixel */
                    trace( &scene, sample_color, &pixel_ray, 0 );
                    /* sum color for subpixel AA */
                    add( pixel_color, pixel_color, sample_color );
                }
            }

            /* at this point, have accumulated (2*halfSamples)^2 samples,
             * so need to average out the final pixel color
             */
            if( halfSamples )
            {
                mul( pixel_color, pixel_color,
                     (1.0/( 4.0 * halfSamples * halfSamples ) ) );
            }

            /* done, final floating point color values are in pixel_color.
             * Scaled color values are stored in a 3-d array to be printed later,
             * in order to avoid race conditions between threads 
             */
            scaled_color[px][py][0] = gamma( pixel_color[0] ) * max_color;
            scaled_color[px][py][1] = gamma( pixel_color[1] ) * max_color;
            scaled_color[px][py][2] = gamma( pixel_color[2] ) * max_color;

            /* enforce caps, replace with real gamma */
            for( int i=0; i<3; i++)
                scaled_color[px][py][i] = max( min(scaled_color[px][py][i], 255), 0);
        }
    }
    return NULL;
}

int
main( int argc, char **argv )
{
    nthreads = argc == 2 ? atoi( argv[1] ) : 0;

    if( nthreads < 1 ){
      fprintf( stderr, "%s: usage: %s NTHREADS\n", argv[0], argv[0] );
      return 1;
    }

    scene = create_sphereflake_scene( sphereflake_recursion );

    /* Write the image format header */
    /* P3 is an ASCII-formatted, color, PPM file */
    printf( "P3\n%d %d\n%d\n", width, height, max_color );
    printf( "# Rendering scene with %d spheres and %d lights\n",
            scene.sphere_count,
            scene.light_count );

    /* Create an array of pthread_t objects based on specified number of threads */
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if(threads == NULL){
        fprintf(stderr, "An error occurred while allocating memory for threads");
        exit(1);
    }

    /* Created an array of thread ID's represented as integers */
    int *threadNum = (int *)malloc(sizeof(int) * nthreads);
    if(threadNum == NULL){
        fprintf(stderr, "An error occurred while allocating memory for thread IDs");
        exit(1);
    }

    /* Create threads that each calculate the color of specific pixels */
    for(int i = 0; i < nthreads; i++){
	threadNum[i] = i;
	pthread_create(&threads[i], NULL, threadFunction, &threadNum[i]);
    }

    /* Wait for all threads to complete before continuing */
    for(int i = 0; i < nthreads; i++) {
	pthread_join(threads[i], NULL); 
    }

    /* write each pixel out to disk. ppm is forgiving about whitespace,
     * but has a maximum of 70 chars/line, so use one line per pixel
     */
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            printf( "%.0f %.0f %.0f\n", scaled_color[i][j][0], scaled_color[i][j][1], scaled_color[i][j][2] );
        }
        printf( "\n" );
    }
 
    /* Free dynamically allocated memory */
    free_scene( &scene );
    free(threads);
    free(threadNum);

    if( ferror( stdout ) || fclose( stdout ) != 0 )
    {
        fprintf( stderr, "Output error\n" );
	return 1;
    }

    return 0;
}
