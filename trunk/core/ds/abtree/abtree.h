/***************************************************************************
 *            abtree.h
 *
 *  Fri Aug 25 20:12:16 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Auto-Balanced tree.
	
	This is the realization of Auto-Balanced tree.
	\author J. Anton
	\date Wed Feb 07 10:06 2007
	\version 1.2.0
*/

#ifndef KU__ABTREE_H__
#define KU__ABTREE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"

//! Tree node.
typedef
struct STRUCT_TREE_NODE
{
	union
	{
		void *data;	//!< Data pointer.
		int int_d;	//!< Integer data.
	};
	struct STRUCT_TREE_NODE
		*left,		//!< Left child.
		*right,		//!< Right child.
		*parent;	//!< Parent node.
	uint lcnt,		//!< Amount of the left children.
		rcnt;		//!< Amount of the right children.
}	tree_node_t;

//! Tree header.
typedef
struct STRUCT_TREE
{
	ku_comp_f cmpf;	//!< Comparing function.
	tree_node_t
		*root,		//!< Root of the tree.
		*cur;		//!< Current element.
}	tree_t;

//! Create a auto-balanced tree instance.
/*!
	Creates a auto-balanced tree instance.
	\param func Comparing function.
	\param flags Flags.
	\return Pointer to the created tree instance, else \e NULL and \ref kucode is
	set to the valid value.
	\note Parameter \a flags is not used.
	\sa abtree_free().
*/
tree_t *abtree_create( ku_comp_f func, ku_flag32_t flags );

//! Free a auto-balanced tree instance.
/*!
	Frees a auto-balanced tree instance.
	\param tree Tree to be freed.
	\param freef Function to free tree data, \e NULL for doing nothing.
	\return Always \a KE_NONE.
	\sa abtree_create() and abtree_clear().
*/
kucode_t abtree_free( tree_t *tree, ku_act_f freef );

//! Clear the tree.
/*!
	Deletes all elements from the tree.
	\param tree Tree to deal with.
	\param freef Function to free list data, \e NULL for doing nothing.
	\return Always \a KE_NONE.
	\sa abtree_free().
*/
kucode_t abtree_clear( tree_t *tree, ku_act_f freef );

//! Insert data to the tree.
/*!
	Inserts data to the tree.
	\param tree Tree to deal with.
	\param data Data to be inserted.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation has failed.
	\retval KE_DOUBLE Element with the selected data already exists.
	\sa abtree_rem().
*/
kucode_t abtree_ins( tree_t *tree, const void *data );

//! Remove data from the tree.
/*!
	Removes data from the tree.
	\param tree Tree to deal with.
	\param data Data to be deleted.
	\param freef Function to free data, \e NULL for doing nothing.
	\retval KE_NONE No error.
	\retval KE_NOTFOUND Element was not found.
	\sa abtree_ins().
*/
kucode_t abtree_rem( tree_t *tree, void *data, ku_act_f freef );

//! Search for the selected data.
/*!
	Searches for the selected data and returns the found ones.
	\param tree Tree to deal with.
	\param data Data to be searched.
	\return Data, if found, else \e NULL.
	\note \ref kucode is not affected.
*/
void *abtree_search( tree_t *tree, const void *data );

//! Move the current position to the smallest element (first one).
/*!
	Moves the current position to the smallest element of the tree, which
	also is the first one.
	\param tree Tree to deal with.
	\retval KE_NONE No error.
	\retval KE_EMPTY Tree is empty.
	\note \ref kucode is not affected.
	\sa abtree_goto_next().
*/
kucode_t abtree_goto_first( tree_t *tree );

//! Get data of the current element and move to the next one.
/*!
	Gets the data of the current element and moves the current position
	to the next element, which is the next smallest one.
	\param tree Tree to deal with.
	\return Data of the current element, or \e NULL if there are no more
	elements.
	\note \ref kucode is not affected.
	\sa abtree_goto_first().
*/
void *abtree_goto_next( tree_t *tree );

//! Get the first unused index in the tree.
/*!
	Gets the first unused index in the tree.
	\param tree Tree to deal with.
	\param intf Interval function for determining the possible indexes.
	\param [out] pos Unused index position: \n
	\e <0 Unused index is left to the returned element. \n
	\e >0 Unused index is right to the returned element. \n
	\e UNCHANGED No unused index is available.
	\return Data of the border element or \e NULL if no unused index is
	available.
	\note \ref kucode is not affected.
*/
void *abtree_unused_index( tree_t *tree, ku_interval_f intf, int *pos );

#ifdef __cplusplus
}
#endif
#endif
