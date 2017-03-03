
#ifndef utils_hpp
#define utils_hpp

//include library headers
#include <opencv2/core/core.hpp>

//include my own headers
#include "datastructure.hpp"

using namespace cv;

/////////////////////////////////////////
// utilities for Tawan's created type  //
/////////////////////////////////////////

// video /////////////////////////////////////////////////////

Mat video_get_frame ( video* , int ) ;
float video_get_duration ( const video* vsource ) ;
float video_get_duration ( const video* vsource , int num_frames ) ;

/////////////////////////////////////////
// utilities for built-in / lib's type //
/////////////////////////////////////////

// char[] ////////////////////////////////////////////////////

int string_is_uinteger ( char * p ) ;

// int ///////////////////////////////////////////////////////

int int_min ( int , int ) ;

// Mat ///////////////////////////////////////////////////////

int mat_same_exactly ( const Mat& a , const Mat& b );
int mat_sum_gray ( const Mat& source ) ;
int mat_average_gray ( const Mat& source ) ;
void mat_resize ( Mat , float ) ;

// Rect ///////////////////////////////////////////////////////

// ex : ratio ( 5 , 2 ) -> 2.5
xy rect_size_ratio ( const Rect& a , const Rect& b ) ;
xy rect_size_difference ( const Rect& a , const Rect& b ) ;
xy rect_size ( const Rect& a );
xy rect_center ( const Rect& a );
xy rect_distance_xy ( const Rect& a , const Rect& b ) ;
float rect_distance_f ( const Rect& a , const Rect& b ) ;
int rect_close_by ( const Rect& a , const Rect& b , const int max_gap) ;
Rect rect_merge ( const Rect& a , const Rect& b ) ;
int rect_overlap ( const Rect& a , const Rect& b ) ;
int rect_smaller_than_any ( const Rect& rect , const int threshold ) ;
int rect_smaller_than_both ( const Rect& , const int threshold ) ;
int rect_at_edge ( const Rect& rect , const xy frame_size ) ;
int rect_a_in_b ( const Rect& a , const Rect& b ) ;

// Point ///////////////////////////////////////////////////////

double pointdist ( Point a , Point b ) ;

// Vec3f ///////////////////////////////////////////////////////

Vec3f v3fabs ( Vec3f a , Vec3f b ) ;
int v3fsum ( Vec3f vec ) ;

#endif