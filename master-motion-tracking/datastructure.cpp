
//header file of this implementation
#include "datastructure.hpp"

//include my own headers
#include "utils.hpp"


///////////////////////////////////////////////////
// pseudo constructor of xy
//////////////////////////////////////////////////
xy xy_ ( const float x , const float y )
{
    xy product;
    product.x = x;
    product.y = y;
    
    return product;
}


//////////////////////////////////////////////////
// pseudo constructor of video
//////////////////////////////////////////////////
video video_ ( const char* path )
{
    VideoCapture capture ( path ) ;
    video product;
    
    if ( capture.isOpened() == 0 )
        return product;
    
    strcpy ( product.path , path ) ;
    product._capture = capture;
    product.size = xy_ ( capture.get ( CV_CAP_PROP_FRAME_WIDTH ) , capture.get( CV_CAP_PROP_FRAME_HEIGHT ) );
    product.count_frame = capture.get ( CV_CAP_PROP_FRAME_COUNT );
    product.fps = capture.get ( CV_CAP_PROP_FPS );
    product.duration = (float)product.count_frame/product.fps;
    product.isLoaded = 1;
    
    return product;
}


///////////////////////////////////////////////////
//pseudo constructor of Tracking
//////////////////////////////////////////////////
Tracking Tracking_ ( const int id , Rect roi , const int frame_id )
{
    Tracking product;
    
    product.id = id ;
    product.rect = roi ;
    product.size = rect_size ( roi ) ;
    product.center = rect_center ( roi ) ;
    product.frame_appear = frame_id;
    product.frame_latest = frame_id;
    product.disappeared = 0;
    
    return product;
}

