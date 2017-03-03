
//header of this cpp implementation
#include "control.hpp"

//include library headers
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "string.h"

//include my headers
#include "utils.hpp"

using namespace cv;


//// DISPLAYING FUNCTIONS ////////////////////////////////////////////////////

//privately used
void WaitEnter ( )
{
    while ( getchar() != '\n' );
}

//privately used
void PrintFile ( const char* filename )
{
    FILE* fp = fopen ( filename , "r" );
    
    if ( fp == NULL )
    {
        printf ( "ERROR : cannot load the file %s \n", filename );
        return;
    }
    
    char line[200];
    while ( fgets( line , 200 , fp ) )
        printf("%s",line);
    
    fclose(fp);
}

void ShowTitle ( )
{
    PrintFile("title.txt");
    WaitEnter();
}

void ShowInstruction ( )
{
    PrintFile("instruction.txt");
    WaitEnter();
}

void ShowSourceInformation ( const video vsource )
{
    printf("\n                    ( _ W _) LOADED SOURCE INFO (_ W _ )\n");
    printf("                     ---------------------------------- \n");
    printf("                    (  resolution  : %-5dx %-9d  )\n" , (int)vsource.size.x , (int)vsource.size.y);
    printf("                    (  frame count : %-16d  )\n", vsource.count_frame);
    printf("                    (  fps         : %-16d  )\n" , vsource.fps);
    printf("                    (  duration    : %-8.2f s        )\n" , vsource.duration);
    printf("                     ---------------------------------- \n");
}

void ShowWaitScreen ( )
{
    PrintFile("wait.txt");
}

//privately used
void UpdateProgressBar ( const int init , const int fulfill , const float current , const float max )
{
    static int progress;
    
    if ( init )
        progress = 0;
    else if ( fulfill )
        UpdateProgressBar( 0 , 0 , 100 , 100 );
    else
    {
        if ( max <= 0 )
            while ( progress < PROCESSBAR_SLOT )
            {
                progress++;
                printf("#");
            }
        while ( progress < current / max * PROCESSBAR_SLOT && progress < PROCESSBAR_SLOT )
        {
            progress++;
            printf("#");
        }
    }
}

void ShowProgressBar ( const int full )
{
    if ( full == 0 ) //init
        UpdateProgressBar( 1 , 0 , 0 , 0 );
    
    else //fulfill
        UpdateProgressBar( 0 , 1 , 0 , 0 );
}

void ShowProgressBar ( const float current , const float max )
{
    UpdateProgressBar( 0 , 0 , current , max );
}

void ShowFinishedScreen ( )
{
    PrintFile("finished.txt");
}

void RenderText ( const char* text )
{
    RenderText( text , 0 , 0 );
}

void RenderText ( const char* text , const int x , const int y )
{
    int base;
    const int font_size = DISPLAY_SIZE_FONT;
    const int font_face = 1;
    Size text_size = getTextSize( text , font_face , font_size , DISPLAY_THICK , &base );
    
    const int baseline = text_size.height + base;
    const int height = text_size.height + base*2;
    const int width = text_size.width;
    
    Mat canvas ( height , width , CV_8U );
    canvas.setTo ( 255 );
    
    putText( canvas , text , Point ( 0 , baseline ) , 1, font_size , Scalar(0) , DISPLAY_THICK ) ;
    imshow( "guide" , canvas );
    moveWindow( "guide" , x , y );
}

void ExitProgram ( )
{
    PrintFile("exit.txt");
    exit(1);
}


// GETIING INPUT FUNCTIONS //////////////////////////////////////////////////

//privately used
void ClearBuffer ( )
{
    fseek ( stdin , 0 , SEEK_END );
}

//privately used
int GetNumber ( )
{
    ClearBuffer() ;
    int n;
    scanf ( "%d" , &n );
    return n;
}

char GetSingleLowerCharacter ( )
{
    ClearBuffer() ;
    return tolower( getchar() );
}

video LoadSourceFromInputWithValidation (  )
{
    char path[ 1000 ] ;
    video source ;
    
    while ( 1 )
    {
        printf( "\nPlease enter path to the video source you want to use in this program (q to quit)\n-> " ) ;
        scanf( "%s" , path ) ;
        
        if ( strcmp ( path , "q" ) == 0 || strcmp ( path , "Q" ) == 0 )
            ExitProgram() ;
        
        //check if file exist
        FILE* fp = fopen ( path , "r" );
        if ( fp == NULL )
        {
            printf( "Cannot load source from the given path '%s'.\nTry again\n" , path );
            continue ;
        }
        else
            fclose ( fp );
        
        source = video_( path ) ;
        
        if ( source.isLoaded )
        if ( source.count_frame > 0 )
        if ( source.fps > 0 )
            break;
        
        printf( "The video source given from path '%s' is not valid .\nTry again\n" , path );
    }
    
    return source;
}

int Ask_DetectionMode ( )
{
    int option;
    
    printf("\n            ( _ W _) OBJECT DETECTION METHODS AVAILABLE (_ W _ )\n");
    printf("             -------------------------------------------------- \n");
    printf("            (       1 : background substraction mode           )\n");
    printf("            (       2 : motion-based detection mode            )\n");
    printf("             -------------------------------------------------- \n\n");
    
    do
    {
        printf("Please enter id of method from the list above you want to use ( 1 / 2 ) \n -> ");
        option = GetNumber();
    }
    while(option != MODE_BG_SUBSTRACTION && option != MODE_MOTION_BASED);
    
    return option;
}

int Ask_ShowFrame ( )
{
    char valid[] = "yn";
    char input = ' ';
    
    do
    {
        printf( "\nDo you want to display images during tracking evaluation?\n");
        printf( "by showing images during evaluation the process will be slow down ( y / n )\n -> ");
        input = GetSingleLowerCharacter();
    }
    while ( strchr ( valid , input ) == NULL ) ;
    
    return input == 'y';
}

int Ask_UseSensitive ( )
{
    char valid[] = "yn";
    char input = ' ';
    
    do
    {
        printf( "\nDo you want to use sensitive configuration for this method?\n");
        printf( "by using sensitive config, small objects with less accuracy ( y / n )\n -> ");
        input = GetSingleLowerCharacter() ;
    }
    while ( strchr ( valid , input ) == NULL ) ;
    
    return input == 'y';
}

int Ask_Quality ( )
{
    int quality = -1;
    
    do
    {
        printf ( "\nThe selected method required background extraction\n");
        printf ( "please enter the quality of background extraction ( 1 ... 100 )\n -> ");
        
        quality = GetNumber();
    }
    while ( quality < 1 || quality > 100 );
    
    return quality;
}

int Ask_ConfirmBackground ( int y )
{
    RenderText("  confirm using the extracted background? [Y] | [N]  " , 0 , y );
    
    char key = tolower ( waitKey() ) ;
    while ( key != 'y' && key != 'n' )
        key = tolower ( waitKey() ) ;
    
    return key == 'y';
}

int Ask_Again ( )
{
    char valid[] = "yn";
    char input = ' ';
    
    do
    {
        printf( "\nDo you want pick another video for object tracking processing \n");
        printf( "otherwise it is time to say goodbye ( y / n )\n -> ");
        input = GetSingleLowerCharacter() ;
    }
    while ( strchr ( valid , input ) == NULL ) ;
    
    return input == 'y';
}


