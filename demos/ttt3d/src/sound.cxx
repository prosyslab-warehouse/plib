
#include "p3d.h"

struct Sound
{
   const char *fname ;
   slSample *s ;
} ;


static Sound sfx [] =
{
  { "wavs/ahooga.wav"	, NULL },
  { "wavs/clap.wav"	, NULL },
  { "wavs/frog.wav"	, NULL },
  { "wavs/glasbk.wav"	, NULL },
  { "wavs/pop.wav"	, NULL },
  { "wavs/ugh.wav"	, NULL },
  { "wavs/who_else.wav" , NULL },
  { NULL, NULL }
} ;

static int music_off = FALSE ;
static int   sfx_off = FALSE ;

void SoundSystem::disable_music ()
{
  sched -> stopMusic () ;
  sched -> update    () ;  /* Ugh! Nasty Kludge! */
  sched -> update    () ;  /* Ugh! Nasty Kludge! */

  music_off = TRUE  ;
}


void SoundSystem::change_track ( const char *fname )
{
  if ( fname == NULL )
    fname = "" ;

  if ( strcmp ( fname, current_track ) != 0  )
  {
    strcpy ( current_track, fname ) ;

    if ( ! music_off )
      enable_music  () ;
  }
}

void SoundSystem::enable_music ()
{
  sched -> stopMusic () ;

  if ( current_track [ 0 ] != '\0' )
    sched -> loopMusic ( current_track ) ;
 
  music_off = FALSE ;
}


void SoundSystem::disable_sfx () { sfx_off = TRUE  ; }
void SoundSystem:: enable_sfx () { sfx_off = FALSE ; }



void SoundSystem::playSfx ( int sfx_num )
{
  if ( ! sfx_off )
    sched -> playSample ( sfx[sfx_num].s, 1, SL_SAMPLE_MUTE, 2, NULL ) ;
}


SoundSystem::SoundSystem ()
{
  sched = new slScheduler ;

  setSafetyMargin () ;

  for ( Sound *currsfx = &(sfx[0]) ; currsfx -> fname != NULL ; currsfx++ )
    currsfx -> s  = new slSample ( currsfx -> fname, sched ) ;

  enable_sfx   () ;
  change_track ( "" ) ;
  enable_music () ;
}


void SoundSystem::update ()
{
/*
  Comment this next line out if the
  sound causes big glitches on your
  IRIX machine!
*/

  sched -> update () ;
}


