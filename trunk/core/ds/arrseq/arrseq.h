/*
		arrseq.h
		Tue Apr 17 23:05:35 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
	\file
	\brief Array sequence container.
	
	Scalable sequence of the arrays.
	\author J. Anton
	\date Tue Apr 17 23:05:40 2007
	\version 1.0.0
*/

#ifndef KU__ARRSEQ_H__
#define KU__ARRSEQ_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/types.h"

//! ArrSeq part of array.
typedef
struct STRUCT_ARRSEQ_ARRAY
{
	
}	arrseq_array_t;

//! ArrSeq header.
typedef
struct STRUCT_ARRSEQ
{
	uint free_ptr;
}	arrseq_t;

#ifdef __cplusplus
}
#endif
#endif
