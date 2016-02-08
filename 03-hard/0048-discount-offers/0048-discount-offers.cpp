/*
  Copyright (C) 2015, 2016 Daniel Rodriguez

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

// Headers for test case input/output
#include <fstream>
#include <iostream>

// Headers for the implementation
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#include <cctype>


///////////////////////////////////////////////////////////////////////////////
// Output debugging
///////////////////////////////////////////////////////////////////////////////
template<typename T>
void debugout(const T &t)
{
    std::cout << t << std::endl;
}

template<typename T>
void debugout(const std::vector<T> &v)
{
    std::cout << "{";
    if(v.size()) {
        auto last = std::prev(v.end(), 1);
        for(auto &&p=v.begin(); p != last; p++)
            std::cout << *p << ",";

        std::cout << *last;
    }
    std::cout << "}" << std::endl;
}

template<typename T, typename... Args>
void debugout(const T &t, Args... args) // recursive variadic function
{
    std::cout << t;
    debugout(args...);
}


template <typename T>
struct istream_iterator_until {
    std::istream *is;
    using TIter = std::istream_iterator<T>;
    std::istream_iterator<T> iter;
    std::istream_iterator<T> last;

    typename T::value_type until;

    bool at_end = false;
    bool pre_end = false;

    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

    istream_iterator_until() : at_end(true) {}

    istream_iterator_until(std::istream &is, const typename T::value_type &until) :
        is(&is), iter(is), last(), until(until) { check_end(); }

    auto operator *() const { return *iter; }

    auto check_end() {
        if(not at_end) {
            if(iter == last)
                at_end = true;
            else {
                auto t = is->peek();
                if(t == TIter::traits_type::eof() or t == until)
                    pre_end = true;
            }
        }
    }

    auto &operator ++() {
        if(pre_end)
            at_end = true;

        if(not at_end)
            ++iter;

        check_end();
        return *this;
    }

    auto operator ++(int) {
        istream_iterator_until tmp = *this;
        if(pre_end)
            at_end = true;

        if(not at_end)
            ++(*this);

        check_end();
        return tmp;
    }

    auto operator !=(const istream_iterator_until &other) const {
        return not (*this == other);
    }

    auto operator ==(const istream_iterator_until &other) const {
        if(at_end)
            return other.at_end;

        if(other.at_end)
            return false;

        return iter == other.iter;
    }
};


///////////////////////////////////////////////////////////////////////////////
// Stream Imbuer for Parsing
// http://stackoverflow.com/questions/1894886/parsing-a-comma-delimited-stdstring
//    ss.imbue(std::locale(ss.getloc(), new SeparatorReader(" ,|")));
///////////////////////////////////////////////////////////////////////////////

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


template <typename T>
gcd(T a, T b)
{
  T c;
  while (a != 0) {
     c = a;
     a = b % a;
     b = c;
  }
  return b;
}


template<typename iter1, typename iter2>
auto
suit_score(iter1 first1, iter1 last1, iter2 first2, iter2 last2)
{
    // first1, last1 -> product name characters
    // first2, last2 -> customer name characters
    static const char VOWELS[] = "aeiouyAEIOUY";
    auto VBEGIN = std::begin(VOWELS);
    auto VEND = std::end(VOWELS);

    auto isletter = [](const auto &c) -> bool { return std::isalpha(c); };
    auto pletters = std::count_if(first1, last1, isletter);
    auto cletters = std::count_if(first2, last2, isletter);
    auto sscore = 0.0;

    if (not (pletters % 2)) {
        sscore = 1.5 * std::count_if(
            first2, last2,
            [VBEGIN, VEND](const auto &c)
            { return std::find(VBEGIN, VEND, c) != VEND; });
    } else {
        sscore = std::count_if(
            first2, last2,
            [VBEGIN, VEND](const auto &c)
            { return std::isalpha(c) and std::find(VBEGIN, VEND, c) == VEND; });
    }

    if (gcd(pletters, cletters) > 1)
        sscore *= 1.5;

    return static_cast<ssize_t>(100.0 * sscore);
}

#if 0
template <typename It>
auto
auction(It first, It last, const typename It::value_type &epsilon) {
    using ItType = typename std::iterator_traits<It>::value_type;

    auto msize = static_cast<size_t>(std::sqrt(std::distance(first, last)));

    auto a = std::map<It, It>{};  // assigned
    auto na = std::vector<It>(msize);  // not assigned
    for(auto i = 0; i < msize; i++)
        na[i] = std::next(first, i * msize);

    while (not na.empty()) {
        auto nabegin = na.pop_back();
        auto naend = std::next(nabegin, msize);
        auto bestp = std::max_element(nabegin, naend);

        // finding the 2nd ... especially if there is only 1 element requires
        auto pb1 = std::max_element(nabegin, bestp);
        auto pb2 = std::max_element(std::next(bestp), naend);
        auto bestp2 = *bestp;
        if(pb1 != bestp and pb2 != naend) // 2 findings
            bestp2 = std::max(*pb1, *pb2);
        else
            bestp2 = pb1 != bestp ? *pb1 : *pb2






        auction_round(assigned, notassigned, prods, vals, epsilon);

    return assigned;
}
#endif

template <typename It1, typename It2>
auto
do_auction(It1 first1, It2 last1, It2 first2, It2 last2)
{
    auto rows = std::distance(first1, last1);
    auto cols = std::distance(first2, last2);

    auto msize = n;
    if(rows > cols)
        msize = rows;

    auto values = std::vector<ssize_t>(msize * msize);
    for(auto c=first1; c != last1; c++) {
        for(auto p=first2; p != last2; p++) {
    }

}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(",;\n")));

    using isit_until = istream_iterator_until<std::string>;
    auto slast = isit_until();

    while (stream) {
        auto customers = std::vector<std::string>{};
        std::copy(isit_until(stream, ';'), slast, std::back_inserter(customers));
        // debugout("Customers: ", customers);

        auto products = std::vector<std::string>{};
        std::copy(isit_until(stream, '\n'), slast, std::back_inserter(products));
        // debugout("Products: ", products);

        auto pbegin = products.begin();
        auto pend = products.end();
        auto cbegin = customers.begin();
        auto cend = customers.end();

        auto rows = std::distance(cbegin, cend);
        auto cols = std::distance(pbegin, pend);

        auto erows = rows;
        if(cols < rows):

        auto values = std::vector<ssize_t>(std::max(customers.size(), products.size(), 0));
        for (auto i = cbegin; c != cend; c++) {
            auto ridx = std::distance(cbegin, c);
            for (auto p = pbegin; p != pend; p++) {
                auto cidx =
                auto sscore = suit_score(p->begin(), p->end(), c->begin(), c->end());
                values[std::distance(cbegin, c) * x + std::distance(pbegin, p)] = sscore;
            }
            values.push_back(pv);
        }
    }
    return 0;
}
