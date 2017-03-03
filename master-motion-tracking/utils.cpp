
//heder file of this cpp implementation
#include "utils.hpp"

//include library headers
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

/////////////////////////////////////////
/* utilities for Tawan's created type  */
/////////////////////////////////////////

// video /////////////////////////////////////////////////////

Mat video_get_frame ( video* source , int frameId )
{
    //convert from 0-based to 1-based id
    frameId += 1;
    
    //set position of capter reader to frameId
    source -> _capture.set ( CV_CAP_PROP_POS_FRAMES, frameId ) ;
    
    //read frame at the position
    Mat frame;
    source -> _capture.read(frame);
    
    return frame;
}

float video_get_duration ( const video* vsource )
{
    return (float) vsource->count_frame / vsource->fps;
}

float video_get_duration ( const video* vsource , int num_frames )
{
    return (float) num_frames / vsource->fps ;
}

/////////////////////////////////////////
/* utilities for built-in / lib's type */
/////////////////////////////////////////

// char[] ////////////////////////////////////////////////////

int string_is_uinteger ( char * p )
{
    if ( *p == '\0' )
        return 0;
    
    while ( *p != '\0' )
    {
        if ( *p < '0' || *p > '9' )
            return 0;
        
        p++;
    }
    
    return 1;
}

// int ///////////////////////////////////////////////////////

int int_min ( int a , int b )
{
    return a <= b ? a : b ;
}

// Mat ///////////////////////////////////////////////////////

int mat_same_exactly ( const Mat& a , const Mat& b )
{
    if ( a.rows != b.rows || a.cols != b.cols )
        return 0;
    
    Mat dif = abs ( a-b );
    cvtColor( dif , dif , COLOR_BGR2GRAY );
    
    double minVal,maxVal;
    minMaxLoc(dif, &minVal , &maxVal );
    
    if ( maxVal < 30 )
        return 1;
    else
        return 0;
}

int mat_sum_gray ( const Mat& source )
{
    int sum = 0;
    int i=0,j=0;
    for ( i = 0 ; i < source.rows ; ++i )
    {
        for( j = 0 ; j < source.cols ; ++j )
        {
            sum += source.at<uchar>(i,j);
        }
    }
    
    return sum;
}

int mat_average_gray  ( const Mat& source )
{
    if( source.rows * source.cols == 0 )
        return -1;
    
    return mat_sum_gray( source ) / ( source.rows * source.cols );
}

void mat_resize ( Mat source , float ratio )
{
    resize( source.clone() , source , Size ( ratio*source.cols , ratio*source.rows ));
}

// Rect ///////////////////////////////////////////////////////

xy rect_size_difference ( const Rect& a , const Rect& b )
{
    return xy_ ( abs ( a.width - b.width ) , abs ( a.height - b.height ) );
}

// ( 5 , 2 ) -> 2.5 or ( 2 , 5 ) -> 2.5
xy rect_size_ratio ( const Rect& a , const Rect& b )
{
    float rat_w = ( a.width > b.width ) ? (float)a.width / b.width : (float)b.width/a.width ;
    float rat_h = a.height > b.height ? (float)a.height / b.height : (float)b.height/a.height ;
    
    return xy_ ( rat_w , rat_h ) ;
}

xy rect_size ( const Rect& a )
{
    return xy_ ( a.width , a.height );
}

xy rect_center ( const Rect& a )
{
    return xy_ ( a.x + a.width/2 , a.y + a.height/2 );
}

xy rect_distance_xy ( const Rect& a , const Rect& b )
{
    return xy_ (
        abs ( a.x + a.width/2 - b.x - b.width/2 ) ,
        abs ( a.y + a.height/2 - b.y - b.height/2 )
    );
}

float rect_distance_f ( const Rect& a , const Rect& b )
{
    xy distxy = rect_distance_xy( a , b );
    float dist = sqrt ( distxy.x*distxy.x + distxy.y*distxy.y );
    return dist;
}

int rect_close_by ( const Rect& a , const Rect& b , int max_gap )
{
    xy dist = rect_distance_xy( a , b );
    
    if ( dist.x <= a.width/2 + b.width/2 + max_gap )
    if ( dist.y <= a.height/2 + b.height/2 + max_gap )
        return 1;
    
    return 0;
}

Rect rect_merge ( const Rect& a , const Rect& b )
{
    return a | b ;
}

int rect_overlap ( const Rect& a , const Rect& b )
{
    return (a & b).area() > 0 ;
}

int rect_smaller_than_any ( const Rect& rect , const int threshold )
{
    return rect.width < threshold || rect.height < threshold ;
}

int rect_smaller_than_both ( const Rect& rect , const int threshold )
{
    return rect.width < threshold && rect.height < threshold ;
}

int rect_at_edge ( const Rect& rect , const xy frame_size )
{
    int gap = 4;
    
    return
    rect.x <= 4 ||
    rect.y <= 4 ||
    rect.x+rect.width >= frame_size.x - gap ||
    rect.y+rect.height >= frame_size.y - gap ;
}

int rect_a_in_b ( const Rect& a , const Rect& b )
{
    return
    
    a.x >= b.x
    
    &&
    
    a.y >= b.y
    
    &&
    
    a.x+a.width <= b.x + b.width
    
    &&
    
    a.y + a.height <= b.y + b.height;
}

// Point ///////////////////////////////////////////////////////

double pointdist ( Point a , Point b )
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    
    return sqrt( dx*dx + dy*dy );
}

// Vec3f ///////////////////////////////////////////////////////

Vec3f v3fabs ( Vec3f a , Vec3f b )
{
    Vec3f dif;
    dif[0] = abs(a[0] - b[0]);
    dif[1] = abs(a[1] - b[1]);
    dif[2] = abs(a[2] - b[2]);
    
    return dif;
}

int v3fsum ( Vec3f vec )
{
    return vec[0] + vec[1] + vec[2];
}


