// Grass interpreterer - gri.hpp
//                  Copyright(c) 2010 - 2010 Flast All rights reserved.

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

#ifndef esolang_gri_hpp_
#define esolang_gri_hpp_

#include <istream>
#include <ostream>
#include <iostream>

#include <utility>
#include <initializer_list>
#include <vector>
#include <set>
#include <stack>

#include <type_traits>
#include <algorithm>

#include <string>
#include <memory>
#include "../memory.hpp"
#include <iterator>

#include "../ecci.hpp"

#include <boost/throw_exception.hpp>

namespace grass {

class environment;
class interpreter;

struct grass_error : public ecci::ecci_error
{
    explicit
    grass_error(const std::string &x)
      : ecci::ecci_error("grass", x)
    { }
};

namespace _lambda {

struct lambda_error : public grass_error
{
    explicit
    lambda_error(const std::string &x)
      : grass_error(x)
    {}
};

class lambda;

class lambda_ptr
{
private:
    const lambda *ptr;

    void
    nullcheck() const
    {
        if (ptr == nullptr)
        {
            BOOST_THROW_EXCEPTION(lambda_error("null pointer"));
        }
    }

public:
    explicit
    lambda_ptr(const lambda *ptr = nullptr) noexcept
      : ptr(ptr)
    { }

    explicit
    lambda_ptr(const std::unique_ptr<lambda> &ptr) noexcept
      : lambda_ptr(ptr.get())
    { }

    const lambda *
    get() const noexcept { return ptr; }

    explicit operator const lambda *() const noexcept { return get(); }

    const lambda &
    operator*() const { return nullcheck(), *get(); }

    const lambda *
    operator->() const { return nullcheck(), get(); }
};

class lambda_pool : public std::set<const lambda *>
{
    std::vector<lambda_ptr> build_in_func;

public:
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

    lambda_pool()
      : build_in_func(std::size_t(BUILDIN::_SIZE))
    { }

    lambda_ptr
    operator[](BUILDIN buildin) const
    {
        return build_in_func.at(std::size_t(buildin));
    }
};

} // namespace grass::_lambda

class environment
  : public std::stack<_lambda::lambda_ptr, std::vector<_lambda::lambda_ptr>>
{
public:
    environment() = default;

    environment(const environment &env, std::size_t reserve = 0)
      : std::stack<_lambda::lambda_ptr, std::vector<_lambda::lambda_ptr>>()
    {
        c.reserve(env.size() + reserve);
        c.assign(env.c.begin(), env.c.end());
    }

    _lambda::lambda_ptr
    operator[](std::size_t idx) const { return c[size() - idx - 1]; }

    void
    clear() { c.clear(); }
};

namespace _lambda {

class lambda
{
protected:
    environment env;

    lambda_pool *pool;

    lambda_ptr
    operator[](std::size_t idx) { return env[idx + 1]; }

    void
    push(const lambda_ptr &l) { env.push(l); }

    lambda_pool::value_type
    insert(const lambda *l) const { return *pool->insert(l).first; }

public:
    lambda(const lambda &) = default;
    lambda(lambda &&) = default;

    explicit
    lambda(lambda_pool &pool) noexcept
      : pool(&pool)
    { }

    virtual ~lambda() noexcept { }

    [[noreturn]] virtual lambda_ptr
    real_call(std::vector<lambda_ptr> &&) const
    {
        BOOST_THROW_EXCEPTION(lambda_error("invalid real call"));
    }

    virtual lambda_ptr
    operator()(const lambda_ptr &) const = 0;

    [[noreturn]] virtual char
    operator*() const
    {
        BOOST_THROW_EXCEPTION(lambda_error("invalid reference"));
    }
};

class partial_apply final : public lambda
{
    const unsigned int arg_num;
    const lambda_ptr   func, arg;

    lambda_ptr
    real_call(std::vector<lambda_ptr> &&args) const override
    {
        args.push_back(arg);
        return func->real_call(std::move(args));
    }

public:
    partial_apply(lambda_pool &pool, unsigned int num,
                  const lambda_ptr &func, const lambda_ptr &arg) noexcept
      : lambda(pool), arg_num(num), func(func), arg(arg)
    { }

    lambda_ptr
    operator()(const lambda_ptr &l) const override
    {
        if (arg_num == 1)
        {
            return func->real_call({ arg, l });
        }

        using ecci::make_unique_ptr;
        auto pa = make_unique_ptr<partial_apply>(*pool, arg_num - 1, lambda_ptr(this), l);
        pool->insert(pa.get());
        return lambda_ptr(pa.release());
    }
};

// user defined function
class user : public lambda
{
    friend class grass::interpreter;

public:
    typedef std::pair<unsigned int, unsigned int> app_pair_t;
    typedef std::vector<app_pair_t> body_t;

protected:
    typedef std::initializer_list<app_pair_t> init_body_t;

    const unsigned int arg_num;
    const body_t       body;

    user(const user &l)
      : lambda(l), arg_num(l.arg_num - 1), body(l.body)
    { }

    lambda_ptr
    real_call(std::vector<lambda_ptr> &&args) const
    {
        if (arg_num != args.size())
        {
            BOOST_THROW_EXCEPTION(lambda_error("invalid argument number"));
        }

        environment renv(env, body.size() + arg_num);
        for (auto &l : args) { renv.push(l); }

        for (auto &app : body)
        {
            auto func = renv[app.first];
            auto arg  = renv[app.second];
            renv.push((*func)(arg));
        }
        return renv.top();
    }

public:
    user(lambda_pool &pool, unsigned int num, body_t &&il)
      : lambda(pool), arg_num(num), body(std::move(il))
    { }

    user(lambda_pool &pool, unsigned int num,
      init_body_t &&il = init_body_t())
      : lambda(pool), arg_num(num), body(std::move(il))
    { }

    virtual lambda_ptr
    operator()(const lambda_ptr &l) const override
    {
        if (arg_num == 1)
        {
            return real_call({ l });
        }

        using ecci::make_unique_ptr;
        auto pa = make_unique_ptr<partial_apply>(*pool, arg_num - 1, lambda_ptr(this), l);
        pool->insert(pa.get());
        return lambda_ptr(pa.release());
    }
};

struct id final : public user
{
    explicit
    id(lambda_pool &pool)
      : user(pool, 1)
    { }
};

class boolalpha final : public user
{
    static init_body_t &&
    gen_body(bool b, init_body_t &&true_, init_body_t &&false_) noexcept
    {
        return std::move(b ? true_ : false_);
    }

public:
    explicit
    boolalpha(lambda_pool &pool, bool b)
      : user(pool, 2, gen_body(b, {{ 3, 2 }}, {}))
    {
        if (!b) { return; }
        env.push(pool[lambda_pool::BUILDIN::ID]);
    }
};

namespace primitive {

struct w final : public lambda
{
    explicit
    w(lambda_pool &pool, char x = 'w')
      : lambda(pool), c(x)
    { }

    lambda_ptr
    operator()(const lambda_ptr &l) const override
    {
        using BUILDIN = lambda_pool::BUILDIN;
        return (*pool)[(**l == c) ? BUILDIN::TRUE : BUILDIN::FALSE];
    }

    char
    operator*() const noexcept override { return c; }

private:
    const char c;
};

struct succ final : public lambda
{
    explicit
    succ(lambda_pool &pool) noexcept
      : lambda(pool)
    { }

    lambda_ptr
    operator()( const lambda_ptr &l ) const override
    {
        // FIXME: applicate singleton to character
        return lambda_ptr(insert(new w(*pool, (**l + 1) % 256)));
    }
};

struct in final : public lambda
{
    explicit
    in(lambda_pool &pool, std::istream &in) noexcept
      : lambda(pool), sin(in)
    { }

    lambda_ptr
    operator()(const lambda_ptr &l) const override
    {
        int c = sin.get();
        if (c == EOF) { return l; }

        // FIXME: applicate singleton to character
        return lambda_ptr(insert(new w(*pool, c)));
    }

private:
    std::istream &sin;
};

struct out final : public lambda
{
    explicit
    out(lambda_pool &pool, std::ostream &out, bool f = false) noexcept
      : lambda(pool), sout(out), force(f)
    { }

    lambda_ptr
    operator()(const lambda_ptr &l) const override
    {
        try
        {
            sout.put(**l);
        }
        catch (const lambda_error &)
        {
            if (!force) { throw; }
            sout << "<lambda>";
        }
        return l;
    }

private:
    std::ostream &sout;
    bool force;
};

} // namespace grass::_lambda::primitive

} // namespace grass::_lambda

struct interpreter : public ecci::ecci_base
{
    interpreter &
    operator=(const interpreter &) = delete;
    interpreter &
    operator=(interpreter &&) = delete;

private:
    _lambda::lambda_ptr
    inserter(_lambda::lambda *pl)
    try
    {
        if (auto upl = dynamic_cast<_lambda::user *>(pl))
        {
            upl->env = env;
        }

        _lambda::lambda_ptr lptr(pl);
        env.push(lptr);
        pool.insert(pl);
        return lptr;
    }
    catch (...)
    {
        delete pl;
        throw;
    }

    void
    init(bool force_out)
    {
        release();

        using BUILDIN = _lambda::lambda_pool::BUILDIN;
        namespace prim = _lambda::primitive;
        pool[BUILDIN::IN]   = inserter(new prim::in(pool, in()));
        pool[BUILDIN::W]    = inserter(new prim::w(pool));
        pool[BUILDIN::SUCC] = inserter(new prim::succ(pool));
        pool[BUILDIN::OUT]  = inserter(new prim::out(pool, out(), force_out));
    }

    void
    release() noexcept
    {
        env.clear();
        for (const auto *ptr : pool)
        {
            delete ptr;
        }
        pool.clear();
    }

    void parser_impl();

public:
    explicit
    interpreter(bool force_out = false)
    {
        init(force_out);
    }

    explicit
    interpreter(std::istream &in, std::ostream &out, bool force_out = false)
      : ecci::ecci_base(in, out)
    {
        init(force_out);
    }

    explicit
    interpreter(std::iostream &inout, bool force_out = false)
      : interpreter(inout, inout, force_out)
    { }

    ~interpreter() noexcept override { release(); }

    ecci::ecci_base &
    parse(const std::string &code) override
    {

        std::copy_if(code.begin(), code.end(), std::back_inserter(buf), [](char c)
        {
            return c == 'w' || c == 'W' || c == 'v';
        });
        parser_impl();
        return *this;
    }

    ecci::ecci_base &
    run() override
    {
        if (buf.size() != 0)
        {
            buf += 'v';
            parser_impl();
        }
        env.push((*env.top())(env.top()));
        return *this;
    }

private:
    environment env;
    _lambda::lambda_pool pool;

    std::string buf;
};

namespace {

template <typename ForwardIterator>
inline std::pair<std::size_t, ForwardIterator>
continuos_region(ForwardIterator itr, ForwardIterator last)
{
    std::size_t cnt = 0;
    for (auto &val = *itr; itr != last && val == *itr; ++itr, ++cnt) ;
    return std::make_pair(cnt, itr);
}

} // namespace grass::<anonymous-namespace>

void
interpreter::parser_impl()
{
    auto itr          = buf.begin();
    auto cend         = buf.end();
    auto continue_itr = itr;

    enum
    {
      GR_TOPLEVEL,
      GR_APPLICATION,
      GR_FUNCTION
    } state = GR_TOPLEVEL;

    std::size_t args = 0, func = 0;
    _lambda::user::body_t body;

    // TODO: refactor this block
    while (itr != cend)
    {
        auto region = continuos_region(itr, cend);
        char c = *itr;
        if (c == 'v')
        {
            switch (state)
            {
              case GR_APPLICATION:
                BOOST_THROW_EXCEPTION(
                    grass_error("internal error (unexpected application terminate)"));

              case GR_FUNCTION:
                inserter(new _lambda::user(pool, args, std::move(body)));
                state = GR_TOPLEVEL;
              // PATH THROUGH

              case GR_TOPLEVEL:
                continue_itr = region.second;
                break;
            }
        }
        else
        {
            switch (state)
            {
              case GR_TOPLEVEL:
                switch (c)
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
                if (c != 'w')
                {
                    BOOST_THROW_EXCEPTION(
                        grass_error("internal error (unexpected char in application)"));
                }
                env.push((*env[func])(env[region.first]));
                state = GR_TOPLEVEL;
                continue_itr = region.second;
                break;

              case GR_FUNCTION:
                if (c != 'W')
                {
                    BOOST_THROW_EXCEPTION(
                        grass_error("internal error (unexpected char in define function)"));
                }
                func = region.first;

                region = continuos_region(itr = region.second, cend);
                if (region.second == cend) { break; }

                if (*itr != 'w')
                {
                    BOOST_THROW_EXCEPTION(
                        grass_error("internal error (unexpected char in function args)"));
                }
                body.push_back(decltype(body)::value_type(func - 1, region.first - 1));
                break;
            }
        }
        itr = region.second;
    }

    buf.assign(continue_itr, cend);
}

} // namespace grass

#endif // esolang_gri_hpp_

