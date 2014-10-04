#include <iostream>
#include <boost/timer/timer.hpp>

#include "gri.hpp"

int main() try
{
    boost::timer::auto_cpu_timer t(std::cerr, 3);

    grass::interpreter()
      .parse("wwWWwWWWwvwwWWWwwWwwWWwvwWWWwwwwwWwwvwWWwWWWWWWWwv")
      .parse("wWWWWwwwwwwwWwwvwWWWWWWwwwWwwvwWWWWWWwwwWWWWWWWWwW")
      .parse("wwwvwWWWWWWWwwWwwWWWWwWWWWWWwWWWWWWWWWwvwWWWWWWWWW")
      .parse("WWwvwWWWWWWWWWWWwvwWWWWwwwwwwwwwwwwwwvwWWwWWWWWWWw")
      .parse("WWWWWWwWWWWWWwWWWWWWWwwwwvwWWWwWWWWWWWWwWWWWWWWWWw")
      .parse("WWWWWWWWwvwWWWWWWWWWWWWwvwWWWWWWWWWWWwvwWWWWWWWwvw")
      .parse("WWWWWwWWWWwWWWWWWWWWWWwWWWWWWWwWWWWWWwWWWWWWWwwwww")
      .parse("WWWWWWWWWwwWWWWWWWWWWwWWWWWWWWWWwWWWWWWWWWWWwWWWWW")
      .parse("WWWWWWWWWwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwWWWWWWWWWWW")
      .parse("WWwWWWWWWWWWWWWWWWWWWWwWWWWWWWWWWWWWWWwvwWWWWWWwWW")
      .parse("WWWwWWWWWWwWWWWWWWwWWWWWWWWWWWWWWwWWWWWWWWwWWWWWWW")
      .parse("WWwwwwwWWWWWWWWWWwwwwwwwwwwwwwwwwwwwwwwwwwwwwWWWWW")
      .parse("WWWWWWWWWWWwWWWWWWWWWWWWwvwvwWWwwwwwwwwwwwwwwwwwww")
      .parse("wwwwvwWWWWWWWWWwWWWwWWWWWWWWWWwWWWWWWWWWwWWWWWWWWW")
      .parse("WWwwwwWWWWWWWWWWWwWWWWWWWWWWWWwwwwwwWWWWWWWWWWWWWW")
      .parse("WWWWWWwwwWWWWWWWWWWWWWWWwWWWWWWWWWWWWWWWWwWWWWWWWW")
      .parse("WWWWWWWWWwWWWWWWWWWWWWWWWWWwvwWWWWWWWWWWWWwWWWWWWW")
      .parse("WWWWWWWWwWWWWWWWWWWWWWWWwWWWWWWWWWWWWWWWWwvwWWWWWW")
      .parse("WWWWWWwWWWWWWWWwWWWWWWWWwWWWWWWwWWWWWWw")
      .run();
    t.stop();

    std::cerr << '\n';
}
catch (grass::grass_error &e)
{
    std::cerr << e.what() << std::endl;
}

