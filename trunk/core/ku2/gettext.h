/***************************************************************************
 *            gettext.h
 *
 *  Mon Aug 28 12:02:25 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Gettext header.
	
	This header contains includes needed by GNU Gettext.
	\author J. Anton
	\date Mon Aug 28 12:24:34 2006
	\version 1.1.0
*/

#ifndef KU__GETTEXT_H__
#define KU__GETTEXT_H__
/*! \cond skip */

#if defined(NO_LIBINTL_H) && defined(USE_GETTEXT)
	#undef USE_GETTEXT
#endif	// NO_GETTEXT && USE_GETTEXT

#ifdef USE_GETTEXT
	#include <libintl.h>
#else
	#define gettext( str ) str
#endif

#define _ gettext

/*! \endcond */
#endif
