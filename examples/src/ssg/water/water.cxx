#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <math.h>
#include <plib/ssg.h>
#include <plib/ssgAux.h>
#include <plib/pu.h>
#include <GL/glut.h>

#define GUI_BASE      80
#define VIEW_GUI_BASE 20
#define FONT_COLOUR   1,1,1,1

puSlider    *trainLengthSlider  = (puSlider    *) NULL ;
puSlider    *trainSpeedSlider   = (puSlider    *) NULL ;
puSlider    *trainLambdaSlider  = (puSlider    *) NULL ;
puSlider    *trainHeightSlider  = (puSlider    *) NULL ;
puButton    *trainEnableButton  = (puButton    *) NULL ;
puOneShot   *trainDisableButton = (puOneShot   *) NULL ;
puDial      *trainHeadingDial   = (puDial      *) NULL ;
puSelectBox *trainSelectBox     = (puSelectBox *) NULL ;
puText      *timeText           = (puText      *) NULL ;

puSelectBox *depthSelectBox     = (puSelectBox *) NULL ;

puDial      *viewHeadingDial    = (puDial      *) NULL ;
puDial      *viewPitchDial      = (puDial      *) NULL ;
puSlider    *viewRangeSlider    = (puSlider    *) NULL ;
puButton    *viewWireframeButton= (puButton    *) NULL ;
puButton    *viewEnvMapButton   = (puButton    *) NULL ;

puOneShot   *writeButton        = (puOneShot   *) NULL ;

puSlider    *waveTextureSlider  = (puSlider    *) NULL ;
puSlider    *waveSizeSlider     = (puSlider    *) NULL ;
puSlider    *wavePolycountSlider= (puSlider    *) NULL ;

ssgRoot            *scene        = NULL ;
ssgaLensFlare      *sun_obj      = NULL ;
ssgTransform       *sun          = NULL ;
ssgTransform       *teapot       = NULL ;
ssgTransform       *pedestal     = NULL ;
ssgaWaveSystem     *ocean        = NULL ;
ssgaParticleSystem *fountain     = NULL ;
ssgaCube           *ped_obj      = NULL ;
ssgaTeapot         *tpt_obj      = NULL ;

ssgSimpleState     *sea_state    = NULL ;
ssgSimpleState     *splash_state = NULL ;
ssgSimpleState     *teapot_state = NULL ;
ssgSimpleState     *plinth_state = NULL ;

ssgaWaveTrain trains [ 16 ] ;

char *trainNameList[] =
{
  "Train 0" , "Train 1" , "Train 2" , "Train 3" , "Train 4" ,
  "Train 5" , "Train 6" , "Train 7" , "Train 8" , "Train 9" ,
  "Train 10", "Train 11", "Train 12", "Train 13", "Train 14",
  "Train 15",
  NULL
} ;

int   curr_train = 0 ;
int   curr_depthfunc = 0 ;
int   wireframe  = FALSE ;
int   displayGUI = TRUE  ;
float cam_range  = 25.0f ;

sgCoord campos = { { 0, -20, 8 }, { 0, -30, 0 } } ;
sgVec3  sunpos = { 400, 300, 50 } ;

int enableTexGen ( ssgEntity * )
{
#ifdef GL_ARB_multitexture
  int tx ;
  glGetIntegerv ( GL_TEXTURE_BINDING_2D, &tx ) ;
  glActiveTextureARB ( GL_TEXTURE1_ARB ) ;
#endif
  glTexGeni ( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP ) ;
  glTexGeni ( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP ) ;
  glEnable ( GL_TEXTURE_GEN_S ) ;
  glEnable ( GL_TEXTURE_GEN_T ) ;
  glEnable ( GL_TEXTURE_2D ) ;
#ifdef GL_ARB_multitexture
  glBindTexture ( GL_TEXTURE_2D, tx ) ;
  glActiveTextureARB ( GL_TEXTURE0_ARB ) ;
#endif    
  return TRUE ;
} 

int disableTexGen ( ssgEntity * )
{
#ifdef GL_ARB_multitexture
  glActiveTextureARB ( GL_TEXTURE1_ARB ) ;
#endif
  glDisable ( GL_TEXTURE_GEN_S ) ;
  glDisable ( GL_TEXTURE_GEN_T ) ;
  glDisable ( GL_TEXTURE_2D ) ;
#ifdef GL_ARB_multitexture
  glActiveTextureARB ( GL_TEXTURE0_ARB ) ;
#endif
  return TRUE ;
}
 
void writeCplusplusCode ()
{
  if ( viewEnvMapButton -> getIntegerValue () )
  {
    printf ( "\n" ) ;
    printf ( "static int enableTexGen ( ssgEntity * )\n" ) ;
    printf ( "{\n" ) ;
    printf ( "#ifdef GL_ARB_multitexture\n" ) ;
    printf ( "  int tx ;\n" ) ;
    printf ( "  glGetIntegerv ( GL_TEXTURE_BINDING_2D, &tx ) ;\n" ) ;
    printf ( "  glActiveTextureARB ( GL_TEXTURE1_ARB ) ;\n" ) ;
    printf ( "#endif\n" ) ;
    printf ( "  glTexGeni ( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP ) ;\n" ) ;
    printf ( "  glTexGeni ( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP ) ;\n" ) ;
    printf ( "  glEnable ( GL_TEXTURE_GEN_S ) ;\n" ) ;
    printf ( "  glEnable ( GL_TEXTURE_GEN_T ) ;\n" ) ;
    printf ( "  glEnable ( GL_TEXTURE_2D ) ;\n" ) ;
    printf ( "#ifdef GL_ARB_multitexture\n" ) ;
    printf ( "  glBindTexture ( GL_TEXTURE_2D, tx ) ;\n" ) ;
    printf ( "  glActiveTextureARB ( GL_TEXTURE0_ARB ) ;\n" ) ;
    printf ( "#endif\n" ) ;
    printf ( "  return TRUE ;\n" ) ;
    printf ( "} \n" ) ;
    printf ( "\n" ) ;
    printf ( "static int disableTexGen ( ssgEntity * )\n" ) ;
    printf ( "{\n" ) ;
    printf ( "#ifdef GL_ARB_multitexture\n" ) ;
    printf ( "  glActiveTextureARB ( GL_TEXTURE1_ARB ) ;\n" ) ;
    printf ( "#endif\n" ) ;
    printf ( "  glDisable ( GL_TEXTURE_GEN_S ) ;\n" ) ;
    printf ( "  glDisable ( GL_TEXTURE_GEN_T ) ;\n" ) ;
    printf ( "  glDisable ( GL_TEXTURE_2D ) ;\n" ) ;
    printf ( "#ifdef GL_ARB_multitexture\n" ) ;
    printf ( "  glActiveTextureARB ( GL_TEXTURE0_ARB ) ;\n" ) ;
    printf ( "#endif\n" ) ;
    printf ( "  return TRUE ;\n" ) ;
    printf ( "}\n" ) ;
    printf ( "\n" ) ;
  }
 
  printf ( "\n" ) ;
  printf ( "ssgaWaveSystem *makeWaveSystem ( ssgState *state )\n" ) ;
  printf ( "{\n" ) ;
  printf ( "  ssgaWaveTrain  *wavetrain ;\n" ) ;
  printf ( "  ssgaWaveSystem *ocean ;\n" ) ;
  printf ( "  ocean =  new ssgaWaveSystem ( %d ) ;\n", ocean->getNumTris() ) ;
  printf ( "  ocean -> setSize            ( %g ) ;\n", ocean->getSize()[0] ) ;
  printf ( "  ocean -> setKidState        ( state ) ;\n" ) ;
  printf ( "  ocean -> setTexScale      ( %g, %g ) ;\n",
                                                     ocean->getTexScaleU (),
                                                     ocean->getTexScaleV () ) ;
  printf ( "  ocean -> setWindSpeed     ( 10.0f ) ;\n\n" ) ;

  for ( int i = 0 ; i < SSGA_MAX_WAVETRAIN ; i++ )
  {
    if ( ocean -> getWaveTrain ( i ) != NULL )
    {
      printf ( "  wavetrain = new ssgaWaveTrain () ;\n" ) ;

      printf ( "  wavetrain->setSpeed      ( %g ) ;\n",trains[i].getSpeed ());
      printf ( "  wavetrain->setLength     ( %g ) ;\n",trains[i].getLength ());
      printf ( "  wavetrain->setLambda     ( %g ) ;\n",trains[i].getLambda ());
      printf ( "  wavetrain->setHeading    ( %g ) ;\n",trains[i].getHeading ());
      printf ( "  wavetrain->setWaveHeight ( %g ) ;\n",trains[i].getWaveHeight());

      printf ( "  ocean -> setWaveTrain ( %d, wavetrain ) ;\n\n", i ) ;
    }
  }

  if ( viewEnvMapButton -> getIntegerValue () )
  {
    printf ( "  ocean   -> setKidCallback ( SSG_CALLBACK_PREDRAW ,  enableTexGen ) ;\n" ) ;
    printf ( "  ocean   -> setKidCallback ( SSG_CALLBACK_POSTDRAW, disableTexGen ) ;\n" ) ;
  }
  printf ( "  ocean -> regenerate () ;\n" ) ;
  printf ( "  return ocean ;\n" ) ;
  printf ( "}\n" ) ;
  printf ( "\n" ) ;
}


void waveTextureSlider_cb ( puObject *ob )
{
  ocean   -> setTexScale ( ob -> getFloatValue (),
                           ob -> getFloatValue () ) ;
}


void waveSizeSlider_cb ( puObject *ob )
{
  cam_range = ob -> getFloatValue () ;
  ocean   -> setSize ( ob -> getFloatValue () ) ;
}


void wavePolycountSlider_cb ( puObject *ob )
{
  ocean -> setNumTris ( ob -> getIntegerValue () ) ;
}


void viewHeadingDial_cb ( puObject *ob )
{
  campos.hpr[0] = ob -> getFloatValue () ;

  sgVec3 r = { 0, -cam_range, 0 } ;
  sgMat4 mat ;

  sgMakeRotMat4 ( mat, campos.hpr ) ;
  sgXformVec3 ( campos.xyz, r, mat ) ;
}


void viewPitchDial_cb ( puObject *ob )
{
  campos . hpr [ 1 ] = ob -> getFloatValue () ;

  sgVec3 r = { 0, -cam_range, 0 } ;
  sgMat4 mat ;

  sgMakeRotMat4 ( mat, campos.hpr ) ;
  sgXformVec3 ( campos.xyz, r, mat ) ;
}


void viewRangeSlider_cb ( puObject *ob )
{
  cam_range = ob -> getFloatValue () ;

  sgVec3 r = { 0, -cam_range, 0 } ;
  sgMat4 mat ;

  sgMakeRotMat4 ( mat, campos.hpr ) ;
  sgXformVec3 ( campos.xyz, r, mat ) ;
}


void viewWireframeButton_cb ( puObject *ob )
{
  wireframe = ob -> getIntegerValue () ;
}


void viewEnvMapButton_cb ( puObject *ob )
{
  if ( ob -> getIntegerValue () )
  {
    ocean   -> setKidCallback   ( SSG_CALLBACK_PREDRAW , enableTexGen ) ;
    ocean   -> setKidCallback   ( SSG_CALLBACK_POSTDRAW, disableTexGen ) ;
    ocean   -> regenerate       () ;
  }
  else
  {
    ocean   -> setKidCallback   ( SSG_CALLBACK_PREDRAW , NULL ) ;
    ocean   -> setKidCallback   ( SSG_CALLBACK_POSTDRAW, NULL ) ;
    ocean   -> regenerate       () ;
  }
}


void trainLengthSlider_cb ( puObject *ob )
{
  trains[curr_train].setLength ( ob -> getFloatValue () ) ;
  ob -> setLegend ( ob -> getStringValue () ) ;
}


void trainSpeedSlider_cb ( puObject *ob )
{
  trains[curr_train].setSpeed ( ob -> getFloatValue () ) ;
  ob -> setLegend ( ob -> getStringValue () ) ;
}


void trainLambdaSlider_cb ( puObject *ob )
{
  trains[curr_train].setLambda ( ob -> getFloatValue () ) ;
  ob -> setLegend ( ob -> getStringValue () ) ;
}


void trainHeightSlider_cb ( puObject *ob )
{
  trains[curr_train].setWaveHeight ( ob -> getFloatValue () ) ;
  ob -> setLegend ( ob -> getStringValue () ) ;
}

void trainDisableButton_cb ( puObject *ob )
{
  for ( int i = 0 ; i < SSGA_MAX_WAVETRAIN ; i++ )
    ocean -> setWaveTrain ( i, NULL ) ;

  trainEnableButton -> setValue ( 0 ) ;
}

void writeButton_cb ( puObject *ob )
{
  if ( ob -> getIntegerValue () )
    writeCplusplusCode () ;
}

void trainEnableButton_cb ( puObject *ob )
{
  if ( ob -> getIntegerValue () )
    ocean -> setWaveTrain ( curr_train, & trains [ curr_train ] ) ;
  else
    ocean -> setWaveTrain ( curr_train, NULL ) ;
}

void trainHeadingDial_cb ( puObject *ob )
{
  trains[curr_train].setHeading ( ob -> getFloatValue () ) ;
  ob -> setLegend ( ob -> getStringValue () ) ;
}

void trainSelectBox_cb ( puObject *ob )
{
  curr_train = ((puSelectBox *) ob) -> getCurrentItem () ;

  if ( curr_train < 0 )
    curr_train = 0 ;

  if ( curr_train >= SSGA_MAX_WAVETRAIN )
    curr_train = SSGA_MAX_WAVETRAIN - 1 ;

  trainEnableButton -> setValue (
                              ocean -> getWaveTrain ( curr_train ) != NULL ) ;

  trainLengthSlider -> setValue ( trains[curr_train].getLength     () ) ;
  trainSpeedSlider  -> setValue ( trains[curr_train].getSpeed      () ) ;
  trainLambdaSlider -> setValue ( trains[curr_train].getLambda     () ) ;
  trainHeightSlider -> setValue ( trains[curr_train].getWaveHeight () ) ;
  trainHeadingDial  -> setValue ( trains[curr_train].getHeading    () ) ;
}


float halfMeterEverywhere ( float x, float y )
{
  return 0.5f ;
}

 
float oneMeterEverywhere ( float x, float y )
{
  return 1.0f ;
}

 
float twoMeterEverywhere ( float x, float y )
{
  return 1.5f ;
}

 
float gentleSlope ( float x, float y )
{
  return (1.0f + x / (ocean -> getSize ()[0] / 2.0f)) * 2.0f ;
}


float steepSlope ( float x, float y )
{
  return (1.0f + x / (ocean -> getSize ()[0] / 2.0f)) * 10.0f ;
}


float stepFunction ( float x, float y )
{
  return (x < 0.0f ) ? 0.5f : 20000.0f ;
}


float twoBeaches ( float x, float y )
{
  return fabs ( sin(       x / ocean->getSize()[0]) *
                sin(2.0f * y / ocean->getSize()[1]) * 1.5 + 0.5 ) ;
}

ssgaWSDepthCallback depthFuncs [] =
{
  NULL,   /* Infinite depth */
  halfMeterEverywhere,
  oneMeterEverywhere,
  twoMeterEverywhere,
  gentleSlope,
  steepSlope,
  stepFunction,
  twoBeaches,
  NULL,
} ;

char *depthNames [] =
{
  "Infinite Depth",
  "Half Meter Deep",
  "One Meter Deep",
  "Two Meters Deep",
  "Gentle Slope",
  "Steep Slope",
  "Step Function",
  "Two Curved Beaches",
  NULL,
} ;

void depthSelectBox_cb ( puObject *ob )
{
  curr_depthfunc = ((puSelectBox *) ob) -> getCurrentItem () ;

  if ( curr_depthfunc < 0 )
    curr_depthfunc = 0 ;

  if ( curr_depthfunc >= (int)(sizeof(depthFuncs)/sizeof(ssgaWSDepthCallback)))
    curr_depthfunc = (int)(sizeof(depthFuncs)/sizeof(ssgaWSDepthCallback)) - 1 ;

  ocean -> setDepthCallback ( depthFuncs [ curr_depthfunc ] ) ;
}



void update_motion ()
{
  static ulClock ck ;
  static char s [ 128 ] ;

  ck . update () ;

  double t = ck . getAbsTime   () ;
  float dt = ck . getDeltaTime () ;

  ocean -> setWindDirn ( 25.0 * sin ( t / 100.0 ) ) ;
  ocean -> updateAnimation ( t ) ;

  fountain -> update ( dt ) ;

  dt = ck . getDeltaTime () ;

  sprintf ( s, "CalcTime=%1.1fms", dt * 1000.0 ) ;
  timeText->setLabel ( s ) ;
  sgCoord tptpos ;

  sgSetCoord ( & tptpos, 0.0f,  0.0f, 0.6f, t * 60, 0.0f, 0.0f ) ;
  ssgSetCamera ( & campos ) ;
  teapot  -> setTransform ( & tptpos ) ;
}



/*
  The GLUT window reshape event
*/

static void reshape ( int w, int h )
{
  glViewport ( 0, 0, w, h ) ;
}



/*
  The GLUT keyboard/mouse events
*/


static void keyboard ( unsigned char key, int, int )
{
  if ( ! puKeyboard ( key, PU_DOWN ) )
  {
    switch ( key )
    {
      case ' ' : displayGUI = ! displayGUI ; break ;

      case 0x03 : exit ( 0 ) ;

      default : displayGUI = ! displayGUI ; break ;
    }
  }
}


static void specialfn ( int key, int x, int y )
{
  puKeyboard ( key + PU_KEY_GLUT_SPECIAL_OFFSET, PU_DOWN ) ;
}


static void motionfn ( int x, int y )
{
  if ( displayGUI )
    puMouse ( x, y ) ;
}

static void mousefn ( int button, int updown, int x, int y )
{
  if ( displayGUI )
    puMouse ( button, updown, x, y ) ;
}



/*
  The GLUT redraw event
*/

void redraw ()
{
  update_motion () ;

  glClear  ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

  if ( wireframe )
    glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE ) ;
  else
    glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL ) ;

  ssgCullAndDraw ( scene ) ;

  glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL ) ;

  if ( displayGUI )
    puDisplay () ;

  glutPostRedisplay () ;
  glutSwapBuffers () ;
}



void init_graphics ()
{
  int   fake_argc = 1 ;
  char *fake_argv[3] ;
  fake_argv[0] = "ssgExample" ;
  fake_argv[1] = "Simple Scene Graph : Example Program." ;
  fake_argv[2] = NULL ;

  /*
    Initialise GLUT
  */

  glutInitWindowPosition ( 0, 0 ) ;
  glutInitWindowSize     ( 640, 480 ) ;
  glutInit               ( &fake_argc, fake_argv ) ;
  glutInitDisplayMode    ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ) ;
  glutCreateWindow       ( fake_argv[1] ) ;
  glutDisplayFunc        ( redraw   ) ;
  glutReshapeFunc        ( reshape  ) ;
  glutKeyboardFunc       ( keyboard ) ;
  glutSpecialFunc        ( specialfn ) ;
  glutMouseFunc          ( mousefn   ) ;
  glutMotionFunc         ( motionfn  ) ;
  glutPassiveMotionFunc  ( motionfn  ) ;

  puInit  () ;
  ssgInit () ;

  /*
    Some basic OpenGL setup
  */

  glClearColor ( 0.2f, 0.7f, 1.0f, 1.0f ) ;
  glEnable ( GL_DEPTH_TEST ) ;

  /*
    Set up the viewing parameters
  */

  ssgSetFOV     ( 60.0f, 0.0f ) ;
  ssgSetNearFar ( 1.0f, 700.0f ) ;

  /*
    Set up the Sun.
  */

  ssgGetLight ( 0 ) -> setPosition ( sunpos ) ;
}


/*
  Particle system definitions for the fountain.
*/

#define SPS ssgaParticleSystem  /* Too much typing! */                          

static void droplet_create ( SPS *, int, ssgaParticle *p )
{
  float c = ((float)(rand()%100)/100.0f) * (256.0f-163.0f)/255.0f ;

  sgSetVec4 ( p -> col, 96.0f/255.0f+c, 147.0f/255.0f+c, 163.0f/255.0f+c, 0.5);
  sgSetVec3 ( p -> pos, -2.4, -0.1, 1.9 ) ;
  sgSetVec3 ( p -> vel, 
             -(float)(rand()%1000)/200.0f,
              (float)(rand()%1000 - 500)/400.0f,
              (float)(rand()%1000)/1000.0f + 3.0f ) ;
  sgAddScaledVec3 ( p -> pos, p -> vel, (float)(rand()%1000)/20000.0f ) ;
  sgSetVec3 ( p -> acc, 0, 0, -9.8 ) ;
  p -> time_to_live = 1 ;
}


void init_states ()
{
  plinth_state = new ssgSimpleState () ;
  plinth_state -> setTexture        ( "data/pavement.rgb" ) ;
  plinth_state -> enable            ( GL_TEXTURE_2D ) ;
  plinth_state -> setShadeModel     ( GL_SMOOTH ) ;
  plinth_state -> enable            ( GL_CULL_FACE ) ;
  plinth_state -> enable            ( GL_BLEND ) ;
  plinth_state -> enable            ( GL_LIGHTING ) ;
  plinth_state -> setColourMaterial ( GL_AMBIENT_AND_DIFFUSE ) ;
  plinth_state -> setMaterial       ( GL_EMISSION, 0, 0, 0, 1 ) ;
  plinth_state -> setMaterial       ( GL_SPECULAR, 1, 1, 1, 1 ) ;
  plinth_state -> setShininess      ( 2 ) ;

  teapot_state = new ssgSimpleState () ;
  teapot_state -> setTexture        ( "data/pattern.rgb" ) ;
  teapot_state -> enable            ( GL_TEXTURE_2D ) ;
  teapot_state -> setShadeModel     ( GL_SMOOTH ) ;
  teapot_state -> enable            ( GL_CULL_FACE ) ;
  teapot_state -> enable            ( GL_BLEND ) ;
  teapot_state -> enable            ( GL_LIGHTING ) ;
  teapot_state -> setColourMaterial ( GL_AMBIENT_AND_DIFFUSE ) ;
  teapot_state -> setMaterial       ( GL_EMISSION, 0, 0, 0, 1 ) ;
  teapot_state -> setMaterial       ( GL_SPECULAR, 1, 1, 1, 1 ) ;
  teapot_state -> setShininess      ( 2 ) ;

  sea_state = new ssgSimpleState () ;
  sea_state -> setTexture        ( "data/ocean.rgb" ) ;
  sea_state -> setTranslucent    () ;
  sea_state -> enable            ( GL_TEXTURE_2D ) ;
  sea_state -> setShadeModel     ( GL_SMOOTH ) ;
  sea_state -> enable            ( GL_CULL_FACE ) ;
  sea_state -> enable            ( GL_BLEND ) ;
  sea_state -> enable            ( GL_LIGHTING ) ;
  sea_state -> setColourMaterial ( GL_AMBIENT_AND_DIFFUSE ) ;
  sea_state -> setMaterial       ( GL_EMISSION, 0, 0, 0, 1 ) ;
  sea_state -> setMaterial       ( GL_SPECULAR, 1, 1, 1, 1 ) ;
  sea_state -> setShininess      (  5 ) ;

  splash_state = new ssgSimpleState () ;
  splash_state -> setTexture        ( "data/droplet.rgb" ) ;
  splash_state -> setTranslucent    () ;
  splash_state -> enable            ( GL_TEXTURE_2D ) ;
  splash_state -> setShadeModel     ( GL_SMOOTH ) ;
  splash_state -> enable            ( GL_CULL_FACE ) ;
  splash_state -> enable            ( GL_BLEND ) ;
  splash_state -> enable            ( GL_LIGHTING ) ;
  splash_state -> setColourMaterial ( GL_EMISSION ) ;
  splash_state -> setMaterial       ( GL_AMBIENT, 0, 0, 0, 1 ) ;
  splash_state -> setMaterial       ( GL_DIFFUSE, 0, 0, 0, 1 ) ;
  splash_state -> setMaterial       ( GL_SPECULAR, 0, 0, 0, 1 ) ;
  splash_state -> setShininess      (  0 ) ;
}




void load_database ()
{
  /* Set up the path to the data files */

  ssgModelPath   ( "data" ) ;
  ssgTexturePath ( "data" ) ;

  /* Load the states */

  init_states () ;

  sgVec4  TRANSLUCENT_WHITE  = { 1.0, 1.0, 1.0, 0.8 } ;
  sgVec3  pos    = { 0, 0, 0 } ;
  sgCoord pedpos = { { 0, 0, -1.5 }, { 0, 0, 0 } } ;

  /* Create a the scene content.  */

  fountain = new ssgaParticleSystem ( 1000, 100, 500, TRUE,
                                      0.2, 1000,
                                      droplet_create ) ;
  fountain -> setState ( splash_state ) ;

  /* Set up some interesting defaults. */

  trains[0] . setSpeed      (  3.1f ) ;
  trains[0] . setLength     (  0.8f ) ;
  trains[0] . setLambda     (  0.6f ) ;
  trains[0] . setHeading    ( 47.0f ) ;
  trains[0] . setWaveHeight (  0.2f ) ;

  trains[1] . setSpeed      (  4.6f ) ;
  trains[1] . setLength     (  0.8f ) ;
  trains[1] . setLambda     (  1.0f ) ;
  trains[1] . setHeading    ( 36.0f ) ;
  trains[1] . setWaveHeight (  0.1f ) ;

  trains[2] . setSpeed      (  8.5f ) ;
  trains[2] . setLength     (  0.6f ) ;
  trains[2] . setLambda     (  1.0f ) ;
  trains[2] . setHeading    ( 65.0f ) ;
  trains[2] . setWaveHeight (  0.1f ) ;

  ocean   =  new ssgaWaveSystem ( 10000 ) ;
  ocean   -> setColour        ( TRANSLUCENT_WHITE ) ;
  ocean   -> setSize          ( 100 ) ;
  ocean   -> setTexScale      ( 3, 3 ) ;
  ocean   -> setCenter        ( pos ) ;
  ocean   -> setDepthCallback ( depthFuncs [ curr_depthfunc ] ) ;
  ocean   -> setKidState      ( sea_state ) ;
  ocean   -> setWindSpeed     ( 10.0f ) ;
  ocean   -> setWaveTrain     ( 0, & trains[0] ) ;
  ocean   -> setWaveTrain     ( 1, & trains[1] ) ;
  ocean   -> setWaveTrain     ( 2, & trains[2] ) ;

  ped_obj =  new ssgaCube     () ;
  ped_obj -> setSize          ( 4 ) ;
  ped_obj -> setKidState      ( plinth_state ) ;
  ped_obj -> regenerate       () ;

  tpt_obj =  new ssgaTeapot   ( 1000 ) ;
  tpt_obj -> setSize          ( 2 ) ;
  tpt_obj -> setKidState      ( teapot_state ) ;
  tpt_obj -> regenerate       () ;

  /* Build the scene graph */

  teapot   =  new ssgTransform ;
  teapot   -> addKid          ( tpt_obj  ) ;
  teapot   -> addKid          ( fountain ) ;

  pedestal =  new ssgTransform ;
  pedestal -> setTransform    ( & pedpos ) ;
  pedestal -> addKid          ( ped_obj  ) ;

  sun_obj  = new ssgaLensFlare () ;

  sun      = new ssgTransform ;
  sun      -> setTransform    ( sunpos ) ;
  sun      -> addKid          ( sun_obj  ) ;

  scene    =  new ssgRoot ;
  scene    -> addKid          ( ocean    ) ;
  scene    -> addKid          ( pedestal ) ;
  scene    -> addKid          ( teapot   ) ;
  scene    -> addKid          ( sun      ) ;
}


void init_gui ()
{
  static puFont     *sorority ;
  static fntTexFont *fnt      ;

  fnt      = new fntTexFont () ;
  fnt     -> load ( "data/sorority.txf" ) ;
  sorority = new puFont ( fnt, 12 ) ;

  puSetDefaultFonts        ( *sorority, *sorority ) ;
  puSetDefaultStyle        ( PUSTYLE_SMALL_SHADED ) ;
  puSetDefaultColourScheme ( 0.2, 0.5, 0.2, 0.5 ) ;

  puGroup *window_group = new puGroup ( 0, 0 ) ;

  trainLengthSlider = new puSlider ( 200, GUI_BASE+28, 90, false, 20 ) ;
  trainLengthSlider->setMaxValue   ( 20.0f ) ;
  trainLengthSlider->setMinValue   (  0.1f ) ;
  trainLengthSlider->setStepSize   (  0.1f ) ;
  trainLengthSlider->setCBMode     ( PUSLIDER_ALWAYS ) ;
  trainLengthSlider->setCallback   ( trainLengthSlider_cb ) ;
  trainLengthSlider->setLabel      ( "Wave Length" ) ;
  trainLengthSlider->setLabelPlace ( PUPLACE_CENTERED_LEFT ) ;
  trainLengthSlider->setColour( PUCOL_LABEL, FONT_COLOUR ) ;
  
  trainSpeedSlider = new puSlider  ( 200, GUI_BASE+56, 90, false, 20 ) ;
  trainSpeedSlider->setMaxValue    ( 20.0f ) ;
  trainSpeedSlider->setMinValue    ( 0.0f ) ;
  trainSpeedSlider->setStepSize    ( 0.1f ) ;
  trainSpeedSlider->setCBMode      ( PUSLIDER_ALWAYS ) ;
  trainSpeedSlider->setCallback    ( trainSpeedSlider_cb ) ;
  trainSpeedSlider->setLabel       ( "Wave Speed" ) ;
  trainSpeedSlider->setLabelPlace  ( PUPLACE_CENTERED_LEFT ) ;
  trainSpeedSlider->setColour( PUCOL_LABEL, FONT_COLOUR ) ;
  
  trainLambdaSlider = new puSlider ( 300, GUI_BASE+28, 90, false, 20 ) ;
  trainLambdaSlider->setMaxValue   ( 2.0f ) ;
  trainLambdaSlider->setMinValue   ( 0.0f ) ;
  trainLambdaSlider->setStepSize   ( 0.1f ) ;
  trainLambdaSlider->setCBMode     ( PUSLIDER_ALWAYS ) ;
  trainLambdaSlider->setCallback   ( trainLambdaSlider_cb ) ;
  trainLambdaSlider->setLabel      ( "Wave Curl" ) ;
  trainLambdaSlider->setLabelPlace ( PUPLACE_CENTERED_RIGHT ) ;
  trainLambdaSlider->setColour( PUCOL_LABEL, FONT_COLOUR ) ;
  
  trainHeightSlider = new puSlider ( 300, GUI_BASE+56, 90, false, 20 ) ;
  trainHeightSlider->setMaxValue   ( 5.0f ) ;
  trainHeightSlider->setMinValue   ( 0.0f ) ;
  trainHeightSlider->setStepSize   ( 0.1f ) ;
  trainHeightSlider->setCBMode     ( PUSLIDER_ALWAYS ) ;
  trainHeightSlider->setCallback   ( trainHeightSlider_cb ) ;
  trainHeightSlider->setLabel      ( "Wave Height (meters)" ) ;
  trainHeightSlider->setLabelPlace ( PUPLACE_CENTERED_RIGHT ) ;
  trainHeightSlider->setColour( PUCOL_LABEL, FONT_COLOUR ) ;
  
  trainEnableButton = new puButton ( 200, GUI_BASE+84, " " ) ;
  trainEnableButton->setStyle      ( PUSTYLE_RADIO ) ;
  trainEnableButton->setCallback   ( trainEnableButton_cb ) ;
  trainEnableButton->setLabel      ( "Enable this Wave Train" ) ;
  trainEnableButton->setLabelPlace ( PUPLACE_CENTERED_LEFT ) ;
  trainEnableButton->setColour( PUCOL_LABEL, FONT_COLOUR ) ;
  
  trainDisableButton = new puOneShot( 450, GUI_BASE+109,
                                                "Disable All WaveTrains" ) ;
  trainDisableButton->setCallback  ( trainDisableButton_cb ) ;
  
  trainHeadingDial = new puDial    ( 300, GUI_BASE+82, 50 ) ;
  trainHeadingDial->setWrap        ( 1 ) ;
  trainHeadingDial->setMaxValue    ( 360 ) ;
  trainHeadingDial->setMinValue    ( 0 ) ;
  trainHeadingDial->setStepSize    ( 1 ) ;
  trainHeadingDial->setCBMode      ( PUSLIDER_ALWAYS ) ;
  trainHeadingDial->setCallback    ( trainHeadingDial_cb ) ;
  trainHeadingDial->setLabel       ( "Wave Direction" ) ;
  trainHeadingDial->setLabelPlace  ( PUPLACE_CENTERED_RIGHT ) ;
  trainHeadingDial->setColour( PUCOL_LABEL, FONT_COLOUR ) ;
  trainHeadingDial->setLegendPlace ( PUPLACE_BOTTOM_CENTERED ) ;
  
  trainSelectBox = new puSelectBox ( 200, GUI_BASE+109, 300, GUI_BASE+139,
                                     trainNameList ) ;
  trainSelectBox->setCallback      ( trainSelectBox_cb ) ;
  trainSelectBox->setCurrentItem   ( 0 ) ;
  trainSelectBox->setLabel         ( "Edit Wave Train Number" ) ;
  trainSelectBox->setLabelPlace    ( PUPLACE_CENTERED_LEFT ) ;
  trainSelectBox->setColour( PUCOL_LABEL, FONT_COLOUR ) ;
  
  /* Set everything up on the first time around */
  trainSelectBox_cb ( trainSelectBox ) ;

  depthSelectBox = new puSelectBox ( 200, GUI_BASE, 400, GUI_BASE+20,
                                     depthNames ) ;
  depthSelectBox->setCallback      ( depthSelectBox_cb ) ;
  depthSelectBox->setCurrentItem   ( 0 ) ;
  depthSelectBox->setLabel         ( "Water Depth" ) ;
  depthSelectBox->setLabelPlace    ( PUPLACE_CENTERED_LEFT ) ;
  depthSelectBox->setColour( PUCOL_LABEL, FONT_COLOUR ) ;
  
  /* Set everything up on the first time around */
  depthSelectBox_cb ( depthSelectBox ) ;

  viewHeadingDial = new puDial (  50, VIEW_GUI_BASE, 50 ) ;
  viewHeadingDial->setValue       ( 0.0f ) ;
  viewHeadingDial->setWrap        ( 1 ) ;
  viewHeadingDial->setMaxValue    ( 360 ) ;
  viewHeadingDial->setMinValue    ( 0 ) ;
  viewHeadingDial->setStepSize    ( 0 ) ;
  viewHeadingDial->setCBMode      ( PUSLIDER_ALWAYS ) ;
  viewHeadingDial->setCallback    ( viewHeadingDial_cb ) ;
  viewHeadingDial->setLabel       ( "Pan" ) ;
  viewHeadingDial->setLabelPlace  ( PUPLACE_BOTTOM_CENTERED ) ;
  viewHeadingDial->setColour( PUCOL_LABEL, FONT_COLOUR ) ;

  viewPitchDial  = new puDial ( 100, VIEW_GUI_BASE, 50 ) ;
  viewPitchDial  ->setValue       ( -45.0f ) ;
  viewPitchDial  ->setWrap        ( 1 ) ;
  viewPitchDial  ->setMaxValue    ( 360 ) ;
  viewPitchDial  ->setMinValue    ( 0 ) ;
  viewPitchDial  ->setStepSize    ( 0 ) ;
  viewPitchDial  ->setCBMode      ( PUSLIDER_ALWAYS ) ;
  viewPitchDial  ->setCallback    ( viewPitchDial_cb ) ;
  viewPitchDial  ->setLabel       ( "Tilt" ) ;
  viewPitchDial  ->setLabelPlace  ( PUPLACE_BOTTOM_CENTERED ) ;
  viewPitchDial  ->setColour( PUCOL_LABEL, FONT_COLOUR ) ;

  viewRangeSlider = new puSlider ( 150, VIEW_GUI_BASE, 90, false, 20 ) ;
  viewRangeSlider->setValue      ( 25.0f ) ;
  viewRangeSlider->setMaxValue   ( 150.0f ) ;
  viewRangeSlider->setMinValue   ( 0.0f ) ;
  viewRangeSlider->setStepSize   ( 0 ) ;
  viewRangeSlider->setCBMode     ( PUSLIDER_ALWAYS ) ;
  viewRangeSlider->setCallback   ( viewRangeSlider_cb ) ;
  viewRangeSlider->setLabel      ( "Range" ) ;
  viewRangeSlider->setLabelPlace ( PUPLACE_BOTTOM_CENTERED ) ;
  viewRangeSlider->setColour( PUCOL_LABEL, FONT_COLOUR ) ;
  
  viewWireframeButton= new puButton ( 400, VIEW_GUI_BASE, "Wireframe" ) ;
  viewWireframeButton->setCallback  ( viewWireframeButton_cb ) ;
  viewWireframeButton->setValue     ( FALSE ) ;

  viewEnvMapButton= new puButton ( 300, VIEW_GUI_BASE, "Env.Map" ) ;
  viewEnvMapButton->setCallback  ( viewEnvMapButton_cb ) ;
  viewEnvMapButton->setValue     ( FALSE ) ;

  writeButton = new puOneShot ( 400, VIEW_GUI_BASE + 30, "Write C++" ) ;
  writeButton->setCallback  ( writeButton_cb ) ;
  writeButton->setValue     ( FALSE ) ;

  waveTextureSlider = new puSlider ( 500, VIEW_GUI_BASE   , 90, false, 20 ) ;
  waveTextureSlider->setValue      ( 1.0f ) ;
  waveTextureSlider->setMaxValue   ( 50.0f ) ;
  waveTextureSlider->setMinValue   ( 0.01f ) ;
  waveTextureSlider->setStepSize   ( 0 ) ;
  waveTextureSlider->setCBMode     ( PUSLIDER_ALWAYS ) ;
  waveTextureSlider->setCallback   ( waveTextureSlider_cb ) ;
  waveTextureSlider->setLabel      ( "Texture" ) ;
  waveTextureSlider->setLabelPlace ( PUPLACE_CENTERED_RIGHT ) ;
  waveTextureSlider->setColour( PUCOL_LABEL, FONT_COLOUR ) ;
  
  waveSizeSlider = new puSlider ( 500, VIEW_GUI_BASE+30, 90, false, 20 ) ;
  waveSizeSlider->setValue      ( 25.0f ) ;
  waveSizeSlider->setMaxValue   ( 500.0f ) ;
  waveSizeSlider->setMinValue   (  10.0f ) ;
  waveSizeSlider->setStepSize   ( 0 ) ;
  waveSizeSlider->setCBMode     ( PUSLIDER_ALWAYS ) ;
  waveSizeSlider->setCallback   ( waveSizeSlider_cb ) ;
  waveSizeSlider->setLabel      ( "Size" ) ;
  waveSizeSlider->setLabelPlace ( PUPLACE_CENTERED_RIGHT ) ;
  waveSizeSlider->setColour( PUCOL_LABEL, FONT_COLOUR ) ;

  wavePolycountSlider= new puSlider ( 500, VIEW_GUI_BASE+60, 90, false, 20 ) ;
  wavePolycountSlider->setValue      ( 10000 ) ;
  wavePolycountSlider->setMaxValue   ( 50000 ) ;
  wavePolycountSlider->setMinValue   (  4000 ) ;
  wavePolycountSlider->setStepSize   ( 0 ) ;
  wavePolycountSlider->setCBMode     ( PUSLIDER_ALWAYS ) ;
  wavePolycountSlider->setCallback   ( wavePolycountSlider_cb ) ;
  wavePolycountSlider->setLabel      ( "Polygons" ) ;
  wavePolycountSlider->setLabelPlace ( PUPLACE_CENTERED_RIGHT ) ;
  wavePolycountSlider->setColour( PUCOL_LABEL, FONT_COLOUR ) ;
  
  timeText = new puText ( 500, VIEW_GUI_BASE+80 ) ;
  timeText->setColour( PUCOL_LABEL, FONT_COLOUR ) ;

  window_group->close () ;
}

/*
  The works.
*/

int main ( int, char ** )
{
  init_graphics () ;
  load_database () ;
  init_gui      () ;
  glutMainLoop  () ;
  return 0 ;
}



