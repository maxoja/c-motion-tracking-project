
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


void main_background_substraction ( video* , int , const int ) ;
void main_motion_based_tracking ( video* , int , const int ) ;

//display images and handle control bar
void main_display ( video* , Mat& , Mat& , Mat& , Mat& , int* , int* ) ;

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
                printf("\n             +------------------------------------------------+\n");
                printf("             | ( _ W_) USING BACKGROUND SUBSTRACTION (_ W _ ) |\n");
                printf("             +------------------------------------------------+\n");
                main_background_substraction ( &source , Ask_ShowFrame() , Ask_UseSensitive() ) ;
                break;
                
            case MODE_MOTION_BASED :
                printf("\n              +----------------------------------------------+\n");
                printf("              | ( _ W_) USING MOTION-BASED TRACKING (_ W _ ) |\n");
                printf("              +----------------------------------------------+\n");
                main_motion_based_tracking( &source , Ask_ShowFrame() , Ask_UseSensitive() ) ;
                break;
                
            default :
                printf("\nsomething went wrong\n");
                break;
        }
        
        again = Ask_Again ( ) ;
    }
    
    ExitProgram();
    
    return 0;
}

void main_background_substraction ( video* vsource , int showFrame , const int sensitive )
{
    // EXTRACTING BACKGROUND /////////////////////////////////////////
    
    Mat background;
    int quality;
    int confirm = 0;
    
    while(1)
    {
        quality = Ask_Quality ( ) ;
        
        background = ExtractBackgroundFrame( vsource , quality , showFrame );
        
        imshow ( "extracted background" , background ) ;
        moveWindow ( "extracted background" , 0 , 0 ) ;
        
        confirm = Ask_ConfirmBackground( vsource->size.y + 2*DISPLAY_SIZE_BAR );
        
        destroyAllWindows();
        waitKey(1);
        
        if( confirm )
            break;
    }
    
    // DETECT AND TRACKING OBJECTS ///////////////////////////////////
    
    Mat previous , original, mask , frame_output ;
    vector<vector<Point>> contours;
    vector<Rect> rois;
    vector<Tracking> trackings ;
    
    ShowWaitScreen();
    
    //init progress bar
    ShowProgressBar( 0 );
    
    for ( int i = 0 ; i < vsource->count_frame ; ++i )
    {
        //update progressbar
        ShowProgressBar( i , vsource-> count_frame ) ;
        
        previous = original;
        original = video_get_frame( vsource , i );
        
        //skip redundant frames
        if ( !previous.empty() && mat_same_exactly( previous , original ))
            continue;
        
        mask = ExtractForegroundMask( original , background , sensitive );
        
        contours = ExtractContours( mask );
        
        rois = ExtractROIs( contours , original , mask , background , sensitive );
        
        UpdateTrackings( trackings , rois , vsource , i ) ;
 
        //skip showing images
        if ( ! showFrame )
            continue;
        
        frame_output = original.clone();
        
        DrawTrackings( frame_output , trackings , i );
        
        main_display ( vsource , original , background , mask , frame_output , &showFrame , &i );
    }
    
    //fulfill progress bar
    ShowProgressBar( 1 ) ;
    
    destroyAllWindows ( ) ;
    waitKey(1);
    
    ShowFinishedScreen();
    ExportResultAsFile ( vsource , trackings ) ;
    
    return;
}

void main_motion_based_tracking ( video* vsource , int showFrame , const int sensitive )
{
    Mat original , difference , mask , frame_output ;
    Mat previous = video_get_frame( vsource , 0 ) ;
    
    vector<vector<Point>> contours;
    vector<Rect> rects;
    vector<Tracking> trackings;
    
    ShowWaitScreen( );
    
    //init progress bar
    ShowProgressBar( 0 );
    
    for ( int i = 1 ; i < vsource->count_frame ; i++ )
    {
        //update progress bar
        ShowProgressBar( i , vsource->count_frame );
        
        previous = original;
        original = video_get_frame( vsource , i );
        
        //skip redundant frames
        if ( mat_same_exactly( previous , original ) )
            continue;
        
        difference = abs( original - previous );
        
        mask = ExtractMotionMask( difference ) ;
        
        contours = ExtractContours ( mask );
        
        rects = ExtractROIs ( contours , original , mask , Mat() , sensitive );
        
        UpdateTrackings ( trackings , rects , vsource , i ) ;
        
        //skip displaying images
        if ( ! showFrame )
            continue ;
        
        frame_output = original.clone();
          
        DrawTrackings( frame_output , trackings , i ) ;
            
        main_display ( vsource , original , difference , mask , frame_output , &showFrame , &i ) ;
    }
    
    destroyAllWindows ( ) ;
    waitKey(1);
    
    //fulfill progress bar
    ShowProgressBar( 1 ) ;
    
    ShowFinishedScreen();
    ExportResultAsFile ( vsource , trackings ) ;
}

void main_display ( video* vsource , Mat& img1 , Mat& img2 , Mat& img3 , Mat& img4 , int* showFrame , int* frame_counter )
{
    //Show images on windows
    imshow( "1" , img1 );
    imshow( "2" , img2 );
    imshow( "3" , img3 );
    imshow( "4" , img4 );
    
    //arrange windows position
    moveWindow( "1" , 0 , 0 );
    moveWindow( "2" , vsource->size.x , 0 );
    moveWindow( "3" , 0 , vsource->size.y + DISPLAY_SIZE_BAR*2 );
    moveWindow( "4" , vsource->size.x , vsource->size.y + DISPLAY_SIZE_BAR*2 );
    
    RenderText( "TRACKING OBJECTS : [_] pause / [H] hide / [Q] abort " , 0 , 2*vsource->size.y + DISPLAY_SIZE_BAR*3 );
    
    switch ( waitKey(1) )
    {
        case ' ' :
            RenderText( "TRACKING OBJECTS : [_] resume " , 0 , 2*vsource->size.y + DISPLAY_SIZE_BAR*3 );
            
            while ( waitKey(1) != ' ' );
            break;
            
        case 'h' :
        case 'H' :
            *showFrame = 0;
            destroyAllWindows();
            waitKey(1);
            break;
            
        case 'q' :
        case 'Q' :
            *frame_counter = vsource->count_frame;
            break;
    }
}
