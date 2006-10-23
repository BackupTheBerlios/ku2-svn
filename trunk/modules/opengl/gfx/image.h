/***************************************************************************
 *            image.h
 *
 *  Mon Oct 23 17:54:01 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Image control.
	
	Functions for image control.
	\author J. Anton
	\date Mon Oct 23 17:54:43 2006
	\version 1.0.0
*/

#ifndef KU__IMAGE_H__
#define KU__IMAGE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"

//! Assign an image resource type and add resources from file.
/*!
*/
kucode_t gfx_add_image_resources( int resource_type, const char *resfile );

#ifdef __cplusplus
}
#endif
#endif
