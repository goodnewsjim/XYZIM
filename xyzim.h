	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/

/*
 * xyzim.h
 *
 * Master include file for Xyzim Animation Maker.
 * All other files should include this one.
 */

#ifndef __xyzim_h
#define __xyzim_h

/* We expect to use Crystal Space everywhere. */
#include <cssysdef.h>

/* MSVC users do not run configure, so use special MSVC configuration file. */
#if defined(CS_WIN32_CSCONFIG)
#include "config-msvc.h"
#else
#include "config.h"
#endif

/* Insert additional project-specific declarations here. */

#endif // __xyzim_h
