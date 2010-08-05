// HQ9+ Interpreter - hq9+.hpp
//                  Copyright(c) 2010 - 2010 Flast All rights reserved.

// COPYING {{{
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// }}}

// used C++0x features and required gcc version list {{{
//  gcc 4.4 | New function declarator syntax
// }}}

#ifndef _HQ9p_hpp_
#define _HQ9p_hpp_

// Headers {{{
// container
#include <string>

// others
#include <stdexcept>

#include "../ecci.hpp"
// }}}

namespace HQ9p
{

// class HQ9p::hq9p_error {{{
class hq9p_error
  : public std::runtime_error
{
    typedef std::runtime_error __base;

public:
    explicit inline
    hq9p_error( const std::string &_x )
      : __base( _x ) {}
};
// }}}

// class HQ9p::interpret {{{
class interpret
  : public _ecci::_ecci_base
{
public:
    inline auto
    parse( const std::string &code )
      -> void
    {}

    inline auto
    run( void )
      -> void
    {}
};
// }}}

} // namespace HQ9p

#endif // _HQ9p_hpp_
