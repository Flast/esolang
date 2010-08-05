#include <chrono>
using namespace std;

#include "hq9+.hpp"

auto main( void ) -> int try
{
    auto start = chrono::system_clock::now();

    HQ9p::interpret Interpreter;
    Interpreter.parse( "HHQ+HQ++" );
    Interpreter.run();

    auto end = chrono::system_clock::now();
    cout << endl << "spent: " << chrono::duration_cast< chrono::milliseconds >( end - start ).count() << "ms" << endl;
    return 0;
}
catch ( HQ9p::hq9p_error &e )
{ cout << e.what() << endl; }
