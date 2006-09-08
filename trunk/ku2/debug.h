/***************************************************************************
 *            debug.h
 *
 *  Thu Aug 17 23:15:55 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Debug functions and definitions.
	
	This file contains the functions and definitions for debugging.
	\author J. Anton
	\date Thu Aug 17 23:17:34 2006
	\version 1.1.0
*/

#ifndef KU__DEBUG_H__
#define KU__DEBUG_H__
#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG

void printf_debug( const char *file, const char *func, int line, char *fmt, ... );
void pavoid_debug( const char *file, const char *func, int line );

#define pdebug( m, ... ) \
printf_debug(__FILE__, __FUNCTION__, __LINE__, m, ##__VA_ARGS__)

#define ku_avoid( expr ) \
if ( expr ) \
{ \
	pavoid_debug(__FILE__, __FUNCTION__, __LINE__); \
	return KE_ASSERT; \
}

#else	//	DEBUG

//! Print the debug message.
/*!
	Prints message to the \e stdout if DEBUG is defined else does nothing.
	\param m Message format.
*/
#define pdebug( m, ... )

//! Assert the expression.
/*!
	Checks the expression. If it is true, then logs a message and returnes
	\a KE_ASSERT.
	\param expr Expression.
*/
#define ku_avoid( expr )

#endif	//	DEBUG

#ifdef DEBUG_FUNC
/*! \cond skip */

#include <stdio.h>

#define pstart() printf("### ==> %s >>>\n",__FUNCTION__);
#define pstop() printf("### <== %s <<<\n",__FUNCTION__);

/*! \endcond */
#else	//	DEBUG_FUNC

//! Declare the function start.
/*!
	If \b DEBUG_FUNC is defined then a message informing that the function
	has started is sent to the \e stdout.
	\sa pstop().
*/
#define pstart()

//! Declare the function end.
/*!
	If \b DEBUG_FUNC is defined then a message informing that the function
	has stopped is sent to the \e stdout.
	\sa pstart().
*/
#define pstop()

#endif	// DEBUG_FUNC

//! Print a CHECK POINT message if DEBUG is defined.
#define _ pdebug("CHECK POINT\n");

#ifdef __cplusplus
}
#endif
#endif
