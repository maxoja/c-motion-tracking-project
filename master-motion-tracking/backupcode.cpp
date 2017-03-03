/////extracting background

//    char target_path[256];
//
//    for( int i = 0 ; i < num_sample ; ++i )
//    {
//        printf("%d " , i*interval);
//
//        samples[i] = video_get_frame(source, i*interval + 1);
//        sprintf( target_path , "./temp/sample%d.jpg" ,  i );
//        imwrite( target_path , samples[i] );
//    }
//
//    printf("\n");


//    namedWindow("bg");
//    namedWindow("original");
//    waitKey(0);
//    while(1)
//sdfsdlfjhsiudfhslkdfosidjfsdjfpsdfp;sokpfks
//
//        imshow("bg",bg);
//        imshow("original",samples[i]);
//        if(waitKey(30) >= 0) break;




///////////// VIDEO VERSION 1 ////////////////

//typedef struct _video
//{
//    VideoCapture _capture;
//    xy size;
//    int count_frame;
//    int fps;
//    float duration;
//    int isLoaded = 0;
//} video;
//
//video video_ ( VideoCapture , int ) ;
//Mat video_get_frame ( video , int ) ;

//video video_ ( VideoCapture capture , int fps )
//{
//    video product;
//    
//    if ( capture.isOpened() == 0 )
//        return product;
//    
//    product._capture = capture;
//    product.size = xy_ ( capture.get ( CV_CAP_PROP_FRAME_WIDTH ) , capture.get( CV_CAP_PROP_FRAME_HEIGHT ) );
//    product.count_frame = capture.get ( CV_CAP_PROP_FRAME_COUNT );
//    product.fps = capture.get ( CV_CAP_PROP_FPS );
//    product.duration = (float)product.count_frame/product.fps;
//    product.isLoaded = 1;
//    
//    return product;
//}
//
//Mat video_get_frame ( video source , int frameId)
//{
//    source._capture.set ( CV_CAP_PROP_POS_FRAMES, frameId ) ;
//    
//    Mat frame;
//    source._capture.read(frame);
//    
//    return frame;
//}





////// try to join broken lines
//vector<vector<Point>> FixContours ( vector<vector<Point>> contours , Mat img )
//{
//    
//    printf("before fix : %d\n",contours.size());
//    
//    vector<vector<Point>> fixed(0);
//    
//    vector<char> used(contours.size());
//    
//    for ( int i = 0 ; i < contours.size() ; ++i )
//    {
//        if ( contours[i].size() <= 2 )
//        {
//            used[i] = 1;
//            continue;
//        }
//        
//        if(contourArea(contours[i]) / contours[i].size() <= 3)
//        {
//            contours[i] = vector<Point>(contours[i].begin() , contours[i].end() - contours[i].size()/2);
//        }
//        used[i] = 0;
//    }
//    
//    for ( int i = 0 ; i < contours.size() ; ++i )
//    {
//        if ( used[i] )
//            continue;
//        
//        vector<Point> contour = contours[i];
//        
//        while ( pointdist(contour[0], contour[contour.size() - 1] ) != 0 )
//        {
//            Point tail = contour[contour.size()-1];
//            
//            int best_con_id = i;
//            int best_point_id = 0;
//            double min_dist = pointdist(contour[0], tail);
//            
//            
//            
//            for ( int j = i+1 ; j < contours.size() ; ++j )
//            {
//                if ( used[j] )
//                    continue;
//                
//                vector<Point> another = contours[j];
//                double dist = pointdist( tail  , another[0] );
//                
//                if ( dist < min_dist )
//                {
//                    best_con_id = j;
//                    best_point_id = 0;
//                    min_dist = dist;
//                }
//                
//                dist = pointdist( tail , another[another.size()-1 ] );
//                
//                if ( dist < min_dist )
//                {
//                    best_con_id = j;
//                    best_point_id = (int)another.size() - 1;
//                    min_dist = dist;
//                }
//            }
//            
//            
//            
//            vector<Point> extension = contours[best_con_id];
//            
//            if ( best_point_id == 0 )
//            {
//                if ( best_con_id == i )
//                    contour.push_back( Point(contour[0]) );
//                else
//                {
//                    contour.insert(contour.end(),extension.begin(),extension.end());
//                }
//            }
//            else
//            {
//                contour.insert(contour.end(),extension.rbegin(),extension.rend());
//            }
//            
//            used[best_con_id] = 1;
//            
//        }
//        
//        fixed.push_back(contour);
//    }
//    
//    vector<double> area(fixed.size());
//    
//    srand(0);
//    cvtColor(img, img, COLOR_GRAY2BGR);
//    for( int i = 0 ; i < fixed.size() ; i++)
//    {
//        area[i] = contourArea(fixed[i]);
//        drawContours(img, fixed, i, Scalar(rand()%256,rand()%256,rand()%256) , 2);
//        imshow("contours",img);
//        
//    }
//    
//    printf("after fix : %d\n",fixed.size());
//    
//    waitKey(0);
//    
//    return fixed;
//}



/////try to check if a contour cover white mask
//vector<ROI> FilterROI ( vector<ROI> raw , Mat mask )
//{
//    int size_new = 0;
//    char qualification[raw.size()];
//    
//    for ( int i = 0 ; i < raw.size() ; ++i )
//    {
//        char qualified = 1;
//        ROI roi = raw[i];
//        
//        if( !ContourCoverMask(roi.contour_global, mask) )
//            qualified = 0;
//        
//        if( TooSmallRect(roi.rect) )
//            qualified = 0;
//        
//        qualification[i] = qualified;
//        size_new += qualified;
//    }
//    
//    vector<ROI> filtered_rois(size_new);
//    int count_new = 0;
//    for ( int i = 0 ; i < raw.size() && count_new < size_new ; ++i )
//    {
//        if ( qualification[i] )
//            filtered_rois[count_new++] = raw[i];
//    }
//    
//    return filtered_rois;
//}
//int LeftTopMostPoint_ID ( vector<Point> contour )
//{
//    uint left = (uint) - 1;
//    int id = -1;
//    
//    for ( int i = 0 ; i < contour.size() ; ++i )
//    {
//        Point p = contour[i];
//        
//        if(p.x < left)
//        {
//            left = p.x;
//            id = i;
//        }
//        else if( p.x == left )
//        {
//            if ( p.y < contour[id].y )
//            {
//                id = i;
//            }
//        }
//    }
//    
//    return id;
//}
//int ContourCoverMask ( vector<Point> contour , Mat mask )
//{
//    int id_left = LeftTopMostPoint_ID( contour );
//    int id_next = (id_left+1)%contour.size();
//    int id_prev = (id_left-1)%contour.size();
//
//    int center_x = contour[id_left].x + contour[id_next].x + contour[id_prev].x;
//    center_x /= 3;
//    int center_y = contour[id_left].y + contour[id_next].y + contour[id_prev].y;
//    center_y /= 3;
//
//    return mask.at<uchar>(center_y,center_x) == 0;
//}

/////draw random color contours
//    rois = FilterROI(rois, mask);
//    printf("after fix inside  : %d\n",rois.size());
//
//    srand(0);
//    cvtColor(edges, edges, COLOR_GRAY2BGR);
//    edges.setTo(Scalar(0,0,0));
//
//    for(int i = 0 ; i < rois.size() ; ++i)
//    {
//        vector<vector<Point>> tempcon;
//        tempcon.push_back(rois[i].contour_global);
//
//        drawContours(edges, tempcon, 0 , Scalar( rand()%250,rand()%250,rand()%250 ),2);
//        rectangle(edges, rois[i].rect , Scalar(255,255,255));
//    }
//
//    imshow("contours" , edges);
//    waitKey(30);


////////////// ROI Type and related functions ////////////////////////////

/////////////////////////////////////////////
//
//ROI ROI_ ( Rect rect , Mat full_original , Mat full_mask )
//{
//    ROI product;
//    product.source = full_original(rect).clone();
//    product.mask = full_mask(rect).clone();
//    product.rect = rect;
//    product.size = xy_( rect.width , rect.height );
//    product.center_global = xy_( rect.x + rect.width/2 , rect.y + rect.height/2 );
//    product.center_local = xy_(rect.width/2 , rect.height/2);
//
////    product.contour_global = contour_global;
//
////    product.contour_local = vector<Point>(contour_global.size());
////    for( int i = 0 ; i < contour_global.size() ; ++i )
////    {
////        product.contour_local[i] = Point( contour_global[i].x - rect.x , contour_global[i].y - rect.y );
////    }
//
//    return product;
//}
//
//ManyROI ManyROI_ ( ROI* rois , int size )
//{
//    ManyROI product;
//    product.data = rois;
//    product.size = size;
//
//    return product;
//}
//
//ManyROI ManyROI_ ( int size )
//{
//    ROI* rois = new ROI[size];
//    return ManyROI_ ( rois , size );
//}
//
////////////////////////////////////////////////
//
//typedef struct _ROI
//{
//    Mat source;
//    Mat mask;
//    Rect rect;
//    xy size;
//    xy center_global;
//    xy center_local;
//
//    //    vector<Point> contour_global;
//    //    vector<Point> contour_local;
//} ROI;
//
//ROI ROI_ ( Rect /*, vector<Point>*/ , Mat , Mat );
//
//typedef struct _ManyROI
//{
//    ROI* data;
//    int size;
//} ManyROI;
//
//ManyROI ManyROI_( ROI* , int );
//ManyROI ManyROI_( int );
//
////////////////////////////////////////






/*
 //include library headers
 #include <opencv2/core/core.hpp>
 #include <opencv2/highgui/highgui.hpp>
 #include <opencv2/imgproc/imgproc.hpp>
 #include <ctype.h>
 #include <stdlib.h>
 #include <string.h>
 
 //include my headers
 #include "control.hpp"
 #include "process.hpp"
 #include "utils.hpp"
 
 using namespace cv;
 using namespace std;
 
 //backup code
 void backup ()
 {
 //void Test2 ( )
 //{
 //    VideoCapture cap("crop.mp4");
 //    Mat frame_current, frame_prev;// , difference , blur , edge , closed , dilated;
 //    Mat result;
 //
 //    while(cap.isOpened())
 //    {
 //
 //        frame_prev = frame_current;
 //
 //        frame_current = Mat();
 //        cap.read(frame_current);
 //        if(frame_current.empty())
 //        {
 //            cap = VideoCapture("crop.mp4");
 //            continue;
 //        }
 //
 //        resize(frame_current, frame_current, Size ( frame_current.cols , frame_current.rows ) );
 //        result = frame_current.clone();
 //
 //        imshow("original" , result);
 //
 //        result = abs( result - frame_prev );
 //
 //        imshow("different" , result);
 //
 //        if( mat_average_gray(result) == 0 )
 //            continue;
 //
 //        cvtColor(result, result, COLOR_BGR2GRAY );
 //        imshow("gray", result);
 //
 //
 ////        Mat canny1;
 ////        Canny(result, canny1, 100, 200);
 ////        imshow("canny without blur", canny1);
 //
 //        GaussianBlur(result , result, Size(11,11), 0);
 //        imshow("blur" , result );
 ////
 ////        Mat canny2;
 ////        Canny(result, canny2, 100, 200);
 ////        imshow("canny with blur", canny2);
 ////
 ////        waitKey();
 ////        continue;
 //
 //
 //        threshold(result, result, 10, 255, CV_THRESH_BINARY);
 //        imshow("thresh", result );
 //
 ////        Canny(result, result, 100, 200 );
 ////        imshow("edged",result);
 //
 //
 //        Mat kernel_dilate = getStructuringElement( MORPH_ELLIPSE ,
 //                                           Size( 2*3 + 1, 2*3+1 ),
 //                                           Point( 3, 3 ) );
 //
 //        dilate(result, result, kernel_dilate);
 //        imshow("dilate", result);
 //
 //        Mat kernel_closed = getStructuringElement( MORPH_ELLIPSE ,
 //                                                  Size( 2*2 + 1, 2*2+1 ),
 //                                                  Point( 2, 2 ) );
 //
 //        morphologyEx(result, result, MORPH_CLOSE, kernel_closed);
 //        imshow("closed", result);
 //
 //        vector<vector<Point>> contours;
 //        vector<Vec4i> hierachy;
 //
 //        findContours(result, contours, hierachy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
 //
 //        vector<vector<Point>> fixed(0);
 //
 //        for( int i = 0 ; i < contours.size() ; ++i )
 //        {
 //            if ( hierachy[i][3] < 0 )
 //                fixed.push_back(contours[i]);
 //        }
 //
 //        contours = fixed;
 //        Mat output = frame_current.clone();
 //
 //        vector<Rect> rects(0);
 //        for ( int i = 0 ; i < contours.size() ; ++i )
 //        {
 //            rects.push_back(boundingRect(contours[i]));
 //        }
 //
 //        for ( int i = 0 ; i < rects.size() ; ++i )
 //        {
 //            rectangle(output, rects[i], 255);
 //        }
 //
 //        imshow("output", output);
 //
 //        waitKey(1);
 //    }
 //}
 }
 
 void main_background_substraction ( video* , const int , const int ) ;
 void main_motion_based_tracking ( video* , const int , const int ) ;
 
 int main()
 {
 ShowTitle ( ) ;
 ShowInstruction ( ) ;
 
 int again = 1;
 while ( again )
 {
 video source = LoadSourceFromInputWithValidation ( ) ;
 
 ShowSourceInformation ( source ) ;
 
 switch( Ask_DetectionMode() )
 {
 case MODE_BG_SUBSTRACTION :
 main_background_substraction ( &source , Ask_ShowFrame() , Ask_UseSensitive() ) ;
 break;
 
 case MODE_MOTION_BASED :
 main_motion_based_tracking( &source , Ask_ShowFrame() , Ask_UseSensitive() ) ;
 break;
 
 default :
 printf("\nsomething went wrong\n");
 break;
 }
 
 again = Ask_Again ( ) ;
 }
 return 0;
 }
 
 void main_background_substraction ( video* source , const int showFrame , const int sensitive )
 {
 printf("\n( _ W_) USING BACKGROUND SUBSTRACTION (_ W _ )\n");
 
 // EXTRACTING BACKGROUND /////////////////////////////////////////
 
 Mat background;
 int quality;
 int confirm = 0;
 
 while(1)
 {
 quality = Ask_Quality ( ) ;
 
 background = ExtractBackgroundFrame( source , quality , showFrame );
 
 imshow( "extracted background" , background);
 waitKey(1);
 
 confirm = Ask_ConfirmBackground ( ) ;
 
 destroyAllWindows();
 
 if( confirm )
 break;
 }
 
 // DETECT AND TRACKING OBJECTS ///////////////////////////////////
 
 Mat previous , original, mask , frame_output ;
 vector<vector<Point>> contours;
 vector<Rect> rois;
 vector<Tracking> trackings ;
 
 while(1)
 for ( int i = 0 ; i < source->count_frame ; ++i )
 {
 printf("processing frame %d/%d " , i , source->count_frame );
 printf("(%.2f%%)\n" , (float)(i+1)*100/source->count_frame );
 
 previous = original;
 original = video_get_frame( source , i );
 
 if ( !previous.empty() && mat_same_exactly( previous , original ))
 {
 printf(" redundant frame : skipped\n");
 continue;
 }
 mask = ExtractForegroundMask( original , background , sensitive );
 
 contours = ExtractContours( mask );
 
 rois = ExtractROIs( contours , original , mask , background , sensitive );
 
 UpdateTrackings( trackings , rois , source , i ) ;
 
 if ( showFrame )
 {
 frame_output = original.clone();
 
 //Draw rectangles of ROI on image
 //            for(int j = 0 ; j < rois.size() ; ++j)
 //            {
 //                rectangle(frame_output, rois[j] , Scalar(255,255,255));
 //                char s[10];
 //                sprintf( s , "%d" , 1 );
 //                putText(frame_output, s , Point ( rois[j].x , rois[j].y ) , 5 , 1 , Scalar(0,255,0) );
 //            }
 for(int j = 0 ; j < trackings.size() ; j++)
 {
 if ( trackings[j].frame_latest != i )
 continue;
 int id = trackings[j].id;
 char s[10];
 sprintf( s , "%d" , id );
 rectangle(frame_output, trackings[j].rect , Scalar(255*(id==0),255*(id==1),255*(id==2)) );
 putText(frame_output, s , Point ( trackings[j].rect.x , trackings[j].rect.y ) , 5 , 1 , Scalar(0,255,0) );
 //                putText(frame_output, s , Point ( trackings[j].center.x , trackings[j].center.y  ), 1, 20, Scalar ( 255,255,255));
 }
 
 //Show images on windows
 imshow( "original" , original );
 imshow( "background" , background );
 imshow( "masking frame" , mask );
 imshow( "final result" , frame_output);
 
 if ( i == 0 )
 {
 //arrange windows position
 moveWindow( "original" , 0 , 0 );
 moveWindow( "background" , source->size.x , 0 );
 moveWindow( "masking frame" , 0 , source->size.y + 30 );
 moveWindow( "final result" , source->size.x , source->size.y + 30 );
 }
 
 waitKey(1);
 //            waitKey();
 }
 }
 
 destroyAllWindows ( ) ;
 waitKey(1);
 
 printf("process finished\n");
 
 return;
 }
 
 void main_motion_based_tracking ( video* vsource , const int showFrame , const int sensitive )
 {
 printf("\n( _ W_) USING MOTION-BASED TRACKING (_ W _ )\n");
 
 Mat original;
 Mat difference , blur , thresh , closed , dilated;
 Mat previous = video_get_frame( vsource , 0 );
 Mat result;
 Mat frame_output;
 
 vector<vector<Point>> contours;
 vector<Rect> rects;
 vector<Tracking> trackings;
 
 for ( int i = 1 ; i < vsource->count_frame ; i++ )
 {
 previous = original;
 original = video_get_frame( vsource , i );
 
 resize ( original, original, Size ( original.cols , original.rows ) );
 
 difference = abs( original - previous );
 
 if( mat_average_gray(difference) == 0 )
 continue;
 
 cvtColor(difference, difference, COLOR_BGR2GRAY );
 
 //        Mat canny1;
 //        Canny(result, canny1, 100, 200);
 //        imshow("canny without blur", canny1);
 
 GaussianBlur( difference , blur , Size(11,11), 0);
 
 //        Mat canny2;
 //        Canny(result, canny2, 100, 200);
 //        imshow("canny with blur", canny2);
 //
 //        waitKey();
 //        continue;
 
 threshold ( difference , thresh , 10, 255, CV_THRESH_BINARY);
 
 //        Canny(result, result, 100, 200 );
 //        imshow("edged",result);
 
 
 Mat kernel_dilate = getStructuringElement( MORPH_ELLIPSE ,
 Size( 2*3 + 1, 2*3+1 ),
 Point( 3, 3 ) );
 
 dilate ( thresh , dilated , kernel_dilate);
 
 Mat kernel_closed = getStructuringElement( MORPH_ELLIPSE ,
 Size( 2*2 + 1, 2*2+1 ),
 Point( 2, 2 ) );
 
 morphologyEx ( dilated , closed , MORPH_CLOSE, kernel_closed);
 
 contours = ExtractContours ( closed );
 
 rects = ExtractROIs ( contours , closed , thresh , Mat() , sensitive );
 
 UpdateTrackings ( trackings , rects , vsource , i ) ;
 //        for ( int i = 0 ; i < contours.size() ; ++i )
 //        {
 //            rects.push_back(boundingRect(contours[i]));
 //        }
 
 if ( showFrame )
 {
 frame_output = original.clone();
 
 //Draw rectangles of ROI on image
 //            for(int j = 0 ; j < rois.size() ; ++j)
 //            {
 //                rectangle(frame_output, rois[j] , Scalar(255,255,255));
 //                char s[10];
 //                sprintf( s , "%d" , 1 );
 //                putText(frame_output, s , Point ( rois[j].x , rois[j].y ) , 5 , 1 , Scalar(0,255,0) );
 //            }
 
 
 for(int j = 0 ; j < trackings.size() ; j++)
 {
 if ( trackings[j].frame_latest != i )
 continue;
 int id = trackings[j].id;
 char s[10];
 sprintf( s , "%d" , id );
 rectangle(frame_output, trackings[j].rect , Scalar(0,0,0) ) ;
 putText(frame_output, s , Point ( trackings[j].rect.x , trackings[j].rect.y ) , 5 , 1 , Scalar(0,255,0) );
 //                putText(frame_output, s , Point ( trackings[j].center.x , trackings[j].center.y  ), 1, 20, Scalar ( 255,255,255));
 }
 
 //Show images on windows
 imshow( "original" , thresh );
 imshow( "difference" , dilated );
 imshow( "threshold" , closed );
 imshow( "final result" , frame_output);
 
 if ( i == 0 )
 {
 //arrange windows position
 moveWindow( "original" , 0 , 0 );
 moveWindow( "difference" , vsource->size.x , 0 );
 moveWindow( "threshold" , 0 , vsource->size.y + 30 );
 moveWindow( "final result" , vsource->size.x , vsource->size.y + 30 );
 }
 
 waitKey(1);
 //            waitKey();
 }
 
 waitKey(1);
 }
 
 }
 */














/*
 printf("\n( _ W_) USING MOTION-BASED TRACKING (_ W _ )\n");
 
 Mat frame_prev = video_get_frame( source , 0 );
 Mat frame_current;
 Mat difference , blur , edge , closed , dilated;
 
 Mat result;
 
 for ( int i = 1 ; i < source->count_frame ; i++ )
 {
 
 frame_prev = frame_current;
 frame_current = video_get_frame( source , i );
 
 //        resize(frame_current, frame_current, Size ( frame_current.cols , frame_current.rows ) );
 
 result = frame_current.clone();
 
 imshow("original" , result);
 
 result = abs( result - frame_prev );
 
 imshow("different" , result);
 
 if( mat_average_gray(result) == 0 )
 continue;
 
 cvtColor(result, result, COLOR_BGR2GRAY );
 imshow("gray", result);
 
 
 //        Mat canny1;
 //        Canny(result, canny1, 100, 200);
 //        imshow("canny without blur", canny1);
 
 GaussianBlur(result , result, Size(11,11), 0);
 
 imshow("blur" , result );
 //
 //        Mat canny2;
 //        Canny(result, canny2, 100, 200);
 //        imshow("canny with blur", canny2);
 //
 //        waitKey();
 //        continue;
 
 threshold(result, result, 10, 255, CV_THRESH_BINARY);
 imshow("thresh", result );
 
 //        Canny(result, result, 100, 200 );
 //        imshow("edged",result);
 
 
 Mat kernel_dilate = getStructuringElement( MORPH_ELLIPSE ,
 Size( 2*3 + 1, 2*3+1 ),
 Point( 3, 3 ) );
 
 dilate(result, result, kernel_dilate);
 imshow("dilate", result);
 
 Mat kernel_closed = getStructuringElement( MORPH_ELLIPSE ,
 Size( 2*2 + 1, 2*2+1 ),
 Point( 2, 2 ) );
 
 morphologyEx(result, result, MORPH_CLOSE, kernel_closed);
 imshow("closed", result);
 
 vector<vector<Point>> contours;
 vector<Vec4i> hierachy;
 
 findContours(result, contours, hierachy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
 
 vector<vector<Point>> fixed(0);
 
 for( int i = 0 ; i < contours.size() ; ++i )
 {
 if ( hierachy[i][3] < 0 )
 fixed.push_back(contours[i]);
 }
 
 contours = fixed;
 Mat output = frame_current.clone();
 
 vector<Rect> rects(0);
 for ( int i = 0 ; i < contours.size() ; ++i )
 {
 rects.push_back(boundingRect(contours[i]));
 }
 
 for ( int i = 0 ; i < rects.size() ; ++i )
 {
 rectangle(output, rects[i], 255);
 }
 
 imshow("output", output);
 
 
 
 waitKey(1);
 }
*/



//Draw rectangles of ROI on image
//            for(int j = 0 ; j < rois.size() ; ++j)
//            {
//                rectangle(frame_output, rois[j] , Scalar(255,255,255));
//                char s[10];
//                sprintf( s , "%d" , 1 );
//                putText(frame_output, s , Point ( rois[j].x , rois[j].y ) , 5 , 1 , Scalar(0,255,0) );
//            }


//old sty;e ask for confirmation
