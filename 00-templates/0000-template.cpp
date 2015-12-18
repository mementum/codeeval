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
// STL C++11 Allocator
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


/*
  Keeps track of the allocated chunks in terms of size and delivers from the
  cache accordingly
*/
struct MemCacheDynamic {
    size_t total++;
    std::map<size_t, std::vector<void *>> memcache;

    ~MemCacheDynamic() {
        for(auto &&nlist: memcache)
            for(auto &&p: nlist.second)
                ::free(p);
    }

    void *alloc(size_t n) {
        auto &&nlist = memcache[n];
        if(not nlist.empty()) {
            auto p = nlist.back();
            nlist.pop_back();
            return p;
        }
        total++;
        return ::malloc(n);
    }

    void dealloc(void *p, size_t n) { memcache[n].push_back(p);}
};


/*
  Pops / Pushes to the cache (a vector) allocated/deallocated memory. Meant for
  maps which always request the same size (GCC 4.8.x, GCC 4.9.x)
*/
struct MemCacheOne {
    size_t total;
    std::vector<void *> memcache;

    ~MemCacheOne() {
        for(auto &&p: memcache)
            ::free(p);
    }

    void *alloc(size_t n) {
        if(not memcache.empty()) {
            auto p = memcache.back();
            memcache.pop_back();
            return p;
        }
        total++;
        return ::malloc(n);
    }

    void dealloc(void *p, size_t n) { memcache.push_back(p);}
};

/*
  Direct allocation/deallocation - meant for comparison with other custom
  allocators
*/

struct MemCacheDirect {
    size_t total;
    void *alloc(size_t n) { total++; return ::malloc(n); }
    void dealloc(void *p, size_t n) { ::free(p); }
};


/*
  Supporting pre C++11 allocators

  Specific STL custom allocator mostly for Maps with the MemCacheOne
  suballocating struct

  This relies on the fact that maps do a 1 by 1 allocation (at least in the GCC
  implementation 4.8.x and 4.9.x) which allows a very quick push/pop scheme of
  allocated and to be reused, pointers with no size check.

  Applying it to the maps in "glue" there have been memory and speed gains of
  about 10%.

  If the suballocator is MemCacheOne, the size is taken into account and the
  allocator can be applied to anything. It has been tested with the vectors
  defined in "glue", but unfortunately no memory gains have been seen in this
  case.

  Note: Using forward_list increases memory. Using stack increases memory by
  orders of magnitude (GCC 4.8.1)
*/
template<typename T, typename M>
struct Mallocator
{
    using MemCache = M;

    // typedefs
    typedef T value_type;
    // Pre C++11
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    static MemCache &getcache() {
        static MemCache memcache;
        return memcache;
    };

    MemCache &mcache;

    // Constructor
    Mallocator() noexcept : mcache(getcache()) {}

    // A converting copy constructor:
    T *allocate(const size_t n) {
        return static_cast<T *>(mcache.alloc(n * sizeof(T)));
    }

    // Deallocate
    void deallocate(T *p, size_t n) noexcept { mcache.dealloc(p, n); }

    template<class U> bool operator ==(const Mallocator<U, M> &) const noexcept { return true; }
    template<class U> bool operator !=(const Mallocator<U, M> &) const noexcept { return false; }

    // Boilerplat pre C++11
    template<class U> struct rebind { typedef Mallocator<U, M> other; };

    size_type max_size() const { return std::numeric_limits<size_type>::max() / sizeof(T); }

    pointer address(reference r) { return &r; }
    const_pointer address(const_reference r) { return &r; }

    void construct(pointer p, const T &t) { new(p) T(t); }
    void destroy(pointer p) { p->~T(); }
};


template<typename T> using MAllocDyn = Mallocator<T, MemCacheDynamic>;
template<typename T> using MAllocOne = Mallocator<T, MemCacheOne>;
template<typename T> using MAllocDirect = Mallocator<T, MemCacheDirect>;

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
#include <locale>

struct SeparatorReader: std::ctype<char>
{
    template<typename T>
    SeparatorReader(const T &seps): std::ctype<char>(get_table(seps), true) {}

    template<typename T>
    std::ctype_base::mask const *get_table(const T &seps) {
        auto &&rc = new std::ctype_base::mask[std::ctype<char>::table_size]();
        for(auto &&sep: seps)
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
