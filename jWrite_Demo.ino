// jWrite_Demo.ino
//
// Test sketch for Arduino version of jWrite 
// - tested using Arduino IDE on Arduino UNO
//
// see: https://www.codeproject.com/Articles/887604/jWrite-a-really-simple-JSON-writer-in-C
//
// TonyWilk
// 14mar2018
//
#include "jWrite.hpp"

void setup()
{
	Serial.begin(9600);		// outputs results to Serial Monitor
	Sprint("jWrite Demo for Arduino\n");

	char buffer[513];           // application buffer for JSON text
  buffer[512]= 42;            // telltale off end of buffer
	unsigned int buflen= 512;   // we say actual buffer is 512 bytes long
	int err;
	
	// Create instance of jWrite to use pre-defined buffer.
	jWrite jw( buffer, buflen );

	Sprint("A JSON object example:\n\n" );

	jw.open( JW_OBJECT, JW_PRETTY );		// start root object

	jw.obj_string( "key", "value" );				// add object key:value pairs
	jw.obj_int( "int", 1 );
	jw.obj_double( "double", 1.234 );
	jw.obj_null( "nullThing" );
	jw.obj_bool( "bool", 1 );
	jw.obj_array( "EmptyArray" );
		// empty array
	jw.end();
	jw.obj_array( "anArray" );					// array with elements
		jw.arr_string("array one" );
		jw.arr_int( -2 );
		jw.arr_double( 1234.567 );
		jw.arr_null();
		jw.arr_bool( 0 );
		jw.arr_object();							// object in array
			jw.obj_string( "obj3_one", "one");
			jw.obj_string( "obj3_two", "two");
		jw.end();
		jw.arr_array();							// array in array
			jw.arr_int( 0 );
			jw.arr_int( 1 );
			jw.arr_int( 2 );
		jw.end();
	jw.end();									// end of "anArray" , back to root object

	jw.obj_object( "EmptyObject" );
	jw.end();

	jw.obj_object( "anObject" );					// object in the root object
		jw.obj_string("msg","object in object");
		jw.obj_string("msg2","object in object 2nd entry");
	jw.end();
	jw.obj_string( "ObjEntry", "This is the last one" );

	err= jw.close();								// close and get error code

	Sprint( buffer );
	if( err != JWRITE_OK )
		Sprint( "Error: %s at function call %d\n", jw.errorToString(err), jw.errorPos() );

	Sprint("\n\nA JSON array example:\n\n" );

	jw.open( JW_ARRAY, JW_PRETTY );
	jw.arr_string( "String value" );
	jw.arr_int( 1234 );
	jw.arr_double( 567.89012 );
	jw.arr_bool( 1 );
	jw.arr_null();
	jw.arr_object();		
		// empty object
	jw.end();
	jw.arr_object();
		jw.obj_string( "key", "value" );
		jw.obj_string( "key2", "value2" );
	jw.end();
	jw.arr_array();		// array in array
		jw.arr_string("Array in array");
		jw.arr_string("the end");
	jw.end();
	jw.arr_string("Empty array next...");
	jw.arr_array();
		// empty array
	jw.end();
	err= jw.close();

	Sprint( buffer );
	if( err != JWRITE_OK )
		Sprint( "Error: %s at function call %d\n", jw.errorToString(err), jw.errorPos() );

	Sprint("\n\nExample JSON format error:\n\n" );
	// this is a copy of the above array example with an error introduced...
	//
	jw.open( JW_ARRAY, JW_PRETTY );	      // 1
	jw.arr_string( "String value" );	    // 2
	jw.arr_int( 1234 );								    // 3
	jw.arr_double( 567.89012 );				    // 4
	jw.arr_bool( 1 );								      // 5
	jw.arr_null();									      // 6
	jw.arr_object();									    // 7  <-- start object in array 
		// empty object
	//jw.end();
	jw.arr_object();									    // 8  <-- this is where the error is
		jw.obj_string( "key", "value" );		// 9
		jw.obj_string( "key2", "value2" );	// 10
	jw.end();										          // 11 
	jw.arr_array();		// array in array		// 12
		jw.arr_string("Array in array");		// 13
		jw.arr_string("the end");					  // 14
	jw.end();										          // 15
	jw.arr_string("Empty array next...");	// 16
	jw.arr_array();									      // 17
		// empty array
	jw.end();										          // 18
	err= jw.close();									

	Sprint( buffer );
	if( err != JWRITE_OK )
		Sprint( "Error: %s at function call %d\n", jw.errorToString(err), jw.errorPos() );

  Sprint("\n\nExample buffer overrun error...\n\n");
  jw.open( JW_ARRAY, JW_PRETTY );
  jw.arr_string("The first long string element in an array.....");
  for( int i=0; i<100; i++ )
    jw.arr_string("another long string element in an array.");
  jw.end();    
  err= jw.close();                  

  Sprint( buffer );
  if( err != JWRITE_OK )
    Sprint( "\nError: %s at function call %d\n", jw.errorToString(err), jw.errorPos() );

  Sprint("\n buffer last byte= %d, 'telltale' byte= %d, showing buffer was not actually overrun\n", 
        buffer[511], buffer[512] );

  Sprint("\n\nEnd of examples\n");
}

void loop()
{
	delay(1000);
}

//-----------------------------
// Serial.print helper function
// - a real cut-down printf()
//
void Sprint( const char *fmt, ... )
{
  char c;
  va_list args;
  va_start( args, fmt );
  while( (c=*fmt) != 0 ){
    switch( c )
    {
    case '%':
      c= *(++fmt);
      switch( c )
      {
      case 'd': Serial.print( va_arg(args,int) ); break;
      case 'f': Serial.print( va_arg(args,double) ); break;
      case 'x': Serial.print( va_arg(args,int), HEX ); break;
      case 'c': Serial.print( (char)va_arg(args,int) ); break;
      case 's': Serial.print( va_arg(args, char *) ); break;
      default:  break;
      }
      break;
    case '\\':
      c= *(++fmt);
      if( c == 'n' )
        Serial.println();
      else
        Serial.print( c );
      break; 
    default:
      Serial.print( c );
      break;
    }
    ++fmt;
  }
  va_end( args );
}

