

#include "puLocal.h"

void puFrame::draw ( int dx, int dy )
{
  if ( !visible || ( window != puGetWindow () ) ) return ;

  abox . draw ( dx, dy, style, colour, FALSE ) ;

  /* If greyed out then halve the opacity when drawing the label and legend */

  if ( active )
    glColor4fv ( colour [ PUCOL_LEGEND ] ) ;
  else
    glColor4f ( colour [ PUCOL_LEGEND ][0],
                colour [ PUCOL_LEGEND ][1],
                colour [ PUCOL_LEGEND ][2],
                colour [ PUCOL_LEGEND ][3] / 2.0f ) ; /* 50% more transparent */

  if ( r_cb )
    r_cb ( this, dx, dy, render_data ) ;
  else
  {
    int xx = ( abox.max[0] - abox.min[0] - puGetStringWidth ( legendFont, legend ) ) / 2 ;

    puDrawString ( legendFont, legend,
                  dx + abox.min[0] + xx,
                  dy + abox.min[1] + puGetStringDescender ( legendFont ) + PUSTR_BGAP ) ;
  }

  draw_label ( dx, dy ) ;
}


