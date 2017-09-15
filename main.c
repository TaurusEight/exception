
#include <stdlib.h>
#include <stdio.h>

#include <exception.h>

#define BAD_ERROR -127
#define DIVIDE_BY_ZERO -126


int other( ) {

  EXCEPTION_TRY {

    case PROTECTED_CODE:
      exception_throw( DIVIDE_BY_ZERO );
      printf( "This statement should not print\n" );
      break;

    case EXCEPTION_FINAL:
      printf( "end of the other\n" );
      break;

  } EXCEPTION_RETURN;

  return 0x0;

};  // end other


void stuff( ) {

  EXCEPTION_TRY {

    case PROTECTED_CODE:
      printf( "Nothing bad has happened in here\n" );
      exception_throw( BAD_ERROR );
      other();
      break;
    case BAD_ERROR:
      printf( "A BAD_ERROR!\n" );
      break;
    case EXCEPTION_FINAL:
      printf( "end of stuff\n" );
      break;

  } EXCEPTION_END;

  return;

};


int main( int argc, char* argv[] ) {

  EXCEPTION_TRY {

    case PROTECTED_CODE:
      printf( "inside main-ProtectedCode\n" );
      stuff();
      printf( "after stuff\n" );
      break;

    case DIVIDE_BY_ZERO:
      printf( "divide by zero\n" );
      break;

    case -6:
      printf( "error from all the way up the chain!\n" );
      break;


    case EXCEPTION_FINAL:
      printf( "Clean up\n" );
      break;

  } EXCEPTION_RETURN;

  return 0x0;

};  // end main
