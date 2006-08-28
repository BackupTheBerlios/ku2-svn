/***************************************************************************
 *            res.h
 *
 *  Wed May 24 19:20:00 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Resource manager.
	
	Resource manager routines, such as loading, unloading diffrent types of resources.
	\author J. Anton
	\date Mon Aug 28 12:46:26 2006
	\version 1.1.0
*/

#ifndef KU__RES_H__
#define KU__RES_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"

//! Action type.
typedef
enum
{
	RES_LOAD,		//!< Load a resource.
	RES_ULOAD		//!< Unload a resource.
}	rescontrol_t;

//! Resouce loading/unloading function type.
/*!
	This function is used to load or unload resource data.
	\param path \b Loading: resource file path. \n
		\b Unloading: not used.
	\param action Resource action (see \ref rescontrol_t).
	\param data \b Loading: loading parameters. \n
		\b Unloading: resource data.
	\return \b Loading: loaded data or \e NULL if error. \n
		\b Unloading: \e NULL or not \e NULL if error.
	\note \ref kucode should not be changed.
*/
typedef
void *(*rescontrol_f)( const char *path, rescontrol_t action, void *data );

//! Resource type.
typedef
struct STRUCT_RES_TYPE
{
	int type;		//!< Resource type identificator.
	rescontrol_f
		control;	//!< Control function.
	ku_flag32_t
		flags;		//!< Resource type flags.
}	restype_t;

//! Resource unit.
typedef
struct STRUCT_RES
{
	const char
		*name,		//!< Resource name.
		*path;		//!< Resource file path.
	void *data;		//!< Resource data.
	uint loadcnt;	//!< Loaded instances count.
	ku_flag32_t
		flags;		//!< Resource unit flags.
	void *param;	//!< Loading parameters.
	int type;		//!< Type resource belongs to.
}	res_t;

//! Initialize resource engine.
/*!
	Initializes a resource manager engine. This function should be called before any
	other from this file.
	\retval KE_NONE No error.
	\retval KE_* abtree_create() errors.
	\sa res_halt().
*/
kucode_t res_init( void );

//! Halt resource engine.
/*!
	Halts a resource manager engine. No function should be called after this one,
	but res_init().
	\return Always \a KE_NONE.
	\sa res_init().
*/
kucode_t res_halt( void );

//! Assign flags and a control function for a resource type.
/*!
	Assigns flags and a control function for a resource type.
	\param type Resource type to deal with.
	\param flags Resource type flags.
	\param control Control function.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation error.
	\retval KE_* abtree_ins() errors.
*/
kucode_t res_assign( int type, ku_flag32_t flags, rescontrol_f control );

//! Add a resource.
/*!
	Adds a resource with the selected type.
	\param path Resource file path.
	\param name Resource name.
	\param type Resource type.
	\param param Resource loading parameters.
	\param flags Resource flags.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation error.
	\retval KE_* abtree_ins() errors.
*/
kucode_t res_add( const char *path, const char *name, int type, void *param,
				 ku_flag32_t flags );

//! Get access to resource data.
/*!
	Gets access to resource data. If resource is not loaded yet, data are loaded
	using the control function.
	\param name Resource name.
	\return Resource data, or \e NULL if error and \ref kucode is set to the
	valid value: \n \a KE_NOTFOUND: resource with the selected name was not found. \n
	\a KE_INVALID: resource type does not exist (invalid type). \n
	\a KE_EXTERNAL: control function has returned \e NULL.
	\sa res_release().
*/
void *res_access( const char *name );

//! Release the resource data.
/*!
	Releases the resource data. If it is the last resource instance, data
	are unloaded using the control function.
	\param name Resource name.
	\retval KE_NONE No error.
	\retval KE_NOTFOUND Resource with the selected name was not found.
	\retval KE_INVALID Invalid resource type.
	\retval KE_EXTERNAL control function did not return \e NULL.
	\sa res_access().
*/
kucode_t res_release( const char *name );

#ifdef __cplusplus
}
#endif
#endif
