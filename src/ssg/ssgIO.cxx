
#include "ssgLocal.h" 

static int  read_error = FALSE ;
static int write_error = FALSE ;

int _ssgReadError  (void) { return  read_error ; }
int _ssgWriteError (void) { return write_error ; }

void _ssgReadFloat ( FILE *fd, float *var )
{
  if ( fread ( var, sizeof(float), 1, fd ) == 1 ) return ;
  read_error = TRUE ;
}


void _ssgWriteFloat ( FILE *fd, const float var )
{
  if ( fwrite ( & var, sizeof(float), 1, fd ) == 1 ) return ;
  write_error = TRUE ;
}


void _ssgReadUInt ( FILE *fd, unsigned int *var )
{
  if ( fread ( var, sizeof(unsigned int), 1, fd ) == 1 ) return ;
  read_error = TRUE ;
}


void _ssgWriteUInt ( FILE *fd, const unsigned int var )
{
  if ( fwrite ( & var, sizeof(unsigned int), 1, fd ) == 1 ) return ;
  write_error = TRUE ;
}


void _ssgReadInt ( FILE *fd, int *var )
{
  if ( fread ( var, sizeof(int), 1, fd ) == 1 ) return ;
  read_error = TRUE ;
}


void _ssgWriteInt ( FILE *fd, const int var )
{
  if ( fwrite ( & var, sizeof(int), 1, fd ) == 1 ) return ;
  write_error = TRUE ;
}


void _ssgReadUShort ( FILE *fd, unsigned short *var )
{
  if ( fread ( var, sizeof(unsigned short), 1, fd ) == 1 ) return ;
  read_error = TRUE ;
}


void _ssgWriteUShort ( FILE *fd, const unsigned short var )
{
  if ( fwrite ( & var, sizeof(unsigned short), 1, fd ) == 1 ) return ;
  write_error = TRUE ;
}


void _ssgReadShort ( FILE *fd, short *var )
{
  if ( fread ( var, sizeof(short), 1, fd ) == 1 ) return ;
  read_error = TRUE ;
}


void _ssgWriteShort ( FILE *fd, const short var )
{
  if ( fwrite ( & var, sizeof(short), 1, fd ) == 1 ) return ;
  write_error = TRUE ;
}


void _ssgReadFloat ( FILE *fd, const unsigned int n, float *var )
{
  if ( fread ( var, sizeof(float), n, fd ) == n ) return ;
  read_error = TRUE ;
}


void _ssgWriteFloat ( FILE *fd, const unsigned int n, const float *var )
{
  if ( fwrite ( var, sizeof(float), n, fd ) == n ) return ;
  write_error = TRUE ;
}


void _ssgReadUShort ( FILE *fd, const unsigned int n, unsigned short *var )
{
  if ( fread ( var, sizeof(unsigned short), n, fd ) == n ) return ;
  read_error = TRUE ;
}


void _ssgWriteUShort ( FILE *fd, const unsigned int n, const unsigned short *var )
{
  if ( fwrite ( var, sizeof(unsigned short), n, fd ) == n ) return ;
  write_error = TRUE ;
}



void _ssgReadShort ( FILE *fd, const unsigned int n, short *var )
{
  if ( fread ( var, sizeof(short), n, fd ) == n ) return ;
  read_error = TRUE ;
}


void _ssgWriteShort ( FILE *fd, const unsigned int n, const short *var )
{
  if ( fwrite ( var, sizeof(short), n, fd ) == n ) return ;
  write_error = TRUE ;
}


void _ssgReadUInt ( FILE *fd, const unsigned int n, unsigned int *var )
{
  if ( fread ( var, sizeof(unsigned int), n, fd ) == n ) return ;
  read_error = TRUE ;
}


void _ssgWriteUInt ( FILE *fd, const unsigned int n, const unsigned int *var )
{
  if ( fwrite ( var, sizeof(unsigned int), n, fd ) == n ) return ;
  write_error = TRUE ;
}



void _ssgReadInt ( FILE *fd, const unsigned int n, int *var )
{
  if ( fread ( var, sizeof(int), n, fd ) == n ) return ;
  read_error = TRUE ;
}


void _ssgWriteInt ( FILE *fd, const unsigned int n, const int *var )
{
  if ( fwrite ( var, sizeof(int), n, fd ) == n ) return ;
  write_error = TRUE ;
}



#define MAX_ENTITY_NAME_LENGTH 1024

void _ssgReadString ( FILE *fd, char **var )
{
  int i ;
  char s [ MAX_ENTITY_NAME_LENGTH ] ;

  for ( i = 0 ; i < MAX_ENTITY_NAME_LENGTH ; i++ )
  {
    int c = fgetc ( fd ) ;
    s [ i ] = c ;

    if ( c == '\0' )
      break ;
  }

  if ( i >= MAX_ENTITY_NAME_LENGTH-1 )
    s [ MAX_ENTITY_NAME_LENGTH-1 ] = '\0' ;


  if ( s[0] == '\0' )
    *var = NULL ;
  else
  {
    *var = new char [ strlen(s)+1 ] ;
    strcpy ( *var, s ) ;
  }
}


void _ssgWriteString ( FILE *fd, const char *var )
{
  if ( var != NULL )
    fputs ( var, fd ) ;

  fputc ( '\0', fd ) ;
}


void _ssgReadVec2  ( FILE *fd, sgVec2 var ) { _ssgReadFloat  ( fd, 2, var ) ; }
void _ssgWriteVec2 ( FILE *fd, const sgVec2 var ) { _ssgWriteFloat ( fd, 2, var ) ; }

void _ssgReadVec3  ( FILE *fd, sgVec3 var ) { _ssgReadFloat  ( fd, 3, var ) ; }
void _ssgWriteVec3 ( FILE *fd, const sgVec3 var ) { _ssgWriteFloat ( fd, 3, var ) ; }

void _ssgReadVec4  ( FILE *fd, sgVec4 var ) { _ssgReadFloat  ( fd, 4, var ) ; }
void _ssgWriteVec4 ( FILE *fd, const sgVec4 var ) { _ssgWriteFloat ( fd, 4, var ) ; }

void _ssgReadMat4  ( FILE *fd, sgMat4 var ) { _ssgReadFloat  ( fd, 16, (float *)var ) ; }
void _ssgWriteMat4 ( FILE *fd, const sgMat4 var ) { _ssgWriteFloat ( fd, 16, (float *)var ) ; }

