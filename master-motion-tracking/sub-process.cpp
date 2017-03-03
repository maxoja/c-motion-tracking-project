
//header file of this cpp implementation
#include "sub-process.hpp"

//include library headers
#include <opencv2/imgproc/imgproc.hpp>

//include my own headers
#include "datastructure.hpp"
#include "utils.hpp"

vector<vector<Point>> FilterContoursDuplication ( vector<vector<Point>> contours , vector<Vec4i> hierachy )
{
    vector<vector<Point>> fixed(0);
    
    for( int i = 0 ; i < contours.size() ; ++i )
    {
        if ( hierachy[i][3] < 0 )
            fixed.push_back(contours[i]);
    }
    
    return fixed;
}

vector<Rect> FilterSmallRect( const vector<Rect>& rects , const int size_min )
{
    vector<Rect> fixed(0);
    
    for(int i = 0 ; i < rects.size() ; ++i )
    {
        Rect rect = rects[i];
        char qualified = !rect_smaller_than_both(rect, size_min + 1 );
        
        if(qualified)
            fixed.push_back(rects[i]);
    }
    
    return fixed;
}

vector<Rect> FilterEdgedStackRect( const vector<Rect>& rects , const xy size )
{
    vector<Rect> fixed(0);
    
    for( int i = 0 ; i < rects.size() ; ++i )
    {
        Rect rect = rects[i];
        char qualified = 1;
        
        if ( rect_at_edge(rect , size) )
        {
            for ( int j = 0 ; j < rects.size() ; ++j )
            {
                if( i == j )
                    continue;
                Rect rect_master = rects[j];
                if ( rect_a_in_b ( rect , rect_master ) )
                {
                    qualified = 0;
                    break;
                }
            }
        }
        
        if ( qualified )
            fixed.push_back(rects[i]);
    }
    
    return fixed;
}

vector<Rect> FilterSoftROI ( vector<Rect> rects , const Mat& source , const Mat& background )
{
    int size_new = 0;
    char qualification[rects.size()];
    
    for ( int i = 0 ; i < rects.size() ; ++i )
    {
        char qualified = 1;
        Rect rect = rects[i];
        
        Mat source_crop = source ( rect ) ;
        Mat background_crop = background ( rect ) ;
        
        Mat dif = abs ( source_crop - background_crop ) ;
        
        cvtColor ( dif , dif , COLOR_BGR2GRAY ) ;
        
        GaussianBlur ( dif , dif , Size(5,5) , 0 ) ;
        
        threshold ( dif , dif , 20 , 255 , CV_THRESH_BINARY ) ;
            
        int avg = mat_average_gray(dif);
        if( avg < 60 )
            qualified = 0;
        
        qualification[i] = qualified;
        
        size_new += qualified;
    }
    
    vector<Rect> filtered ( size_new ) ;
    int count_new = 0;
    for ( int i = 0 ; i < rects.size() && count_new < size_new ; ++i )
    {
        if ( qualification[i] )
        {
            filtered[ count_new++ ] = rects[i];
        }
    }
    
    return filtered;
}

vector<Rect> GroupRect ( const vector<Rect>& rects , const xy size_screen )
{
    const int side_small = int_min( size_screen.x , size_screen.y ) ;
    const int gap_max = side_small * 0.025;
    const int size_max = side_small * 0.2 ;
    vector<Rect> fixed;
    
    int i , j ;
    for ( i = 0 ; i < rects.size() ; ++i )
    {
        int matched = 0;
        
        for ( j = 0 ; j < fixed.size() ; ++j )
        {
            const Rect& a = rects[i];
            const Rect& b = fixed[j];
            
            if ( ! rect_smaller_than_any( a , size_max ) && ! rect_smaller_than_any( b , size_max ) )
                continue;
            
            if ( rect_a_in_b ( a , b ) )
            {
                matched = 1;
                break;
            }
            
            if ( rect_a_in_b ( b , a ) )
            {
                matched = 1;
                fixed[j] = a;
                break;
            }
            
            //dont do with large rect
            if ( rect_overlap( a , b ) )
            {
                matched = 1;
                
                fixed[j] = rect_merge ( a , b );
                break;
            }

            if ( rect_close_by( a , b , gap_max ) )
            {
                matched = 1;
                fixed[j] = rect_merge( a , b );
                break;
            }
        }
        
        if ( ! matched )
            fixed.push_back( rects[i] );
    }
    
    return fixed;
}

vector<Tracking> FilterGarbageTracking ( const video* vsource ,  const vector<Tracking>& trackings )
{
    vector<Tracking> fixed;
    
    int i;
    for ( i = 0 ; i < trackings.size() ; ++i )
    {
        if ( video_get_duration( vsource , trackings[i].frame_latest - trackings[i].frame_appear)  < 4 )
            continue;
        fixed.push_back( trackings[i] );
    }
    
    return fixed;
}
