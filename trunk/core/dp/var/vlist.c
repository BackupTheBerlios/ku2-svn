#include <unistd.h>
#include <string.h>

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "dp/var/vlist.h"
#include "dp/var/var.h"
#include "ds/list/list.h"

static int vlist_freef( void *vlist_node )
{
	dfree(vlist_node);
	return 0;
}

vlist_t *vlist_define( void )
{
	vlist_t *vlist;
	pstart();
	
	vlist = (vlist_t*)dmalloc(sizeof(vlist_t));
	if ( vlist == NULL )
	{
		KU_SET_ERROR(KE_MEMORY);
		preturn NULL;
	}
	
	vlist->vals = dl_list_create(NULL,0);
	if ( vlist->vals == NULL )
	{
		dfree(vlist);
		preturn NULL;
	}
	
	vlist->datasz = 0;
	
	preturn vlist;
}

kucode_t vlist_undef( vlist_t *vlist )
{
	pstart();
	
	dl_list_free(vlist->vals, vlist_freef);
	dfree(vlist);
	
	preturn KE_NONE;
}

kucode_t vlist_clear( vlist_t *vlist )
{
	pstart();
	
	if ( dl_list_first(vlist->vals) == KE_NONE )
		dl_list_delete_n(vlist->vals, vlist->vals->size, vlist_freef);
	vlist->datasz = 0;
	
	preturn KE_NONE;
}

kucode_t vlist_add( vlist_t *vlist, char val_type, void *data )
{
	vlist_node_t *node;
	uint size;
	pstart();
	
	switch ( val_type )
	{
		case VAL_INTEGER:
		case VAL_BOOLEAN:
			size = sizeof(int);
			break;
		case VAL_LONGINT:
			size = sizeof(long int);
			break;
		case VAL_DOUBLE:
			size = sizeof(double);
			break;
		case VAL_STRING:
			size = strlen(data)+1;
			break;
		default:
			KU_ERRQ(KE_INVALID);
	}
	
	node = (vlist_node_t*)dmalloc(size+sizeof(vlist_node_t));
	if ( node == NULL )
		KU_ERRQ(KE_MEMORY);
	
	if ( dl_list_ins_last(vlist->vals, node) != KE_NONE )
	{
		dfree(node);
		preturn KU_GET_ERROR();
	}
	
	switch ( val_type )
	{
		case VAL_INTEGER:
		case VAL_BOOLEAN:
			*(int*)(node+1) = *(int*)data;
			break;
		case VAL_LONGINT:
			*(long int*)(node+1) = *(long int*)data;
			break;
		case VAL_DOUBLE:
			*(double*)(node+1) = *(double*)data;
			break;
		case VAL_STRING:
			strcpy((char*)(node+1), (char*)data);
			break;
		default:
			KU_ERRQ(KE_INVALID);
	}
	node->val_type = val_type;
	vlist->datasz += size;
	
	preturn KE_NONE;
}
