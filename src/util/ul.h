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

#if defined (WIN32)
#  include <windows.h>
#  ifdef __CYGWIN__
#    include <unistd.h>
#  endif
#elif defined (__BEOS__)
#    include <be/kernel/image.h>
#elif defined (macintosh)
#  include <CodeFragments.h>
#  else
#    include <unistd.h>
#    include <dlfcn.h>
#  endif


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
  
#ifdef WIN32
  static double res ;
  static int perf_timer ;
  void initPerformanceTimer () ;
#endif

  double getRawTime () const ;

public:

  ulClock () { reset () ; }

  void reset ()
  {
#ifdef WIN32
	  initPerformanceTimer () ;
#endif
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

// file handling

char* ulMakePath( char* path, const char* dir, const char* fname );

int ulFileExists ( char *fileName );

void ulFindFile( char *filenameOutput, const char *path, 
											  const char * tfnameInput, const char *sAPOM ) ;


/*
  Endian handling
*/

static const int _ulEndianTest = 1;
#define ulIsLittleEndian (*((char *) &_ulEndianTest ) != 0)
#define ulIsBigEndian    (*((char *) &_ulEndianTest ) == 0)
static inline void _ulEndianSwap(unsigned int *x) {
  *x = (( *x >> 24 ) & 0x000000FF ) | 
    (( *x >>  8 ) & 0x0000FF00 ) | 
    (( *x <<  8 ) & 0x00FF0000 ) | 
    (( *x << 24 ) & 0xFF000000 ) ;
}
  
static inline void _ulEndianSwap(unsigned short *x) {
  *x = (( *x >>  8 ) & 0x00FF ) | 
    (( *x <<  8 ) & 0xFF00 ) ;
}
  
inline unsigned short ulEndianLittle16(unsigned short x) {
  if (ulIsLittleEndian) {
    return x;
  } else {
    _ulEndianSwap(&x);
    return x;
  }
}

inline unsigned int ulEndianLittle32(unsigned int x) {
  if (ulIsLittleEndian) {
    return x;
  } else {
    _ulEndianSwap(&x);
    return x;
  }
}

inline float ulEndianLittleFloat(float x) {
  if (ulIsLittleEndian) {
    return x;
  } else {
    _ulEndianSwap((unsigned int*)&x);
    return x;
  }
}

inline void ulEndianLittleArray16(unsigned short *x, int length) {
  if (ulIsLittleEndian) {
    return;
  } else {
    for (int i = 0; i < length; i++) {
      _ulEndianSwap(x++);
    }
  }
}

inline void ulEndianLittleArray32(unsigned int *x, int length) {
  if (ulIsLittleEndian) {
    return;
  } else {
    for (int i = 0; i < length; i++) {
      _ulEndianSwap(x++);
    }
  }
}

inline void ulEndianLittleArrayFloat(float *x, int length) {
  if (ulIsLittleEndian) {
    return;
  } else {
    for (int i = 0; i < length; i++) {
      _ulEndianSwap((unsigned int*)x++);
    }
  }
}

inline void ulEndianBigArray16(unsigned short *x, int length) {
  if (ulIsBigEndian) {
    return;
  } else {
    for (int i = 0; i < length; i++) {
      _ulEndianSwap(x++);
    }
  }
}

inline void ulEndianBigArray32(unsigned int *x, int length) {
  if (ulIsBigEndian) {
    return;
  } else {
    for (int i = 0; i < length; i++) {
      _ulEndianSwap(x++);
    }
  }
}

inline void ulEndianBigArrayFloat(float *x, int length) {
  if (ulIsBigEndian) {
    return;
  } else {
    for (int i = 0; i < length; i++) {
      _ulEndianSwap((unsigned int*)x++);
    }
  }
}

inline unsigned short ulEndianBig16(unsigned short x) {
  if (ulIsBigEndian) {
    return x;
  } else {
    _ulEndianSwap(&x);
    return x;
  }
}

inline unsigned int ulEndianBig32(unsigned int x) {
  if (ulIsBigEndian) {
    return x;
  } else {
    _ulEndianSwap(&x);
    return x;
  }
}

inline float ulEndianBigFloat(float x) {
  if (ulIsBigEndian) {
    return x;
  } else {
    _ulEndianSwap((unsigned int*)&x);
    return x;
  }
}

inline unsigned short ulEndianReadLittle16(FILE *f) {
  unsigned short x;
  fread(&x, 2, 1, f);
  return ulEndianLittle16(x);
}

inline unsigned int ulEndianReadLittle32(FILE *f) {
  unsigned int x;
  fread(&x, 4, 1, f);
  return ulEndianLittle32(x);
}

inline float ulEndianReadLittleFloat(FILE *f) {
  float x;
  fread(&x, 4, 1, f);
  return ulEndianLittleFloat(x);
}

inline unsigned short ulEndianReadBig16(FILE *f) {
  unsigned short x;
  fread(&x, 2, 1, f);
  return ulEndianBig16(x);
}

inline unsigned int ulEndianReadBig32(FILE *f) {
  unsigned int x;
  fread(&x, 4, 1, f);
  return ulEndianBig32(x);
}

inline float ulEndianReadBigFloat(FILE *f) {
  float x;
  fread(&x, 4, 1, f);
  return ulEndianBigFloat(x);
}

/*
  UDP Networking Class NetWork Libriary
  by Ben Woodhead
*/

#define UL_UDP_DEFAULT_PORT_NUMBER  5100

class ulUDPConnection
{
  struct sockaddr_in  *in_addr ;
  struct sockaddr_in *out_addr ;

  int sockfd ;
  int  port  ;
  int iport  ;
  int oport  ;

 public:

   ulUDPConnection () ;
  ~ulUDPConnection () ;
  
  void disconnect  () ;

  int connect      ( char *hostname = "localhost",
                     int   _port    = UL_UDP_DEFAULT_PORT_NUMBER ) ;

  int sendMessage  ( char *mesg, int length ) ;
  int recvMessage  ( char *mesg, int length ) ;
} ;

/*
  TCP Networking Class NetWork Library
  by Ben Woodhead
*/

#define UL_TCP_DEFAULT_PORT_NUMBER  5100

class ulTCPConnection
{
  struct sockaddr_in  *in_addr ;
  struct sockaddr_in *out_addr ;

  int sockfd ;
  int c_sockfd;  /* This is questionable if I actually need this,
			  I would love it if someone would tighten my code */
  int  port  ;

 public:

  ulTCPConnection () ; 
  ~ulTCPConnection () ;

  void disconnect  () ;
  int startServer  (int _port = UL_TCP_DEFAULT_PORT_NUMBER ) ;  

  int connect      ( char *hostname = "localhost",
                     int   _port    = UL_TCP_DEFAULT_PORT_NUMBER ) ;

  int sendMessage  ( char *mesg, int length ) ;
  int recvMessage  ( char *mesg, int length ) ;
} ;


/*
  Windoze/BEOS code based on contribution from Sean L. Palmer 
*/


#if defined (WIN32)

class ulDynamicLibrary
{
  HMODULE handle ;

public:

  ulDynamicLibrary ( const char *libname )
  {
    char dllname[1024];
    strcpy ( dllname, libname ) ;
    strcat ( dllname, ".dll"  ) ;
    handle = (HMODULE) LoadLibrary ( dllname ) ;
  }

  void *getFuncAddress ( const char *funcname )
  {
    return (void *) GetProcAddress ( handle, funcname ) ;
  }

  ~ulDynamicLibrary ()
  {
    if ( handle != NULL )
      FreeLibrary ( handle ) ;
  }
} ;

#elif defined (macintosh)

class ulDynamicLibrary
{
    CFragConnectionID connection;
    OSStatus          error;

public:
        
    ulDynamicLibrary ( const char *libname )
    {
        Str63    pstr;
        int        sz;
        
        sz = strlen (libname);
     
        if (sz < 64) {
        
            pstr[0] = sz;
            memcpy (pstr+1, libname, sz);
            
            error = GetSharedLibrary (pstr, kPowerPCCFragArch, kReferenceCFrag,
                                      &connection, NULL, NULL);                              
        }
        else 
            error = 1;
    }

    ~ulDynamicLibrary ()
    {
        if ( ! error )
            CloseConnection (&connection);
    
    }
        
    void* getFuncAddress (const char *funcname)
    {
        if ( ! error ) {
        
            char*  addr;
            Str255 sym;
            int    sz;
            
            sz = strlen (funcname);
            if (sz < 256) {
                
                sym[0] = sz;
                memcpy (sym+1, funcname, sz);

                error = FindSymbol (connection, sym, &addr, 0);
                if ( ! error )
                    return addr;
            }
        }
        
        return NULL;
    }
};

#elif defined (__BEOS__)

class ulDynamicLibrary
{
  image_id *handle ;

public:

  ulDynamicLibrary ( const char *libname )
  {
    char addonname[1024] ;
    strcpy ( addonname, libname ) ;
    strcat ( addonname, ".so" ) ;
    handle = new image_id ;

    *handle = load_add_on ( addonname ) ;

    if ( *handle == B_ERROR )
    {
      delete handle ;
      handle = NULL ;
    }
  }

  void *getFuncAddress ( const char *funcname )
  {
    void *sym = NULL ;

    if ( handle &&
         get_image_symbol ( handle, "funcname",
                            B_SYMBOL_TYPE_TEXT, &sym ) == B_NO_ERROR )
      return sym ;

    return NULL ;
  }

  ~ulDynamicLibrary ()
  {
    if ( handle != NULL )
      unload_add_on ( handle ) ;

    delete handle ;
  }
} ;

#  else

/*
  Linux/UNIX
*/

class ulDynamicLibrary
{
  void *handle ;

public:

  ulDynamicLibrary ( const char *libname )
  {
    char dsoname [ 1024 ] ;
    strcpy ( dsoname, libname ) ;
    strcat ( dsoname, ".so"  ) ;
    handle = (void *) dlopen ( dsoname, RTLD_NOW | RTLD_GLOBAL ) ;

    if ( handle == NULL )
      ulSetError ( UL_WARNING, "ulDynamicLibrary: %s\n", dlerror() ) ;
  }

  void *getFuncAddress ( const char *funcname )
  {
    return (handle==NULL) ? NULL : dlsym ( handle, funcname ) ;
  }

  ~ulDynamicLibrary ()
  {
    if ( handle != NULL )
      dlclose ( handle ) ;
  }
} ;

#endif /* if defined(WIN32) */


#endif


