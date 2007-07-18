#ifndef KU__VLIST_H__
#define KU__VLIST_H__

#include "ku2/ecode.h"
#include "ds/list/list.h"

typedef
struct
{
	char val_type;
	// Further goes data.
}	vlist_node_t;

typedef
struct
{
	list_t *vals;
	uint datasz;
}	vlist_t;

vlist_t *vlist_define( void );
kucode_t vlist_undef( vlist_t *vlist );

kucode_t vlist_clear( vlist_t *vlist );
kucode_t vlist_add( vlist_t *vlist, char val_type, void *data );


#endif
