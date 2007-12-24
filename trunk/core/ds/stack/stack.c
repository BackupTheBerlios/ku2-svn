/***************************************************************************
 *            stack.c
 *
 *  Sun Oct 15 13:38:28 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <stdlib.h>

#include "stack.h"
#include "ku2/debug.h"
#include "ku2/ecode.h"
#include "ku2/types.h"
#include "ku2/memory.h"

stack_t *stack_create( uint elements, ku_flag32_t flags __attribute__((unused)))
{
	stack_t *stack;
	pstart();
	
	stack = dmalloc(sizeof(stack_t)+sizeof(void*)*elements);
	if ( stack == NULL )
		KU_ERRQ_VALUE(KE_MEMORY, NULL);
	
	stack->pos = 0;
	stack->nodesz = (elements != 0) ? elements : STACK_DEAULT_NODE_SIZE;
	stack->data = (void**)((uint8_t*)stack+sizeof(stack_t));
	
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
		KU_ERRQ(KE_FULL);
	
	stack->data[stack->pos++] = data;
	
	preturn KE_NONE;
}

void *stack_pop( stack_t *stack )
{
	pstart();
	
	if ( stack->pos == 0 )
		KU_ERRQ_VALUE(KE_EMPTY, NULL);
	
	preturn stack->data[--stack->pos];
}
