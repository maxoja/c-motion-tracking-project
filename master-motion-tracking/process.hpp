
#ifndef process_hpp
#define process_hpp

//include library headers
#include <vector>
#include <opencv2/core/core.hpp>

//include my own headers
#include "datastructure.hpp"

using namespace cv;


Mat ExtractBackgroundFrame ( video* , int , char ) ;
//////////////////////////////////////////////////////////////////
// evaluate frames from the video past in and generate its background frame
// the int parameter is number of frames to pick from the video for extraction
// if the int is non-positive means use every frame for evaluation
// last parameter is a true-false value to trigger and show processing frames
//////////////////////////////////////////////////////////////////


Mat ExtractForegroundMask ( const Mat& , const Mat&, const int ) ;
/////////////////////////////////////////////////////////////////
// return a foreground mask by finding the difference in color
// between the source frame and the backgroung frame given
/////////////////////////////////////////////////////////////////


Mat ExtractMotionMask ( Mat& difference ) ;
/////////////////////////////////////////////////////////////////
// return a mask product of threshold and morphological processed
// on the difference image past in
/////////////////////////////////////////////////////////////////


vector<vector<Point>> ExtractContours ( const Mat& ) ;
/////////////////////////////////////////////////////////////////
// return contours of the mask image given after the process of
// child-contour elimination
/////////////////////////////////////////////////////////////////


vector<Rect> ExtractROIs ( const vector<vector<Point>>& contours , const Mat& source , const Mat& mask , const Mat& background , const int sensitive );
/////////////////////////////////////////////////////////////////
// return bounding rectangless of the contours past in
/////////////////////////////////////////////////////////////////


void UpdateTrackings ( vector<Tracking>&  , const vector<Rect>& , video* , int ) ;
/////////////////////////////////////////////////////////////////
// performs matching tracked objects with tracking objects
// based on relation of position , intersection and size
// then update changes for matched tracked area
// finally create unmatched new tracked area
/////////////////////////////////////////////////////////////////


void DrawTrackings ( Mat& source , const vector<Tracking>& trackings , const int current_frame ) ;
/////////////////////////////////////////////////////////////////
// performs drawing colored & numbered tracking rectangles
// on the image past in
/////////////////////////////////////////////////////////////////

void ExportResultAsFile ( const video* vsource , vector<Tracking> trackings ) ;
//////////////////////////////////////////////////////////////////
// fillter garbage tracking data then save the video source information
// and tracking data retrieved from object tracking processes
// as a text file named as execution date/time
//////////////////////////////////////////////////////////////////



#endif

