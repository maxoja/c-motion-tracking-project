
#ifndef datastructure_hpp
#define datastructure_hpp

//include library headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

/////////////////////////////////////////

typedef struct _xy
{
    float x = 0;
    float y = 0;
}xy;

/* pseudo constructor */
xy xy_ ( const float , const float );

////////////////////////////////////////

typedef struct _video
{
    char path[256];
    VideoCapture _capture;
    xy size;
    int count_frame;
    int fps;
    float duration;
    int isLoaded = 0;
} video;

/* pseodo constructor */
video video_ ( const char* ) ;

////////////////////////////////////////

typedef struct _Tracking
{
    int id;
    Rect rect;
    xy size;
    xy center;
    int frame_appear = -1; //-1 flag null
    int frame_latest = -1;
    int disappeared = 0;
} Tracking;

Tracking Tracking_ ( const int, Rect , const int ) ;

/////////////////////////////////////////

#endif