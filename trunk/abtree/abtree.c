/***************************************************************************
 *            abtree.c
 *
 *  Fri Aug 25 20:27:03 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include "abtree.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/ecode.h"
#include "other/other.h"

static void ab_rotate_left( tree_t *tree, tree_node_t *node )
{
	tree_node_t *t = node->right;
	pstart();
	
	if ( t != NULL )
	{
		if ( (t->right != NULL) && (t->rcnt >= t->lcnt) )
		{
			//	ACE стиль
			t->parent = node->parent;
			
			node->right = t->left;
			node->rcnt = t->lcnt;
			if ( t->left != NULL ) t->left->parent = node;
			
			t->left = node;
			t->lcnt = MAXint(node->lcnt, node->rcnt)+1;
			node->parent = t;
		}	else
		{
			//	ACD стиль
			tree_node_t *d = t->left;
			d->parent = node->parent;
			t->parent = d;
			node->parent = d;
			
			node->right = d->left;
			node->rcnt = d->lcnt;
			if ( node->right != NULL ) node->right->parent = node;
			
			t->left = d->right;
			t->lcnt = d->rcnt;
			if ( t->left != NULL ) t->left->parent = t;
			
			d->left = node;
			d->lcnt = MAXint(node->lcnt, node->rcnt)+1;
			d->right = t;
			d->rcnt = MAXint(t->lcnt, t->rcnt)+1;
			
			t = d;
		}
		
		if ( t->parent != NULL )
		{
			if ( t->parent->left == node )
			{
				t->parent->left = t;
				t->parent->lcnt = MAXint(t->lcnt, t->rcnt)+1;
			}	else
			{
				t->parent->right = t;
				t->parent->rcnt = MAXint(t->lcnt, t->rcnt)+1;
			}
		}	else
		{
			tree->root = t;
		}
	}
	
	pstop();
}

static void ab_rotate_right( tree_t *tree, tree_node_t *node )
{
	tree_node_t *t = node->left;
	pstart();
	
	if ( t != NULL )
	{
		if ( (t->left != NULL) && (t->lcnt >= t->rcnt) )
		{
			//	ACE стиль
			t->parent = node->parent;
			
			node->left = t->right;
			node->lcnt = t->rcnt;
			if ( t->right != NULL ) t->right->parent = node;
			
			t->right = node;
			t->rcnt = MAXint(node->lcnt, node->rcnt)+1;
			node->parent = t;
		}	else
		{
			//	ACD стиль
			tree_node_t *d = t->right;
			d->parent = node->parent;
			t->parent = d;
			node->parent = d;
			
			node->left = d->right;
			node->lcnt = d->rcnt;
			if ( node->left != NULL ) node->left->parent = node;
			
			t->right = d->left;
			t->rcnt = d->lcnt;
			if ( t->right != NULL ) t->right->parent = t;
			
			d->right = node;
			d->rcnt = MAXint(node->lcnt, node->rcnt)+1;
			d->left = t;
			d->lcnt = MAXint(t->lcnt, t->rcnt)+1;
			
			t = d;
		}
		
		if ( t->parent != NULL )
		{
			if ( t->parent->left == node )
			{
				t->parent->left = t;
				t->parent->lcnt = MAXint(t->lcnt, t->rcnt)+1;
			}	else
			{
				t->parent->right = t;
				t->parent->rcnt = MAXint(t->lcnt, t->rcnt)+1;
			}
		}	else
		{
			tree->root = t;
		}
	}
	
	pstop();
}

tree_t *abtree_create( ku_comp_f func, ku_flag32_t flags )
{
	tree_t *tree;
	pstart();
	
	tree = (tree_t*)dmalloc(sizeof(tree_t));
	if ( tree == NULL )
	{
		kucode = KE_MEMORY;
		return NULL;
	}
	
	tree->root = tree->cur = NULL;
	tree->cmpf = func;
	
	pstop();
	return tree;
}

kucode_t abtree_free( tree_t *tree, ku_act_f freef )
{
	pstart();
	
	abtree_clear(tree, freef);
	dfree(tree);
	
	pstop();
	return KE_NONE;
}

kucode_t abtree_clear( tree_t *tree, ku_act_f freef )
{
	tree_node_t *a = tree->root, *b;
	pstart();
	
	while ( a != NULL )
	{
		b = a->left;
		if ( b == NULL )
		{
			b = a->right;
			if ( b == NULL )
			{
				b = a->parent;
				if ( b != NULL )
				{
					if ( b->left == a )
						b->left = NULL; else
						b->right = NULL;
				}
				if ( freef )
					freef(a->data);
				dfree(a);
			}
		}
		a = b;
	}
	
	tree->root = NULL;
	
	pstop();
	return KE_NONE;
}

kucode_t abtree_ins( tree_t *tree, void *data )
{
	tree_node_t *cur = tree->root;
	tree_node_t *p = NULL, *newnode;
	pstart();
	
	//	создание элемента
	newnode = (tree_node_t*)dmalloc(sizeof(tree_node_t));
	if ( newnode==NULL )
		KU_ERRQ(KE_MEMORY);
	newnode->left = newnode->right = NULL;
	newnode->data = data;
	newnode->lcnt = newnode->rcnt = 0;
	
	//	поиск места
	while ( cur != NULL )
	{
		p = cur;
		if ( tree->cmpf(data, cur->data) > 0 )
		{
			cur=cur->right;
		}	else
		if ( tree->cmpf(data, cur->data) < 0 )
		{
			cur=cur->left;
		}	else
		{
			dfree(newnode);
			KU_ERRQ(KE_DOUBLE);
		}
	}
	
	//	вставка
	if ( p == NULL )
	{
		//	первый элемент в списке
		tree->root = newnode;
		newnode->parent = NULL;
	}	else
	{
		newnode->parent = p;
		if ( tree->cmpf(data, p->data) > 0 )
		{
			p->right = newnode;
		}	else
		{
			p->left = newnode;
		}
	}
	
	//	балансировка
	while ( p != NULL )
	{
		if ( p->left == newnode )
			p->lcnt = MAXint(newnode->lcnt, newnode->rcnt)+1;
		else
			p->rcnt = MAXint(newnode->lcnt, newnode->rcnt)+1;
		
		if ( p->lcnt-p->rcnt == 2 )
		{
			ab_rotate_right(tree, p);
			p = p->parent;
		}	else
		if ( p->rcnt-p->lcnt == 2 )
		{
			ab_rotate_left(tree, p);
			p = p->parent;
		}
		
		newnode = p;
		p = p->parent;
	}
	
	pstop();
	return KE_NONE;
}

kucode_t abtree_rem( tree_t *tree, void *data, ku_act_f freef )
{
	tree_node_t *cur = tree->root;
	tree_node_t *p;
	pstart();
	
	//	поиск элемента
	while ( cur != NULL )
	{
		if ( tree->cmpf(data,cur->data) > 0 )
		{
			cur = cur->right;
		}	else
		if ( tree->cmpf(data,cur->data) < 0 )
		{
			cur = cur->left;
		}	else break;
	}
	if ( cur == NULL )
		KU_ERRQ(KE_NOTFOUND);
	
	//	замена его минимальным большим элементом
	if ( freef )
		freef(cur->data);
	p = cur->right;
	if ( p == NULL )
	{
		p = cur->parent;
		if ( p != NULL )
		{
			if ( p->left == cur )
			{
				p->left = cur->left;
				if ( cur->left != NULL )
				{
					p->lcnt = MAXint(cur->left->lcnt, cur->left->rcnt)+1;
					cur->left->parent = p;
				}	else
					p->lcnt = 0;
			}	else
			{
				p->right = cur->left;
				if ( cur->left != NULL )
				{
					p->rcnt = MAXint(cur->left->lcnt, cur->left->rcnt)+1;
					cur->left->parent = p;
				}	else
					p->rcnt = 0;
			}
		}	else
		{
			tree->root = cur->left;
			if ( cur->left != NULL )
				cur->left->parent = NULL;
		}
	}	else
	{
		while ( p->lcnt > 0 ) p = p->left;
		cur->data = p->data;
		cur = p;
		p = cur->parent;
		if ( p->left == cur )
		{
			p->left = cur->right;
			if ( cur->right != NULL )
			{
				p->lcnt = MAXint(cur->right->lcnt, cur->right->rcnt)+1;
				cur->right->parent = p;
			}	else
				p->lcnt = 0;
		}	else
		{
			p->right = cur->right;
			if ( cur->right != NULL )
			{
				p->rcnt = MAXint(cur->right->lcnt, cur->right->rcnt)+1;
				cur->right->parent = p;
			}	else
				p->rcnt = 0;
		}
	}
	dfree(cur);
	
	//	балансировка
	if ( p != NULL )
	{
		if ( p->lcnt-p->rcnt == 2 )
		{
			ab_rotate_right(tree, p);
			p = p->parent;
		}	else
		if ( p->rcnt-p->lcnt == 2 )
		{
			ab_rotate_left(tree, p);
			p = p->parent;
		}
		cur = p;
		p = p->parent;
	}
	while ( p != NULL )
	{
		if ( p->left == cur )
			p->lcnt = MAXint(cur->lcnt, cur->rcnt)+1;
		else
			p->rcnt = MAXint(cur->lcnt, cur->rcnt)+1;
		
		if ( p->lcnt-p->rcnt == 2 )
		{
			ab_rotate_right(tree, p);
			p = p->parent;
		}	else
		if ( p->rcnt-p->lcnt == 2 )
		{
			ab_rotate_left(tree, p);
			p = p->parent;
		}
		cur = p;
		p = p->parent;
	}
	
	pstop();
	return KE_NONE;
}

void *abtree_search( tree_t *tree, void *data )
{
	tree_node_t *cur = tree->root;
	pstart();
	
	while ( cur != NULL )
	{
		if ( tree->cmpf(data, cur->data) > 0 )
		{
			cur = cur->right;
		}	else
		if ( tree->cmpf(data, cur->data) < 0 )
		{
			cur = cur->left;
		}	else
		{
			pstop();
			return cur->data;
		}
	}
	
	return NULL;
}

kucode_t abtree_goto_first( tree_t *tree )
{
	tree_node_t *a = tree->root;
	pstart();
	
	if ( a != NULL )
	{
		while ( (a->left != NULL) || (a->right != NULL) )
		{
			if ( a->left != NULL )
				a = a->left; else
				a = a->right;
		}
	}	else
		return KE_EMPTY;
	
	tree->cur = a;
	
	pstop();
	return KE_NONE;
}

void *abtree_goto_next( tree_t *tree )
{
	tree_node_t *a, *b = tree->cur;
	pstart();
	
	if ( b != NULL )
	{
		a = b;
		b = b->parent;
		if ( b != NULL )
		{
			if ( (b->left == a) && (b->right != NULL) )
			{
				b = b->right;
				while ( (b->left != NULL) || (b->right != NULL) )
				{
					if ( b->left != NULL )
						b = b->left; else
						b = b->right;
				}
			}
		}
		tree->cur = b;
	}	else
	{
		return NULL;
	}
	
	pstop();
	return tree->cur->data;
}