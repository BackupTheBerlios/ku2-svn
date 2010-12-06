/*
 *	core:ku2:debug.h
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2006+
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

/*!
 * \file
 * \brief Debug functions and definitions.
 *
 * This file contains the functions and definitions for debugging.
 */

#ifndef KU__DEBUG_H__
#define KU__DEBUG_H__
#include "ku2/host.h"
KU_BEGIN_DECLS

#ifdef DEBUG

#include <stdlib.h>

void ku_printf_debug( const char *file, const char *func, int line, char *fmt, ... ) __THROW;
void ku_pavoid_debug( const char *file, const char *func, int line ) __THROW;
void ku_func_debug( int status ) __THROW;
void ku_pstart_debug( const char *func, char *fmt, ... ) __THROW;
void ku_pstop_debug( const char *func, char *fmt, ... ) __THROW;

#define pdebug( m, ... ) \
ku_printf_debug(__FILE__, __FUNCTION__, __LINE__, m, ##__VA_ARGS__)

#define ku_avoid( __expr ) \
if ( __expr ) \
{ \
	ku_pavoid_debug(__FILE__, __PRETTY_FUNCTION__, __LINE__); \
	KU_ERRQ(KE_ASSERT, "Expression '" #__expr "` was not avoided"); \
}

#define ku_avoid_adv( __expr, __retval ) \
if ( __expr ) \
{ \
	ku_pavoid_debug(__FILE__, __PRETTY_FUNCTION__, __LINE__); \
	KU_ERRQ_V(KE_ASSERT, "Expression '" #__expr "` was not avoided", __retval); \
}

#define ku_avoid_thr( __expr ) \
if ( __expr ) {\
	ku_pavoid_debug(__FILE__, __PRETTY_FUNCTION__, __LINE__); \
	throw std::exception; \
}

#define func_debug_on ku_func_debug(1)
#define func_debug_off ku_func_debug(0)

#define pstart() ku_pstart_debug(__FUNCTION__, NULL)
#define pstartp( m, ... ) ku_pstart_debug(__FUNCTION__, m, ##__VA_ARGS__)
#define pstop() ku_pstop_debug(__FUNCTION__, NULL)
#define pstopp( m, ... ) ku_pstop_debug(__FUNCTION__, m, ##__VA_ARGS__)
#define preturn return ku_pstop_debug(__FUNCTION__, NULL),
#define preturnp( m, ... ) return ku_pstop_debug(__FUNCTION__, m, ##__VA_ARGS__),

#else	//	DEBUG

//! Print the debug message.
/*!
	Prints message to the \e stdout if DEBUG is defined else does nothing.
	\param m Message format.
*/
#define pdebug( m, ... )

//! Assess the expression.
/*!
	Checks the expression. If it is true, then logs a message and returns
	\a KE_ASSERT.
	\param expr Expression.
	\sa ku_avoid_adv().
*/
#define ku_avoid( expr )

//! Assess the expression (advanced).
/*!
	Checks the expression. If it is true, then logs a message and returns
	an error value.
	\param expr Expression.
	\param retval Value to be returned.
	\sa ku_avoid().
*/
#define ku_avoid_adv( expr, retval )

//! Asses the expression.
/*!
 * \param expr Boolean expression.
 * \sa ku_avoid().
 * \since 2.0.0
 * Checks the expression. If it is true, then logs a message and throws an
 * exception.
 */
#define ku_avoid_thr( expr )

//! Turn on function debug wath.
#define func_debug_on

//! Turn off function debug watch.
#define func_debug_off

//! Declare the function start.
/*!
	If function debug watch is enabled then a message informing that the function
	has started is sent to the \e stdout.
	\sa pstop() and preturn().
*/
#define pstart()

#define pstartp( m, ... )

//! Declare the function end.
/*!
	If function debug watch is enabled then a message informing that the function
	has stopped is sent to the \e stdout.
	\sa pstart() and preturn().
*/
#define pstop()

#define pstopp( m, ... )

//! Declare the function end and return from it.
/*!
	If function debug watch is enabled then a message informing that the function
	has stopped is sent to the \e stdout.
	And function returnes.
	\param __retval Returned value.
	\sa pstart() and pstop().
*/
#define preturn return

#define preturnp( m, ... ) return

#endif	//	DEBUG

//! Print a CHECK POINT message if DEBUG is defined.
#define __ pdebug("CHECK POINT\n");

KU_END_DECLS
#endif
