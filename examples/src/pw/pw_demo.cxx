
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <GL/gl.h>
#include <plib/ul.h>
#include <plib/pw.h>

void exFunc ()
{
  fprintf ( stderr, "Exiting.\n" ) ;
  pwCleanup () ;
  exit ( 0 ) ;
}


void rsFunc ( int w, int h )
{
  fprintf ( stderr, "Resized to %d x %d\n", w, h ) ;
}


void mpFunc ( int x, int y )
{
  fprintf ( stderr, "Mouse is at (%4dx%4d)\r", x, y ) ;
}


void msFunc ( int button, int updn, int x, int y )
{
  fprintf ( stderr, "Mouse button %d dirn %d at (%4dx%4d)\n",
                                           button, updn, x, y ) ;
}


void kbFunc ( int key, int updn, int x, int y )
{
  fprintf ( stderr, "Keyboard key %d dirn %d at (%4dx%4d)\n",
                                           key, updn, x, y ) ;

  if ( key == 0x1B ) /* ESC */
  {
    pwCleanup () ;
    exit ( 0 ) ;
  }
}


int main ( int, char ** )
{
  pwInit ( 100, 100, 640, 480, false, "PLIB Window Demo", true, 0,
              kbFunc, msFunc, mpFunc, rsFunc, exFunc ) ;

  while ( 1 )
  {
    glClearColor ( 0.5, 0.1, 0.1, 1.0 ) ;
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
    pwSwapBuffers () ;
  }

  return 0;
}


