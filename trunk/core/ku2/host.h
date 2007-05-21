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
	\date Mon May 21 18:54:56 2007
	\version 1.6.0
*/

#ifndef KU__HOST_H__
#define KU__HOST_H__
#ifdef __cplusplus
extern "C" {
#endif

//! Declare the variable or the function as unused.
#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

#ifdef __cplusplus
}
#endif
#endif
