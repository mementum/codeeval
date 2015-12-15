/*
  Copyright (C) 2015 Daniel Rodriguez

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Headers for test case
#include <fstream>
#include <iostream>
#include <string>


// Headers for the implementation
#include <algorithm>
#include <array>
#include <bitset>
#include <ios>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <type_traits>
#include <utility>
#include <vector>

// Some C library header
#include <cmath>
#include <cstdint>

// STL Allocator
///////////////////////////////////////////////////////////////////////////////
// Typename Printer (GCC and CLang)
#include <string>
#include <cstdlib>
#include <cxxabi.h>

template<typename T>
std::string type_name()
{
    int status;
    std::string tname = typeid(T).name();
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    if(status == 0) {
        tname = demangled_name;
        std::free(demangled_name);
    }
    return tname;
}


///////////////////////////////////////////////////////////////////////////////
template <class T>
struct Mallocator
{
    typedef T value_type;
    Mallocator() noexcept {} //default ctor not required by STL

    // A converting copy constructor:
    template<class U> Mallocator(const Mallocator<U>&) noexcept {}
    template<class U> bool operator ==(const Mallocator<U> &) const noexcept { return true; }
    template<class U> bool operator !=(const Mallocator<U> &) const noexcept { return false; }

    T* allocate(const size_t n) const {
        void* const pv = ::malloc(n * sizeof(T));
        return static_cast<T*>(pv);
    }

    void deallocate(T* const p, size_t n) const noexcept {
        ::free(p);
    }
};


///////////////////////////////////////////////////////////////////////////////
// Output debugging
///////////////////////////////////////////////////////////////////////////////
template<typename T>
void debugout(const T &t)
{
    std::cout << t << std::endl;
}

template<typename T, typename... Args>
void debugout(const T &t, Args... args) // recursive variadic function
{
    std::cout << t;
    debugout(args...);
}


///////////////////////////////////////////////////////////////////////////////
// Stream Imbuer for Parsing
// http://stackoverflow.com/questions/1894886/parsing-a-comma-delimited-stdstring
//    ss.imbue(std::locale(ss.getloc(), new SeparatorReader(" ,|")));
///////////////////////////////////////////////////////////////////////////////
struct SeparatorReader: std::ctype<char>
{
    SeparatorReader(const std::string &seps):
        std::ctype<char>(get_table(seps), true) {}

    std::ctype_base::mask const *get_table(const std::string &seps) {
        auto rc = new std::ctype_base::mask[std::ctype<char>::table_size]();
        for(auto &sep: seps)
            rc[static_cast<unsigned char>(sep)] = std::ctype_base::space;
        return &rc[0];
    }
};

///////////////////////////////////////////////////////////////////////////////
// String utilities
///////////////////////////////////////////////////////////////////////////////
template<typename T, typename C>
auto
split(const T &str, const C &delim, bool empty=true)
//    -> std::vector<T>
{
    T buf;
    std::vector<T> v;

    for(auto c: str) {
        if(c != delim)
            buf.push_back(c);
        else
            if(c == delim && (empty || !buf.empty())) {
                v.push_back(buf);
                buf.clear();
            }
    }
    if(!buf.empty())
        v.push_back(buf);

    return v;
}


auto
ltrim(const std::string& str)
//    -> std::string
{
    auto first = str.find_first_not_of(' ');
    return str.substr(first);
}

auto
rtrim(const std::string& str)
//    -> std::string
{
    auto last = str.find_last_not_of(' ');
    return str.substr(0, last);
}

auto
trim(const std::string& str)
//    -> std::string
{
    auto first = str.find_first_not_of(' ');
    auto last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;
    while (std::getline(stream, line)) {
        // Do something with the line
    }
    return 0;
}
