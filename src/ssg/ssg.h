
#ifndef _INCLUDED_SSG_H_
#define _INCLUDED_SSG_H_

#include "ul.h"
#include "sg.h"

/*
  Configuration
*/

#define _SSG_USE_PICK   1
#define _SSG_USE_DLIST  1

/*
  For optional use of PNG textures, download the glpng library from
  http://www.wyatt100.freeserve.co.uk/download.htm and un-comment
  the following line.
 */

//#define _SSG_USE_GLPNG  1

#ifndef _SSG_PUBLIC
#define _SSG_PUBLIC  protected
#endif

extern int sgebug ;

enum ssgCullResult
{
  SSG_OUTSIDE  = SG_OUTSIDE,
  SSG_INSIDE   = SG_INSIDE,
  SSG_STRADDLE = SG_STRADDLE
} ;

#define SSG_MAXPATH   50

#define SSGTRAV_CULL   1
#define SSGTRAV_ISECT  2
#define SSGTRAV_HOT    4

class ssgList          ;
class ssgKidList       ;
class ssgBase          ;
class ssgEntity        ;
class ssgLeaf          ;
class ssgVTable        ;
class ssgVtxTable      ;
class ssgVtxArray      ;
class ssgBranch        ;
class ssgInvisible     ;
class ssgBaseTransform ;
class ssgTransform     ;
class ssgTexTrans      ;
class ssgCutout        ;
class ssgSelector      ;
class ssgRangeSelector ;
class ssgTimedSelector ;
class ssgRoot          ;

void  ssgDeRefDelete ( ssgBase *br ) ;

#define SSG_BACKWARDS_REFERENCE 0x0000000  /* For SSG format files */

#define SSG_TYPE_BASE          0x00000001

/* ssgEntities */
#define SSG_TYPE_ENTITY        0x00000002
#define SSG_TYPE_LEAF          0x00000004
#define SSG_TYPE_VTABLE        0x00000008
#define SSG_TYPE_BRANCH        0x00000010
#define SSG_TYPE_BASETRANSFORM 0x00000020
#define SSG_TYPE_TRANSFORM     0x00000040
#define SSG_TYPE_TEXTRANS      0x00000080
#define SSG_TYPE_SELECTOR      0x00000100
#define SSG_TYPE_TIMEDSELECTOR 0x00000200
#define SSG_TYPE_ROOT          0x00000400
#define SSG_TYPE_CUTOUT        0x00000800
#define SSG_TYPE_RANGESELECTOR 0x00001000
#define SSG_TYPE_INVISIBLE     0x00002000
#define SSG_TYPE_VTXTABLE      0x00004000
#define SSG_TYPE_VTXARRAY      0x00008000

/* ssgStates */
#define SSG_TYPE_STATE         0x00000004
#define SSG_TYPE_SIMPLESTATE   0x00000008
#define SSG_TYPE_STATESELECTOR 0x00000010

#define SSG_FILE_VERSION       0x00
#define SSG_FILE_MAGIC_NUMBER  (('S'<<24)+('S'<<16)+('G'<<8)+SSG_FILE_VERSION)

inline int ssgTypeBase         () { return SSG_TYPE_BASE ; }
inline int ssgTypeEntity       () { return SSG_TYPE_ENTITY    | ssgTypeBase    () ; }


inline int ssgTypeLeaf         () { return SSG_TYPE_LEAF      | ssgTypeEntity  () ; }
inline int ssgTypeVTable       () { return SSG_TYPE_VTABLE    | ssgTypeLeaf    () ; }
inline int ssgTypeVtxTable     () { return SSG_TYPE_VTXTABLE  | ssgTypeLeaf    () ; }
inline int ssgTypeVtxArray     () { return SSG_TYPE_VTXARRAY  | ssgTypeVtxTable() ; }
inline int ssgTypeBranch       () { return SSG_TYPE_BRANCH    | ssgTypeEntity  () ; }
inline int ssgTypeBaseTransform() { return SSG_TYPE_BASETRANSFORM | ssgTypeBranch  () ; }
inline int ssgTypeTransform    () { return SSG_TYPE_TRANSFORM | ssgTypeBaseTransform () ; }
inline int ssgTypeTexTrans     () { return SSG_TYPE_TEXTRANS  | ssgTypeBaseTransform () ; }
inline int ssgTypeSelector     () { return SSG_TYPE_SELECTOR  | ssgTypeBranch  () ; }
inline int ssgTypeRangeSelector() { return SSG_TYPE_RANGESELECTOR | ssgTypeSelector () ; }
inline int ssgTypeTimedSelector() { return SSG_TYPE_TIMEDSELECTOR | ssgTypeSelector () ; }
inline int ssgTypeRoot         () { return SSG_TYPE_ROOT      | ssgTypeBranch  () ; }
inline int ssgTypeCutout       () { return SSG_TYPE_CUTOUT    | ssgTypeBranch  () ; }
inline int ssgTypeInvisible    () { return SSG_TYPE_INVISIBLE | ssgTypeBranch  () ; }

inline int ssgTypeState        () { return SSG_TYPE_STATE     | ssgTypeBase  () ; }
inline int ssgTypeSimpleState  () { return SSG_TYPE_SIMPLESTATE | ssgTypeState () ; }
inline int ssgTypeStateSelector() { return SSG_TYPE_STATESELECTOR | ssgTypeSimpleState () ; }

/*
  It's critical that these numbers don't change without
  some pretty pressing need because significant change to
  ssgSimpleState.cxx and ssgStateTables.cxx would be needed.
*/

#define SSG_GL_TEXTURE_EN        0
#define SSG_GL_CULL_FACE_EN      1
#define SSG_GL_COLOR_MATERIAL_EN 2
#define SSG_GL_BLEND_EN          3
#define SSG_GL_ALPHA_TEST_EN     4
#define SSG_GL_LIGHTING_EN       5
 
#define SSG_GL_TEXTURE           6
#define SSG_GL_COLOR_MATERIAL    7
#define SSG_GL_DIFFUSE           8
#define SSG_GL_AMBIENT           9
#define SSG_GL_SPECULAR         10
#define SSG_GL_EMISSION         11
#define SSG_GL_SHININESS        12
#define SSG_GL_ALPHA_TEST       13
#define SSG_GL_SHADE_MODEL      14

#define SSG_CLONE_RECURSIVE        1
#define SSG_CLONE_GEOMETRY         2
#define SSG_CLONE_USERDATA         4
#define SSG_CLONE_STATE            8
#define SSG_CLONE_STATE_RECURSIVE 16
#define SSG_CLONE_TEXTURE         32

int ssgGetFrameCounter () ;
void ssgSetFrameCounter ( int fc ) ;

class ssgList
{
protected:
  unsigned int total ;  /* The total number of entities in the list */
  unsigned int limit ;  /* The current limit on number of entities  */
  unsigned int next  ;  /* The next entity when we are doing getNext ops */

  ssgEntity **entity_list ;  /* The list. */

  void sizeChk (void) ;

public:

  ssgList ( int init_max = 1 ) ;
  virtual ~ssgList (void) ;

  ssgEntity *getEntity ( unsigned int n )
  {
    next = n ;
    return ( n >= total ) ? (ssgEntity *) NULL : entity_list [ n ] ;
  }

  virtual void addEntity ( ssgEntity *entity ) ;
  virtual void removeEntity ( unsigned int n ) ;

  void removeAllEntities () ;

  void removeEntity ( ssgEntity *entity )
  {
    removeEntity ( searchForEntity ( entity ) ) ;
  }

  int        getNumEntities    (void) { return total ; }
  ssgEntity *getNextEntity     (void) { return getEntity ( next+1 ) ; }
  int        searchForEntity   ( ssgEntity *entity ) ;
} ;


class ssgKidList : public ssgList
{
public:

  ssgKidList ( int init_max = 1 ) ;
  virtual ~ssgKidList (void) ;

  void addEntity ( ssgEntity *entity ) ;
  void removeEntity ( unsigned int n ) ;

  void removeEntity ( ssgEntity *entity )
  {
    removeEntity ( searchForEntity ( entity ) ) ;
  }
} ;


class ssgBase 
{
  int   refc   ;  /* The number of references to this node */
  int   unique ;  /* A unique number for this node */

protected :

  int   type  ;
  int   spare ;  /* This spare field is used in a bunch of short-term hacks */

  char    *name ;
  ssgBase *user_data ;
  virtual void copy_from ( ssgBase *src, int clone_flags ) ;

_SSG_PUBLIC:

  void deadBeefCheck () { assert ( type != (int) 0xDeadBeef ) ; }

public:
  void *operator new  ( size_t size ) ;
  void  operator delete ( void *ptr ) ;

  virtual void zeroSpareRecursive ();
  virtual void zeroSpare ()         ;
  virtual void incSpare  ()         ;
  virtual void setSpare  ( int ss ) ;
  virtual int  getSpare  ()         ;

  int  getUnique () { return unique ; }

  ssgBase (void) ;
  virtual ~ssgBase (void) ;

  void ref   () { refc++ ; }
  void deRef () { assert ( refc > 0 ) ; refc-- ; }
  int  getRef() { return refc ; }

  virtual ssgBase *clone ( int clone_flags = 0 ) ;

  /* Type checking mechanism */

  virtual char *getTypeName(void) ;

  int   getType    (void)     { return type ; }
  int   isA        ( int ty ) { return getType() == ty ; }
  int   isAKindOf  ( int ty ) { return ( getType() & ty ) == ty ; }

  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;

  ssgBase *getUserData () { return user_data ; }

  void setUserData ( ssgBase *s )
  {
    ssgDeRefDelete ( user_data ) ;

    user_data = s ;
    if ( s != NULL )
      s -> ref () ;
  }

  void  setName ( const char *nm ) ;
  char *getName () { return name ; }
  const char *getPrintableName () { return (name == NULL) ? "NoName" : name ; }
} ;



class ssgSimpleList : public ssgBase
{
protected:

  unsigned int total   ;  /* The total number of things in the list */
  unsigned int limit   ;  /* The current limit on number of things  */
  unsigned int size_of ;  /* The size of each thing */
  char         *list   ;  /* The list. */

  void sizeChk (void)
  {
    if ( total >= limit )
    {
      limit += limit ;
      char *nlist = new char [ limit * size_of ] ;
      memmove ( nlist, list, total * size_of ) ;
      delete [] list ;
      list = nlist ;
    }
  }

  virtual void copy_from ( ssgSimpleList *src, int clone_flags ) ;

_SSG_PUBLIC:

  ssgSimpleList ()
  {
    limit = 0 ;
    size_of = 0 ;
    total = 0 ;
    list = NULL ;
  }

public:

  ssgSimpleList ( int sz, int init = 3 )
  {
    limit = init ;
    size_of = sz ;
    total = 0 ;
    list = new char [ limit * size_of ] ;
  }

  virtual ssgBase *clone ( int clone_flags = 0 ) ;

  virtual ~ssgSimpleList (void)
  {
    delete [] list ;
  } ;

  char *raw_get ( unsigned int n )
  {
    return ( n >= total ) ? ((char *) 0) : & list [ n * size_of ] ;
  }

  void raw_add ( char *thing )
  {
    sizeChk () ;
    memcpy ( & list [ size_of * total++ ], thing, size_of ) ;
  } ;

  void removeLast ()
  {
    if ( total > 0 )
      total-- ;
  }

  void removeAll ()
  {
    delete [] list ;
    list = NULL ;
    limit = total = 0 ;
  }

  int getSizeOf (void) { return size_of ; }
  int getNum (void) { return total ; }

  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
} ;


class ssgVertexArray : public ssgSimpleList
{
public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgVertexArray ( int init = 3 ) : ssgSimpleList ( sizeof(sgVec3), init ) {} 
  float *get ( unsigned int n ) { return (float *) raw_get ( n ) ; }
  void   add ( sgVec3   thing ) { raw_add ( (char *) thing ) ; } ;
  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
} ;


class ssgNormalArray : public ssgSimpleList
{
public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgNormalArray ( int init = 3 ) : ssgSimpleList ( sizeof(sgVec3), init ) {} 
  float *get ( unsigned int n ) { return (float *) raw_get ( n ) ; }
  void   add ( sgVec3   thing ) { raw_add ( (char *) thing ) ; } ;
  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
} ;


class ssgTexCoordArray : public ssgSimpleList
{
public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgTexCoordArray ( int init = 3 ) : ssgSimpleList ( sizeof(sgVec2), init ) {} 
  float *get ( unsigned int n ) { return (float *) raw_get ( n ) ; }
  void   add ( sgVec2   thing ) { raw_add ( (char *) thing ) ; } ;
  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
} ;


class ssgColourArray : public ssgSimpleList
{
public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgColourArray ( int init = 3 ) : ssgSimpleList ( sizeof(sgVec4), init ) {} 
  float *get ( unsigned int n ) { return (float *) raw_get ( n ) ; }
  void   add ( sgVec4   thing ) { raw_add ( (char *) thing ) ; } ;
  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
} ;


class ssgIndexArray : public ssgSimpleList
{
public:

  ssgIndexArray ( int init = 3 ) : ssgSimpleList ( sizeof(short), init ) {} 
  short *get ( unsigned int n ) { return (short *) raw_get ( n ) ; }
  void   add ( short    thing ) { raw_add ( (char *) &thing ) ; } ;
} ;


struct ssgInterleavedArrayElement
{
  sgVec2 texCoord ;
  sgVec4 colour ;
  sgVec3 normal ;
  sgVec3 vertex ;
} ;


class ssgInterleavedArray : public ssgSimpleList
{
public:

  ssgInterleavedArray ( int init = 3 ) : ssgSimpleList ( sizeof(ssgInterleavedArrayElement), init ) {} 
  ssgInterleavedArrayElement *get ( unsigned int n ) { return (ssgInterleavedArrayElement *) raw_get ( n ) ; }
  void add ( ssgInterleavedArrayElement  thing ) { raw_add ( (char *) &thing ) ; } ;
  void add ( ssgInterleavedArrayElement *thing ) { raw_add ( (char *)  thing ) ; } ;
} ;


void ssgLoadTexture ( const char *fname ) ;
int ssgGetNumTexelsLoaded () ;


class ssgTexture : public ssgBase
{
  char *filename ; // path + filename
	char *filename_from_model; // the filename like it is in the model
  GLuint handle ;

protected:

  virtual void copy_from ( ssgTexture *src, int clone_flags ) ;
  ssgTexture () ;
public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgTexture ( const char *fname, int wrapu = TRUE, int wrapv = TRUE,
	       int mipmap = TRUE )
  {
#ifdef GL_VERSION_1_1
    glGenTextures ( 1, & handle ) ;
    glBindTexture ( GL_TEXTURE_2D, handle ) ;
#else
    /* This is only useful on some ancient SGI hardware */
    glGenTexturesEXT ( 1, & handle ) ;
    glBindTextureEXT ( GL_TEXTURE_2D, handle ) ;
#endif

    filename = NULL ;
		filename_from_model = NULL;
    setFilename ( fname ) ;
		
    ssgLoadTexture( getFilename() ) ;

    glTexEnvi ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ) ;

    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) ;
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		      mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR ) ;
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapu ? GL_REPEAT : GL_CLAMP ) ;
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapv ? GL_REPEAT : GL_CLAMP ) ;
#ifdef GL_VERSION_1_1
    glBindTexture ( GL_TEXTURE_2D, 0 ) ;
#else
    glBindTextureEXT ( GL_TEXTURE_2D, 0 ) ;
#endif
  }

  GLuint getHandle () { return handle ; }

  char *getFilename(void) { return filename ; }
	char *getFilenameFromModel(void) { return filename_from_model ; }

  void  setFilename(const char *fname)
  {
    delete filename ;

    if ( fname == NULL )
      filename = NULL ;
    else
    {
      filename = new char [ strlen(fname)+1 ] ;
      strcpy ( filename, fname ) ;
    }
  }

	
  void  setFilenameFromModel(const char *fname)
  {
    delete filename_from_model ;

    if ( fname == NULL )
      filename_from_model = NULL ;
    else
    {
      filename_from_model = new char [ strlen(fname)+1 ] ;
      strcpy ( filename_from_model, fname ) ;
    }
  }

  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
  virtual char *getTypeName(void) ;
} ;


class ssgState : public ssgBase
{
  int translucent ;

  int external_property_index ;

protected:
  virtual void copy_from ( ssgState *src, int clone_flags ) ;

public:
  ssgState (void) ;
  virtual ~ssgState (void) ;

  virtual char *getTypeName(void) ;

  int  getExternalPropertyIndex (void) { return external_property_index ; }
  void setExternalPropertyIndex ( int i ) { external_property_index = i ; }

  virtual int  isTranslucent (void)  { return translucent ;  }
  virtual void setTranslucent (void) { translucent = TRUE  ; }
  virtual void setOpaque      (void) { translucent = FALSE ; }
  virtual void force (void) = 0 ;
  virtual void apply (void) = 0 ;

  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
} ;


class ssgSimpleState : public ssgState
{
_SSG_PUBLIC:

  int    dont_care ; 
  int    enables   ; 
  GLuint texture_handle ;
  char  *filename  ;
  int    wrapu ;
  int    wrapv ;
  int    mipmap;

  int colour_material_mode ;
  sgVec4 specular_colour ;
  sgVec4 emission_colour ;
  sgVec4  ambient_colour ;
  sgVec4  diffuse_colour ;

  GLenum shade_model ;

  float  shininess ;
  float  alpha_clamp ;

  ssgSimpleState ( int I_am_current_state ) ;

protected:
  virtual void copy_from ( ssgSimpleState *src, int clone_flags ) ;

public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgSimpleState (void) ;
  virtual ~ssgSimpleState (void) ;
  virtual char *getTypeName(void) ;

  virtual void force (void) ;
  virtual void apply (void) ;

  virtual void      care_about ( int mode ) { dont_care &= ~(1<<mode) ; }
  virtual void dont_care_about ( int mode ) { dont_care |=  (1<<mode) ; }
  virtual int     getCareAbout ( int mode ) { return dont_care & (1<<mode) ; }

  virtual int  isEnabled ( GLenum mode ) ;
  virtual void disable   ( GLenum mode ) ;
  virtual void enable    ( GLenum mode ) ;
  virtual void set       ( GLenum mode, int val )
                                 { val ? enable(mode) : disable(mode) ; }

  virtual char *getTextureFilename(void) { return filename ; }
  virtual void  setTextureFilename ( const char *fname ) ;
  virtual void  setTexture ( char *fname,
                             int _wrapu = TRUE,
                             int _wrapv = TRUE,
			     int _mipmap = TRUE ) ;

  virtual GLuint getTextureHandle (void)
  {
    return texture_handle ;
  }

  virtual void setTexture ( ssgTexture *tex )
  {
    /* Don't change the order of these two statements! */
    setTexture         ( tex -> getHandle   () ) ;
    setTextureFilename ( tex -> getFilename () ) ;
  }

  /*
    WARNING - THIS FORM OF setTexture IS DEPRECATED
    BECAUSE IT PREVENTS ssgSave FROM SAVING THE
    TEXTURE FILENAME!
  */

  virtual void setTexture ( GLuint tex )
  {
    texture_handle = tex ;
    care_about ( SSG_GL_TEXTURE ) ;
    setTextureFilename ( NULL ) ;
  }

  virtual void setColourMaterial ( GLenum which )
  {
    colour_material_mode = which ;
    care_about ( SSG_GL_COLOR_MATERIAL ) ;
  }

  virtual void setMaterial ( GLenum which, float r, float g,
                                           float b, float a = 1.0f )
  {
    sgVec4 rgba ;
    sgSetVec4 ( rgba, r, g, b, a ) ;
    setMaterial ( which, rgba ) ;
  }

  virtual void setMaterial   ( GLenum which, sgVec4 rgba )
  {
    switch ( which )
    {
      case GL_EMISSION : sgCopyVec4 ( emission_colour, rgba ) ;
                         care_about ( SSG_GL_EMISSION ) ;
                         break ;
      case GL_SPECULAR : sgCopyVec4 ( specular_colour, rgba ) ;
                         care_about ( SSG_GL_SPECULAR ) ;
                         break ;
      case GL_AMBIENT  : sgCopyVec4 ( ambient_colour , rgba ) ;
                         care_about ( SSG_GL_AMBIENT  ) ;
                         break ;
      case GL_DIFFUSE  : sgCopyVec4 ( diffuse_colour , rgba ) ;
                         care_about ( SSG_GL_DIFFUSE  ) ;
                         break ;
      default :          break ;
    }
  }

  virtual float *getMaterial ( GLenum which )
  {
    switch ( which )
    {
      case GL_EMISSION : return emission_colour ;
      case GL_SPECULAR : return specular_colour ;
      case GL_AMBIENT  : return ambient_colour  ;
      case GL_DIFFUSE  : return diffuse_colour  ;
      default: break ;
    }

    return NULL ;
  }

  virtual float getShininess (void)
  {
    return shininess ;
  }
 
  virtual void setShininess ( float sh )
  {
    care_about ( SSG_GL_SHININESS ) ;
    shininess = sh ;
  }
 
  virtual void setShadeModel ( GLenum model )
  {
    care_about ( SSG_GL_SHADE_MODEL ) ;
    shade_model = model ;
  }
 
  virtual void setAlphaClamp ( float clamp )
  {
    care_about ( SSG_GL_ALPHA_TEST ) ;
    alpha_clamp = clamp ;
  }
 
  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
} ;

class ssgStateSelector : public ssgSimpleState
{
  int              nstates   ;
  int              selection ;
_SSG_PUBLIC:
  ssgSimpleState **statelist ;

protected:
  virtual void copy_from ( ssgStateSelector *src, int clone_flags ) ;

public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgStateSelector () ;
  ssgStateSelector ( int ns ) ;

  virtual ~ssgStateSelector (void) ;
  virtual char *getTypeName(void) ;

  int             getNumSteps ( void ) { return nstates ; }
  void            selectStep ( unsigned int s ) ;
  unsigned int    getSelectStep   (void) ;
  ssgSimpleState *getCurrentStep  (void) ;
  void            setStep ( int i, ssgSimpleState *step ) ;
  ssgSimpleState *getStep ( int i ) ;

  void force (void) ;
  void apply (void) ;

  void      care_about ( int mode ) ;
  void dont_care_about ( int mode ) ;

  int    isEnabled ( GLenum mode ) ;
  void   disable   ( GLenum mode ) ;
  void   enable    ( GLenum mode ) ;

  char *getTextureFilename(void) ;
  void  setTextureFilename(char *fname) ;

  void   setTexture ( char *fname,
		      int _wrapu = TRUE,
		      int _wrapv = TRUE,
		      int _mipmap = TRUE ) ;
  GLuint getTextureHandle (void)   ; 
  void   setTexture ( ssgTexture *tex ) ; 

  /*
    WARNING - THIS FORM OF setTexture IS DEPRECATED
    BECAUSE IT PREVENTS ssgSave FROM SAVING THE
    TEXTURE FILENAME!
  */

  void   setTexture ( GLuint      tex ) ; 
  void   setColourMaterial(GLenum which); 
  void   setMaterial ( GLenum which, float r, float g,
                                   float b, float a = 1.0f ) ;

  void   setMaterial   ( GLenum which, sgVec4 rgba ) ; 
  float *getMaterial ( GLenum which ) ; 
  float  getShininess (void) ; 
  void   setShininess ( float sh ) ; 
  void   setShadeModel ( GLenum model ) ; 
  void   setAlphaClamp ( float clamp ) ; 
  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
} ;

struct ssgEntityBinding
{
  ssgEntity **entity     ;
  char       *nameOrPath ;
} ;


typedef int (*ssgCallback)( ssgEntity * ) ;
#define SSG_CALLBACK_PREDRAW   1
#define SSG_CALLBACK_POSTDRAW  2

typedef int (*ssgTravCallback)( ssgEntity *entity, int traversal_mask ) ;
#define SSG_CALLBACK_PRETRAV   1
#define SSG_CALLBACK_POSTTRAV  2

class ssgEntity : public ssgBase
{
  ssgList parents ;

  int traversal_mask ;
  ssgTravCallback  preTravCB ;
  ssgTravCallback postTravCB ;

protected:
  sgSphere bsphere ;
  int bsphere_is_invalid ;

  void emptyBSphere  ()              { bsphere.empty  ()    ; }
  void visualiseBSphere () ;
  void extendBSphere ( sgSphere *s ) { bsphere.extend ( s ) ; }
  void extendBSphere ( sgBox    *b ) { bsphere.extend ( b ) ; }
  void extendBSphere ( sgVec3    v ) { bsphere.extend ( v ) ; }

  virtual ssgCullResult cull_test  ( sgFrustum *f, sgMat4 m, int test_needed ) ;
  virtual ssgCullResult isect_test ( sgSphere  *s, sgMat4 m, int test_needed ) ;
  virtual ssgCullResult hot_test   ( sgVec3     s, sgMat4 m, int test_needed ) ;

  virtual void copy_from ( ssgEntity *src, int clone_flags ) ;
public:
 
  ssgEntity (void) ;
  virtual ~ssgEntity (void) ;
  
  int  getTraversalMask     ()        { return traversal_mask ;}
  void setTraversalMask     ( int t ) { traversal_mask  =  t ; }
  void setTraversalMaskBits ( int t ) { traversal_mask |=  t ; }
  void clrTraversalMaskBits ( int t ) { traversal_mask &= ~t ; }

  ssgTravCallback getTravCallback ( int cb_type )
  {
    return ( cb_type == SSG_CALLBACK_PRETRAV ) ? preTravCB : postTravCB ;
  }

  void setTravCallback ( int cb_type, ssgTravCallback cb )
  {
    if ( cb_type == SSG_CALLBACK_PRETRAV )
      preTravCB = cb ;
    else
      postTravCB = cb ;
  }

  int preTravTests ( int *test_needed, int which ) ;
  void postTravTests ( int which ) ;

  /* for backward compatibility */
  ssgCallback getCallback ( int cb_type ) ;
  void setCallback ( int cb_type, ssgCallback cb ) ;

  virtual ssgEntity* getByName  ( char *nm ) ;
  virtual ssgEntity* getByPath  ( char *path ) ;
  int  bindEntities ( ssgEntityBinding *bind ) ;

  virtual void recalcBSphere (void) = 0 ;
  int  isDirtyBSphere (void) { return bsphere_is_invalid ; }
  void dirtyBSphere  () ;

  sgSphere *getBSphere ()
  {
    if ( isDirtyBSphere () )
      recalcBSphere () ;

    return & bsphere ;
  }

  virtual int getNumKids (void) { return 0 ; }
  int getNumParents () { return parents.getNumEntities () ; }
  ssgBranch *getParent ( int p ) { return (ssgBranch *) parents.getEntity ( p ) ; }
  ssgBranch *getNextParent () { return (ssgBranch *) parents.getNextEntity () ; }
  void addParent    ( ssgEntity *entity ) { parents.addEntity    ( entity ) ; }
  void removeParent ( ssgEntity *entity ) { parents.removeEntity ( entity ) ; }

  virtual char *getTypeName(void) ;

  virtual void cull  ( sgFrustum *f, sgMat4 m, int test_needed ) = 0 ;
  virtual void isect ( sgSphere  *s, sgMat4 m, int test_needed ) = 0 ;
  virtual void hot   ( sgVec3     s, sgMat4 m, int test_needed ) = 0 ;
  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
} ;



class ssgLeaf : public ssgEntity
{
  int cull_face ;
  ssgState *state ;

protected:
  ssgCallback  preDrawCB ;
  ssgCallback postDrawCB ;

#ifdef _SSG_USE_DLIST
  GLuint dlist ;
#endif

  virtual void draw_geometry () = 0 ;

  int preDraw () ;

  virtual void copy_from ( ssgLeaf *src, int clone_flags ) ;
public:
  ssgLeaf (void) ;
  virtual ~ssgLeaf (void) ;

  virtual void drawHighlight ( sgVec4 colour ) = 0 ;
  virtual void drawHighlight ( sgVec4 colour, int i ) = 0 ;
#ifdef _SSG_USE_PICK
  virtual void pick ( int baseName ) = 0 ;
#endif

#ifdef _SSG_USE_DLIST
  void makeDList () ;
  void deleteDList () ;
  GLuint getDListIndex () { return dlist ; }
#endif

  int  getExternalPropertyIndex ()
                 { return state ? state->getExternalPropertyIndex() : 0 ; }

  int  isTranslucent () { return state ? state->isTranslucent() : FALSE ; }
  int       hasState () { return state != NULL ; }

  ssgState *getState () { return state ; }
  void      setState ( ssgState *st ); //~~ T.G. Body extended & moved into CXX file
 
  ssgCallback getCallback ( int cb_type )
  {
    return ( cb_type == SSG_CALLBACK_PREDRAW ) ? preDrawCB : postDrawCB ;
  }

  void setCallback ( int cb_type, ssgCallback cb )
  {
    if ( cb_type == SSG_CALLBACK_PREDRAW )
      preDrawCB = cb ;
    else
      postDrawCB = cb ;
  }

  virtual int getNumVertices  () { return 0 ; }
  virtual int getNumNormals   () { return 0 ; }
  virtual int getNumColours   () { return 0 ; }
  virtual int getNumTexCoords () { return 0 ; }

  virtual float *getVertex   ( int i ) = 0 ;
  virtual float *getNormal   ( int i ) = 0 ;
  virtual float *getColour   ( int i ) = 0 ;
  virtual float *getTexCoord ( int i ) = 0 ;
  virtual int  getNumTriangles () = 0 ;
  virtual void getTriangle ( int n, short *v1, short *v2, short *v3 ) = 0 ;

  virtual void transform ( sgMat4 m ) = 0 ;

  void setCullFace ( int cf ) { cull_face = cf ; }
  int  getCullFace () { return cull_face ; }

  virtual void recalcBSphere () = 0 ;
  virtual char *getTypeName(void) ;
  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;

  virtual void cull  ( sgFrustum *f, sgMat4 m, int test_needed ) ;
  virtual void isect ( sgSphere  *s, sgMat4 m, int test_needed ) ;
  virtual void hot   ( sgVec3     s, sgMat4 m, int test_needed ) ;
  virtual void isect_triangles ( sgSphere *s, sgMat4 m, int test_needed ) = 0 ;
  virtual void hot_triangles   ( sgVec3    s, sgMat4 m, int test_needed ) = 0 ;
  virtual void draw  () = 0 ;
} ;

extern sgVec3 _ssgVertex000   ;
extern sgVec4 _ssgColourWhite ;
extern sgVec3 _ssgNormalUp    ;
extern sgVec2 _ssgTexCoord00  ;
extern short  _ssgIndex0      ;


class ssgVTable : public ssgLeaf
{
protected:
  sgBox bbox ;
  int indexed ;
  GLenum gltype ;

  sgVec3 *vertices  ; int num_vertices  ; unsigned short *v_index ;
  sgVec3 *normals   ; int num_normals   ; unsigned short *n_index ;
  sgVec2 *texcoords ; int num_texcoords ; unsigned short *t_index ;
  sgVec4 *colours   ; int num_colours   ; unsigned short *c_index ;

  virtual void draw_geometry () ;
  virtual void copy_from ( ssgVTable *src, int clone_flags ) ;

_SSG_PUBLIC:
  int isIndexed () { return indexed ; }

public:
  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgVTable () ;
  ssgVTable ( GLenum ty,
              int nv, unsigned short *vi, sgVec3 *vl,
              int nn, unsigned short *ni, sgVec3 *nl,
              int nt, unsigned short *ti, sgVec2 *tl,
              int nc, unsigned short *ci, sgVec4 *cl ) ;

  ssgVTable ( GLenum ty,
              int nv, sgVec3 *vl,
              int nn, sgVec3 *nl,
              int nt, sgVec2 *tl,
              int nc, sgVec4 *cl ) ;

  virtual void drawHighlight ( sgVec4 colour ) ;
  virtual void drawHighlight ( sgVec4 colour, int i ) ;
#ifdef _SSG_USE_PICK
  virtual void pick ( int baseName ) ;
#endif
  virtual void transform ( sgMat4 m ) ;

  int getNumVertices  () { return num_vertices  ; }
  int getNumNormals   () { return num_normals   ; }
  int getNumColours   () { return num_colours   ; }
  int getNumTexCoords () { return num_texcoords ; }
  int getNumTriangles () ;
  void getTriangle ( int n, short *v1, short *v2, short *v3 ) ;

  void getColourList ( void **list, unsigned short **idx )
  {
    *list = colours ;
    *idx  = c_index  ; 
  }

  void getTexCoordList ( void **list, unsigned short **idx )
  {
    *list = texcoords ;
    *idx  = t_index  ; 
  }

  void getNormalList ( void **list, unsigned short **idx )
  {
    *list = normals ;
    *idx  = n_index  ; 
  }

  void getVertexList ( void **list, unsigned short **idx )
  {
    *list = vertices ;
    *idx  = v_index  ; 
  }

  float *getVertex  (int i){ if(i>=num_vertices)i=num_vertices-1;
                             return (num_vertices<=0) ? _ssgVertex000 :
                                    ((indexed)?vertices [v_index[i]]:vertices [i]);}
  float *getColour  (int i){ if(i>=num_colours)i=num_colours-1;
                             return (num_colours<=0) ? _ssgColourWhite :
                                    ((indexed)?colours  [c_index[i]]:colours  [i]);}
  float *getNormal  (int i){ if(i>=num_normals)i=num_normals-1;
                             return (num_normals<=0) ? _ssgNormalUp :
                                    ((indexed)?normals  [n_index[i]]:normals  [i]);}
  float *getTexCoord(int i){ if(i>=num_texcoords)i=num_texcoords-1;
                             return (num_texcoords<=0) ? _ssgTexCoord00 :
                                    ((indexed)?texcoords[t_index[i]]:texcoords[i]);}

  GLenum getGLtype () { return gltype ; }

  virtual ~ssgVTable (void) ;

  virtual char *getTypeName(void) ;
  virtual void recalcBSphere () ;
  virtual void draw () ;

  virtual void isect_triangles ( sgSphere  *s, sgMat4 m, int test_needed ) ;
  virtual void hot_triangles   ( sgVec3     s, sgMat4 m, int test_needed ) ;
  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
} ;


class ssgVtxTable : public ssgLeaf
{
protected:
  sgBox bbox ;
  GLenum gltype ;

  virtual void draw_geometry () ;
  virtual void copy_from ( ssgVtxTable *src, int clone_flags ) ;

_SSG_PUBLIC:

  ssgVertexArray   *vertices  ;
  ssgNormalArray   *normals   ;
  ssgTexCoordArray *texcoords ;
  ssgColourArray   *colours   ;

public:
  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgVtxTable () ;

  ssgVtxTable ( GLenum ty, ssgVertexArray   *vl,
                           ssgNormalArray   *nl,
                           ssgTexCoordArray *tl,
                           ssgColourArray   *cl ) ;

  virtual void drawHighlight ( sgVec4 colour ) ;
  virtual void drawHighlight ( sgVec4 colour, int i ) ;
#ifdef _SSG_USE_PICK
  virtual void pick ( int baseName ) ;
#endif
  virtual void transform ( sgMat4 m ) ;

  void setPrimitiveType ( GLenum ty ) { gltype = ty ; }
  GLenum getPrimitiveType () { return gltype ; }

  void setVertices  ( ssgVertexArray   *vl ) ;
  void setNormals   ( ssgNormalArray   *nl ) ;
  void setTexCoords ( ssgTexCoordArray *tl ) ;
  void setColours   ( ssgColourArray   *cl ) ;

  int getNumVertices  () { return vertices  -> getNum () ; }
  int getNumNormals   () { return normals   -> getNum () ; }
  int getNumColours   () { return colours   -> getNum () ; }
  int getNumTexCoords () { return texcoords -> getNum () ; }

  virtual int getNumTriangles () ;
  void getTriangle ( int n, short *v1, short *v2, short *v3 ) ;

  void getVertexList   ( void **list ) { *list = vertices  -> get ( 0 ) ; }
  void getNormalList   ( void **list ) { *list = normals   -> get ( 0 ) ; } 
  void getTexCoordList ( void **list ) { *list = texcoords -> get ( 0 ) ; } 
  void getColourList   ( void **list ) { *list = colours   -> get ( 0 ) ; } 

  float *getVertex  (int i){ if(i>=getNumVertices())i=getNumVertices()-1;
                             return (getNumVertices()<=0) ?
				      _ssgVertex000 : vertices->get(i);}
  float *getNormal  (int i){ if(i>=getNumNormals())i=getNumNormals()-1;
			     return (getNumNormals()<=0) ?
				    _ssgNormalUp    : normals->get(i);}
  float *getTexCoord(int i){ if(i>=getNumTexCoords())i=getNumTexCoords()-1;
                             return (getNumTexCoords()<=0) ?
                                    _ssgTexCoord00  : texcoords->get(i);}
  float *getColour  (int i){ if(i>=getNumColours())i=getNumColours()-1;
			     return (getNumColours()<=0) ?
				    _ssgColourWhite : colours->get(i);}

  GLenum getGLtype () { return gltype ; }

  virtual ~ssgVtxTable (void) ;

  virtual char *getTypeName(void) ;
  virtual void recalcBSphere () ;
  virtual void draw () ;

  virtual void isect_triangles ( sgSphere  *s, sgMat4 m, int test_needed ) ;
  virtual void hot_triangles   ( sgVec3     s, sgMat4 m, int test_needed ) ;
  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
} ;


class ssgVtxArray : public ssgVtxTable
{
protected:
  ssgIndexArray      *indices;

  virtual void draw_geometry () ;

public:
  ssgVtxArray () ;

  ssgVtxArray ( GLenum ty, ssgVertexArray   *vl,
                           ssgNormalArray   *nl,
                           ssgTexCoordArray *tl,
                           ssgColourArray   *cl,
						   ssgIndexArray    *il ) ;

  virtual void drawHighlight ( sgVec4 colour ) ;
  virtual void drawHighlight ( sgVec4 colour, int i ) ;
#ifdef _SSG_USE_PICK
  virtual void pick ( int baseName ) ;
#endif

  void setIndices ( ssgIndexArray *il ) ;

  int getNumIndices () { return indices -> getNum () ; }

  int getNumTriangles () ;

  void getTriangle ( int n, short *v1, short *v2, short *v3 ) ;

  void getIndexList ( void **list ) { *list = indices  -> get ( 0 ) ; }

  short *getIndex  (int i){ if(i>=getNumIndices())i=getNumIndices()-1;
                             return (getNumIndices()<=0) ?
				      &_ssgIndex0 : indices->get(i);}

  virtual ~ssgVtxArray (void) ;

  virtual char *getTypeName(void) ;

  virtual void print ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
} ;


// class ssgVtxInterleavedArray


class ssgBranch : public ssgEntity
{
  ssgKidList kids ;

protected:
  virtual void copy_from ( ssgBranch *src, int clone_flags ) ;

public:
  virtual void zeroSpareRecursive ();

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgBranch (void) ;
  virtual ~ssgBranch (void) ;

  virtual int getNumKids (void)   { return kids.getNumEntities() ; }
  ssgEntity *getKid     ( int n ) { return kids.getEntity  ( n ) ; }
  ssgEntity *getNextKid (void)    { return kids.getNextEntity () ; }
  int        searchForKid ( ssgEntity *entity )
                                  { return kids.searchForEntity(entity); }

  void addKid        ( ssgEntity *entity ) ;
  void removeKid     ( int n ) ;
  void removeKid     ( ssgEntity *entity ) ;
  void removeAllKids (void) ;

  virtual ssgEntity *getByName ( char *match ) ;
  virtual ssgEntity *getByPath ( char *path  ) ;
 
  virtual char *getTypeName(void) ;
  virtual void cull          ( sgFrustum *f, sgMat4 m, int test_needed ) ;
  virtual void isect         ( sgSphere  *s, sgMat4 m, int test_needed ) ;
  virtual void hot           ( sgVec3     s, sgMat4 m, int test_needed ) ;
  virtual void print         ( FILE *fd = stderr, char *indent = "" ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
  virtual void recalcBSphere () ;
} ;

class ssgInvisible : public ssgBranch
{
protected:

  virtual void copy_from ( ssgInvisible *src, int clone_flags ) ;

public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgInvisible (void) ;
  virtual ~ssgInvisible (void) ;

  virtual void cull ( sgFrustum *f, sgMat4 m, int test_needed ) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
} ;



class ssgSelector : public ssgBranch
{
  unsigned char* selection ;

protected:
  int max_kids ;

  virtual void copy_from ( ssgSelector *src, int clone_flags ) ;

public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgSelector ( int max_kids = 32 ) ;
  virtual ~ssgSelector (void) ;

  void selectStep ( unsigned int s )
  {
    memset ( selection, 0, max_kids ) ;
    selection [s] = 1 ;
  }

  void select ( unsigned int s )
  {
    for ( int i=0; i<max_kids && i<32; i++ ) 
      selection [i] = ( (1<<i) & s ) != 0 ; 
  }

  unsigned int getSelect ()
  {
    unsigned int s = 0 ;
    for ( int i=0; i<max_kids && i<32; i++ ) 
      if ( selection [i] ) s |= (1<<i) ;
    return s ;
  }

  int isSelected ( unsigned int i ) const
  {
    return (i<(unsigned int)max_kids) ? selection [ i ] : FALSE ;
  }
  
  int getFirstSelection ()
  {
    int res ;
    
    for ( res = 0 ; res < max_kids && ! selection[res] ; res++ )
      /* Look for first selection */ ;
      
    return ( res < max_kids ) ? res : -1 ;
  }
  
  ssgEntity *getFirstSelectedKid ()
  {
    int k = getFirstSelection () ;
    return (k >= 0) ? getKid ( k ) : NULL ;
  }
  
  int getMaxKids (void) const { return max_kids ; }

  virtual char *getTypeName(void) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
  virtual void cull  ( sgFrustum *f, sgMat4 m, int test_needed ) ;
  virtual void isect ( sgSphere  *s, sgMat4 m, int test_needed ) ;
  virtual void hot   ( sgVec3     s, sgMat4 m, int test_needed ) ;
} ;


class ssgRangeSelector : public ssgSelector
{
  int additive ;
  float rng_list [ 33 ] ;

protected:
  virtual void copy_from ( ssgRangeSelector *src, int clone_flags ) ;
public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgRangeSelector (void) ;
  virtual ~ssgRangeSelector (void) ;

  void setRanges ( float *ranges, unsigned int nranges )
  {
    for ( unsigned int i = 0 ; i < 33 ; i++ )
      if ( i < nranges )
        rng_list [ i ] = ranges [ i ] ;
      else
        rng_list [ i ] = SG_MAX ;
  }

   void setRange ( unsigned int which, float range )
   {
 	  if ( which < 33 )
 		  rng_list [ which ] = range ;
   }
   
  float getRange ( unsigned int which )
  {
    return ( which < 33 ) ? rng_list[which] : SG_MAX ;
  }

  void setAdditive ( int add ) { additive = add ; }
  int  isAdditive  () { return additive ; }

  virtual char *getTypeName(void) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
  virtual void cull  ( sgFrustum *f, sgMat4 m, int test_needed ) ;
  virtual void isect ( sgSphere  *s, sgMat4 m, int test_needed ) ;
  virtual void hot   ( sgVec3     s, sgMat4 m, int test_needed ) ;
} ;


enum ssgAnimEnum
{
  SSG_ANIM_START,
  SSG_ANIM_STOP,
  SSG_ANIM_PAUSE,
  SSG_ANIM_RESUME
} ;

enum ssgAnimDirection
{
  SSG_ANIM_SWING,
  SSG_ANIM_ONESHOT,
  SSG_ANIM_SHUTTLE
} ;


class ssgTimedSelector : public ssgSelector
{
_SSG_PUBLIC:
  ssgAnimEnum      running ;
  ssgAnimDirection mode    ;

  float start_time    ;
  float pause_time    ;
  float loop_time     ;
  float* times  ;
  int   curr  ;
  int   start ;
  int   end   ;

  void compute_loop_time ()
  {
    loop_time = 0 ;

    for ( int k = start ; k <= end ; k++ )
      loop_time += times [ k ] ;
  }

protected:
  virtual void copy_from ( ssgTimedSelector *src, int clone_flags ) ;
public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgTimedSelector ( int max_kids = 32 ) ;
  virtual ~ssgTimedSelector (void) ;

  virtual char *getTypeName(void) ;

  int getStep () ;	

  float getDuration ( int i = 0 ) { return times [ i ] ; }

  void setDuration ( float ti, int i = -1 )
  {
    if ( i >= 0 && i < max_kids )
      times [ i ] = ti ;
    else
    for ( int j = 0 ; j < max_kids ; j++ )
      times [ j ] = ti ;

    compute_loop_time () ;
  }

  void control ( ssgAnimEnum m )
  {
    compute_loop_time () ;

    if ( m == SSG_ANIM_PAUSE )
    {
      pause_time = (float) ssgGetFrameCounter () ;
      curr = getStep () ;
    }
    else
    if ( m == SSG_ANIM_RESUME )
    {
      start_time += (float) ssgGetFrameCounter () - pause_time ;
      
      if ( running != SSG_ANIM_STOP )
        m = SSG_ANIM_START ;
    }
    else
    if ( m == SSG_ANIM_START )
    {
      start_time = (float) ssgGetFrameCounter () ;
      curr = getStep () ;
    }

    running = m ;
  }

  ssgAnimEnum getControl () { return running ; }

  void setMode ( ssgAnimDirection m ) { mode = m ; }
  ssgAnimDirection getMode () { return mode ; }

  void setLimits ( int st, int en )
  {
    curr  = st ;
    start = st ;
    end   = en ;
    compute_loop_time () ;
  }

  void getLimits ( int *st, int *en )
  {
    if ( st != NULL ) *st = start ;
    if ( en != NULL ) *en = end ;
  }

  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
  virtual void cull  ( sgFrustum *f, sgMat4 m, int test_needed ) ;
  virtual void isect ( sgSphere  *s, sgMat4 m, int test_needed ) ;
  virtual void hot   ( sgVec3     s, sgMat4 m, int test_needed ) ;
} ;


class ssgBaseTransform : public ssgBranch
{
protected:

  sgMat4 transform ;
  sgMat4 last_transform ;
  int    last_updated ;
  int    first_time ;
  virtual void copy_from ( ssgBaseTransform *src, int clone_flags ) ;
public:

  ssgBaseTransform (void) ;
  virtual ~ssgBaseTransform (void) ;

  void firsttime ()
  {
    if ( first_time )
    {
      first_time = FALSE ;
      updateTransform () ;
    }
  }

  void updateTransform ()
  {
    sgCopyMat4 ( last_transform, transform ) ;
    last_updated = ssgGetFrameCounter () ;
  }

  void getLastTransform ( sgMat4 xform )
  {
    /*
      If the transform was not updated this - or last frame
      then we need to equate the two transforms.
    */

    if ( last_updated < ssgGetFrameCounter () - 1 )
      updateTransform () ;

    sgCopyMat4 ( xform, last_transform ) ;
  }

  void getTransform ( sgMat4 xform )
  {
    sgCopyMat4 ( xform, transform ) ;
  }

  virtual void setTransform ( sgVec3 xyz ) = 0 ;
  virtual void setTransform ( sgCoord *xform ) = 0 ;
  virtual void setTransform ( sgCoord *xform, float sx, float sy, float sz ) = 0 ;
  virtual void setTransform ( sgMat4 xform ) = 0 ;

  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;

  virtual char *getTypeName(void) ;
} ;


class ssgTransform : public ssgBaseTransform
{
protected:
  virtual void copy_from ( ssgTransform *src, int clone_flags ) ;
public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgTransform (void) ;
  ssgTransform ( sgCoord *c ) ;
  virtual ~ssgTransform (void) ;

  virtual void setTransform ( sgVec3 xyz ) ;
  virtual void setTransform ( sgCoord *xform ) ;
  virtual void setTransform ( sgCoord *xform, float sx, float sy, float sz ) ;
  virtual void setTransform ( sgMat4 xform ) ;

  virtual char *getTypeName(void) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
  virtual void cull  ( sgFrustum *f, sgMat4 m, int test_needed ) ;
  virtual void isect ( sgSphere  *s, sgMat4 m, int test_needed ) ;
  virtual void hot   ( sgVec3     s, sgMat4 m, int test_needed ) ;
  virtual void recalcBSphere () ;
} ;


class ssgTexTrans : public ssgBaseTransform
{
protected:
  virtual void copy_from ( ssgTexTrans *src, int clone_flags ) ;
public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgTexTrans (void) ;
  ssgTexTrans ( sgCoord *c ) ;
  virtual ~ssgTexTrans (void) ;

  virtual void setTransform ( sgVec3 xyz ) ;
  virtual void setTransform ( sgCoord *xform ) ;
  virtual void setTransform ( sgCoord *xform, float sx, float sy, float sz ) ;
  virtual void setTransform ( sgMat4 xform ) ;

  virtual char *getTypeName(void) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
  virtual void cull  ( sgFrustum *f, sgMat4 m, int test_needed ) ;
} ;


class ssgCutout : public ssgBranch
{
  int point_rotate ;
protected:
  virtual void copy_from ( ssgCutout *src, int clone_flags ) ;
public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgCutout (int pntrot=FALSE) ;
  virtual ~ssgCutout (void) ;

  int isPointRotate (void) { return point_rotate ; }

  virtual char *getTypeName(void) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
  virtual void cull  ( sgFrustum *f, sgMat4 m, int test_needed ) ;
  virtual void isect ( sgSphere  *s, sgMat4 m, int test_needed ) ;
  virtual void hot   ( sgVec3     s, sgMat4 m, int test_needed ) ;
} ;


class ssgRoot : public ssgBranch
{
protected:
  virtual void copy_from ( ssgRoot *src, int clone_flags ) ;
public:

  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  ssgRoot (void) ;
  virtual ~ssgRoot (void) ;
  virtual char *getTypeName(void) ;
  virtual int load ( FILE *fd ) ;
  virtual int save ( FILE *fd ) ;
} ;


class ssgLight
{
  int id ;
  int is_headlight ;
  int is_turned_on ;

  sgVec4 ambient ;
  sgVec4 diffuse ;
  sgVec4 specular ;

  sgVec4 position ;

public:

  ssgLight ()
  {
    id = 0 ;
    is_turned_on = FALSE ;
    is_headlight = FALSE ;
    sgSetVec4 ( position, 0.0f, 0.0f, 1.0f, 0.0f ) ;
    sgSetVec4 ( ambient , 0.2f, 0.2f, 0.2f, 1.0f ) ;
    sgSetVec4 ( diffuse , 1.0f, 1.0f, 1.0f, 1.0f ) ;
    sgSetVec4 ( specular, 1.0f, 1.0f, 1.0f, 1.0f ) ;
  }

  void setID ( int i ) { id = i ; }
  int  isOn () { return is_turned_on  ; }
  void on  () { is_turned_on = TRUE  ; }
  void off () { is_turned_on = FALSE ; }
  void setPosition ( sgVec3 pos ) { sgCopyVec3 ( position, pos ) ; }

  void setColour   ( GLenum which, sgVec4 col )
  {
    switch ( which )
    {
      case GL_AMBIENT  : sgCopyVec4 ( ambient , col ) ; break ;
      case GL_DIFFUSE  : sgCopyVec4 ( diffuse , col ) ; break ;
      case GL_SPECULAR : sgCopyVec4 ( specular, col ) ; break ;
      default : break ;
    }
    setup () ;
  }

  void setHeadlight ( int head ) { is_headlight = head ; }
  int  isHeadlight () { return is_headlight ; }

  void setup ()
  {
    if ( is_turned_on )
    {
      glEnable  ( (GLenum)(GL_LIGHT0+id) ) ;
      glLightfv ( (GLenum)(GL_LIGHT0+id), GL_AMBIENT , ambient  ) ;
      glLightfv ( (GLenum)(GL_LIGHT0+id), GL_DIFFUSE , diffuse  ) ;
      glLightfv ( (GLenum)(GL_LIGHT0+id), GL_SPECULAR, specular ) ;
      glLightfv ( (GLenum)(GL_LIGHT0+id), GL_POSITION, position ) ;
    }
    else
      glDisable ( (GLenum)(GL_LIGHT0+id) ) ;
  }
} ;

class ssgHit
{
_SSG_PUBLIC:

  int num_entries ;
  ssgEntity *path [ SSG_MAXPATH ] ;

public:
  ssgLeaf *leaf ;
  int      triangle ;
  sgVec4   plane ;
  sgMat4   matrix ;

  ssgHit ()
  {
    leaf = NULL ;
    init () ;
  } ;

  void init () { num_entries = 0 ; }

  void addPath ( ssgEntity *e )
  {
    if ( num_entries < SSG_MAXPATH )
      path [ num_entries++ ] = e ;
  }
 
  int getNumPathEntries () { return num_entries ; }

  ssgEntity *getPathEntry ( int i )
  {
    return ( i >= 0 && i < num_entries ) ? path[i] : (ssgEntity *) NULL ;
  }
} ;

class ssgContext
{
  ssgSimpleState *currentState ;
  ssgSimpleState *basicState   ;
  sgFrustum      *frustum      ;

  int    orthographic         ;
  sgMat4 cameraMatrix         ;
  int    cullFace             ;
  int    ovTexture            ;
  int    ovCullface           ;

public:

   ssgContext () ;
  ~ssgContext () ;

  void forceBasicState () ;

  void makeCurrent () ;
  int    isCurrent () ;

  void overrideTexture   ( int on_off ) ;
  void overrideCullface  ( int on_off ) ;

  void setCullface       ( int on_off )
  {
    if ( cullFace == on_off || ovCullface )
      return ;

    cullFace = on_off ;

    if ( cullFace ) glEnable  ( GL_CULL_FACE ) ;
               else glDisable ( GL_CULL_FACE ) ;
  }

  int  textureOverridden  () { return ovTexture  ; }
  int  cullfaceOverridden () { return ovCullface ; }
  int  cullfaceIsEnabled  () { return cullFace   ; }

  sgFrustum *getFrustum () { return frustum ; }

  void getNearFar ( float *n, float *f ) ;
  void getFOV     ( float *w, float *h ) ;
  void getOrtho   ( float *w, float *h ) ;
  void setNearFar ( float  n, float  f ) ;
  void setOrtho   ( float  w, float  h ) ;
  void setFOV     ( float  w, float  h ) ;

  int  isOrtho () { return orthographic ; }

  ssgSimpleState *getState () { return currentState ; }
  void cull ( ssgRoot *r ) ;

  void getCameraPosition ( sgVec3 pos ) ;
  void setCamera ( sgMat4 mat ) ;
  void setCamera ( sgCoord *coord ) ;
  void setCameraLookAt ( const sgVec3 eye, const sgVec3 center, const sgVec3 up ) ;
  void setCameraLookAt ( const sgVec3 eye, const sgVec3 center ) ;

  void loadProjectionMatrix () ;
  void loadModelviewMatrix  () ;

  void getProjectionMatrix  ( sgMat4 dst ) ;
  void getModelviewMatrix   ( sgMat4 dst ) ;

  void pushProjectionMatrix () ;
  void pushProjectionMatrix ( sgFrustum *f ) ;
  void loadModelviewMatrix  ( sgMat4 mat ) ;

} ;



/* Compatibility inlines for pre-ssgContext applications */

extern ssgContext *_ssgCurrentContext ;

inline ssgContext *ssgGetCurrentContext () { return _ssgCurrentContext ; }

inline void ssgGetCameraPosition ( sgVec3 pos )
{
  _ssgCurrentContext -> getCameraPosition ( pos ) ;
}

inline void ssgOverrideTexture ( int on_off )
{
  _ssgCurrentContext->overrideTexture ( on_off ) ;
}

inline void ssgOverrideCullface ( int on_off )
{
  _ssgCurrentContext->overrideCullface ( on_off ) ;
}

inline void ssgGetNearFar ( float *n, float *f )
{
  _ssgCurrentContext->getNearFar ( n, f ) ;
}

inline void ssgGetFOV ( float *w, float *h )
{
  _ssgCurrentContext->getFOV ( w, h ) ;
}

inline void ssgGetOrtho ( float *w, float *h )
{
  _ssgCurrentContext->getOrtho ( w, h ) ;
}

inline void ssgSetFOV ( float w, float h )
{
  _ssgCurrentContext->setFOV ( w, h ) ;
}

inline void ssgSetOrtho ( float w, float h )
{
  _ssgCurrentContext->setOrtho ( w, h ) ;
}

inline void ssgSetNearFar ( float n, float f )
{
  _ssgCurrentContext->setNearFar ( n, f ) ;
}

inline void ssgSetCamera ( sgMat4 mat )
{
  _ssgCurrentContext-> setCamera ( mat ) ;
}

inline void ssgSetCamera ( sgCoord *coord )
{
  _ssgCurrentContext-> setCamera ( coord ) ;
}

inline void ssgSetCameraLookAt ( const sgVec3 eye, const sgVec3 center, const sgVec3 up )
{
  _ssgCurrentContext-> setCameraLookAt ( eye, center, up ) ;
}

inline void ssgSetCameraLookAt ( const sgVec3 eye, const sgVec3 center )
{
  _ssgCurrentContext-> setCameraLookAt ( eye, center ) ;
}

inline void ssgLoadProjectionMatrix ()
{
  _ssgCurrentContext->loadProjectionMatrix () ;
}

inline void ssgLoadProjectionMatrix ( sgFrustum *f )
{
  glLoadIdentity () ;
  _ssgCurrentContext->pushProjectionMatrix ( f ) ;
}

inline void ssgGetProjectionMatrix ( sgMat4 dst )
{
  _ssgCurrentContext->getProjectionMatrix ( dst ) ;
}

inline void ssgGetModelviewMatrix ( sgMat4 dst )
{
  _ssgCurrentContext->getModelviewMatrix ( dst ) ;
}

inline void ssgLoadModelviewMatrix ()
{
  _ssgCurrentContext->loadModelviewMatrix () ;
}

inline void ssgLoadModelviewMatrix ( sgMat4 mat )
{
  _ssgCurrentContext->loadModelviewMatrix ( mat ) ;
}

inline void ssgForceBasicState ()
{
  _ssgCurrentContext -> forceBasicState () ;
}


void ssgInit () ;

void ssgCullAndDraw ( ssgRoot *root ) ;
#ifdef _SSG_USE_PICK
void ssgCullAndPick ( ssgRoot *root, sgVec2 botleft, sgVec2 topright ) ;
#endif
int  ssgIsect       ( ssgRoot *root, sgSphere *s, sgMat4 m, ssgHit **results ) ;
int  ssgHOT         ( ssgRoot *root, sgVec3    s, sgMat4 m, ssgHit **results ) ;
int  ssgLOS         ( ssgRoot *root, sgVec3    s, sgMat4 m, ssgHit **results ) ;

/* Weird private stuff */

int _ssgStrEqual ( const char *s1, const char *s2 ) ;
char* _ssgMakePath( char* path, const char* dir, const char* fname ) ;

/* Load/Save functions */

enum {
	SSG_MD2_STAND,
	SSG_MD2_RUN,
	SSG_MD2_ATTACK,
	SSG_MD2_PAIN_1,
	SSG_MD2_PAIN_2,
	SSG_MD2_PAIN_3,
	SSG_MD2_JUMP,
	SSG_MD2_FLIPOFF,
	SSG_MD2_SALUTE,
	SSG_MD2_TAUNT,
	SSG_MD2_WAVE,
	SSG_MD2_POINT,
	SSG_MD2_CROUCH_STAND,
	SSG_MD2_CROUCH_WALK,
	SSG_MD2_CROUCH_PAIN,
	SSG_MD2_CROUCH_DEATH,
	SSG_MD2_DEATH_1,
	SSG_MD2_DEATH_2,
	SSG_MD2_DEATH_3,
	SSG_MD2_ALL,
	SSG_MD2_POSE
};

typedef ssgState *(*ssgAppStateFunc)(char*) ;
typedef ssgLeaf *(*ssgCreateLeafFunc)(ssgLeaf*,const char*) ;
typedef ssgBranch *(*ssgHookFunc)(char *) ;
typedef ssgTexture *(*ssgCreateTextureFunc)(char*,int,int,int) ;

class ssgLoaderOptions
{
  ssgAppStateFunc app_state_cb ;
  ssgCreateLeafFunc create_leaf_cb ;
  ssgHookFunc hook_cb ;
  ssgCreateTextureFunc create_texture_cb ;

  //NOTES: we could add more later
  //...model scale factor...
  //...create normals or read them from the file?...
  //...cutoff angle for smooth shading where there isn`t one in the file format...
  //...which direction is front-facing in formats that don`t get it right...
  //...etc...

  ssgLeaf* defaultCreateLeaf ( ssgLeaf* leaf,
			       const char* parent_name ) const ;
  ssgTexture *defaultCreateTexture ( char *tfname, 
				     int wrapu, int wrapv,
				     int mipmap ) const ;

public:
  ssgLoaderOptions (
      ssgAppStateFunc      _app_state_cb,
      ssgCreateLeafFunc    _create_leaf_cb = 0,
      ssgHookFunc          _hook_cb = 0,
      ssgCreateTextureFunc _create_texture_cb = 0
    ) :
    app_state_cb      ( _app_state_cb ),
    create_leaf_cb    ( _create_leaf_cb ),
    hook_cb           ( _hook_cb ),
    create_texture_cb ( _create_texture_cb )
  {}

  void setAppStateCallback ( ssgAppStateFunc cb )
  {
    app_state_cb = cb ;
  }

  ssgState* createState ( char* tfname ) const
  {
    if ( app_state_cb )
      return (*app_state_cb)(tfname) ;
    else
      return NULL ;
  }

  ssgLeaf* createLeaf ( ssgLeaf* leaf, const char* parent_name ) const
  {
    if ( create_leaf_cb )
      return (*create_leaf_cb)(leaf,parent_name) ;
    else
      return defaultCreateLeaf(leaf,parent_name) ;
  }

  ssgTexture* createTexture ( char* tfname, 
			      int wrapu  = TRUE, int wrapv = TRUE, 
			      int mipmap = TRUE ) const
  {
    if ( create_texture_cb )
      return (*create_texture_cb)(tfname, wrapu, wrapv, mipmap) ;
    else
      return defaultCreateTexture(tfname, wrapu, wrapv, mipmap) ;
  }

  ssgHookFunc getHookFunc () const
  {
    return hook_cb ;
  }

  ssgBranch* invokeHookFunc ( char* text ) const
  {
    if ( hook_cb )
      return (*hook_cb)(text) ;
    else
      return NULL ;
  }

  void begin () const
  {
    createLeaf ( 0, 0 ) ;
  }

  void end () const
  {
    createLeaf ( 0, 0 ) ;
  }
} ;

int        ssgSave     ( const char *fname, ssgEntity *ent ) ;
int        ssgSaveAC   ( const char *fname, ssgEntity *ent ) ;
int        ssgSaveASE  ( const char *fname, ssgEntity *ent ) ;
int        ssgSaveSSG  ( const char *fname, ssgEntity *ent ) ;
int        ssgSaveDXF  ( const char *fname, ssgEntity *ent ) ;
int        ssgSaveTRI  ( const char *fname, ssgEntity *ent ) ;
int        ssgSaveOBJ  ( const char *fname, ssgEntity *ent ) ;
int        ssgSaveX    ( const char *fname, ssgEntity *ent ) ;

ssgEntity *ssgLoad     ( const char *fname, const ssgLoaderOptions *options = NULL ) ;
ssgEntity *ssgLoadVRML ( const char *fname, const ssgLoaderOptions *options = NULL ) ;
ssgEntity *ssgLoad3ds  ( const char *fname, const ssgLoaderOptions *options = NULL ) ;
ssgEntity *ssgLoadAC3D ( const char *fname, const ssgLoaderOptions *options = NULL ) ;
ssgEntity *ssgLoadSSG  ( const char *fname, const ssgLoaderOptions *options = NULL ) ;
ssgEntity *ssgLoadASE  ( const char *fname, const ssgLoaderOptions *options = NULL ) ;
ssgEntity *ssgLoadDXF  ( const char *fname, const ssgLoaderOptions *options = NULL ) ;
ssgEntity *ssgLoadTRI  ( const char *fname, const ssgLoaderOptions *options = NULL ) ;
ssgEntity *ssgLoadOBJ  ( const char *fname, const ssgLoaderOptions *options = NULL ) ;
ssgEntity *ssgLoadMD2  ( const char *fname, const ssgLoaderOptions *options = NULL ) ;
ssgEntity *ssgLoadMDL  ( const char *fname, const ssgLoaderOptions *options = NULL ) ;
ssgEntity *ssgLoadX    ( const char *fname, const ssgLoaderOptions *options = NULL ) ;

/* For backwards compatibility */

ssgEntity *ssgLoadAC   ( const char *fname, const ssgLoaderOptions *options = NULL ) ;

extern ssgLoaderOptions _ssgDefaultOptions ;

inline void ssgSetAppStateCallback ( ssgAppStateFunc cb )
{
  _ssgDefaultOptions.setAppStateCallback ( cb ) ;
}

void ssgFlatten  ( ssgEntity *ent ) ;
void ssgStripify ( ssgEntity *ent ) ;

void ssgArrayTool ( ssgEntity *ent, float* vtol = 0, bool make_normals = false ) ;
void ssgTransTool ( ssgEntity *ent, const sgMat4 trans ) ;

void ssgModelPath   ( const char *path ) ;
void ssgTexturePath ( const char *path ) ;

ssgLight *ssgGetLight ( int i ) ;

char *ssgShowStats () ;
void  ssgDelete ( ssgBranch *br ) ;
char *ssgGetVersion () ;
char * ssgGetAPOM();


#endif

