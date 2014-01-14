//------------------------------------------------
#ifndef __UTILS_H__
#define __UTILS_H__
//------------------------------------------------

#include <iostream>

//------------------------------------------------

#ifdef __BORLANDC__
  #include <ios.h>
#endif
#include <fstream>
#include <string>

//------------------------------------------------

using std::ios_base ;
using std::string ;
using std::fstream ;
using std::ifstream ;

//------------------------------------------------

class uu_Utils
{

  public:

  static string ReadLine( ifstream * File_p_a )
  {
    char Character_c_v ;
    char Characters_ac_v[200] ;
    string String_s_v ;

    File_p_a->get(Characters_ac_v,200,'\n') ;
    String_s_v = Characters_ac_v ;
    File_p_a->get( Character_c_v ) ;

    return String_s_v ;
  }

  static void ShowMessage(char* Message_pc_a)
  {
//    MessageBox(NULL,Message_pc_a,"Animacion message",  MB_ICONERROR ||  MB_OK );
  }

  static void ReportException(char* ExceptionString_pc_a)
  {
  //  MessageBox(NULL,ExceptionString_pc_a,"Exception is thrown !",  MB_ICONERROR ||  MB_OK );
  }

  static void Assert( bool Assertion_b_a, char* String_b_a)
  {
    if (!(Assertion_b_a))
    {
    //  MessageBox(NULL,String_b_a,"Assertion failed !",  MB_ICONERROR ||  MB_OK );
    }
  }

  static void FlushConsole( string Message_s_v )
  {
 /*   #ifdef __BORLANDC__
      fstream Console_file_v( "console.txt", ios_base::app ) ;
    #else
      fstream Console_file_v( "console.txt",_IO_APPEND ) ;
    #endif

    Console_file_v << Message_s_v ;

    Console_file_v.close() ;*/
  }

  static void Beeping( int Frequenty_i_a, int Duration_i_a )
  {
    //Beep(Frequenty_i_a, Duration_i_a ) ;
  }

} ;

//------------------------------------------------

#endif // __UTILS_H__
