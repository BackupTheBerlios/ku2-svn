/*
 *	host.h
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2007
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

/*!
 * \file
 * \author J. Anton
 * \brief Host computer system specific parameters.
 *
 * Host computer system specific parameters. OS definitions, compiler
 * definitions, etc.
 */

#ifndef KU__HOST_H__
#define KU__HOST_H__
#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \page host.h Host computer system specific parameters
 *
 * \section Header file begin and end declarations.
 */

// Include platform specific macros (also for windows?)
#include <features.h>

#ifdef __cplusplus
#	define KU_BEGIN_DECLS __BEGIN_DECLS namespace ku2 {
#	define KU_BEGIN_CPPDECLS namespace ku2 {
#	define KU_END_DECLS } __END_DECLS
#	define KU_END_CPPDECLS }
#else
//! Declare the begin of the Ku2 header file.
#	define KU_BEGIN_DECLS __BEGIN_DECLS
//! Declare the end of the Ku2 header file.
#	define KU_END_DECLS __END_DECLS
#endif

/*
 * Check if we are compiled by GCC. Visual studio should be detected by
 * SConstruc script.
 */
#ifdef __GNUC__
#	define KU_TOOL_GCC
#endif

//! Use this macro to avoid 'unused variable` warning message. \since 2.0.0
#define KU_DO_NOT_USE( __var ) (void)__var

//! Declare the variable or the function as unused. \since 2.0.0
#ifdef KU_TOOL_GCC
#define KU_UNUSED( __var ) __var __attribute__((unused))
#else
#define KU_UNUSED( __var ) __var
#endif

//! Declare the function as DLL exportable. \since 2.0.0
#if defined(KU_OS_WIN32)
#	if defined(KU_TOOL_GCC)
#		define KU_DLLEXPORT( __func ) __func __attribute__((dllexport))
#	elif defined(KU_TOOL_VCPP)
#		define KU_DLLEXPORT( __func ) __declspec(dllexport) __func
#	else
#		error "KU_DLLEXPORT cannot be defined on Windows platform"
#	endif
#else
#	define KU_DLLEXPORT( __func ) __func
#endif

//! Declare a function as a constructor. \since 2.0.0
#if defined(KU_TOOL_GCC)
#	define KU_CONSTRUCTOR( __func ) __attribute__((constructor)) __func
#else
#	error "KU_CONSTRUCTOR cannot be defined"
#endif

//! Declare a function as a destructor. \since 2.0.0
#if defined(KU_TOOL_GCC)
#	define KU_DESTRUCTOR( __func ) __attribute__((destructor)) __func
#else
#	error "KU_DESTRUCTOR cannot be defined"
#endif

//! Declare the variable as thread-local. \since 2.0.0
#if defined(_REENTRANT)
#	if defined(KU_TOOL_GCC)
#		define KU_THREAD_LOCAL( __var ) __thread __var
#	elif defined(KU_TOOL_VCPP)
#		define KU_THREAD_LOCAL( __var ) __declspec(thread) __var
#	else
#		error "KU_THREAD_LOCAL cannot be defined"
#	endif
#endif

//! Make sure that the source/header is compiled by C++. \since 2.0.0
#ifdef __cplusplus
#	define KU_ENSURE_CPLUSPLUS
#else
#	define KU_ENSURE_CPLUSPLUS static void must_be_compiled_by_cplusplus[-1];
#endif

#ifdef __cplusplus
}
#endif
#endif
