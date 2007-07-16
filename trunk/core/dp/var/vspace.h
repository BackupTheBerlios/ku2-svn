/*
		vspace.h
		Sat Jul 14 21:22:21 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
	\file
	\brief BRIEF.
	
	DESC.
	\author J. Anton
	\version 1.6.4
*/

#ifndef KU__VSPACE_H__
#define KU__VSPACE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ds/abtree/abtree.h"

typedef
struct STRUCT_VSPACE
{
	char *name;
	tree_t *vars,
		*spaces;
}	vspace_t;

vspace_t *vspace_define( const char *name );
kucode_t vspace_undef( vspace_t *space );

kucode_t vspace_addv( vspace_t *space, const char *name, const char *val_types, ... );
kucode_t vspace_adds( vspace_t *space, const char *name );

#ifdef KU__VAR_H__
const var_t *vspace_getv( vspace_t *space, const char *path );
#endif

vspace_t *vspace_gets( vspace_t *space, const char *path );

#ifdef __cplusplus
}
#endif
#endif
