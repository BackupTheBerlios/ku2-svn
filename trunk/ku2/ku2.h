/***************************************************************************
 *            ku2.h
 *
 *  Sun Aug 27 21:22:46 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef KU__KU2_H__
#define KU__KU2_H__

#if defined(NO_GETTEXT) && defined(USE_GETTEXT)
	#warning both NO_GETTEXT and USE_GETTEXT are defined, not using GETTEXT
	#undef USE_GETTEXT
#endif	// NO_GETTEXT && USE_GETTEXT

#ifdef USE_GETTEXT
	#define INCLUDE_GETTEXT #include <libintl.h>
#else
	#define INCLUDE_GETTEXT
	#define gettext( ... )
#endif

#endif
