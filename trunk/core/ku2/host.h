/*
		host.h
		Mon May 21 18:54:34 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
	\file
	\brief Host computer system specific parameters.
	
	Host computer system specific parameters. OS definitions, compiller
	definitions, etc.
	\author J. Anton
*/

#ifndef KU__HOST_H__
#define KU__HOST_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define DO_NOT_USE( __var ) (void)__var

//! Declare the variable or the function as unused.
#ifdef __GNUC__
#define UNUSED_VAR( __var ) __var __attribute__((unused))
#else
#define UNUSED_VAR( __var ) __var
#endif

//! Declare the function as DLL exportable.
#if defined(WIN32)
#ifdef __GNUC__
#define DLLEXPORT( __func ) __func __attribute__((dllexport))
#else
#define DLLEXPORT( __func ) __declspec(dllexport)
#endif
#else
#define DLLEXPORT( __func ) __func
#endif

#ifdef __cplusplus
}
#endif
#endif
