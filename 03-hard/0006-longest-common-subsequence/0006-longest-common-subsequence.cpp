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
#include <future>
#include <iterator>
#include <limits>
#include <memory>
#include <type_traits>
#include <vector>

/*
Hirschberg algorithm:
  - http://dl.acm.org/citation.cfm?id=360861
  - http://bioinfo.ict.ac.cn/~dbu/AlgorithmCourses/Lectures/Hirschberg1975.pdf
*/

// C++ 14 autodetects return type from algo_c
// GCC 4.9.2: compile with -std=c++14 (c++1y is deprecated but works)
// Uncomment trailing return types below if C++14 is unavailable

const size_t maxsize_t = std::numeric_limits<size_t>::max();


template<typename iter>
auto
algo_b(iter a1, iter a2, iter b1, iter b2, size_t kmax=maxsize_t)
    // -> std::vector<size_t>
{
    std::vector<size_t> k1(std::distance(b1, b2) + 1, 0);

    for(auto a=a1; a != a2; ++a) {
        auto aelem = *a;

        size_t k0j1 = 0;
        size_t k1j1 = 0;
        auto j = 1;
        for(auto b=b1; b != b2; b++, j++) {
            auto k0j = k1[j];
            k1[j] = k1j1 = (aelem == *b) ? k0j1 + 1 : std::max(k1j1, k0j);
            if(k1j1 >= kmax)
                return k1;
            k0j1 = k0j;
        }
    }
    return k1;
}

template<typename iter, typename iterout>
auto
algo_c2(iter a1, iter a2, iter b1, iter b2, iterout v, size_t lmax, bool parallel=false)
    // -> void
{
    auto m = std::distance(a1, a2);
    auto n = std::distance(b1, b2);

    if(not m or not n)
        return;  // one (or both) of the sequences are empty

    // Optimize out common prefixes
    auto abmis = std::mismatch(a1, std::next(a1, std::min(m, n)), b1);
    if(a1 != abmis.first) {
        std::copy(a1, abmis.first, v);
        a1 = abmis.first;
        b1 = abmis.second;
    }

    if(b1 == b2 or a1 == a2)
        return;  // After optimization one (or both) of the sequences are empty

    // Common Suffix optimization
    auto suffix_len = 0;
    auto a2rs = std::reverse_iterator<decltype(a2)>(a2);
    auto b2rs = std::reverse_iterator<decltype(b2)>(b2);

    m = std::distance(a1, a2);  // a1 may have changed above
    n = std::distance(b1, b2);
    auto abrmis = std::mismatch(a2rs, std::next(a2rs, std::min(m, n)), b2rs);
    if(a2rs != abrmis.first) {
        suffix_len = std::distance(a2rs, abrmis.first);
        a2 = std::prev(a2, suffix_len);
        b2 = std::prev(b2, suffix_len);
    }

    if(b1 == b2 or a1 == a2) {
        if(suffix_len)  // suffix was optimized away
            std::copy(a2, std::next(a2, suffix_len), v);
        return;  // After optimization one (or both) of the sequences are empty
    }

    m = std::distance(a1, a2);  // a1 may have changed above
    if(m == 1) {
        // Trivial case. Longest subsequence can be 1 and must be a1 if any
        if(std::find(b1, b2, *a1) != b2)
            *(v++) = *a1;  // ++ is no-op in back_insert, but iterout may be anything

        if(suffix_len)  // suffix was optimized away
            std::copy(a2, std::next(a2, suffix_len), v);

        return;
    }

    // Partition a
    auto ai = std::next(a1, m / 2);

    // Retrieve LCS lengths for forward part of a
    auto l1 = algo_b(a1, ai, b1, b2, lmax);  // will be used as output of l1 + l2

    // Prepare reversal of 2nd part of a and complete b
    auto air = std::reverse_iterator<decltype(ai)>(ai);
    auto a2r = std::reverse_iterator<decltype(a2)>(a2);

    auto b1r = std::reverse_iterator<decltype(b1)>(b1);
    auto b2r = std::reverse_iterator<decltype(b2)>(b2);

    // Retrieve LCS lengths for back part of a
    const auto &l2 = algo_b(a2r, air, b2r, b1r, lmax);

    // find best patition point k for b
    // sum l1 + l2_rev onto l1 (actually l1_r + l2 to get k in 1 step)
    auto l1rbegin = l1.rbegin();
    std::transform(l1rbegin, l1.rend(), l2.begin(), l1rbegin, std::plus<size_t>());

    // lowest pos with max sum - max yields right iter ... reversed sum above
    auto l1begin = l1.begin();
    auto k = std::distance(l1begin, std::max_element(l1begin, l1.end()));
    // Partition b
    auto bk = std::next(b1, k);

    // Limit the maximum sum to the already known max
    lmax = l1[k];


    // Solve the smaller problems
#ifdef _GLIBCXX_HAS_GTHREADS
    if(parallel) {
#if 0
        std::vector<typename std::decay<decltype(*a1)>::type> vasync1;
        auto v1 = std::back_inserter(vasync1);
        auto fut1 = std::async(std::launch::async, algo_c2<iter, iterout>,
                               a1, ai, b1, bk, v1, lmax, parallel);
#else
        // Partitions: a1 -> ai & b1 -> bk
        algo_c2(a1, ai, b1, bk, v, lmax, parallel);
#endif
        std::vector<typename std::decay<decltype(*a1)>::type> vasync2;
        auto v2 = std::back_inserter(vasync2);
        // Partitions: a1 -> a1 & b1 -> bk
        auto fut2 = std::async(std::launch::async, algo_c2<iter, iterout>,
                              ai, a2, bk, b2, v2, lmax, parallel);
#if 0
        fut1.get();
        std::copy(vasync1.begin(), vasync1.end(), v);
#endif
        fut2.get();
        std::copy(vasync2.begin(), vasync2.end(), v);
    } else
#else
    {
        // Partitions: a1 -> ai & b1 -> bk
        algo_c2(a1, ai, b1, bk, v, lmax, parallel);
        // Partitions: a1 -> a1 & b1 -> bk
        algo_c2(ai, a2, bk, b2, v, lmax, parallel);
    }
#endif

    // A suffix may have been saved ... add it
    if(suffix_len)
        std::copy(a2, std::next(a2, suffix_len), v);

    return;
}


// Overload to avoid having to pass a vector
template<typename iter>
auto
algo_c(iter a1, iter a2, iter b1, iter b2, bool parallel=true)
//    -> std::vector<typename std::decay<decltype(*a1)>::type>
{
    using itertype = typename std::decay<decltype(*a1)>::type;

    std::vector<itertype> v;
    algo_c2(a1, a2, b1, b2, std::back_inserter(v), maxsize_t, parallel);
    return v;
}


template <typename T>
auto
compute_lcs_vec(const T &a, const T &b, bool parallel=false)
//    -> std::vector<typename std::decay<decltype(*a.begin())>::type>
{
    return algo_c(a.begin(), a.end(), b.begin(), b.end(), parallel);
}


template <typename T>
auto
compute_lcs(const T &a, const T &b, bool parallel=false) // -> T
{
    auto v = compute_lcs_vec(a, b, parallel);
    return T(v.begin(), v.end());
}


int main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;

    while(std::getline(stream, line)) {
        auto lbegin = line.begin();
        auto lend = line.end();
        auto sep = std::find(lbegin, lend, ';');

        const auto &v = algo_c(lbegin, sep, std::next(sep), lend, false);
        for(auto c: v)
            std::cout << c;
        std::cout << std::endl;
    }
    return 0;
}
