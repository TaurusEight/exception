#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

// Time-stamp: <2017-09-14 16:52:00 dmendyke>

// Based on work that appeared in Dr Dobbs Journal November 01, 2000
// http://www.drdobbs.com/cpp/exception-handling-in-c-without-c/184404317?queryText=RTFiles
//
// Initially copyright (c) 1998, 99 On Time, http://www-on-time.com

// Overview:
// This set of functions defines a means of error handling within
// straight "C" application in a manner simalure to C++ exceptions.


// Header file and macros to use the functions
//-----------------------------------------------------------------------------
#include <setjmp.h>
#define EXCEPTION_ENV jmp_buf
#define EXCEPTION_SAVE( c ) setjmp( *( c ) )
#define EXCEPTION_RESTORE( c, v ) longjmp( *( c ), v )


// Define the record that will hold data on each exception
//-----------------------------------------------------------------------------
typedef struct _exception_record {

  struct _exception_record* next;
  EXCEPTION_ENV env;
  int code;
  int handled;
  enum { user_code, handling, finally } state;

} exception_record;


// special exception codes

// Exception codes used by internal functions
//-----------------------------------------------------------------------------
#define NO_EXCEPTION 0  // No exception has been thrown
#define PROTECTED_CODE NO_EXCEPTION  // Run the users protected code
#define EXCEPTION_FINAL -1  // Always run this code
#define DEFAULT_EXCEPTION 1  // used when unknown error occurs


// predefinitions of internal function
//-----------------------------------------------------------------------------
void exception_link( exception_record* );
int exception_unlink( exception_record*, int );

// API macros and functions



// Macros defining flag for exception_unlink
//-----------------------------------------------------------------------------
#define CAN_RETURN 1
#define CAN_NOT_RETURN 0


// Macros used by client program to wrap code inside exceptions
//-----------------------------------------------------------------------------

// Define the start of an exception block
#define EXCEPTION_TRY \
  while (1) { \
    exception_record rec_; \
    exception_link( &rec_ ); \
    switch ( EXCEPTION_SAVE( &rec_.env ) )

// used within functions that return values
#define EXCEPTION_RETURN \
{ int x = rec_.code; \
  if ( exception_unlink( &rec_, CAN_RETURN ) ) return rec_.code; \
  else if ( x <= 0 ) break; } }

// used within functions that do not return values
#define EXCEPTION_END \
  if ( exception_unlink( &rec_, CAN_NOT_RETURN ) == 0 ) break; }

#define EXCEPTION_CODE rec_.code

// External functions used by the application
void exception_throw( int );  // Cause an exception to be executed


#endif  // __EXCEPTION_H__
