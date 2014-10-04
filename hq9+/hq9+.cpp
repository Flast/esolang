#include <iostream>
#include <boost/timer/timer.hpp>

#include "hq9+.hpp"

int main() try
{
    boost::timer::auto_cpu_timer t(std::cerr, 3);

    hq9p::interpreter()
      .parse("HHQ+HQ++")
      .run();
    t.stop();

    std::cerr << '\n';
}
catch (hq9p::hq9p_error &e)
{
    std::cerr << e.what() << std::endl;
}

