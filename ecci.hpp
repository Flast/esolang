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
// stream
#include <istream>
#include <ostream>
#include <iostream>
#include <sstream>

// others
#include <string>
#include <stdexcept>
// }}}

namespace _ecci
{

// class _ecci::_ecci_error {{{
class _ecci_error
  : public std::runtime_error
{
    typedef std::runtime_error __base;

private:
    std::string _buf;

    static inline std::string &&
    cat( const std::string &lang, const std::string &_x,
      std::string &&_buf = std::string() )
    {
        std::stringstream ss;
        ss << "ecci faltal (" << lang << "): " << _x;
        auto rtol = []( std::string &&str ) -> std::string &
        { return static_cast< std::string & >( str ); };
        _buf.swap( rtol( ss.str() ) );
        return std::move( _buf );
    }

public:
    _ecci_error( const std::string &lang, const std::string &_x )
      : __base( cat( lang, _x ) ) {}

    virtual
    ~_ecci_error( void ) noexcept {}
};
// }}}

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

    inline std::istream &
    in( void ) noexcept
    { return this->_sin; }

    inline const std::istream &
    in( void ) const noexcept
    { return this->_sin; }

    inline std::ostream &
    out( void ) noexcept
    { return this->_sout; }

    inline const std::ostream &
    out( void ) const noexcept
    { return this->_sout; }

public:
    virtual
    ~_ecci_base( void ) noexcept {}

    virtual void
    parse( const std::string & ) =0;

    virtual void
    run( void ) = 0;
};
// }}}

} // namespace _ecci

#endif // _ecci_hpp_

