/***************************************************************************
 *            stack.h
 *
 *  Sun Oct 15 13:56:31 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Stack container.
	
	This is the realization of stack.
	\author J. Anton
	\date Thu Oct 26 22:14:04 2006
	\version 1.0.1
*/

#ifndef KU__STACK_H__
#define KU__STACK_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"

//! Default stack size.
#define STACK_DEAULT_NODE_SIZE 32

//! Stack.
typedef
struct STRUCT_STACK
{
	int pos,		//!< Top element position.
		nodesz;		//!< Elements in a node.
	void **data;	//!< Stack data.
}	stack_t;

//! Create a stack.
/*!
	Creates a stack instance.
	\param elements Stack elements (stack size).
	\param flags Flags.
	\return Pointer to the created stack instance, else \e NULL and \ref kucode is
	set to the valid value.
	\retval KE_MEMORY Memory allocation has failed.
	\note Parameter \a flags is not used.
	\note If \ref elements is 0 (zero) then the default stack size is used.
	\sa stack_free().
*/
stack_t *stack_create( uint elements, ku_flag32_t flags );

//! Free a stack/queue.
/*!
	Frees a stack instance.
	\param stack Stack to be freed.
	\param freef Function to free list data, \e NULL for doing nothing.
	\return Always \a KE_NONE.
	\sa stack_create().
*/
kucode_t stack_free( stack_t *stack, ku_act_f freef );

//! Push data to the top of the stack.
/*!
	Adds data to the top of the stack.
	\param stack Stack to place data to.
	\param data Data to be placed.
	\retval KE_NONE No error.
	\retval KE_FULL Stack is full.
	\sa stack_pop().
*/
kucode_t stack_push( stack_t *stack, void *data );

//! Pop data from the top of the stack.
/*!
	Gets data from the top of the stack.
	\param stack Stack to get data from.
	\return Data or \e NULL and \ref kucode is set to
	\c KE_EMPTY is stack is empty.
	\sa stack_push().
*/
void *stack_pop( stack_t *stack );

#ifdef __cplusplus
}
#endif
#endif
