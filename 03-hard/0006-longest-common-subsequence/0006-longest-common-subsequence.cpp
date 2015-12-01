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

template<typename iter>
auto
algo_b(iter a1, iter a2, iter b1, iter b2) // -> std::vector<size_t>
{
    std::vector<size_t> k0(std::distance(b1, b2) + 1, 0);
    auto k1(k0);

    for(auto a=a1; a != a2; ++a) {
        auto aelem = *a;
        std::swap(k0, k1);

        auto j = 1;
        for(auto b=b1; b != b2; b++, j++)
            k1[j] = (aelem == *b) ? k0[j - 1] + 1 : std::max(k1[j - 1], k0[j]);
    }
    return k1;
}


template<typename iter, typename iterout>
auto
algo_c(iter a1, iter a2, iter b1, iter b2, iterout v)  // -> void
{
    auto m = std::distance(a1, a2);
    auto n = std::distance(b1, b2);

    if(n == 0)
        return;  // Trivial case. b is empty -> cannot be a subsequence of a

    if(m == 1) {
        // Trivial case. Longest subsequence can be 1 and must be a1 if any
        if(std::find(b1, b2, *a1) != b2)
            *(v++) = *a1;  // increment v must not be forgotten
        return;
    }

    // Optimize out common prefixes
    if(n <= m) {
        auto abmis = std::mismatch(a1, std::prev(a2, m - n), b1);
        std::copy(a1, abmis.first, v);
        a1 = abmis.first;
        b1 = abmis.second;
    } else {
        auto abmis = std::mismatch(b1, std::prev(b2, n - m), a1);
        std::copy(b1, abmis.first, v);
        b1 = abmis.first;
        a1 = abmis.second;
    }

    if( a1 == a2 || b1 == b2)
        return;  // one (or both) fully copied, nothing left

    // Partition a - Recalc distance ... a1 may have been changed
    auto ai = std::next(a1, std::distance(a1, a2) / 2);

    // Retrieve LCS lengths for forward part of a
    auto l1 = algo_b(a1, ai, b1, b2);  // will be used as output of l1 + l2

    // Prepare reversal of 2nd part of a and complete b
    auto air = std::reverse_iterator<decltype(ai)>(ai);
    auto a2r = std::reverse_iterator<decltype(a2)>(a2);

    auto b1r = std::reverse_iterator<decltype(b1)>(b1);
    auto b2r = std::reverse_iterator<decltype(b2)>(b2);

    // Retrieve LCS lengths for back part of a
    const auto &l2 = algo_b(a2r, air, b2r, b1r);

    // find best patition point k for b
    auto l1rbegin = l1.rbegin();
    // sum l1 + l2_rev onto l1 (actually l1_r + l2 to get k in 1 step)
    std::transform(l1rbegin, l1.rend(), l2.begin(), l1rbegin, std::plus<size_t>());

    // lowest pos with max sum - max yields right iter ... reversed sum above
    auto l1begin = l1.begin();
    auto k = std::distance(l1begin, std::max_element(l1begin, l1.end()));
    // Partition b
    auto bk = std::next(b1, k);

    // Solve the smaller problems
    algo_c(a1, ai, b1, bk, v);  // Partitions: a1 -> ai & b1 -> bk
    algo_c(ai, a2, bk, b2, v);  // Partitions: ai -> a2 & bk -> b2

    return;
}


// Overload to avoid having to pass a vector
template<typename iter>
auto
algo_c(iter a1, iter a2, iter b1, iter b2)
//    -> std::vector<typename std::decay<decltype(*a1)>::type>
{
    using itertype = typename std::decay<decltype(*a1)>::type;

    std::vector<itertype> v;
    algo_c(a1, a2, b1, b2, std::back_inserter(v));
    return v;
}


template <typename T>
auto
compute_lcs_vec(const T &a, const T &b)
//    -> std::vector<typename std::decay<decltype(*a.begin())>::type>
{
    return algo_c(a.begin(), a.end(), b.begin(), b.end());
}


template <typename T>
auto
compute_lcs(const T &a, const T &b) // -> T
{
    auto v = compute_lcs_vec(a, b);
    return T(v.begin(), v.end());
}


int main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;

    while(std::getline(stream, line)) {
#if 1
        auto lbegin = line.begin();
        auto lend = line.end();
        auto sep = std::find(lbegin, lend, ';');

        const auto &v = algo_c(lbegin, sep, std::next(sep), lend);
        for(auto c: v)
            std::cout << c;
        std::cout << std::endl;
#else
        auto sep = line.find(';');
        auto lcs = compute_lcs(line.substr(0, sep), line.substr(sep++));
        std::cout << lcs << std::endl;
#endif
    }
    return 0;
}
