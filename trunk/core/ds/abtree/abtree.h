/*
		abtree.h
		Fri Aug 25 20:12:16 2006
		Mon Jun 23 10:32:53 2008

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2008
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
	\file
	\brief Auto-Balanced tree.
	
	This is the realization of Auto-Balanced tree.
	\author J. Anton
*/

#ifndef KU__ABTREE_H__
#define KU__ABTREE_H__
#include "ku2/host.h"

#include "ku2/ecode.h"
#include "ku2/types.h"

KU_BEGIN_DECLS

/*
	After version 1.7.2 a prefix ku_* was added
	to the types and functions
*/
#ifdef KU_COMPAT_1_7_2
#	define tree_t				ku_tree_t
#	define abtree_create		ku_abtree_create
#	define abtree_free			ku_abtree_free
#	define abtree_clear			ku_abtree_clear
#	define abtree_ins			ku_abtree_ins
#	define abtree_replace		ku_abtree_replace
#	define abtree_rem			ku_abtree_rem
#	define abtree_goto_first	ku_abtree_goto_first
#	define abtree_goto_next		ku_abtree_goto_next
#	define abtree_unused_index	ku_abtree_unused_index
#endif

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
}	ku_tree_node_t;

//! Tree header.
typedef
struct STRUCT_TREE
{
	ku_comp_f cmpf;	//!< Comparing function.
	ku_tree_node_t
		*root,		//!< Root of the tree.
		*cur;		//!< Current element.
}	ku_tree_t;

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
ku_tree_t *ku_abtree_create( ku_comp_f func, ku_flag32_t flags );

//! Free a auto-balanced tree instance.
/*!
	Frees a auto-balanced tree instance.
	\param tree Tree to be freed.
	\param freef Function to free tree data, \e NULL for doing nothing.
	\return Always \a KE_NONE.
	\sa abtree_create() and abtree_clear().
*/
kucode_t ku_abtree_free( ku_tree_t *tree, ku_act_f freef );

//! Clear the tree.
/*!
	Deletes all elements from the tree.
	\param tree Tree to deal with.
	\param freef Function to free list data, \e NULL for doing nothing.
	\return Always \a KE_NONE.
	\sa abtree_free().
*/
kucode_t ku_abtree_clear( ku_tree_t *tree, ku_act_f freef );

//! Insert data to the tree.
/*!
	Inserts data to the tree.
	\param tree Tree to deal with.
	\param data Data to be inserted.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation has failed.
	\retval KE_DOUBLE Element with the selected data already exists.
	\sa abtree_replace() and abtree_rem().
*/
kucode_t ku_abtree_ins( ku_tree_t *tree, const void *data );

//! Replace the data in the tree.
/*!
	Replaces the data in the tree. Old data might be freed if selected.
	\param tree Tree to deal with.
	\param odata Data to be searched and replaced (if exists).
	\param ndata Data to be replaced by. If \e odata was not found, just
	inserting is being done.
	\param freef Function to free replaced data, \e NULL for doing nothing.
	\retval KE_NONE No error.
	\note Parameter \e odata is not affected and is not freed!. Only data replaced
	in the tree can be freed: \e odata is used as a searching pattern.
	\sa abtree_ins() and abtree_rem().
*/
kucode_t ku_abtree_replace( ku_tree_t *tree, const void *odata,
						    const void *ndata, ku_act_f freef );

//! Remove data from the tree.
/*!
	Removes data from the tree.
	\param tree Tree to deal with.
	\param data Data to be deleted.
	\param freef Function to free found data, \e NULL for doing nothing.
	\retval KE_NONE No error.
	\retval KE_NOTFOUND Element was not found.
	\note Parameter \e data is not affected and is not freed!. Only data stored
	in the tree can be freed: \e data is used as a searching pattern.
	\sa abtree_ins() and abtree_replace().
*/
kucode_t ku_abtree_rem( ku_tree_t *tree, const void *data, ku_act_f freef );

//! Search for the selected data.
/*!
	Searches for the selected data and returns the found ones.
	Current position is set to the found data, or \e NULL otherwise.
	\param tree Tree to deal with.
	\param data Data to be searched.
	\return Data, if found, else \e NULL.
	\note \ref kucode is not affected.
*/
void *ku_abtree_search( ku_tree_t *tree, const void *data );

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
kucode_t ku_abtree_goto_first( ku_tree_t *tree );

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
void *ku_abtree_goto_next( ku_tree_t *tree );

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
void *ku_abtree_unused_index( ku_tree_t *tree, ku_interval_f intf, int *pos );

KU_END_DECLS
#endif
