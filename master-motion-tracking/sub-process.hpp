//
//  sub-process.hpp
//  master-motion-tracking
//
//  Created by Admin on 10/24/2559 BE.
//  Copyright © 2559 3-Ta-wan-Go. All rights reserved.
//

#ifndef sub_process_hpp
#define sub_process_hpp

//include library headers
#include <opencv2/core/core.hpp>
#include <vector>

//include my own headers
#include "datastructure.hpp"

using namespace std;
using namespace cv;

vector<vector<Point>> FilterContoursDuplication ( vector<vector<Point>> contours , vector<Vec4i> hierachy ) ;

vector<Rect> FilterSmallRect ( const vector<Rect>& rects , const int size_min ) ;

vector<Rect> FilterEdgedStackRect( const vector<Rect>& rects , const xy size ) ;

vector<Rect> GroupRect ( const vector<Rect>& , const xy size_screen ) ;

vector<Rect> FilterSoftROI ( vector<Rect> , const Mat& , const Mat& ) ;

vector<Tracking> FilterGarbageTracking ( const video* vsource ,  const vector<Tracking>& trackings ) ;
#endif