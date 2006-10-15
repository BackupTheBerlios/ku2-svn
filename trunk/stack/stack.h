/***************************************************************************
 *            stack.h
 *
 *  Sun Oct 15 13:56:31 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Stack.
	
	This is the realization of stack, queue and deque.
	\author J. Anton
	\date Sun Oct 15 13:58:27 2006
	\version 1.0.0
*/

#ifndef KU__STACK_H__
#define KU__STACK_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"

//! Stack.
typedef
struct STRUCT_STACK
{
	int pos,		//!< Top element position.
		nodesz;		//!< Elements in a node.
	void **data;
}	stack_t;

//! Create a stack/queue.
stack_t *stack_create( uint elements, ku_flag32_t flags );

//! Free a stack/queue.
kucode_t stack_free( stack_t *stack, ku_act_f freef );

//! Push data to the top of the stack.
kucode_t stack_push( stack_t *stack, void *data );

//! Pop data from the top of the stack.
void *stack_pop( stack_t *stack );

#ifdef __cplusplus
}
#endif
#endif
