//
//  UL - utility library
//
//  Contains:
//  - necessary system includes
//  - basic types
//  - error message routines
//  - high performance clocks
//  - more to come (endian support, version ID)
//

#ifndef _INCLUDED_UL_H_
#define _INCLUDED_UL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <assert.h>

#include <limits.h>
#include <math.h>

/* the next lines are to define BSD */
/* see http://www.freebsd.org/handbook/porting.html for why we do this */

#if (defined(__unix__) || defined(unix)) && !defined(USG)
#include <sys/param.h>
#endif
 
#ifdef BSD
#include <float.h>
#endif
#ifdef __MWERKS__
#include <float.h>
#endif
#ifdef WIN32
#include <float.h>
#endif
#ifdef __CYGWIN__
#include <float.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#ifndef WIN32
#  ifndef macintosh
#    include <GL/glx.h>
#  else
#    include <agl.h>
#  endif
#endif

/*
  glGetCurrentContext()
*/

#ifdef WIN32
#  define glGetCurrentContext() wglGetCurrentContext()
#else
#  if defined(macintosh)
#     define glGetCurrentContext() aglGetCurrentContext()
#  else
#     define glGetCurrentContext() glXGetCurrentContext()
#  endif
#endif

/* SGI machines seem to suffer from a lack of FLT_EPSILON so... */

#ifndef FLT_EPSILON
#define FLT_EPSILON 1.19209290e-07f        
#endif

#ifndef DBL_EPSILON
#define DBL_EPSILON 1.19209290e-07f
#endif

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

/* SUNWspro 4.2 and earlier need bool to be defined */

#if defined(__SUNPRO_CC) && __SUNPRO_CC < 0x500
typedef int bool ;
const   int true  = 1 ;
const   int false = 0 ;
#endif


/*
  Basic Types
*/


/*
  High precision clocks.
*/

class ulClock
{
  double start ;
  double now   ;
  double delta ;
  double last_time ;
  double max_delta ;

  double getRawTime () const ;

public:

  ulClock () { reset () ; }

  void reset ()
  {
    start     = getRawTime () ;
    now       = start ;
    max_delta = 0.2 ; 
    delta     = 1.0 / 30.0 ;  /* Faked so stoopid programs won't div0 */
    last_time = now - delta ;
  }

  void   setMaxDelta  ( double maxDelta ) { max_delta = maxDelta ; }
  double getMaxDelta  () const { return max_delta ; }
  void   update       () ;
  double getAbsTime   () const { return now   ; }
  double getDeltaTime () const { return delta ; }
  double getFrameRate () const { return 1.0 / delta ; }
} ;


inline void ulSleep ( int seconds )
{
#ifdef WIN32
  Sleep ( 1000 * seconds ) ;
#else
  sleep ( seconds ) ;
#endif
}


inline void ulMilliSecondSleep ( int milliseconds )
{
#ifdef WIN32
  Sleep ( milliseconds ) ;
#else
  usleep ( milliseconds * 1000 ) ;
#endif
}


/*
  This is extern C to enable 'configure.in' to
  find it with a C-coded probe.
*/

extern "C" void ulInit () ;

/*
  Error handler.
*/

enum ulSeverity
{
  UL_DEBUG,    // Messages that can safely be ignored.
  UL_WARNING,  // Messages that are important.
  UL_FATAL,    // Errors that we cannot recover from.
  UL_MAX_SEVERITY
} ;


typedef void (*ulErrorCallback) ( int severity, char* msg ) ;

void            ulSetError         ( int severity, const char *fmt, ... ) ;
char*           ulGetError         ( void ) ;
void            ulClearError       ( void ) ;
ulErrorCallback ulGetErrorCallback ( void ) ;
void            ulSetErrorCallback ( ulErrorCallback cb ) ;

/*
  Directory Reading
*/

#define UL_NAME_MAX 256
typedef struct _ulDir ulDir ;
struct ulDirEnt
{
  char d_name [ UL_NAME_MAX+1 ];
  bool d_isdir ;
} ;

ulDir* ulOpenDir ( const char* dirname ) ;
ulDirEnt* ulReadDir ( ulDir* dir ) ;
void ulCloseDir ( ulDir* dir ) ;

#endif

