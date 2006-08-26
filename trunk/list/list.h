/***************************************************************************
 *            list.h
 *
 *  Tue Aug 22 21:01:00 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief \a List containers.
	
	This is the realization of different kinds of lists.
	\author J. Anton
	\date Tue Aug 22 21:22:34 2006
	\version 1.1.0
	
	\section dl_list Double linked list.
	These lists have a head node and a tail node. Each node is linked to the
	next node and to the previous node. \n Functions: \b dl_list_*
*/

#ifndef KU__LIST_H__
#define KU__LIST_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"

//! List node.
typedef
struct STRUCT_LIST_NODE
{
	void *data;		//!< Data.
	struct STRUCT_LIST_NODE
		*next,		//!< Next node in the list.
		*prev;		//!< Previous node in the list.
}	ls_node_t;

//! List header.
typedef
struct STRUCT_LIST
{
	uint size;		//!< Node count in the list.
	ku_comp_f cmpf;	//!< Comparing function.
	ls_node_t *ptr,	//!< Current node.
		*head,		//!< Head node.
		*tail;		//!< Tail node.
}	list_t;

//! Create a double linked list instance.
/*!
	Creates a double linked list instance.
	\param func Comparing function.
	\param flags Flags.
	\return Pointer to the created list instance, else \e NULL and \ref kucode is
	set to the valid value.
	\note Parameter \a flags is not used.
	\sa dl_list_free().
*/
list_t *dl_list_create( ku_comp_f func, ku_flag32_t flags );

//! Free a double linked list instance.
/*!
	Frees a double linked list instance.
	\param list List to be freed.
	\param freef Function to free list data, \e NULL for doing nothing.
	\return Always \a KE_NONE.
	\sa dl_list_create().
*/
kucode_t dl_list_free( list_t *list, ku_act_f freef );

//! Insert new data \a before the current element.
/*!
	Inserts new data \a before the current element. If list is empty then data
	are placed first. New element becomes a current one.
	\param list List to deal with.
	\param data Data to place in.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation has failed.
	\sa dl_list_ins_after(), dl_list_ins_last() and dl_list_rem().
*/
kucode_t dl_list_ins( list_t *list, void *data );

//! Insert new data \a after the current element.
/*!
	Inserts new data \a after the current element. If list is empty then data
	are placed first. New element becomes a current one.
	\param list List to deal with.
	\param data Data to place in.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation has failed.
	\sa dl_list_ins(), dl_list_ins_last() and dl_list_rem().
*/
kucode_t dl_list_ins_after( list_t *list, void *data );

//! Insert new data \a at \a the \a end of the list.
/*!
	Appends new data \a to \a the \a end of the list. If list is empty then data
	are placed first.
	\param list List to deal with.
	\param data Data to place in.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation has failed.
	\note Current position pointer in the list is not affected.
	\sa dl_list_ins(), dl_list_ins_after() and dl_list_rem().
*/
kucode_t dl_list_ins_last( list_t *list, void *data );

//! Remove the current element.
/*!
	Removes the current element from the list. The next element, if exists,
	becomes a current one.
	\param list List to deal with.
	\param freef Function to free element data, \e NULL for doing nothing.
	\retval KE_NONE No error.
	\retval KE_EMPTY List is empty.
	\sa dl_list_ins(), dl_list_ins_after() and dl_list_ins_last().
*/
kucode_t dl_list_rem( list_t *list, ku_act_f freef );

//! Search for the element.
/*!
	Searches for the element with the selected data. Current position is set
	to the found element, else does not change.
	\param list List to deal with.
	\param data Data to be searched for.
	\retval KE_NONE Data were found.
	\retval KE_NOTFOUND Data were not found.
	\note \ref kucode is not affected.
*/
kucode_t dl_list_search( list_t *list, void *data );

//! Get data of the current element.
/*!
	Gets the data of the current element of the list.
	\param list List to deal with.
	\return Data of the current element, or \e NULL if list is empty.
	\note \ref kucode is not affected.
	\sa dl_list_next() and dl_list_prev().
*/
void *dl_list_cur( list_t *list );

//! Get data of the current element and move to the next one.
/*!
	Gets the data of the current element of the list and the next element,
	if exists, becomes the current one.
	\param list List to deal with.
	\return Data of the current element, or \e NULL if list is empty.
	\note \ref kucode is not affected.
	\sa dl_list_cur() and dl_list_prev().
*/
void *dl_list_next( list_t *list );

//! Get data of the current element and move to the previous one.
/*!
	Gets the data of the current element of the list and the previous element,
	if exists, becomes the current one.
	\param list List to deal with.
	\return Data of the current element, or \e NULL if list is empty.
	\note \ref kucode is not affected.
	\sa dl_list_cur() and dl_list_next().
*/
void *dl_list_prev( list_t *list );

//! Check whether the current element is the last one.
/*!
	Checks whether the current element is the last one.
	\param list List to deal with.
	\retval 0 Current element is not the last one.
	\retval 1 Current element is the last one, or list it empty.
	\note \ref kucode is not affected.
	\sa dl_list_isfirst().
*/
int dl_list_islast( list_t *list );

//! Check whether the current element is the first one.
/*!
	Checks whether the current element is the first one.
	\param list List to deal with.
	\retval 0 Current element is not the first one.
	\retval 1 Current element is the first one, or list it empty.
	\note \ref kucode is not affected.
	\sa dl_list_islast().
*/
int dl_list_isfirst( list_t *list );

//! Go the the first element of the list.
/*!
	Makes the first element to become a current one.
	\param list List to deal with.
	\retval KE_NONE No error.
	\retval KE_EMPTY List is empty.
	\note \ref kucode is not affected.
	\sa dl_list_last().
*/
kucode_t dl_list_first( list_t *list );

//! Go the the last element of the list.
/*!
	Makes the last element to become a current one.
	\param list List to deal with.
	\retval KE_NONE No error.
	\retval KE_EMPTY List is empty.
	\note \ref kucode is not affected.
	\sa dl_list_first().
*/
kucode_t dl_list_last( list_t *list );

//! Move the current element forward.
/*!
	Moves the current element forward (just after the next one, if it exists).
	\param list List to deal with.
	\retval KE_NONE No error.
	\retval KE_NOTFOUND Is is the last element in the list.
	\retval KE_EMPTY List is empty.
	\sa dl_list_move_back(), dl_list_move_first() and dl_list_mode_last().
*/
kucode_t dl_list_move_fwd( list_t *list );

//! Move the current element backward.
/*!
	Moves the current element backward (just before the previous one, if it
	exists).
	\param list List to deal with.
	\retval KE_NONE No error.
	\retval KE_NOTFOUND Is is the first element in the list.
	\retval KE_EMPTY List is empty.
	\sa dl_list_move_fwd(), dl_list_move_first() and dl_list_mode_last().
*/
kucode_t dl_list_move_back( list_t *list );

//! Move the current element to the head.
/*!
	Moves the current element to the head (makes it to be the first one).
	\param list List to deal with.
	\retval KE_NONE No error.
	\retval KE_EMPTY List is empty.
	\sa dl_list_move_fwd(), dl_list_move_back() and dl_list_mode_last().
*/
kucode_t dl_list_move_first( list_t *list );

//! Move the current element to the tail.
/*!
	Moves the current element to the tail (makes it to be the last one).
	\param list List to deal with.
	\retval KE_NONE No error.
	\retval KE_EMPTY List is empty.
	\sa dl_list_move_fwd(), dl_list_move_back() and dl_list_mode_first().
*/
kucode_t dl_list_move_last( list_t *list );

//! Delete some elements from the list.
/*!
	Deletes \a cnt elements starting from the current element from the list. An
	element just after the last deleted one, if exists, becomes the current one,
	else the last element becomes the current one.
	\param list List to deal with.
	\param cnt How many element are to be deleted.
	\param freef Function to free element data, \e NULL for doing nothing.
	\retval KE_NONE No error.
	\retval KE_EMPTY List is empty.
	\note It may be deleted less than \a cnt elements if end of list is found.
*/
kucode_t dl_list_delete_n( list_t *list, uint cnt, ku_act_f freef );

#ifdef __cplusplus
}
#endif
#endif
