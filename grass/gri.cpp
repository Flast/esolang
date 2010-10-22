#include <iostream>
#include <chrono>
using namespace std;

#include "gri.hpp"

int
main( void )
try
{
    auto start = chrono::system_clock::now();
    grass::interpret Interpreter;
    Interpreter.parse( "wwWWwWWWwvwwWWWwwWwwWWwvwWWWwwwwwWwwvwWWwWWWWWWWwvwWWWWwwwwwwwWwwvwWWWWWWwwwWwwvwWWWWWWwwwWWWWWWWWwW" );
    Interpreter.parse( "wwwvwWWWWWWWwwWwwWWWWwWWWWWWwWWWWWWWWWwvwWWWWWWWWWWWwvwWWWWWWWWWWWwvwWWWWwwwwwwwwwwwwwwvwWWwWWWWWWWw" );
    Interpreter.parse( "WWWWWWwWWWWWWwWWWWWWWwwwwvwWWWwWWWWWWWWwWWWWWWWWWwWWWWWWWWwvwWWWWWWWWWWWWwvwWWWWWWWWWWWwvwWWWWWWWwvw" );
    Interpreter.parse( "WWWWWwWWWWwWWWWWWWWWWWwWWWWWWWwWWWWWWwWWWWWWWwwwwwWWWWWWWWWwwWWWWWWWWWWwWWWWWWWWWWwWWWWWWWWWWWwWWWWW" );
    Interpreter.parse( "WWWWWWWWWwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwWWWWWWWWWWWWWwWWWWWWWWWWWWWWWWWWWwWWWWWWWWWWWWWWWwvwWWWWWWwWW" );
    Interpreter.parse( "WWWwWWWWWWwWWWWWWWwWWWWWWWWWWWWWWwWWWWWWWWwWWWWWWWWWwwwwwWWWWWWWWWWwwwwwwwwwwwwwwwwwwwwwwwwwwwwWWWWW" );
    Interpreter.parse( "WWWWWWWWWWWwWWWWWWWWWWWWwvwvwWWwwwwwwwwwwwwwwwwwwwwwwwvwWWWWWWWWWwWWWwWWWWWWWWWWwWWWWWWWWWwWWWWWWWWW" );
    Interpreter.parse( "WWwwwwWWWWWWWWWWWwWWWWWWWWWWWWwwwwwwWWWWWWWWWWWWWWWWWWWWwwwWWWWWWWWWWWWWWWwWWWWWWWWWWWWWWWWwWWWWWWWW" );
    Interpreter.parse( "WWWWWWWWWwWWWWWWWWWWWWWWWWWwvwWWWWWWWWWWWWwWWWWWWWWWWWWWWWwWWWWWWWWWWWWWWWwWWWWWWWWWWWWWWWWwvwWWWWWW" );
    Interpreter.parse( "WWWWWWwWWWWWWWWwWWWWWWWWwWWWWWWwWWWWWWw" );
    Interpreter.run();
    auto end = chrono::system_clock::now();
    cout << endl << "spent: " << chrono::duration_cast< chrono::milliseconds >( end - start ).count() << "ms" << endl;
}
catch ( grass::grass_error &e )
{ cout << e.what() << endl; }

