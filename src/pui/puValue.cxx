/*
     PLIB - A Suite of Portable Game Libraries
     Copyright (C) 2001  Steve Baker
 
     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Library General Public
     License as published by the Free Software Foundation; either
     version 2 of the License, or (at your option) any later version.
 
     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Library General Public License for more details.
 
     You should have received a copy of the GNU Library General Public
     License along with this library; if not, write to the Free
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 
     For further information visit http://plib.sourceforge.net

     $Id$
*/


#include "puLocal.h"

void puValue::re_eval ()
{
  if ( res_floater ) setValue ( *res_floater ) ; else
  if ( res_integer ) setValue ( *res_integer ) ; else
  if ( res_string  ) setValue ( res_string ) ;
}

void puValue::update_res ()
{
  if ( res_integer ) *res_integer = integer ;
  if ( res_floater ) *res_floater = floater ;
  if ( res_string  ) strcpy ( res_string, string ) ;
}


