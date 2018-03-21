# jWriteCpp
C++ JSON Writer

Credit to [Tony Wilk](https://www.codeproject.com/Members/tonywilk)

Orginal publish [jWrite-a-really-simple-JSON-writer-in-C](https://www.codeproject.com/Articles/887604/jWrite-a-really-simple-JSON-writer-in-C)
 
## Introduction
jWriteCpp is a simple way of writing JSON to a char buffer in C++, directly from native variables. It manages the output buffer so you don't overrun, it handles all the fiddly quotes, brackets and commas and reports where you have tried to create invalid JSON.

## Using the Code
The original [jWrite](https://github.com/jonaskgandersson/jWrite) (in straight C) has been converted to a C++ class which can be used on any platform.

Using the C++ version is very similar to the C version, the C example:
```c
jwOpen( buffer, buflen, JW_OBJECT, JW_PRETTY );  // open root node as object
jwObj_string( "key", "value" );                  // writes "key":"value"
jwObj_int( "int", 1 );                           // writes "int":1
jwObj_array( "anArray");                         // start "anArray": [...] 
    jwArr_int( 0 );                              // add a few integers to the array
    jwArr_int( 1 );
    jwArr_int( 2 );
jwEnd();                                         // end the array
err= jwClose(); 
```
becomes:
```cpp
jWrite jw( buffer, buflen );        // Create jWrite instance to use application buffer 
jw.open( JW_OBJECT, JW_PRETTY );    // open root node as object 
jw.obj_string( "key", "value" );    // writes "key":"value" 
jw.obj_int( "int", 1 );             // writes "int":1 
jw.obj_array( "anArray");           // start "anArray": [...] 
   jw.arr_int( 0 );                 // add a few integers to the array 
   jw.arr_int( 1 ); 
   jw.arr_int( 2 ); 
jw.end();                           // end the array 
err= jw.close();                    // close root object - done
```
The jWrite_Demo.ino sketch shows several longer examples.

## License

This article, along with any associated source code and files, is licensed under [The Code Project Open License (CPOL)](https://www.codeproject.com/info/cpol10.aspx)
