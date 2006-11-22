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
#include "ku2/types.h"

//! Image.
typedef
struct
{
	uint32_t format;
					//!< GL pixel format.
	uint w,			//!< Image width.
		h;			//!< Image height.
	void *pixels;	//!< Pixel data.
}	gfx_image_t;

//! Assign an image resource type and add resources from file.
/*!
	Assigns an image resource type (see \ref res.c) and adds resources from file.
	\param resource_type Resource ID.
	\param resfile File with list of resources.
	\retval KE_NONE No error.
	\return res_assign(), cfg_open(), cfg_query() and cfg_process() errors.
*/
kucode_t gfx_add_image_resources( int resource_type, const char *resfile );

#ifdef __cplusplus
}
#endif
#endif
