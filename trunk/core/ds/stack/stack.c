/*
 *	stack.c
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2006
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#include "stack.h"

#include <stdlib.h>

#include "ku2/debug.h"
#include "ku2/ecode.h"
#include "ku2/types.h"
#include "ku2/memory.h"

stack_t *stack_create( uint elements, KU_UNUSED(ku_flag32_t flags) )
{
	stack_t *stack;
	pstart();
	
	stack = dmalloc(sizeof(stack_t) + sizeof(void*) * elements);
	if ( stack == NULL )
		KU_ERRQ_V(KE_MEMORY, NULL, NULL);
	
	stack->pos = 0;
	stack->nodesz = (elements != 0) ? elements : STACK_DEAULT_NODE_SIZE;
	stack->data = (void**)((uint8_t*)stack + sizeof(stack_t));
	
	preturn stack;
}

kucode_t stack_free( stack_t *stack, ku_act_f freef )
{
	pstart();
	
	if ( freef && (stack->pos != 0) )
	{
		int i;
		for ( i = 0; i < stack->pos; i++ )
			freef(stack->data[i]);
	}
	dfree(stack);
	
	preturn KE_NONE;
}

kucode_t stack_push( stack_t *stack, void *data )
{
	pstart();
	
	if ( stack->pos == stack->nodesz )
		KU_ERRQ(KE_FULL, NULL);
	
	stack->data[stack->pos++] = data;
	
	preturn KE_NONE;
}

void *stack_pop( stack_t *stack )
{
	pstart();
	
	if ( stack->pos == 0 )
		KU_ERRQ_V(KE_EMPTY, NULL, NULL);
	
	preturn stack->data[--stack->pos];
}
