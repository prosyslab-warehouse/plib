
#include "puLocal.h"

void puText::draw ( int dx, int dy )
{
  if ( !visible || ( window != puGetWindow () ) ) return ;

  if ( r_cb )
    r_cb ( this, render_data ) ;

  draw_label ( dx, dy ) ;
}

