

#define ROOT_BONE  9999
 
class Bone : public ssgBase
{
public:
  int           id        ;
  int           parent    ;
  ssgTransform *effector  ;

  sgVec3        xlate     ;
  sgMat4        netMatrix ;

  sgVec3             vx [ 2 ] ;  /* Relative to joint */
  sgVec3        orig_vx [ 2 ] ;  /* Absolute */

  puGroup *widget ;
  puOneShot *rs ;
  puButton  *hb ;
  puButton  *pb ;
  puButton  *rb ;
  puDial    *sh ;
  puDial    *sp ;
  puDial    *sr ;
  puInput   *na ;
  sgVec4 colour ;
  Bone () ;

  void read  ( FILE *fd ) ;
  void write ( FILE *fd ) ;
  void createJoint () ;

  void computeTransform ( Event *prev, Event *next, float tim ) ;

  void setAngle  ( int which, float a ) ;
  void setAngles ( float h, float p, float r ) ;
  void setAngles ( sgVec3 a ) ;

  float *getXYZ () { return xlate ; }

  float *getDialAngles () ;

  sgCoord *getXForm ( Event *prev, Event *next, float tim ) ;
  sgCoord *getXForm () ;

  void transform ( sgVec3 dst, sgVec3 src ) ;
  void swapEnds() ;
  void init ( ssgLeaf *l, sgMat4 newmat, short vv[2], int id ) ;
  void print ( FILE *fd, int which ) ;
  ssgBranch *generateGeometry ( int root ) ;
} ;
 
float *getCurrTranslate () ;

void opaqueBones () ;
void blendBones  () ;

void init_bones () ;

int getNumBones () ;
Bone *getBone   ( int i ) ;

void createJoint ( sgVec4 colour, Bone *b ) ;
 
void setShowAngle ( float a ) ;

extern int rootBone   ;
extern int nextBone   ;
extern int nextVertex ;

struct Vertex
{
  int     boneID ;
  sgVec3  rel_vx ;
  float  *vx     ;
} ;
 
#define MAX_VERTICES  (64*1024)
extern Vertex vertex [] ;
extern sgVec3 curr_translate ;

void syncTranslators ( sgVec3 newTranslate ) ;

