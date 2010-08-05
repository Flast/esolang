// Esolang compiler collections interpreter - ecci.hpp
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
//  gcc 4.6 | noexcept expression
// }}}

#ifndef _ecci_hpp_
#define _ecci_hpp_

// Headers {{{
#include <string>
#include <istream>
#include <ostream>
#include <iostream>
// }}}

namespace _ecci
{

// class _ecci::_ecci_base {{{
class _ecci_base
{
private:
    std::istream &_sin;
    std::ostream &_sout;

protected:
    _ecci_base( std::istream &_in, std::ostream &_out ) noexcept
      : _sin( _in ), _sout( _out ) {}

    _ecci_base( void ) noexcept
      : _sin( std::cin ), _sout( std::cout ) {}
//    : _ecci_base( std::cin, std::cout ) {}

    explicit
    _ecci_base( std::iostream &_inout ) noexcept
      : _sin( _inout ), _sout( _inout ) {}
//    : _ecci_base( std::cin, std::cout ) {}

    inline auto
    in( void ) noexcept
      -> std::istream &
    { return this->_sin; }

    inline auto
    in( void ) const noexcept
      -> const std::istream &
    { return this->_sin; }

    inline auto
    out( void ) noexcept
      -> std::ostream &
    { return this->_sout; }

    inline auto
    out( void ) const noexcept
      -> const std::ostream &
    { return this->_sout; }

public:
    virtual
    ~_ecci_base( void ) noexcept {}

    virtual auto
    parse( const std::string & )
      -> void = 0;

    virtual auto
    run( void )
      -> void = 0;
};
// }}}

} // namespace _ecci

#endif // _ecci_hpp_
