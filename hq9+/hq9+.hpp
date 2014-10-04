// HQ9+ Interpreter - hq9+.hpp
//                  Copyright(c) 2010 - 2014 Flast All rights reserved.

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

#ifndef esolang_HQ9p_hpp_
#define esolang_HQ9p_hpp_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <boost/throw_exception.hpp>

#include "../ecci.hpp"

namespace hq9p {

struct hq9p_error : public ecci::ecci_error
{
    explicit
    hq9p_error(const std::string &x)
      : ecci::ecci_base("HQ9+", x)
    {}
};

namespace insn {

struct base
{
    virtual ~base() noexcept { }

    virtual void
    operator()(ecci::ecci_base &) const = 0;
};

struct H final : public base
{
    void
    operator()(ecci::ecci_base &i) const override
    {
        i.out() << "Hello, world!";
    }
};

struct Q final : public base
{
    void
    operator()(ecci::ecci_base &) const override
    {
        BOOST_THROW_EXCEPTION(hq9p_error("todo: implement hq9p::insn::Q"));
    }
};

struct nine final : public base
{
    void
    operator()(ecci::ecci_base &) const override
    {
        BOOST_THROW_EXCEPTION(hq9p_error("todo: implement hq9p::insn::nine"));
    }
};

template <typename Acc>
struct plus final : public base
{
    constexpr
    plus() noexcept : acc() {}

    void
    operator()(ecci::ecci_base &) const override
    {
        BOOST_THROW_EXCEPTION(hq9p_error("todo: implement hq9p::insn::plus"));
    }

private:
    Acc acc;
};

} // namespace insn

struct interpreter : public ecci::ecci_base
{
private:
    insn::H         h;
    insn::Q         q;
    insn::nine      n;
    insn::plus<int> p;

public:
    interpreter()
      : pool {
          {"H", h},
          {"Q", q},
          {"9", n},
          {"+", p},
        }
    { }

    ecci::ecci_base &
    parse(const std::string &code) override
    {
        return *this;
    }

    ecci::ecci_base &
    run() override
    {
        for (auto &c : body)
        {
            c->second(*this);
        }
        return *this;
    }

private:
    template <typename T>
    using ref = std::reference_wrapper<T>;

    std::unordered_map<std::string, ref<insn::base>> pool;
    std::vector<decltype(pool)::iterator> body;
};

} // namespace hq9p

#endif // esolang_HQ9p_hpp_

