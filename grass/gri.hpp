// Grass Interpreter - gri.hpp
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
//  gcc 4.3 | Rvalue reference
//          | Declared type of an expression
//  gcc 4.4 | initializer lists
//          | auto-typed variables
//          | New function declarator syntax
//          | Strongly-typed enums
//          | Default and deleted functions
//          | unique_ptr that using move semantics container
//  gcc 4.5 | C++0x lambdas
//          | Explicit conversion operators
//  gcc 4.6 | Null pointer constant
//          | noexcept expression
// }}}

#ifndef _gri_hpp_
#define _gri_hpp_

// Headers {{{
// stream
#include <istream>
#include <ostream>
#include <iostream>

// container
#include <utility>
#include <initializer_list>
#include <vector>
#include <set>
#include <stack>

// algorithm
#include <type_traits>
#include <algorithm>

// others
#include <string>
#include <memory>

#include "../ecci.hpp"
// }}}

namespace grass
{

class environment;
class interpret;

// class grass::grass_error {{{
class grass_error
  : public _ecci::_ecci_error
{
    typedef _ecci::_ecci_error __base;

public:
    explicit inline
    grass_error( const std::string &_x )
      : __base( "grass", _x ) {}
};
// }}}

namespace _lambda
{

// class grass::_lambda::lambda_error {{{
class lambda_error
  : public grass_error
{
    typedef grass_error __base;

public:
    explicit inline
    lambda_error( const std::string &_x )
      : __base( _x ) {}
};
// }}}

class lambda;
typedef std::unique_ptr< lambda > lambda_unique_ptr;

// class grass::_lambda::lambda_ptr {{{
class lambda_ptr
{
private:
    const lambda *_ptr;

    inline auto
    nullcheck( void ) const throw( lambda_error )
      -> void
    {
        if ( this->_ptr == nullptr )
        { throw lambda_error( "null pointer" ); }
    }

public:
    lambda_ptr( const lambda_ptr & ) = default;
    lambda_ptr( lambda_ptr && ) = default;
    inline auto
    operator=( const lambda_ptr & )
      -> lambda_ptr & = default;

    explicit inline
    lambda_ptr( const lambda *ptr = nullptr )
      : _ptr( ptr ) {}

    explicit inline
    lambda_ptr( const lambda_unique_ptr &ptr )
      : _ptr( ptr.get() ) {}
//    : lambda_ptr( ptr.get() ) {}

    explicit inline
    lambda_ptr( lambda_unique_ptr &&ptr )
      : _ptr( ptr.get() ) {}
//    : lambda_ptr( ptr ) {}

    inline auto
    get( void ) const
      -> const lambda *
    { return this->_ptr; }

    explicit inline
    operator const lambda *( void ) const
    { return this->get(); }

    inline auto
    operator*( void ) const
      -> const lambda &
    { return this->nullcheck(), *this->get(); }

    inline auto
    operator->( void ) const
      -> const lambda *
    { return this->nullcheck(), this->get(); }
};
// }}}

// grass::_lambda::lambda_pool {{{
class lambda_pool
  : public std::set< const lambda * >
{
    typedef std::set< const lambda * > __base;

private:
    std::vector< lambda_ptr > build_in_func;

public:
    // enum class BUILDIN {{{
    enum class BUILDIN
    {
      // primitive
      IN = 0,
      W,
      SUCC,
      OUT,

      // others
      ID,
      TRUE,
      FALSE,

      _SIZE
    };
    // }}}

    lambda_pool( void )
      : build_in_func( std::size_t( BUILDIN::_SIZE ) ) {}

    inline auto
    operator[]( BUILDIN buildin ) const
      -> lambda_ptr
    { return this->build_in_func.at( std::size_t( buildin ) ); }
};
// }}}

} // namespace _lambda

// class grass::environment {{{
class environment
  : public std::stack< _lambda::lambda_ptr,
      std::vector< _lambda::lambda_ptr > >
{
    typedef std::stack< _lambda::lambda_ptr,
      std::vector< _lambda::lambda_ptr > > __base;

public:
    environment( void ) = default;

    inline
    environment( const environment &env, std::size_t reserve = 0 )
    {
        this->c.reserve( env.size() + reserve );
        this->c.assign( env.c.begin(), env.c.end() );
    }

    inline auto
    operator[]( unsigned int idx ) const
      -> _lambda::lambda_ptr
    { return this->c[ this->size() - idx - 1 ]; }

    inline auto
    clear( void )
      -> void
    { this->c.clear(); }
};
// }}}

namespace _lambda
{

// class grass::_lambda::lambda {{{
class lambda
{
protected:
    environment env;

    mutable lambda_pool &pool;

    inline auto
    operator[]( unsigned int idx )
      -> lambda_ptr
    { return this->env[ idx + 1 ]; }

    inline auto
    push( const lambda_ptr &l )
      -> void
    { this->env.push( l ); }

    inline auto
    insert( const lambda *l ) const
      -> lambda_pool::value_type
    { return *this->pool.insert( l ).first; }

public:
    lambda( const lambda & ) = default;
    lambda( lambda && ) = default;

    explicit inline
    lambda( lambda_pool &_pool ) noexcept
      : pool( _pool ) {}

    virtual inline
    ~lambda( void ) noexcept {}

    virtual auto
    real_call( std::vector< lambda_ptr > && ) const
      -> lambda_ptr
    {
        throw lambda_error( "invalid real call" );
        return lambda_ptr();
    }

    virtual auto
    operator()( const lambda_ptr & ) const
      -> lambda_ptr = 0;

    virtual inline auto
    operator*( void ) const throw( lambda_error )
      -> char
    {
        throw lambda_error( "invalid reference" );
        return 0;
    }
};
// }}}

// class grass::_lambda::partial_apply {{{
class partial_apply
  : public lambda
{
    typedef lambda __base;

private:
    const unsigned int arg_num;
    const lambda_ptr   func, arg;

    inline auto
    real_call( std::vector< lambda_ptr > &&args ) const
      -> lambda_ptr
    {
        args.push_back( arg );
        return this->func->real_call( std::move( args ) );
    }

public:
    partial_apply( lambda_pool &_pool, unsigned int _num,
      const lambda_ptr &_func, const lambda_ptr &_arg ) noexcept
      : __base( _pool ), arg_num( _num ), func( _func ), arg( _arg ) {}

    inline auto
    operator()( const lambda_ptr &l ) const
      -> lambda_ptr
    {
        if ( this->arg_num != 1 )
        {
            lambda_unique_ptr uptr( new partial_apply( this->pool,
              this->arg_num - 1, lambda_ptr( this ), l ) );
            this->pool.insert( uptr.get() );
            return lambda_ptr( uptr.release() );
        }
        return this->func->real_call( { arg, l } );
    }
};
// }}}

// user defined function
// class grass::_lambda::user {{{
class user
  : public lambda
{
    typedef lambda __base;

    friend class grass::interpret;

public:
    typedef std::pair< unsigned int, unsigned int > app_pair_t;
    typedef std::vector< app_pair_t > body_t;

protected:
    typedef std::initializer_list< app_pair_t > init_body_t;

    const unsigned int arg_num;
    const body_t       body;

    inline
    user( const user &l )
      : __base( l ), arg_num( l.arg_num - 1 ),
        body( l.body ) {}

    inline auto
    real_call( std::vector< lambda_ptr > &&args ) const
      -> lambda_ptr
    {
        if ( this->arg_num != args.size() )
        { throw lambda_error( "invalid argument number" ); }

        environment renv( this->env, this->body.size() + this->arg_num );
        std::for_each( args.begin(), args.end(),
          [&]( lambda_ptr &l ) { renv.push( l ); } );

        std::for_each( this->body.begin(), this->body.end(),
          [&]( const app_pair_t &app )
        {
            const auto &func = renv[ app.first ],
                       &arg  = renv[ app.second ];
            renv.push( ( *func )( arg ) );
        } );
        return renv.top();
    }

public:
    inline
    user( lambda_pool &_pool, unsigned int _num, body_t &&il )
      : __base( _pool ), arg_num( _num ), body( std::move( il ) ) {}

    inline
    user( lambda_pool &_pool, unsigned int _num,
      init_body_t &&il = init_body_t() )
      : __base( _pool ), arg_num( _num ), body( std::move( il ) ) {}

    virtual inline
    ~user( void ) noexcept {}

    virtual inline auto
    operator()( const lambda_ptr &l ) const
      -> lambda_ptr
    {
        if ( this->arg_num != 1 )
        {
            lambda_unique_ptr uptr( new partial_apply( this->pool,
              this->arg_num - 1, lambda_ptr( this ), l ) );
            this->pool.insert( uptr.get() );
            return lambda_ptr( uptr.release() );
        }
        return this->real_call( { l } );
    }
};
// }}}

// class grass::_lambda::id {{{
class id
  : public user
{
    typedef user __base;

public:
    id( const id & ) = default;
    id( id && ) = default;

    explicit inline
    id( lambda_pool &_pool )
      : __base( _pool, 1 ) {}
};
// }}}

// class grass::_lambda::boolalpha {{{
class boolalpha
  : public user
{
    typedef user __base;

private:
    static inline auto
    gen_body( bool _b, init_body_t &&true_body,
      init_body_t &&false_body ) noexcept
      -> init_body_t &&
    { return std::move( _b ? true_body : false_body ); }

public:
    boolalpha( const boolalpha & ) = default;
    boolalpha( boolalpha && ) = default;

    inline
    boolalpha( lambda_pool &_pool, bool _b )
      : __base( _pool, 2, gen_body( _b, { { 3, 2 } }, {} ) )
    {
        if ( !_b )
        { return; }

        this->env.push( this->pool[ lambda_pool::BUILDIN::ID ] );
    }
};
// }}}

namespace primitive
{

// class grass::_lambda::primitive::w {{{
class w
  : public lambda
{
    typedef lambda __base;

private:
    const char c;

public:
    w( const w & ) = default;
    w( w && ) = default;

    inline
    w( lambda_pool &_pool, char _x = 'w' ) noexcept
      : __base( _pool ), c( _x ) {}

    inline auto
    operator()( const lambda_ptr &l ) const
      -> lambda_ptr
    {
        return this->pool[ **l == this->c
          ? lambda_pool::BUILDIN::TRUE
          : lambda_pool::BUILDIN::FALSE ];
    }

    inline auto
    operator*( void ) const noexcept
      -> char
    { return this->c; }
};
// }}}

// class grass::_lambda::primitive::succ {{{
class succ
  : public lambda
{
    typedef lambda __base;

public:
    succ( const succ & ) = default;
    succ( succ && ) = default;

    explicit inline
    succ( lambda_pool &_pool ) noexcept
      : __base( _pool ) {}

    inline auto
    operator()( const lambda_ptr &l ) const
      -> lambda_ptr
    {
        // FIXME: applicate singleton to character
        return lambda_ptr( this->insert(
          new w( this->pool, ( **l + 1 ) % 256 ) ) );
    }
};
// }}}

// class grass::_lambda::primitive::in {{{
class in
  : public lambda
{
    typedef lambda __base;

private:
    std::istream &sin;

public:
    in( const in & ) = default;
    in( in && ) = default;

    inline
    in( lambda_pool &_pool, std::istream &_in ) noexcept
      : __base( _pool ), sin( _in ) {}

    inline auto
    operator()( const lambda_ptr &l ) const
      -> lambda_ptr
    {
        int c = this->sin.get();
        if ( c == EOF )
        { return l; }

        // FIXME: applicate singleton to character
        return lambda_ptr( this->insert(
          new w( this->pool, c ) ) );
    }
};
// }}}

// class grass::_lambda::primitive::out {{{
class out
  : public lambda
{
    typedef lambda __base;

private:
    std::ostream &sout;
    bool force;

public:
    out( const out & ) = default;
    out( out && ) = default;

    inline
    out( lambda_pool &_pool, std::ostream &_out, bool _f = false ) noexcept
      : __base( _pool ), sout( _out ), force( _f ) {}

    inline auto
    operator()( const lambda_ptr &l ) const
      -> lambda_ptr
    {
        try
        { this->sout.put( **l ); }
        catch ( const lambda_error & )
        {
            if ( !this->force )
            { throw; }
            this->sout << "<lambda>";
        }
        return l;
    }
};
// }}}

} // namespace primitive

} // namespace _lambda

// class grass::interpret {{{
class interpret
  : public _ecci::_ecci_base
{
    typedef _ecci::_ecci_base __base;
    typedef _lambda::lambda_pool::BUILDIN BUILDIN;

    interpret &
    operator=( const interpret & ) = delete;
    interpret &
    operator=( interpret && ) = delete;

private:
    environment env;
    _lambda::lambda_pool pool;

    std::string buf;

    inline auto
    inserter( _lambda::lambda *ptr )
      -> _lambda::lambda_ptr
    try
    {
        if ( auto uptr = dynamic_cast< _lambda::user * >( ptr ) )
        { uptr->env = this->env; }

        _lambda::lambda_ptr lptr( ptr );
        this->env.push( lptr );
        this->pool.insert( ptr );
        return lptr;
    }
    catch ( ... )
    {
        delete ptr;
        throw;
    }

    inline auto
    init( bool force_out )
      -> void
    {
        this->release();

        namespace lp = _lambda::primitive;
        this->pool[ BUILDIN::IN ] =
          this->inserter( new lp::in( this->pool, this->in() ) );
        this->pool[ BUILDIN::W ] =
          this->inserter( new lp::w( this->pool ) );
        this->pool[ BUILDIN::SUCC ] =
          this->inserter( new lp::succ( this->pool ) );
        this->pool[ BUILDIN::OUT ] =
          this->inserter( new lp::out( this->pool, this->out(), force_out ) );
    }

    inline auto
    release( void ) noexcept
      -> void
    {
        this->env.clear();
        std::for_each( this->pool.begin(), this->pool.end(),
          []( const _lambda::lambda *ptr ) noexcept { delete ptr; } );
        this->pool.clear();
    }

    inline auto
    parser_impl( void )
      -> void;

public:
    explicit inline
    interpret( bool _force_out = false )
    { this->init( _force_out ); }

    inline
    interpret( std::istream &_in, std::ostream &_out,
      bool _force_out = false )
      : __base( _in, _out )
    { this->init( _force_out ); }
//    : __base( _in, _out ), interpret( _force_out ) {}

    explicit inline
    interpret( std::iostream &_inout, bool _force_out = false )
      : __base( _inout )
    { this->init( _force_out ); }
//    : __base( _inout ), interpret( _force_out ) {}

    inline
    ~interpret( void ) noexcept
    { this->release(); }

    inline auto
    parse( const std::string &code )
      -> void
    {
        auto validity_checker = []( char c )
        { return c == 'w' || c == 'W' || c == 'v'; };

        std::string tmp_code( std::count_if(
          code.begin(), code.end(), validity_checker ), 0 );
        std::copy_if( code.begin(), code.end(),
          tmp_code.begin(), validity_checker );
        this->buf += tmp_code;
        this->parser_impl();
    }

    inline auto
    run( void )
      -> void
    {
        if ( this->buf.size() != 0 )
        {
            this->buf += 'v';
            this->parser_impl();
        }
        auto &top = this->env.top();
        this->env.push( ( *top )( top ) );
    }
};
// }}}

namespace
{

// grass::<anonymous namespace>::continuos_region() {{{
template < typename _ForwardIterator >
inline auto
continuos_region( _ForwardIterator _first, _ForwardIterator _last )
  -> std::pair< std::size_t, _ForwardIterator >
{
    std::size_t cnt = 0;
    for ( auto &_val = *_first;
      _first != _last && _val == *_first;
      ++_first, ++cnt );
    return std::make_pair( cnt, _first );
}
// }}}

}

// grass::interpret::parser_impl() {{{
auto interpret::parser_impl( void )
  -> void
{
    const auto &cend = this->buf.end();
    auto       &&itr = this->buf.begin(), continue_itr = itr;

    enum
    {
      GR_TOPLEVEL,
      GR_APPLICATION,
      GR_FUNCTION
    } state = GR_TOPLEVEL;

    std::size_t args = 0, func = 0;
    _lambda::user::body_t body;

    // TODO: refactor this block
    while ( itr != cend )
    {
        auto region = continuos_region( itr, cend );
        char c = *itr;
        if ( c == 'v' )
        {
            switch ( state )
            {
              case GR_APPLICATION:
                throw grass_error( "internal error (unexpected application terminate)" );

              case GR_FUNCTION:
                this->inserter( new _lambda::user( this->pool, args, std::move( body ) ) );
                state = GR_TOPLEVEL; // PATH THROUGH

              case GR_TOPLEVEL: // PATH THROUGH
                continue_itr = region.second;
                break;
            }
        }
        else
        {
            switch ( state )
            {
              case GR_TOPLEVEL:
                switch ( c )
                {
                  case 'w':
                    state = GR_FUNCTION;
                    args  = region.first;
                    break;

                  case 'W':
                    state = GR_APPLICATION;
                    func  = region.first;
                    break;
                }
                break;

              case GR_APPLICATION:
                if ( c != 'w' )
                { throw grass_error( "internal error (unexpected char in application)" ); }
                this->env.push( ( *this->env[ func ] )( this->env[ region.first ] ) );
                state = GR_TOPLEVEL;
                continue_itr = region.second;
                break;

              case GR_FUNCTION:
                if ( c != 'W' )
                { throw grass_error( "internal error (unexpected char in define function)" ); }
                func = region.first;

                region = continuos_region( itr = region.second, cend );
                if ( region.second == cend )
                { break; }

                if ( *itr != 'w' )
                { throw grass_error( "internal error (unexpected char in function args)" ); }
                body.push_back( typename std::enable_if< true, decltype( body ) >::type::value_type( func - 1, region.first - 1 ) );
//  And, I hope to become able to write down follow code.
//              body.push_back( typename decltype( body )::value_type( func - 1, region.first - 1 ) );
                break;
            }
        }
        itr = region.second;
    }

    this->buf = std::string( continue_itr, cend );
}
// }}}

} // namespace grass

#endif // _gri_hpp_
