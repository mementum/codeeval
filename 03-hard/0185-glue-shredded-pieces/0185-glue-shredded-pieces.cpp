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
#include <iterator>
#include <map>
#include <type_traits>
#include <vector>

#include <cstddef>
#include <cstdlib>
#include <cstring>

/*
  This problem can be solved by applying the Eulerian Path approach. The
  problem constraints dictate that there is one and only one solution which
  allows to make several simplifications (there is always a head, no need to
  look for the tail to bind to the head)

  Further ocnstraints to max piece length allow memory savings/speed gains by
  adding custom String classes/custom memory allocators

  Note: Although in glue the vectors would be better expressed in terms of
  "stacks" this (under GCC 4.8.1) increases memory consumption 10x.
*/

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


/*
  Two suballocators to be called from the Mallocator custom STL allocators.

  Meant to be policies.
*/

/*
  Keeps track of the allocated chunks in terms of size and delivers from the
  cache accordingly
*/
struct MemCacheDynamic {
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
        return ::malloc(n);
    }

    void dealloc(void *p, size_t n) { memcache[n].push_back(p);}
};


/*
  Pops / Pushes to the cache (a vector) allocated/deallocated memory. Meant for
  maps which always request the same size (GCC 4.8.x, GCC 4.9.x)
*/
struct MemCacheOne {
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
        return ::malloc(n);
    }

    void dealloc(void *p, size_t n) { memcache.push_back(p);}
};


/*
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

    // Constructor Destructor
    Mallocator() noexcept : mcache(getcache()) {}
    // ~Mallocator() { for(auto &&pv: cache) ::free(pv); }

    // A converting copy constructor:
    T *allocate(const size_t n) {
        // debugout(typeid(T).name(), ".allocate: ", n);
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


/*
  Specific std::string substitute ad-hoc for the problem
    Methods provided: substr, size

  Purpose:
    - The constraints of the problem dictate that the strings will have a
      maximum length k (28 chars) and therefore the k1mers a max k - 1 length
     (27).

     The standard allocation scheme grabs a huge chunk and does not reuse the
     strings which have been freed with the size of the given tests.

     This class uses the maximum given length TSize for each and every string
     (even if the string has the minimum length of the problem which is 6) but
     reuses the memory for each test, which ensures a maximum amount of memory
     no larger than the larger of the tests.

     With the default allocator the memory consumption is 4x and used time is
     2x (see also the contribution by the std::map with custom allocators)

  Note: For std::map a custom allocator is a much better solutions. Unluckily
  std::string is already a specialized version of basic_string and a custom
  allocator cannot be provided.
*/
template<size_t TSize=0, size_t VSize=0>
struct PoolString
{
    struct MemPool
    {
        size_t totalalloc;
        size_t msize;
        std::vector<void *> mpool;

        MemPool(size_t msize=0, size_t vsize=0) : msize(msize), mpool(vsize) {}
        ~MemPool() { for(auto &&mp: mpool) ::free(mp); }

        void *alloc(size_t n=0) {
            if(mpool.empty()) {
                totalalloc++;
                return ::malloc(msize ? msize : n);
            }

            auto &&ret = mpool.back();
            mpool.pop_back();
            return ret;
        }

        void dealloc(void *p) {
            if(p != nullptr)
                mpool.push_back(p);
        }
    };

    static MemPool &getpool() {
        static MemPool mempool(TSize + 1, VSize);
        return mempool;
    }

    char *data;

    PoolString() : data(static_cast<char *>(getpool().alloc())) {
        data[0] = '\0';  // Null terminate the string
    }

    PoolString(const PoolString &ps) : PoolString() { std::strcpy(data, ps.data); }

    PoolString &operator =(const PoolString &ps) {
        std::strcpy(data, ps.data);
        return *this;
    }

    PoolString(PoolString &&ps) : data(nullptr) { std::swap(data, ps.data); }

    PoolString &operator =(PoolString &&ps) {
        std::swap(data, ps.data);
        return *this;
    }

    ~PoolString() { getpool().dealloc(static_cast<void *>(data)); }

    friend void
    swap(const PoolString &ps1, const PoolString &ps2) { std::swap(ps1.data, ps2.data); }

    friend std::istream &
    operator >>(std::istream &is, PoolString &ps) { return is >> ps.data; }

    friend std::ostream &
    operator <<(std::ostream &os, const PoolString &ps) { return os << ps.data; }

    size_t size() const noexcept { return std::strlen(data); }

    PoolString substr(size_t pos, size_t length) const noexcept {
        auto ps = PoolString();
        std::strncpy(ps.data, data + pos, length);
        ps.data[length] = '\0';  // null terminate the copy
        return ps;
    }

    bool operator ==(const PoolString &ps) const noexcept { return not std::strcmp(data, ps.data); }
    bool operator !=(const PoolString &ps) const noexcept { return std::strcmp(data, ps.data); }
    bool operator <(const PoolString &ps) const noexcept { return std::strcmp(data, ps.data) < 0; }

    bool operator !=(const char other[]) const noexcept { return std::strcmp(data, other); }
};


template<typename iter, typename iterout>
auto
glue(iter &&a1, iter &&a2, iterout &&o1, bool eol=false)
{
    using TKmer = typename std::decay<decltype(*a1)>::type;

    using TNodePair = std::pair<TKmer, int>;
    // Using the allocator saves memory/time
    using TNodeBalance = std::map<TKmer, int, std::less<TKmer>, MAllocOne<TNodePair>>;

    using TNBalType = typename TNodeBalance::value_type;

    using TNodeList = std::vector<const TKmer *>;
    // Consumes more memory and is slightly slower
    // using TNodeList = std::vector<const TKmer *, MAllocDyn<const TKmer *>>;

    using TEdgePair = std::pair<const TKmer *, TNodeList>;
    // Using the allocator saves memory/time
    using TEdge = std::map<const TKmer *, TNodeList, std::less<const TKmer *>, MAllocOne<TEdgePair>>;

    using TNodeResult = std::vector<TKmer>;
    // Consumes more memory and is slightly slower
    // using TNodeResult = std::vector<TKmer, MAllocDyn<TKmer>>;

    auto edges = TEdge{};  // Edges (one to many vertex relationship)
    auto nodebals = TNodeBalance{};  // final edge in-out balance

    auto k1len = a1->size() - 1;  // length of string at vertex
    auto kmers = 0;
    for(auto &&a=a1; a != a2 and (not eol or *a != "\n"); a++, kmers++) {
        auto &&kl = a->substr(0, k1len);  // edge's left vertex
        auto &&kr = a->substr(1, k1len);  // edged's right vertex

        nodebals[kl] -= 1;  // leaving from left to right
        nodebals[kr] += 1;  // entering right from left

        // write down the left-right edge
        edges[&(nodebals.find(kl)->first)].push_back(&(nodebals.find(kr)->first));
    }

    // Find the head (more outs than ins)
    auto headit = std::find_if(
        nodebals.begin(), nodebals.end(),
        [&](const TNBalType &nbal) { return nbal.second < 0; });

    auto &&src = &(headit->first);  // Get the pointer to the head
    auto &&head = src;  // make a copy for the latest (if needed) padding

    auto path = TNodeResult{};  // keep track of the path
    auto pstack = TNodeList{};  // temporary storage for visited (left behind) vertex

    // Hierholzer's algorithm to find th epath
    for(auto i=0; ;) {
        if(edges[src].empty()) {  // this vertex has no further outputs
            if(not (i++ % k1len))  // only each k1len (to skip overlapping
                                   // kmerized conten)
                path.push_back(*src);  // write down the fina value and not the ptr

            if(pstack.empty())  // no exit and no node left to visit ..
                break;
            src = pstack.back();  // else, pop the node out and continue
            pstack.pop_back();
        } else  {
            pstack.push_back(src);  // outs left, store current node and
            src = edges[src].back();  // get next vertex to go to i
            edges[pstack.back()].pop_back();  // src has changed - use "back"
        }
    }
    // Some text may be missing from the head (we stored in reverse order) due
    // to the k1len jumps
    auto missing_text = (kmers + k1len) - (path.size() * k1len);
    if(missing_text)
        *o1++ = head->substr(0, missing_text);

    // Copy all stored content to the output iterator
    std::copy(path.crbegin(), path.crend(), o1);
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader("|")));

    using PString = PoolString<28>;

    auto itin2 = std::istream_iterator<PString>();
    auto itout = std::ostream_iterator<PString>(std::cout);

    while (stream) {
        auto itin1 = std::istream_iterator<PString>(stream);
        glue(itin1, itin2, itout, true);
        std::cout << std::endl;
    }

    return 0;
}
