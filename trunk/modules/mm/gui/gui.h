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

#include "SDL.h"

#include "ku2/ecode.h"
#include "ku2/types.h"
#include "list/list.h"

//! GUI object.
typedef
struct STRUCT_GUI_OBJ gui_obj_t;

//! Widget callback function.
/*!
	This function is used to make a widget do some action.
	\param obj GUI object.
	\param data Function parameters.
	\return \a KE_NONE if there were no error, else some error code is
	returned. For more details see widget related documentation.
*/
typedef
kucode_t (*gui_cb_f)( gui_obj_t *obj, void *data );

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
kucode_t (*gui_dim_f)( gui_obj_t *obj );

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

//! Drawing function.
typedef
kucode_t (*gui_draw_f)( gui_obj_t *obj, int x, int y, int w, int h );

//! GUI event status.
typedef
enum
{
	GUIE_CRITICAL,	//!< A major error has accured. Break the procession.
	GUIE_DRAW,		//!< Event is managed end screen needs to be updated.
	GUIE_EAT,		//!< "Eat" an event (event is managed).
	GUIE_ERROR,		//!< A minor error has accured (event stays unmanaged).
	GUIE_LEAVE		//!< Leave an event (event is unmanaged).
}	gui_event_st;

//! Mouse event function.
typedef
gui_event_st (*gui_mouse_f)( gui_obj_t *obj, int x, int y, int z );

//! Keyboard event function.
typedef
gui_event_st (*gui_keyb_f)( gui_obj_t *obj, char ch );

//! GUI object status.
typedef
enum
{
	GUI_NOTLOADED,	//!< Object is not loaded.
	GUI_ENABLED,	//!< Normal status.
	GUI_DISABLED,	//!< Objects is disabled.
	GUI_HIDDEN		//!< Objects is hidden (conseq. diabled).
}	gui_status_t;

//! Flags for GUI object and gui_obj_create().
enum GUI_FLAGS
{
	GUI_ZFL = 0,	//!< Zero flag.
	GUI_KEEPCHILDREN = 1
					//!< Do not delete a children list even if it becomes empty.
};

//! Flags for fonts and text in the objects.
enum GUI_FONT_FLAGS
{
	GUI_FONT_ZERO = 0,
					//!< Zero flag.
	GUI_FONT_ALLIG_RIGHT = 1,
					//!< Right alligment, else left alligment.
	GUI_FONT_ALLIG_HCENTRE = 3,
					//!< Horizontal centre alligment.
	GUI_FONT_ALLIG_BOTTOM = 4,
					//!< Bottom alligment, else top alligment.
	GUI_FONT_ALLIG_VCENTRE = 12,
					//!< Vertical centre alligment.
	GUI_FONT_BOLD = 16,
					//!< Bold font.
	GUI_FONT_ITALIC = 32,
					//!< Italic font.
	GUI_FONT_ULINED = 64,
					//!< Underlined font.
	GUI_FONT_MULTILINE = 128,
					//!< Use multiline text, if needed.
	GUI_FONT_EXPAND = 256,
					//!< Expand text to the object width, unless the last line.
	GUI_FONT_SPLIT = 512
					//!< Split text into lines even at the middle of the words.
};

//! GUI object.
struct STRUCT_GUI_OBJ
{
	uint id;		//!< Object ID.

	gui_obj_t
		*parent;	//!< Parent object.
	list_t
		*children;	//!< Children list.
	gui_status_t
		status;		//!< Object status.
	ku_flag32_t
		flags;		//!< Object flags.

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
		hide;		//!< Hide a widget.

	gui_dim_f
		dim;		//!< Change the dimensions of a widget.

	gui_sg_f
		set,		//!< Set the attribute of a widget.
		get;		//!< Get the attribute of a widget.

	gui_load_f
		initf,		//!< Function for initializing a widget.
		destroyf;	//!< Function for destroying a widget.

	gui_mouse_f
		mon,		//!< Mouse move event handler.
		moff,		//!< Mouse off widget handler.
		mdown,		//!< Mouse button press handler.
		mup;		//!< Mouse button release handler.
	
	gui_keyb_f
		kdown,		//!< Key press handler.
		kup;		//!< Key release handler.

	gui_draw_f
		draw;		//!< Draw a widget.

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
	\param initf Widget initialization function.
	\param widget_sz Widget related information size.
	\param flags Creation flags (see \ref GUI_FLAGS).
	\return Created object, or \e NULL if there were an error and \ref
	kucode is set to the valid value: \n
	\a KE_MEMORY: Memory allocation has failed. \n
	gui_load_f() and abtree_ins() errors.
*/
gui_obj_t *gui_obj_create( gui_load_f initf, uint widget_sz, ku_flag32_t flags );

#if 0
gui_obj_t *gui_obj_clone( gui_obj_t *obj );
#endif

//! Delete a GUI object.
/*!
	Deletes a GUI object (widget) and its children, excluding it from the
	parent`s children list.
	\param obj Object to be deleted.
	\return Always \a KE_NONE.
*/
kucode_t gui_obj_delete( gui_obj_t *obj );

//! Move and place an object to the different place.
/*!
	Moves an objects to the different place (maybe as a child of other object)
	and resizes it.
	\param obj Object to be moved.
	\param host Host object where object \a obj will be placed.
	\param x Object left size \a x.
	\param y Object top side \a y.
	\param w Object width.
	\param h Object height.
	\retval KE_NONE No error.
	\retval KE_* dl_list_create() and dl_list_ins() errors.
	\note If \a w and \a h are zeros then the object is not resized. \n
	If \a host is NULL then the object becomes a root object.
*/
kucode_t gui_move( gui_obj_t *obj, gui_obj_t *host, int x, int y, int w, int h );

//! Set the widget attribute.
/*!
	Sets the widget attribute.
	\sa gui_sg_f() and gui_get().
*/
kucode_t gui_set( gui_obj_t *obj, int param, void *data );

//! Get the widget attribute.
/*!
	Gets the widhet attribute.
	\sa gui_sg_f() and gui_set().
*/
kucode_t gui_get( gui_obj_t *obj, int param, void *data );

//! Change the object status.
/*!
	Changes the object status. Switching from \a GUI_NOTLOADED status couses
	object to be loaded. Conseq., switching to \a GUI_NOTLOADED status
	couses object to be unloaded.
	\sa gui_status_f() and gui_status_t.
*/
kucode_t gui_ch_status( gui_obj_t *obj, gui_status_t status );

//! Draw the object.
/*!
	Draws an object and its subobjects.
	\note If \a obj is \e NULL then the root object is used.
	\sa gui_draw_f().
*/
kucode_t gui_draw( gui_obj_t *obj, int x, int y, int w, int h );

//! Process an event.
/*!
	GUI loop. Processes an event on the GUI. This function makes all objects to "live".
	\param event Current SDL event.
	\retval GUIE_EAT Event was handled by the GUI.
	\retval GUIE_LEAVE Event was not handled by the GUI.
*/
gui_event_st gui_process( SDL_Event *event );

#ifdef __cplusplus
}
#endif
#endif