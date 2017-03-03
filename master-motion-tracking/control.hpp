#ifndef control_hpp
#define control_hpp

//include my own headers
#include "datastructure.hpp"

#define DISPLAY_SIZE_FONT 2
#define DISPLAY_THICK 3
#define DISPLAY_SIZE_BAR 25

#define PROCESSBAR_SLOT 73

enum { MODE_BG_SUBSTRACTION = 1 , MODE_MOTION_BASED };


//// DISPLAYING FUNCTIONS ////////////////////////////////////////////////////

void ShowTitle ( ) ;
void ShowInstruction ( ) ;
void ShowSourceInformation ( const video ) ;
void ShowWaitScreen ( ) ;
void ShowFinishedScreen ( ) ;
void ShowProgressBar ( const int full ) ; //does not work in terminal
void ShowProgressBar ( const float current , const float max ) ;
void RenderText ( const char* text ) ;
void RenderText ( const char* text , int x , int y ) ;
void ExitProgram ( ) ;

// GETIING INPUT FUNCTIONS //////////////////////////////////////////////////

char GetSingleLowerCharacter ( ) ;
video LoadSourceFromInputWithValidation ( ) ;
int Ask_DetectionMode ( ) ;
int Ask_ShowFrame ( ) ;
int Ask_UseSensitive ( ) ;
int Ask_Again ( ) ;
int Ask_Quality ( ) ;
int Ask_ConfirmBackground ( int y ) ;

#endif
