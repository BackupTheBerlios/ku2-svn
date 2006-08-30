/***************************************************************************
 *            gui.h
 *
 *  Tue Jun  6 11:29:11 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief GUI library.
	
	Functions for GUI management.
	\author J. Anton
	\date Wed Aug 30 12:08:32 2006
	\version 1.0.0
*/

#ifndef KU__GUI_H__
#define KU__GUI_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"
#include "list/list.h"

//! GUI object.
typedef
struct STRUCT_GUI_OBJ gui_obj_t;

//! Widget loading function.
/*!
	This function is used for creating and destroying GUI objects.
	Each widget has its own loading function, so this function determines
	the type of an object. It set-ups the widget related information.
	\param obj Pointer to allocated and set-up GUI object structure.
	\return \a KE_NONE if there were no error, else some error code is
	returned. For more details see widget related documentation.
*/
typedef
kucode_t (*gui_load_f)( gui_obj_t *obj );

//! Object status function.
/*!
	This function changes the object status.
	\param obj GUI object.
	\return \a KE_NONE if there were no error, else some error code is
	returned. For more details see widget related documentation.
*/
typedef
kucode_t (*gui_status_f)( gui_obj_t *obj );

//! Object dimension function.
typedef
kucode_t (*gui_dim_f)( gui_obj_t *obj, int x, int y, int w, int h );

//! Set/Get function.
/*!
	This function sets or gets the selected parameter to/from the widget.
	\param obj GUI object.
	\param param Parameter (attribute) to set or get.
	\param data Data to set or get.
	\return \a KE_NONE if there were no error, else some error code is
	returned. For more details see widget related documentation.
*/
typedef
kucode_t (*gui_sg_f)( gui_obj_t *obj, int param, void *data );

//! GUI object status.
typedef
enum
{
	GUI_NOTLOADED,	//!< Object is not loaded.
	GUI_ENABLED,	//!< Normal status.
	GUI_DISABLED,	//!< Objects is disabled.
	GUI_HIDDEN		//!< Objects is hidden (conseq. diabled).
}	gui_status_t;

//! GUI object.
struct STRUCT_GUI_OBJ
{
	gui_obj_t
		*parent;	//!< Parent object.
	list_t
		*children;	//!< Children list.
	gui_status_t
		status;		//!< Object status.

	int x,			//!< Relative object left side \a x.
		y,			//!< Relative object top size \a y.
		width,		//!< Object width.
		height,		//!< Object height.
		rx,			//!< Real object \a x.
		ry;			//!< Real object \a y.

	uint widget_sz;	//!< Size of the widget related information.

	gui_status_f
		load,		//!< Load a widget.
		uload,		//!< Unload a widget.
		enable,		//!< Enable a widget.
		disable,	//!< Disable a widget.
		show,		//!< Show a widget.
		hide;		//!< Hide a widget.

	gui_dim_f
		dim;		//!< Change the dimensions of a widget.

	gui_sg_f
		set,		//!< Set the attribute of a widget.
		get;		//!< Get the attribute of a widget.

	gui_load_f
		loadf,		//!< Function for loading a widget.
		uloadf;		//!< Function for unloading a widget.

	void *widget;	//!< Widget related information.
};

//! Initialize the GUI engine.
/*!
	Initializes the GUI engine. Other functions can be called only
	after this one is called.
	\retval KE_NONE No error.
	\retval KE_* abtree_create() errors.
	\sa gui_halt().
*/
kucode_t gui_init( void );

//! Halt the GUI engine.
/*!
	Halts the GUI engine. No other function can be called after
	this one is called, but gui_init().
	\return Always \a KE_NONE.
	\sa gui_init().
*/
kucode_t gui_halt( void );

//! Create a GUI object.
/*!
	Creates a GUI object (widget).
	\param loadf Widget loaging function.
	\param widget_sz Widget related information size.
	\return Created object, or \e NULL if there were an error and \ref
	kucode is set to the valid value: \n
	\a KE_MEMORY: Memory allocation has failed. \n
	gui_load_f() and abtree_ins() errors.
*/
gui_obj_t *gui_obj_create( gui_load_f loadf, uint widget_sz );

#if 0
gui_obj_t *gui_obj_clone( gui_obj_t *obj );
#endif

//! Delete a GUI object.
kucode_t gui_obj_delete( gui_obj_t *obj );

#ifdef __cplusplus
}
#endif
#endif
