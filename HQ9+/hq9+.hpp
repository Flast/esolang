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
// stream
#include <ostream>

// container
#include <string>
#include <vector>
#include <unordered_map>

// others
#include <type_traits>
#include <memory>
#include <stdexcept>
#include <algorithm>

#include "../ecci.hpp"
// }}}

namespace hq9p
{

// class hq9p::hq9p_error {{{
class hq9p_error
  : public _ecci::_ecci_error
{
    typedef _ecci::_ecci_error __base;

public:
    explicit inline
    hq9p_error( const std::string &_x )
      : __base( "HQ9+", _x ) {}
};
// }}}

namespace _inst
{

// class hq9p::_inst::instruction {{{
class instruction
{
public:
    virtual
    ~instruction( void ) noexcept {}

    virtual auto
    operator()( void ) const
      -> void = 0;
};
// }}}

// class hq9p::_inst::H {{{
class H
  : public instruction
{
    typedef instruction __base;

private:
    std::ostream &sout;

public:
    H( std::ostream &_sout ) noexcept
      : sout( _sout ) {}

    auto
    operator()( void ) const
      -> void
    { this->sout << "Hello, world!"; }
};
// }}}

// class hq9p::_inst::Q {{{
class Q
  : public instruction
{
    typedef instruction __base;

public:
    auto
    operator()( void ) const
      -> void
    { throw hq9p_error( "todo: implement hq9p::_inst::Q" ); }
};
// }}}

// class hq9p::_inst::nine {{{
class nine
  : public instruction
{
    typedef instruction __base;

private:
    std::ostream &sout;

public:
    nine( std::ostream &_sout ) noexcept
      : sout( _sout ) {}

    auto
    operator()( void ) const
      -> void
    { throw hq9p_error( "todo: implement hq9p::_inst::nine" ); }
};
// }}}

// template < typename > class hq9p::_inst::plus {{{
template < typename Acc >
class plus
  : public instruction
{
    typedef instruction __base;

private:
    Acc &acc;

public:
    plus( Acc &_acc ) noexcept
      : acc( _acc ) {}

    auto
    operator()( void ) const
      -> void
    { throw hq9p_error( "todo: implement hq9p::_inst::plus" ); }
};
// }}}

} // namespace _inst

typedef std::unique_ptr< _inst::instruction > inst_ptr;

// class hq9p::interpret {{{
class interpret
  : public _ecci::_ecci_base
{
    typedef _ecci::_ecci_error __base;

private:
    std::ostream &sout;
    std::unordered_map< std::string, inst_ptr > pool;
    std::vector< typename std::enable_if< true,
      decltype( pool ) >::type::iterator > body;

public:
    explicit
    interpret( std::ostream &_sout = std::cout )
      : sout( _sout ) {}

    ~interpret( void ) noexcept {}

    inline auto
    parse( const std::string &code )
      -> void
    {}

    inline auto
    run( void )
      -> void
    {
// gcc (GCC) 4.6.0 20100805 (experimental)
// compiler internal error occurs at following code
//      std::for_each( this->body.begin(), this->body.end(),
//        [&]( const typename std::enable_if< true,
//          decltype( body ) >::type::value_type &c )
        std::for_each( this->body.begin(), this->body.end(),
          [&]( const typename std::enable_if< true,
            decltype( this->body ) >::type::value_type &c )
        { ( *c->second )(); } );
    }
};
// }}}

} // namespace hq9p

#endif // _HQ9p_hpp_
