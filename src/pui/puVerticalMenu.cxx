
#include "puLocal.h"

extern void puMenuBar_drop_down_the_menu ( puObject *b );

void puVerticalMenu::add_submenu ( char *str, char *items[], puCallback cb[] )
{
  int w, h ;
  getSize ( &w, &h ) ;

  if (items[1] != NULL)
  {
    
    // More than one entry in the submenu, set the callback to drop down the menu
    // and make the mouse activation edge up or down.  Create the popup menu                                                      
    // and load the items into it.

    puOneShot    *b = new puOneShot ( w+10, 0, str ) ;
    b -> setStyle ( PUSTYLE_SPECIAL_UNDERLINED ) ;
    b -> setColourScheme ( colour[PUCOL_FOREGROUND][0],
                           colour[PUCOL_FOREGROUND][1],
                           colour[PUCOL_FOREGROUND][2],
                           colour[PUCOL_FOREGROUND][3] ) ;

    b -> setCallback ( puMenuBar_drop_down_the_menu ) ;
    b -> setActiveDirn ( PU_UP_AND_DOWN ) ;

    puPopupMenu *p = new puPopupMenu ( w+10, 0 ) ;      

    b -> setUserData ( p ) ;

    for ( int i = 0 ; items[i] != NULL ; i++ )
      p -> add_item ( items[i], cb[i] ) ;

    p->close () ;
  }
  else
  {
    // Only one entry in the submenu, set the callback to the entry's callback
    // and make the mouse activation edge up only.  Set the user data to null.

    puOneShot    *b = new puOneShot ( w+10, 0, str ) ;
    b -> setStyle ( PUSTYLE_SPECIAL_UNDERLINED ) ;
    b -> setColourScheme ( colour[PUCOL_FOREGROUND][0],
                           colour[PUCOL_FOREGROUND][1],
                           colour[PUCOL_FOREGROUND][2],
                           colour[PUCOL_FOREGROUND][3] ) ;

    b -> setCallback ( cb[0] );
    b -> setActiveDirn ( PU_UP ) ;

    b -> setUserData ( NULL ) ;
  }

  recalc_bbox () ;
}

void puVerticalMenu::close (void)
{
  puGroup::close () ;

  if ( dlist == NULL )
    return ;

  int height = 0 ;
  int fullheight;
  puObject *ob ;

  // Use alternate objects - which gets the puOneShot/puPopupMenu pairs
  // If the "puOneShot" has a user data of null, it is a single entry and its      
  // puPopup item is missing.

  for ( ob = dlist ; ob != NULL ; ob = ob -> next )
  {
    int w, h ;

    // Reposition the button so it looks nice

    ob -> getSize ( &w, &h ) ;
    ob -> setPosition ( 0, height ) ;

    if (ob -> getUserData () )     
    {

      ob = ob -> next ;

      // Reposition the submenu so it sits under the button

      int w2, h2 ;
      ob -> getSize ( &w2, &h2 ) ;
      ob -> setPosition ( 0, height-h2 ) ;
    }

    // Next please!
    fullheight = height;
    height -= h ;
  }

  // Now step through the list again, adding the final value of "height" to
  // all the vertical positions so that the (0,0) point will be the lower left-
  // hand corner.

  for ( ob = dlist ; ob != NULL ; ob = ob -> next )
  {
    int x, y ;

    // Reposition the button so it looks nice

    ob -> getPosition ( &x, &y ) ;
    ob -> setPosition ( x, y-fullheight ) ;

    if (ob -> getUserData () )      
    {

      ob = ob -> next ;        

      // Reposition the submenu so it sits under the button

      ob -> getPosition ( &x, &y ) ;
      ob -> setPosition ( x, y-fullheight ) ;
    }
  }

  recalc_bbox () ;

  // If in its own window (its window number is not the same as its parent's
  // window number), then resize the window

  if (window != parent -> getWindow () )
  {
    int temp_window = puGetWindow () ;
    glutSetWindow ( window ) ;
    glutReshapeWindow ( bbox.max[0] - bbox.min[0], bbox.max[1] - bbox.min[1] ) ;
    setPosition ( 0, 0 ) ;

    glutSetWindow ( temp_window ) ;
  }

}


