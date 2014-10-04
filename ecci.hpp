// Esolang compiler collections interpreter - ecci.hpp
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

#ifndef esolang_ecci_hpp_
#define esolang_ecci_hpp_

#include <istream>
#include <ostream>
#include <iostream>
#include <sstream>

#include <string>
#include <stdexcept>

namespace ecci {

class ecci_error : public std::runtime_error
{
    static inline std::string
    cat(const std::string &lang, const std::string &x)
    {
        std::ostringstream ss;
        ss << "ecci faltal (" << lang << "): " << x;
        return ss.str();
    }

public:
    explicit
    ecci_error(const std::string &lang, const std::string &x)
      : std::runtime_error(cat(lang, x))
    {}
};

class ecci_base
{
    std::istream &sin;
    std::ostream &sout;

protected:
    constexpr explicit
    ecci_base(std::istream &in, std::ostream &out) noexcept
      : sin(in), sout(out)
    {}

    constexpr
    ecci_base() noexcept
      : ecci_base(std::cin, std::cout)
    {}

    constexpr explicit
    ecci_base(std::iostream &inout) noexcept
      : ecci_base(inout, inout)
    {}

    virtual ~ecci_base() noexcept {}

public:
    std::istream &
    in() noexcept { return sin; }

    const std::istream &
    in() const noexcept { return sin; }

    std::ostream &
    out() noexcept { return sout; }

    const std::ostream &
    out() const noexcept { return sout; }

    virtual ecci_base &
    parse(const std::string &) = 0;

    virtual ecci_base &
    run() = 0;
};

} // namespace ecci

#endif // esolang_ecci_hpp_

