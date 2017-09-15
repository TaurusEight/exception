
// Time-stamp: <2017-09-14 16:51:57 dmendyke>

// Based on work that appeared in Dr Dobbs Journal November 01, 2000
// http://www.drdobbs.com/cpp/exception-handling-in-c-without-c/184404317?queryText=RTFiles
//
// Initially copyright (c) 1998, 99 On Time, http://www-on-time.com

// Overview:
// This set of functions defines a means of error handling within
// straight "C" application in a manner simalure to C++ exceptions.


// Required header files
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <exception.h>


// If DEBUG is defined than additional error checking is performed and reported
//-----------------------------------------------------------------------------
#define DEBUG


// Used as flags to single if an exception has been handled
//-----------------------------------------------------------------------------
#define EXCEPTION_HANDLED 1
#define EXCEPTION_NOT_HANDLED 0


// Pointer to the current exception record
//-----------------------------------------------------------------------------
static void* current_record = NULL;


// Output an error message and then exit with given code
//-----------------------------------------------------------------------------
void exception_error( const char* message, int code ) {

  printf( "%s\n", message );
  exit( code );

};  // end exception_error


// Repetative error
//-----------------------------------------------------------------------------
void exception_corruption( ) {

  exception_error( "Exception chain is corrupted!", DEFAULT_EXCEPTION );

};  // end exception_corruption


// Throw an exception of the given value code
//-----------------------------------------------------------------------------
void exception_throw( int code ) {

   exception_record* record;

#ifdef DEBUG
   if ( code >= EXCEPTION_FINAL )
      exception_error( "A positive or reserved error code!", code );
#endif

   record = current_record;
   while ( record != NULL ) {

     record->handled = EXCEPTION_HANDLED;
     switch ( record->state ) {

     case user_code:
       record->code = code;
       record->state  = handling;
       EXCEPTION_RESTORE( &record->env, code );

     case handling:
       record->code = code;
       record->state  = finally;
       EXCEPTION_RESTORE( &record->env, EXCEPTION_FINAL );

     case finally:
#ifdef DEBUG
       exception_error( "Raise within finally", DEFAULT_EXCEPTION );
#endif
       record = record->next;
       current_record = record;
       break;

     default: exception_corruption();

     };  // end switch statement

   };  // end while statement

   exception_error( "An unhandled exception!", code );

};  // end exception_throw


// Set or reset the values of this exception record
//-----------------------------------------------------------------------------
void exception_set_record( exception_record* record ) {

  record->code = NO_EXCEPTION;
  record->state = user_code;
  record->handled = EXCEPTION_NOT_HANDLED;

};  // end exception_set_record


// Attach this exception record to the chain of record - set values
//-----------------------------------------------------------------------------
void exception_link( exception_record* record ) {

   record->next = current_record;
   current_record = record;
   exception_set_record( record );

};  // end XLinkExceptionRecord


// Unlink an exception record from the chain
//-----------------------------------------------------------------------------
int exception_unlink( exception_record* record, int returnable ) {

#ifdef DEBUG
  if ( current_record != record ) exception_corruption();
#endif

  switch ( record->state ) {

  case handling:              // exception is now handled
  case user_code:                  // no exception
    record->state = finally;
    EXCEPTION_RESTORE( &record->env, EXCEPTION_FINAL );

  case finally:               // we are done
    current_record = record->next;
    if ( record->handled == EXCEPTION_NOT_HANDLED ) return 0;
    else {
      if ( ( returnable == CAN_RETURN ) &&
           ( record->next == NULL ) ) return DEFAULT_EXCEPTION;
      exception_throw( record->code );
    };  // end else

  default: exception_corruption();

  };  // end switch statement

  return 0;

};  // end exception_unlink
