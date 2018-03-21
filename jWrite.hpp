//
// jWrite.hpp
//
// A *really* simple JSON writer in C++ (example target= Arduino UNO)
// - a collection of functions to generate JSON semi-automatically
//
// see: https://www.codeproject.com/Articles/887604/jWrite-a-really-simple-JSON-writer-in-C
//
// The idea is to simplify writing native C values into a JSON string and
// to provide some error trapping to ensure that the result is valid JSON.
//
// Example:
//		jWrite jw( buffer, buflen );		// create jWrite to write into supplied buffer
//		jw.open( JW_OBJECT, JW_PRETTY );	// open root node as object
//		jw.obj_string( "key", "value" );
//		jw.obj_int( "int", 1 );
//		jw.obj_array( "anArray");
//			jw.arr_int( 0 );
//			jw.arr_int( 1 );
//			jw.arr_int( 2 );
//		jw.end();
//		err= jw.close();					// close root object
//
// results in:
//
//		{
//		    "key": "value",
//		    "int": 1,
//		    "anArray": [
//		        0,
//		        1,
//		        2
//		    ]
//		}
//
// Note that jWrite handles string quoting and getting commas in the right place.
// If the sequence of calls is incorrect
// e.g.
//		jw.open( buffer, buflen, JW_OBJECT, 1 );
//		jw.obj_string( "key", "value" );
//			jw.arr_int( 0 );
//      ...
//
// then the error code returned from jw.close() would indicate that you attempted to
// put an array element into an object (instead of a key:value pair)
// To locate the error, the supplied buffer has the JSON created upto the error point
// and a call to jw.errorPos() would return the function call at which the error occurred
// - in this case 3, the 3rd function call "jw.arr_int(0)" is not correct at this point.
//
// The root JSON type can be JW_OBJECT or JW_ARRAY.
//
// For more information on each function, see the methods below.
//
//
// Original jWrite in C: TonyWilk, Mar 2015
// This C++ version: TonyWilk, Mar 2018
// 

#define JWRITE_STACK_DEPTH 32			// max nesting depth of objects/arrays

#define JW_COMPACT	0					// output string control for jwOpen()
#define JW_PRETTY	1					// pretty adds \n and indentation

// Error Codes
// -----------
#define JWRITE_OK           0
#define JWRITE_BUF_FULL     1		// output buffer full
#define JWRITE_NOT_ARRAY	2		// tried to write Array value into Object
#define JWRITE_NOT_OBJECT	3		// tried to write Object key/value into Array
#define JWRITE_STACK_FULL	4		// array/object nesting > JWRITE_STACK_DEPTH
#define JWRITE_STACK_EMPTY	5		// stack underflow error (too many 'end's)
#define JWRITE_NEST_ERROR	6		// nesting error, not all objects closed when jwClose() called

enum jwNodeType{
	JW_OBJECT= 1,
	JW_ARRAY
};


class jWrite{
  private:
	// Variables:
	char *buffer;						// pointer to application's buffer
	unsigned int buflen;		// length of buffer
	char *bufp;							// current write position in buffer
	char tmpbuf[32];				// local buffer for int/double convertions
	int error;							// error code
	int callNo;							// API call on which error occurred
	struct jwNodeStack{
		enum jwNodeType nodeType;
		int elementNo;
	} nodeStack[JWRITE_STACK_DEPTH];	// stack of array/object nodes
	int stackpos;
	int isPretty;						// 1= pretty output (inserts \n and spaces)
	// private methods:
	void putch( const char c );
	void putstr( const char *str );
	void putraw( const char *str );
	void modp_itoa10(int32_t value, char* str);
	void modp_dtoa2(double value, char* str, int prec);
	void pretty();
	enum jwNodeType pop();
	void push( enum jwNodeType nodeType );	
	int _jwObj( const char *key );
	int _jwArr( );
	
  public:
	jWrite( char *pbuffer, int buf_len ) : buffer(pbuffer), buflen(buf_len){
		open( JW_OBJECT, JW_COMPACT );
	};
	
	// open
	// - initialises jWrite
	// - rootType is the base JSON type: JW_OBJECT or JW_ARRAY
	// - isPretty controls 'prettifying' the output: JW_PRETTY or JW_COMPACT
	void open( enum jwNodeType rootType, int is_Pretty );

	// close
	// - closes the element opened by open()
	// - returns error code (0 = JWRITE_OK)
	// - after an error, all following jWrite calls are skipped internally
	//   so the error code is for the first error detected
	int close( );

	// errorPos
	// - if jwClose returned an error, this function returns the number of the jWrite function call
	//   which caused that error.
	int errorPos( );

	// Object insertion functions
	// - used to insert "key":"value" pairs into an object
	//
	void obj_string( const char *key, const char *value );
	void obj_int( const char *key, int value );
	void obj_double( const char *key, double value );
	void obj_bool( const char *key, int oneOrZero );
	void obj_null( const char *key );
	void obj_object( const char *key );
	void obj_array( const char *key );

	// Array insertion functions
	// - used to insert "value" elements into an array
	//
	void arr_string( const char *value );
	void arr_int( int value );
	void arr_double( double value );
	void arr_bool( int oneOrZero );
	void arr_null( );
	void arr_object( );
	void arr_array( );

	// jwEnd
	// - defines the end of an Object or Array definition
	int end( );

	// these 'raw' routines write the JSON value as the contents of rawtext
	// i.e. enclosing quotes are not added
	// - use if your app. supplies its own value->string functions
	//
	void obj_raw( const char *key, const char *rawtext );
	void arr_raw( const char *rawtext );
	
	// Returns '\0'-termianted string describing the error (as returned by jwClose())
	//
	const char * errorToString( int err );

};

/* end of jWrite.hpp */
