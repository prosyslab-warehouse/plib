#ifndef _INCLUDED_SSGCONF_H_
#define _INCLUDED_SSGCONF_H_

/* ssgconf.h - machine configurable file for libssg */

#define _SSG_USE_PICK   1
#define _SSG_USE_DLIST  1

/* The following defines give you the ability to remove code from the
 * library that you will not be using.  I wish I could figure out how to
 * automate this, but I can't do that without making it seriously hard
 * on the users.  So if you are not using an ability, change the #define
 * to and #undef, and that part of the library will not be compiled.  If
 * your linker can't find a function, you may want to make sure the
 * ability is defined here.
 */

/*
  For optional use of PNG textures, download the glpng library from
  http://www.wyatt100.freeserve.co.uk/download.htm and un-comment
  the following line.
 */

#undef  SSG_LOAD_PNG_SUPPORTED

#define SSG_LOAD_SGI_SUPPORTED
#define SSG_LOAD_TGA_SUPPORTED
#define SSG_LOAD_BMP_SUPPORTED
#define SSG_LOAD_MDL_SUPPORTED

#endif
