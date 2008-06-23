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

// Temporaly..
#define KU_COMPAT_1_7_2

// For compability with programmes, that use older API
#if defined(KU_COMPAT) && !defined(KU_COMPAT_1_7_2)
#	define KU_COMPAT_1_7_2
#endif

// Include platform specific macros (also for windows?)
#include <features.h>

#ifdef __cplusplus
#	define KU_BEGIN_DECLS __BEGIN_DECLS namespace ku2 {
#	define KU_END_DECLS } __END_DECLS
#else
#	define KU_BEGIN_DECLS __BEGIN_DECLS
#	define KU_END_DECLS __END_DECLS
#endif

//! Use this macro to avoid 'unused variable` warning message.
#define DO_NOT_USE( __var ) (void)__var

//! Declare the variable or the function as unused.
#ifdef __GNUC__
#define UNUSED_VAR( __var ) __var __attribute__((unused))
#else
#define UNUSED_VAR( __var ) __var
#endif

//! Declare the function as DLL exportable.
#if defined(WIN32)
#	if defined(__GNUC__)
#		define DLLEXPORT( __func ) __func __attribute__((dllexport))
#	else
#		define DLLEXPORT( __func ) __declspec(dllexport) __func
#	endif
#else
#	define DLLEXPORT( __func ) __func
#endif

//! Declare the variable as thread-local.
#if defined(__GNUC__)
#	define THREAD( __var ) __thread __var
#else
#	define THREAD( __var ) __declspec(thread) __var
#endif

#ifdef __cplusplus
}
#endif
#endif
