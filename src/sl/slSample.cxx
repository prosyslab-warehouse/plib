
#include "sl.h"
#include <math.h>

void slSample::autoMatch ( slDSP *dsp )
{
  if ( dsp == NULL  || dsp->notWorking () ) return ;

  changeRate   ( dsp->getRate   () ) ;
  changeBps    ( dsp->getBps    () ) ;
  changeStereo ( dsp->getStereo () ) ;
}

void slSample::adjustVolume ( float vol )
{
  for ( int i = 0 ; i < length ; i++ )
  {
    int s = (int)(((float) buffer[i] - (float) 0x80) * vol) + 0x80 ;

    buffer [ i ] = ( s > 255 ) ? 255 :
                     ( s < 0 ) ? 0 : s ;
  }
}


void slSample::changeRate   ( int r )
{
  if ( r == rate ) return ;

  int    length1 = length / (getBps ()/8) ;
  int    length2 = (int) ( (float) length1 * ( (float) r / (float) rate ) ) ;
  Uchar *buffer2 = new Uchar [ length2 ] ;

  float step = (float) length1 / (float) length2 ;

  for ( int i = 0 ; i < length2 / (getBps()/8); i++ )
  {
    float pos = (float) i * step ;

    int p1 = (int) floor ( pos ) ;
    int p2 = (int) ceil  ( pos ) ;

    if ( stereo )
    {
      if ( ( p1 & 1 ) != ( i & 1 ) ) { pos++ ; p1++ ; p2++ ; }
      p2++ ;
    }

    float ratio = pos - (float) p1 ;

    float b1 = (getBps()==8) ?
        (float)           buffer [(p1<0)?0:(p1>=length1)?length1-1:p1] :
        (float) ((Ushort*)buffer)[(p1<0)?0:(p1>=length1)?length1-1:p1] ;
    float b2 = (getBps()==8) ?
        (float)           buffer [(p2<0)?0:(p2>=length1)?length1-1:p2] :
        (float) ((Ushort*)buffer)[(p2<0)?0:(p2>=length1)?length1-1:p2] ;

    float res = b1 * (1.0f-ratio) + b2 * ratio ;

    if ( getBps () == 8 )
      buffer2 [ i ] = (Uchar) ( (res < 0) ? 0 : (res > 255) ? 255 : res ) ;
    else
      ((Ushort *) buffer2 ) [ i ] = 
                (Ushort) ( (res < 0) ? 0 : (res > 65535) ? 65535 : res ) ;
  }

  rate   = r ;
  length = length2 ;
  delete buffer ;
  buffer = buffer2 ;
}


void slSample::changeToUnsigned ()
{
  if ( getBps() == 16 )
  {
    int length2 = length / 2 ;
    Ushort *buffer2 = (Ushort *) buffer ;

    for ( int i = 0 ; i < length2 ; i++ )
      buffer2 [ i ] = buffer2 [ i ] + 32768 ;
  }
  else
  {
    for ( int i = 0 ; i < length ; i++ )
      buffer [ i ] = (buffer [ i ]>0x80) ? (buffer[i]-0x80) :
                                           (0xFF-buffer[i]) ;
  }
}



void slSample::changeBps    ( int b )
{
  if ( b == getBps () ) return ;

  if ( b == 8 && getBps() == 16 )
  {
    length /= 2 ;
    Uchar *buffer2 = new Uchar [ length ] ;

    for ( int i = 0 ; i < length ; i++ )
      buffer2 [ i ] = ((Ushort *)buffer) [ i ] >> 8 ;

    delete buffer ;
    buffer = buffer2 ;
    setBps ( b ) ;
  }
  else
  if ( b == 16 && getBps() == 8 )
  {
    Ushort *buffer2 = new Ushort [ length ] ;

    for ( int i = 0 ; i < length ; i++ )
      buffer2 [ i ] = buffer [ i ] << 8 ;

    delete buffer ;
    buffer = (Uchar *) buffer2 ;
    length *= 2 ;
    setBps ( b ) ;
  }
}

void slSample::changeStereo ( int s )
{
  if ( s == getStereo () )
    return ;

  if ( s && ! getStereo () )
  {
    if ( getBps () == 8 )
    {
       Uchar *buffer2 = new Uchar [ length * 2 ] ;

       for ( int i = 0 ; i < length ; i++ )
         buffer2 [ i*2 ] = buffer2 [ i*2+1 ] = buffer [ i ] ;

       delete buffer ;
       buffer = buffer2 ;
       length *= 2 ;
       setStereo ( SL_TRUE ) ;
    }
    else
    {
       Ushort *buffer2 = new Ushort [ length ] ;

       for ( int i = 0 ; i < length / 2 ; i++ )
         buffer2 [ i*2 ] = buffer2 [ i*2+1 ] = ((Ushort *) buffer) [ i ] ;

       delete buffer ;
       buffer = (Uchar *)buffer2 ;
       length *= 2 ;
       setStereo ( SL_TRUE ) ;
    }
  }
  else
  {
    if ( getBps () == 8 )
    {
       Uchar *buffer2 = new Uchar [ length / 2 ] ;

       for ( int i = 0 ; i < (length-1)/2 ; i++ )
         buffer2 [ i ] = ((int)buffer [ i*2 ] + (int)buffer [ i*2 + 1 ] ) / 2 ;

       delete buffer ;
       buffer = buffer2 ;
       length /= 2 ;
       setStereo ( SL_FALSE ) ;
    }
    else
    {
       Ushort *buffer2 = new Ushort [ length / 4 ] ;

       for ( int i = 0 ; i < (length-3) / 4 ; i++ )
         buffer2 [ i ] = ((int)((Ushort *)buffer) [ i*2 ] +
                          (int)((Ushort *)buffer) [ i*2 + 1 ] ) / 2 ;

       delete buffer ;
       buffer = (Uchar *)buffer2 ;
       length /= 4 ;
       setStereo ( SL_FALSE ) ;
    }
  }
}


static void swap_Ushort ( Ushort *i )
{
  *i = ((*i << 8) & 0xFF00) +
       ((*i >> 8) & 0x00FF) ;
}

static void swap_int ( int *i )
{
  *i = ((*i << 24) & 0xFF000000) +
       ((*i <<  8) & 0x00FF0000) +
       ((*i >>  8) & 0x0000FF00) +
       ((*i >> 24) & 0x000000FF) ;
}

/*
  I'm sick of half the machines on the planet supporting
  strcasecmp and the other half stricmp - so here is my own
  offering:
*/

static int _slStrEqual ( char *s1, char *s2 )
{
  int l1 = (s1==NULL)? 0 : strlen ( s1 ) ;
  int l2 = (s2==NULL)? 0 : strlen ( s2 ) ;

  if ( l1 != l2 ) return SL_FALSE ;

  for ( int i = 0 ; i < l1 ; i++ )
  {
    char c1 = s1[i] ;
    char c2 = s2[i] ;

    if ( c1 == c2 )
     continue ;

    if ( c1 >= 'a' && c1 <= 'z' )
      c1 = c1 - ('a'-'A') ;

    if ( c2 >= 'a' && c2 <= 'z' )
      c2 = c2 - ('a'-'A') ;

    if ( c1 != c2 )
     return SL_FALSE ;
  }

  return SL_TRUE ;
}

int slSample::loadFile ( char *fname )
{
  if ( _slStrEqual ( & fname [ strlen ( fname ) - 4 ], ".wav" ) )
    return loadWavFile ( fname ) ;

  if ( _slStrEqual ( & fname [ strlen ( fname ) - 3 ], ".au" ) )
    return loadAUFile ( fname ) ;

  if ( _slStrEqual ( & fname [ strlen ( fname ) - 3 ], ".ub" ) )
    return loadRawFile ( fname ) ;

  fprintf ( stderr, "slSample:loadFile: Unknown file type for '%s'.\n",
                                    fname ) ;
  return SL_FALSE ;
}


int slSample::loadWavFile ( char *fname )
{
  int found_header   = SL_FALSE ;
  int needs_swabbing = SL_FALSE ;

  delete buffer ;
  buffer = NULL ;
  length = 0 ;

  FILE *fd = fopen ( fname, "rb" ) ;

  if ( fd == NULL )
  {
    fprintf ( stderr,
	     "slSample: loadWavFile: Cannot open '%s' for reading.\n",
	     fname ) ;
    return SL_FALSE ;
  }

  char magic [ 8 ] ;

  if ( fread ( magic, 4, 1, fd ) == 0 ||
       magic[0] != 'R' || magic[1] != 'I' ||
       magic[2] != 'F' || magic[3] != 'F' )
  {
    fprintf ( stderr, "slWavSample: File '%s' has wrong magic number\n", fname ) ;
    fprintf ( stderr, "            - it probably isn't in '.wav' format.\n" ) ;
    fclose ( fd ) ;
    return SL_FALSE ;
  }

  int leng1 ;

  if ( fread ( & leng1, sizeof(int), 1, fd ) == 0 )
  {
    fprintf ( stderr, "slSample: File '%s' has premature EOF in header\n", fname ) ;
    fclose ( fd ) ;
    return SL_FALSE ;
  }

  fread ( magic, 4, 1, fd ) ;

  if ( magic[0] != 'W' || magic[1] != 'A' ||
       magic[2] != 'V' || magic[3] != 'E' )
  {
    fprintf ( stderr, "slSample: File '%s' has no WAVE tag.\n", fname ) ;
    fclose ( fd ) ;
    return SL_FALSE ;
  }

  while ( ! feof ( fd ) )
  {
    fread ( magic, 4, 1, fd ) ;

    if ( magic[0] == 'f' && magic[1] == 'm' &&
	 magic[2] == 't' && magic[3] == ' ' )
    {
      found_header = SL_TRUE ;

      if ( fread ( & leng1, sizeof(int), 1, fd ) == 0 )
      {
	fprintf ( stderr, "slSample: File '%s' has premature EOF in header\n", fname ) ;
	fclose ( fd ) ;
	return SL_FALSE ;
      }

      if ( leng1 > 65536 )
      {
	needs_swabbing = SL_TRUE ;
	swap_int ( & leng1 ) ;
      }

      Ushort header [ 8 ] ;

      if ( leng1 != sizeof ( header ) )
	fprintf ( stderr,
               "slSample: File '%s' has unexpectedly long (%d byte) header\n",
               fname, leng1 ) ;

      fread ( & header, sizeof(header), 1, fd ) ;

      for ( int junk = sizeof(header) ; junk < leng1 ; junk++ )
        fgetc ( fd ) ;

      if ( needs_swabbing )
      {
	swap_Ushort ( & header[0] ) ;
	swap_Ushort ( & header[1] ) ;
	swap_int    ( (int *) & header[2] ) ;
	swap_int    ( (int *) & header[4] ) ;
	swap_Ushort ( & header[6] ) ;
	swap_Ushort ( & header[7] ) ;
      }

      if ( header [ 0 ] != 0x0001 )
      {
	fprintf ( stderr, "slSample: File '%s' is not WAVE_FORMAT_PCM!\n", fname ) ;
	fclose ( fd ) ;
	return SL_FALSE ;
      }

      setStereo ( header[1] > 1 ) ;
      setRate   ( *((int *) (& header[2])) ) ;
      setBps    ( header[7] ) ;
    }
    else
    if ( magic[0] == 'd' && magic[1] == 'a' &&
	 magic[2] == 't' && magic[3] == 'a' )
    {
      if ( ! found_header )
      {
	fprintf ( stderr, "slSample: File '%s' has no data section\n", fname ) ;
	fclose ( fd ) ;
	return SL_FALSE ;
      }

      if ( fread ( & length, sizeof(int), 1, fd ) == 0 )
      {
	fprintf ( stderr, "slSample: File '%s' has premature EOF in data\n", fname ) ;
	fclose ( fd ) ;
	return SL_FALSE ;
      }

      if ( needs_swabbing )
	swap_int ( & length ) ;

      buffer = new Uchar [ length ] ;

      fread ( buffer, 1, length, fd ) ;

      if ( getBps () == 16 )
      {
        Ushort *b = (Ushort*) buffer ;

        for ( int i = 0 ; i < length/2 ; i++ )
          b [ i ] = (Ushort) ( (int)((short) b [ i ]) + 32768 ) ;

        changeToUnsigned () ;
      }

      fclose ( fd ) ;
      return SL_TRUE ;
    }
  }

  fclose ( fd ) ;
  return SL_FALSE ;
}

int slSample::loadAUFile ( char *fname )
{
  delete buffer ;
  buffer = NULL ;
  length = 0 ;

  FILE *fd = fopen ( fname, "rb" ) ;

  if ( fd == NULL )
  {
    fprintf ( stderr,
	     "slSample: loadAUFile: Cannot open '%s' for reading.\n",
	     fname ) ;
    return SL_FALSE ;
  }

  char magic [ 4 ] ;

  if ( fread ( magic, 4, 1, fd ) == 0 ||
       magic[0] != '.' || magic[1] != 's' ||
       magic[2] != 'n' || magic[3] != 'd' )
  {
    fprintf ( stderr, "slSample: File '%s' has wrong magic number\n", fname ) ;
    fprintf ( stderr, "            - it probably isn't in '.au' format.\n" ) ;
    fclose ( fd ) ;
    return SL_FALSE ;
  }

  int hdr_length ;
  int dat_length ;
  int nbytes ;
  int irate  ;
  int nchans ;

  if ( fread ( & hdr_length, sizeof(int), 1, fd ) == 0 ||
       fread ( & dat_length, sizeof(int), 1, fd ) == 0 ||
       fread ( & nbytes    , sizeof(int), 1, fd ) == 0 ||
       fread ( & irate     , sizeof(int), 1, fd ) == 0 ||
       fread ( & nchans    , sizeof(int), 1, fd ) == 0 )
  {
    fprintf ( stderr, "slSample: File '%s' has premature EOF in header\n", fname ) ;
    fclose ( fd ) ;
    return SL_FALSE ;
  }

  if ( hdr_length > 65536 )
  {
    swap_int ( & hdr_length ) ;
    swap_int ( & dat_length ) ;
    swap_int ( & nbytes ) ;
    swap_int ( & irate  ) ;
    swap_int ( & nchans ) ;
  }

  bps    = nbytes * 8 ;
  stereo = (nchans>1) ;
  rate   = irate      ;

  if ( nbytes > 2 || nbytes <= 0 || hdr_length > 512 || hdr_length < 24 ||
       irate > 65526 || irate <= 1000 || nchans < 1 || nchans > 2 )
  {
    fprintf ( stderr, "slSample: File '%s' has a very strange header\n", fname ) ;

    fprintf ( stderr, "  Header Length = %d\n", hdr_length ) ;
    fprintf ( stderr, "  Data   Length = %d\n", dat_length ) ;
    fprintf ( stderr, "  Bytes/sample  = %d\n", nbytes     ) ;
    fprintf ( stderr, "  Sampling Rate = %dHz\n",irate     ) ;
    fprintf ( stderr, "  Num Channels  = %d\n", nchans     ) ;
    fprintf ( stderr, "\n" ) ;
    fclose ( fd ) ;
    return SL_FALSE ;
  }

  if ( hdr_length > 24 )
  {
    delete comment ;
    comment = new char [ hdr_length - 24 + 1 ] ;

    fread ( comment, 1, hdr_length - 24, fd ) ;
  }

  if ( dat_length > 0 )
  {
    buffer = new Uchar [ dat_length ] ;
    length = fread ( buffer, 1, dat_length, fd ) ;

    if ( length != dat_length )
      fprintf ( stderr, "slAUSample: File '%s' has premature EOF in data.\n", fname ) ;
    changeToUnsigned () ;
  }

  fclose ( fd ) ;
  return SL_TRUE ;
}


int slSample::loadRawFile ( char *fname )
{
  delete buffer ;
  buffer = NULL ;
  length = 0 ;

  FILE *fd = fopen ( fname, "rb" ) ;

  if ( fd == NULL )
  {
    fprintf ( stderr,
              "slSample: loadRawFile: Cannot open '%s' for reading.\n",
             fname ) ;
    return SL_FALSE ;
  }

  struct stat stat_buf ;

  if ( fstat ( fileno ( fd ), & stat_buf ) != 0 )
  {
    fprintf ( stderr,
             "slSample: loadRawFile: Cannot get status for '%s'.\n",
             fname ) ;        
    fclose ( fd ) ;
    return SL_FALSE ;
  }

  length = stat_buf . st_size ;

  if ( length > 0 )
  {
    buffer = new Uchar [ length ] ;
    length = fread ( buffer, 1, length, fd ) ;
  }

  bps    = 8     ;
  stereo = SL_FALSE ;
  rate   = 8000  ;  /* Guess */

  fclose ( fd ) ;
  return SL_TRUE ;
}


