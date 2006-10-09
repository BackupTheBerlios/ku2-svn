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
	\version 1.2.0
*/

#ifndef KU__DEBUG_H__
#define KU__DEBUG_H__
#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG

void printf_debug( const char *file, const char *func, int line, char *fmt, ... );
void pavoid_debug( const char *file, const char *func, int line );
void func_debug( int status );
void pstart_debug( const char *func );
void pstop_debug( const char *func );

#define pdebug( m, ... ) \
printf_debug(__FILE__, __FUNCTION__, __LINE__, m, ##__VA_ARGS__)

#define ku_avoid( expr ) \
if ( expr ) \
{ \
	pavoid_debug(__FILE__, __FUNCTION__, __LINE__); \
	KU_ERRQ(KE_ASSERT); \
}

#define ku_avoid_adv( expr, retval ) \
if ( expr ) \
{ \
	pavoid_debug(__FILE__, __FUNCTION__, __LINE__); \
	kucode = KE_ASSERT; \
	return retval; \
}

#define func_debug_on func_debug(1)
#define func_debug_off func_debug(0)

#define pstart() pstart_debug(__FUNCTION__)
#define pstop() pstop_debug(__FUNCTION__)

#else	//	DEBUG

//! Print the debug message.
/*!
	Prints message to the \e stdout if DEBUG is defined else does nothing.
	\param m Message format.
*/
#define pdebug( m, ... )

//! Assess the expression.
/*!
	Checks the expression. If it is true, then logs a message and returnes
	\a KE_ASSERT.
	\param expr Expression.
	\sa ku_avoid_adv().
*/
#define ku_avoid( expr )

//! Assess the expression (advanced).
/*!
	Checks the expression. If it is true, then logs a message and returnes
	an error value.
	\param expr Expression.
	\param retval Value to be returned.
	\sa ku_avoid().
*/
#define ku_avoid_adv( expr, retval )

//! Turn on function debug wath.
#define func_debug_on

//! Turn off function debug watch.
#define func_debug_off

//! Declare the function start.
/*!
	If function debug watch is enabled then a message informing that the function
	has started is sent to the \e stdout.
	\sa pstop().
*/
#define pstart()

//! Declare the function end.
/*!
	If function debug watch is enabled then a message informing that the function
	has stopped is sent to the \e stdout.
	\sa pstart().
*/
#define pstop()

#endif	//	DEBUG

//! Print a CHECK POINT message if DEBUG is defined.
#define _ pdebug("CHECK POINT\n");

#ifdef __cplusplus
}
#endif
#endif
