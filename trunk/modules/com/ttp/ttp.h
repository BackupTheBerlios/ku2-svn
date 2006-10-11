/*!
	\file
	\brief Text Transfer Protocol.
	
	Communication protocol for text data transfer.
	\author J. Anton
	\date
	\version 1.0.0
*/

#ifndef KU__TTP_H__
#define KU__TTP_H__
#ifdef __cplusplus
extern "C" {
#endif

//! Size of the message buffer.
#define TTP_BUFFER_SIZE 2048

//! Decode the protocol message.
/*!
	Decodes the protocol message and splits it into
	parameters. Rules:\n
	1. A message ends with the symbol with code 0.\n
	2. Any two parameters are separetad by one space.\n
	3. After @ folows non-controling symbol.\n
	4. After / folows ASCII code for a symbol.\n
	5. Symbols between quotation marks are the one parameter.
	\param msg Message to be parsed.
	\param args Array of pointers to parameters.
	\param argc_max Maximum parameter count.
	\return Parameter count of this protocol message or 0 if there was an error and
	\ref kucode is set to the valid value. Possible values: \n
	\e KE_EMPTY Command is empty.\n
	\e KE_FULL Too many parameters.\n
	\e KE_INVALID Invalid symbol code after /.\n
	\e KE_SYNTAX @ at the end of the command or no closing quotation mark.
*/
int ttp_decode( const char *msg, char **args, int argc_max );

#ifdef __cplusplus
}
#endif
#endif
