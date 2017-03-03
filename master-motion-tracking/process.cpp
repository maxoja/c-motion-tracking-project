//header file of this cpp implementation
#include "process.hpp"

//include library headers
#include <stdio.h>
#include <time.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/filesystem.hpp>

//include my own headers
#include "sub-process.hpp"
#include "utils.hpp"
#include "control.hpp"

using namespace std;
using namespace cv;
using namespace boost;
using namespace filesystem;

//privately used
void CreateResultPath ( )
{
    path folder_dir = current_path() / "result";
    create_directory(folder_dir);
}

Mat ExtractBackgroundFrame ( video* source , int quality , char showFrames )
{
    int num_sample = source->count_frame * ((float)quality/100);
    if ( num_sample == 0 )
        num_sample = 1;
    
    int interval = source->count_frame/num_sample;
    
    Mat bg = Mat( source->size.y , source->size.x , CV_8UC3 , Vec3b(0,0,0));
    Mat temp = Mat( source->size.y , source->size.x , CV_8UC3 , Vec3b(0,0,0));
    Mat sample;
    
    uint combo_bg [ (int)source->size.y ][ (int)source->size.x ];
    uint combo_temp [ (int)source->size.y ][ (int)source->size.x ];
    
    ShowWaitScreen( );
    ShowProgressBar( 0 ) ;
    
    for ( int i = 0 ; i < num_sample ; ++i )
    {
        ShowProgressBar( i , num_sample );
        
        sample = video_get_frame( source , i*interval ).clone();
        
        for ( int y = 0 ; y < source->size.y ; ++y )
        {
            for ( int x = 0 ; x < source->size.x ; ++x )
            {
                int threshVal = 30;
                threshVal = 3;
                
                Vec3f pix_sample = sample.at<Vec3b>(y,x);
                
                if( i == 0 )
                {
                    bg.at<Vec3b>(y,x) = Vec3b(pix_sample);
                    
                    combo_bg[y][x] = 1;
                    combo_temp[y][x] = 0;
                    
                    continue;
                }
                
                Vec3f pix_bg = bg.at<Vec3b> ( y , x );
                Vec3f pix_temp = temp.at<Vec3b> ( y , x );
                Vec3f pix_dif_bg = v3fabs ( pix_sample , pix_bg );
                Vec3f pix_dif_temp = v3fabs ( pix_sample , pix_temp );
                
                if( v3fsum ( pix_dif_bg )  < threshVal )
                {
                    combo_bg[y][x]++;
//                    bg.at<Vec3b>( y , x ) = pix_bg*0.8 + pix_sample*0.2;
                    continue;
                    
                }
                
                if ( v3fsum ( pix_dif_temp ) < threshVal )
                {
                    combo_temp[y][x]++;
                    
                    if( combo_temp[y][x] >= combo_bg[y][x] )
                    {
                        bg.at<Vec3b> ( y , x ) = Vec3b(pix_temp);
                        combo_bg    [ y ][ x ] = combo_temp[y][x];
                        combo_temp  [ y ][ x ] = 0;
                        temp.at<Vec3b> ( y , x ) = Vec3b(0,0,0);
                    }
                }
                else
                {
                    temp.at<Vec3b> ( y , x ) = Vec3b(pix_sample);
                    combo_temp[y][x] = 1;
                }
            }
        }
        
        if( showFrames )
        {
            //show images
            imshow ( "extracted background", bg );
            imshow ( "current frame in evaluation" , sample );
            
            RenderText( " EXTRACTING BACKGROUND FRAME : [_] pause / [H] hide / [Q] abort" , 0 , source->size.y + DISPLAY_SIZE_BAR*2 ) ;
            
            //arrange windows
            moveWindow( "extracted background" , 0 , 0 );
            moveWindow( "current frame in evaluation" , source->size.x , 0 );
            
            switch ( waitKey(1) )
            {
                case ' ' :
                    RenderText( " EXTRACTING BACKGROUND FRAME : [_] resume " , 0 , source->size.y + DISPLAY_SIZE_BAR*2 ) ;
                    while ( waitKey(1) != ' ' );
                    break;
                    
                case 'h' :
                case 'H' :
                    showFrames = 0;
                    destroyAllWindows();
                    waitKey(1);
                    break;
                    
                case 'q' :
                case 'Q' :
                    i = num_sample;
                    break;
            }
        }
    }

    destroyAllWindows();
    waitKey( 1 );
    
    ShowProgressBar( 1 ) ;
    
    printf("\nBackground extraction finished, please confirm the background\n\n");
    return bg;
}

Mat ExtractForegroundMask ( const Mat& source, const Mat& background , const int sensitive )
{
    int size_erosion = 1;
    int size_dilation = 2;
    int size_closing = ( sensitive ? 4 : 8 ) ;
    
    Mat element_erosion = getStructuringElement
    (
        MORPH_ERODE,
        Size( size_erosion*2 + 1 , size_erosion*2 + 1 ),
        Point( size_erosion, size_erosion )
    );
    Mat element_dilation = getStructuringElement
    (
        MORPH_ELLIPSE,
        Size(  size_dilation*2 + 1 , size_dilation*2 + 1 ),
        Point( size_dilation, size_dilation )
    );
    
    Mat element_closing = getStructuringElement
    (
        MORPH_ELLIPSE,
        Size( 2*size_closing + 1, 2*size_closing+1 ),
        Point( size_closing, size_closing )
     );
    
    Mat dif =  abs ( source - background ) ;
    
    cvtColor( dif, dif , COLOR_BGR2GRAY);
    
    threshold(dif, dif, 20, 255, CV_THRESH_BINARY);
    
    erode(dif, dif, element_erosion);
    
    dilate(dif,dif, element_dilation);
    
    morphologyEx(dif, dif, MORPH_CLOSE, element_closing);
    
    return dif;
}

Mat ExtractMotionMask ( Mat& difference )
{
    Mat blur , thresh , closed , dilated ;
    
    Mat kernel_dilate = getStructuringElement
    (
     MORPH_ELLIPSE ,
     Size( 2*3 + 1, 2*3+1 ),
     Point( 3, 3 )
     );
    
    Mat kernel_closed = getStructuringElement
    (
     MORPH_ELLIPSE ,
     Size( 2*2 + 1, 2*2+1 ),
     Point( 2, 2 )
     );
    
    cvtColor(difference, difference, COLOR_BGR2GRAY );
    
    GaussianBlur( difference , blur , Size(11,11) , 0);
    
    threshold ( blur , thresh , 15, 255, CV_THRESH_BINARY);
    
    dilate ( thresh , dilated , kernel_dilate );
    
    morphologyEx ( dilated , closed , MORPH_CLOSE, kernel_closed);
    
    return closed;
}

vector<vector<Point>> ExtractContours ( const Mat& mask )
{
    vector<vector<Point>> contours ;
    vector<Vec4i> hierachy ;
    
    findContours(mask.clone(), contours, hierachy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    
    contours = FilterContoursDuplication( contours , hierachy ) ;
    
    return contours;
}

vector<Rect> ExtractROIs ( const vector<vector<Point>>& contours , const Mat& source , const Mat& mask , const Mat& background , const int sensitive )
{
    vector<Rect> rects;
    
    for( int i = 0 ; i < contours.size() ; ++i )
        rects.push_back( boundingRect(contours[i]) ) ;
    
    int smallSize;
    
    if ( sensitive )
        smallSize = 5;
    else
        smallSize = int_min ( source.rows , source.cols ) * 0.07 ;

    rects = FilterSmallRect( rects , smallSize ) ;
    rects = FilterEdgedStackRect( rects , xy_ ( source.cols , source.rows ) ) ;

    if ( ! sensitive )
        rects = GroupRect ( rects , xy_ ( source.cols , source.rows ) ) ;
    
    if ( ! sensitive && ! background.empty() )
        rects = FilterSoftROI( rects , source , background ) ;
    
    return rects ;
}

//privately used
typedef struct {
    int id_track;
    int id_rect;
    float dist = 1000000;
    float intersect = 0;
} cunit ; //cunit stand for compare unit

void UpdateTrackings ( vector<Tracking>& trackings , const vector<Rect>& rects , video* vsource , const int frameId )
{
    //comparing table
    cunit* table[trackings.size()];
    
    for ( int i = 0 ; i < trackings.size() ; i++ )
    {
        if ( ! trackings [ i ] . disappeared )
            if ( video_get_duration( vsource , frameId - trackings[i].frame_latest ) >= 2 )
                trackings [ i ] . disappeared = 1 ;
        
        table[i] = (cunit*) malloc ( sizeof ( cunit ) * rects.size() ) ;
        
        for ( int o = 0 ; o < rects.size() ; o++ )
        {
            float dist = rect_distance_f( rects[o] , trackings[i].rect ) ;
            float intersect = ( rects[o] & trackings[i].rect ).area() ;
            
            table [ i ] [ o ].id_track = i;
            table [ i ] [ o ].id_rect = o;
            table [ i ] [ o ].dist = dist;
            table [ i ] [ o ].intersect = intersect;
        }
        
        if ( trackings [ i ] . disappeared )
        {
            table [ i ] [ 0 ] . intersect = -1 ;
            continue;
        }
        
        for ( int o = 0 ; o < rects.size() ; o++ )
        {
            for ( int p = o ; p < rects.size() - 1 ; p++ )
            {
                if ( table [ i ] [ p ] . dist > table [ i ] [ p + 1 ] . dist )
                {
                    cunit t = table [ i ] [ p ] ;
                    table [ i ] [ p ] = table [ i ] [ p + 1 ] ;
                    table [ i ] [ p + 1 ] = t ;
                }
            }
        }
        
        for ( int o = 0 ; o < rects.size() ; o++ )
        {
            for ( int p = o ; p < rects.size() - 1 ; p++ )
            {
                if ( table [ i ] [ p ] . intersect < table [ i ] [ p + 1 ] . intersect )
                {
                    cunit t = table [ i ] [ p ] ;
                    table [ i ] [ p ] = table [ i ] [ p + 1 ] ;
                    table [ i ] [ p + 1 ] = t ;
                }
            }
        }
    }
    
    for ( int i = 0 ; i < trackings.size() ; i++ )
    {
        for ( int o = i ; o < trackings.size()-1 ; o++)
        {
            if ( table [ o ] [ 0 ] . intersect < table [ o+1 ] [ 0 ] . intersect)
            {
                cunit* t = table [ o ];
                table [ o ] = table [ o + 1 ];
                table [ o + 1 ] = t;
            }
        }
    }
    
    //matching
    char* used = (char*) calloc ( sizeof(char) , rects.size() );
    for ( int i = 0 ; i < trackings.size() ; i++ )
    {
        
        for ( int o = 0 ; o < rects.size() ; o++ )
        {
            const int id_track = table[i][o].id_track;
            const int id_rect = table[i][o].id_rect;
            
            const Rect& a = trackings [ id_track ].rect ;
            const Rect& b = rects [ id_rect ] ;
            
            if ( trackings [ id_track ] . disappeared )
                break;
            
            if ( used [ id_rect ] )
                continue;
            
            //filter too far pair
            if ( rect_distance_f( trackings[ id_track ].rect , rects [ id_rect ] ) >= vsource->size.y*0.2 )
                break;
            
            //filter too large difference in scale pair
            xy size_dif = rect_size_difference( a , b );
            xy size_ratio = rect_size_ratio( a , b );
            int side_small = int_min ( vsource->size.x , vsource->size.y );
            
            if ( size_dif.x >= side_small*0.075 && size_ratio.x >= 2)
            if ( size_dif.y >= side_small*0.075 && size_ratio.y >= 2)
                continue;
            
            
            trackings [ id_track ].rect = rects [ id_rect ];
            trackings [ id_track ].center = rect_center( rects [ id_rect ] ) ;
            trackings [ id_track ].size = rect_size( rects [ id_rect ] ) ;
            trackings [ id_track ].frame_latest = frameId ;
            
            used [ id_rect ] = 1;
            
            break;
        }
    }
    
    // free table memory
    for ( int i = 0 ; i < trackings.size() ; ++i )
    {
        free ( table[i] ) ;
    }
    
    for ( int i = 0 ; i < rects.size() ; i++ )
    {
        if ( used [ i ] )
            continue;
        
        trackings.push_back( Tracking_ ( trackings.size() , rects[i] , frameId ));
    }
    
    free(used);
}

void DrawTrackings ( Mat& source , const vector<Tracking>& trackings , const int current_frame )
{
    Scalar colors[] = {
//        Scalar(0,0,0),
        Scalar(255,0,0),
        Scalar(0,255,0),
        Scalar(0,0,255),
        Scalar(255,255,0),
        Scalar(255,0,255),
        Scalar(0,255,255)
//        Scalar(255,255,255)
    };
    
    int i ;
    for ( i = 0 ; i < trackings.size() ; ++i )
    {
        Tracking tracking = trackings[i];
        
        if ( tracking.disappeared )
            continue;
        
        if ( tracking.frame_latest != current_frame )
            continue;
        
        char tag[17];
        sprintf( tag , "object %d" , tracking.id+1 );
        
        rectangle( source , tracking.rect , colors [ tracking.id%6 ] );
        putText( source , tag , Point ( tracking.rect.x , tracking.rect.y ) , 5 , 1 , Scalar(255,255,255) );
    }
}

void ExportResultAsFile ( const video* vsource , vector<Tracking> trackings )
{
    trackings = FilterGarbageTracking( vsource , trackings );
    
    CreateResultPath ( ) ;
    
    char fname[256];
    
    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    sprintf( fname , "result/%s" , asctime(timeinfo) );
    
    FILE* fp = NULL;
    char temp[256];
    
    do
    {
        if ( fp != NULL )
            fclose ( fp ) ;
        
        strcpy( temp , fname );
        strcat( temp , ".txt" );
        
        fp = fopen ( temp , "r" );
        
        strcat( fname , "*" );
    }
    while ( fp != NULL );
    
    fp = fopen( temp, "w" );
    
    fprintf( fp , "-------------------------------------------------------------------------\n" );
    fprintf( fp , "\t\t\tvideo source : %s\n" , vsource->path );
    fprintf( fp , "\t\t\tresolution w : %d\n" , (int) vsource->size.x );
    fprintf( fp , "\t\t\tresolution h : %d\n" , (int) vsource->size.y );
    fprintf( fp , "\t\t\tduration     : %.2f secs\n" , vsource->duration );
    
    fprintf( fp , "-------------------------------------------------------------------------\n" );
    int i;
    for ( i = 0 ; i < trackings.size() ; ++i )
    {
        Tracking tracking = trackings[i] ;
        fprintf( fp , "\t\t\tID :       %d\n" , i+1 );
        fprintf( fp , "\t\t\tAppeared : %.2f secs\n" , video_get_duration( vsource , tracking.frame_appear ) ) ;
        fprintf( fp , "\t\t\tGone     : %.2f secs\n" , video_get_duration( vsource , tracking.frame_latest ) ) ;
        fprintf( fp , "\t\t\tStay     : %.2f secs\n" , video_get_duration( vsource , tracking.frame_latest - tracking.frame_appear ) ) ;
        fprintf( fp , "-------------------------------------------------------------------------\n" );
    }
    
    fclose ( fp );
}

