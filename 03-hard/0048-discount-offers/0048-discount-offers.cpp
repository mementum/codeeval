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
#include <iomanip>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#include <cctype>


// This iterator wrapper is thought for an istream_iterator overload of
// std::string (and potentially something else) for non-ws is good.
// If ",;" are the only characters defined as ws and ';' is the terminator but
// ',' is the token separator) the wrapper will peek the next incoming char when
// incrementing ++ to see if the upcoming ws is the "terminator" and set itself
// to "at_end". Else it must be the other ws, which is the token separator and
// parsing can continue

// During init, the incoming char has to be checked to avoid initialization of
// "iter" with the stream which will automatically engage into parsing and a
// token may me unwillfully skipped.

template <typename T>
struct istream_iterator_until {
    std::istream *is;
    bool at_end = false;

    using TIter = std::istream_iterator<T>;
    std::istream_iterator<T> iter;
    std::istream_iterator<T> last;

    typename T::value_type until;

    bool pre_end = false;

    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

    istream_iterator_until() : at_end(true) {}

    istream_iterator_until(std::istream &is, const typename T::value_type &until) :
        is(&is), at_end((not is) or is.peek() == until), iter(), last(), until(until) {

        // avoid init with stream if at_end else the next token will be
        // automaticall consumed (and therfore will be skipped by the logic)
        if(not at_end)
            iter = TIter(is);

        check_end();
    }

    const T &operator *() const { return *iter; }

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

    istream_iterator_until &operator ++() {
        if(pre_end)
            at_end = true;

        if(not at_end)
            ++iter;
        else if(*is)  // at_end and stream is still good skip terminator
            is->get();
        check_end();

        return *this;
    }

    istream_iterator_until operator ++(int) {
        istream_iterator_until tmp = *this;
        if(pre_end)
            at_end = true;

        if(not at_end)
            ++(*this);
        else if(*is)  // at_end and stream is still good skip terminator
            is->get();
        check_end();

        return tmp;
    }

    bool operator !=(const istream_iterator_until &other) const {
        return not (*this == other);
    }

    bool operator ==(const istream_iterator_until &other) const {
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
auto
gcd(T a, T b) -> T
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
    -> ssize_t
{
    // first1, last1 -> product name characters
    // first2, last2 -> customer name characters
    static const char VOWELS[] = "aeiouyAEIOUY";
    auto VBEGIN = std::begin(VOWELS);
    auto VEND = std::end(VOWELS);

    auto isletter = [](const char &c) -> bool { return std::isalpha(c); };
    auto pletters = std::count_if(first1, last1, isletter);
    auto cletters = std::count_if(first2, last2, isletter);
    auto sscore = 0.0;

    if (not (pletters % 2)) {
        sscore = 1.5 * std::count_if(
            first2, last2,
            [VBEGIN, VEND](const char &c)
            { return std::find(VBEGIN, VEND, c) != VEND; });
    } else {
        sscore = std::count_if(
            first2, last2,
            [VBEGIN, VEND](const char &c)
            { return std::isalpha(c) and std::find(VBEGIN, VEND, c) == VEND; });
    }

    if (gcd(pletters, cletters) > 1)
        sscore *= 1.5;

    return static_cast<ssize_t>(100.0 * sscore);
}


auto
auction_round(std::map<int, int> &assigned,
              std::vector<int> &notassigned,
              std::vector<std::vector<int>> &values,
              int epsilon) -> void {

    auto unassigned = notassigned.back();  // get one unassigned
    notassigned.pop_back();

    auto &vecua = values[unassigned];  // products for unassigned

    // Find the best product for this unassigned
    auto bestvalit = std::max_element(vecua.begin(), vecua.end());
    auto bestval = *bestvalit;
    auto bestvalidx = std::distance(vecua.begin(), bestvalit);

    // Find the 2nd best product for the unassigend
    // set bestval to min, find next max and restore value
    auto bestval2 = std::numeric_limits<int>::min();
    for_each(vecua.begin(), vecua.end(),
        [&bestval2, &bestval] (const int &v)
        { if(v < bestval and v > bestval2) bestval2 = v; });

    if(bestval2 == std::numeric_limits<int>::min())
        bestval2 = bestval;

    auto bindincrement = bestval - bestval2 + epsilon;  // price increase

    // Decrease profit (equivalent to price increase but here we can modify values)
    for(auto &&v: values)
        v[bestvalidx] -= bindincrement;

    // if the product was assigned, deassign
    if(assigned.find(bestvalidx) != assigned.end())
        notassigned.push_back(assigned[bestvalidx]);

    // assignenment contains: first -> product index / second -> customer index
    assigned[bestvalidx] = unassigned;  // assign to the explored not assigned
}


// Force passing copy of values -> values will be modified
auto
auction(std::vector<std::vector<int>> values, int epsilon=0)
    -> std::map<int, int>
{
    // assignenment contains: first -> product index / second -> customer index
    auto assigned = std::map<int, int>{};
    auto notassigned = std::vector<int>(values.size());
    std::iota(notassigned.begin(), notassigned.end(), 0);

    if(not epsilon)
        epsilon = 25;

    while(notassigned.size())
        auction_round(assigned, notassigned, values, epsilon);

    return assigned;
}


auto
max_score(std::map<int, int> assigned, std::vector<std::vector<int>> &values)
    -> float
{
    // given an auction assignment returns the total value
    auto maxscore = 0;
    // assignenment contains: first -> product index / second -> customer index
    // values -> 1st index is customer (assignment second)
    // values -> 2nd index is product (assignment first)
    for(auto &&a: assigned)
        maxscore += values[a.second][a.first];

    return static_cast<float>(maxscore) / 100.0;
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

        auto products = std::vector<std::string>{};
        std::copy(isit_until(stream, '\n'), slast, std::back_inserter(products));

        auto pbegin = products.begin();
        auto pend = products.end();
        auto cbegin = customers.begin();
        auto cend = customers.end();

        auto ncusts = std::distance(cbegin, cend);  // rows (customers) size
        auto nprods = std::distance(pbegin, pend);  // cols (products) size

        if(not ncusts or not nprods) {
            std::cout << std::fixed << std::setprecision(2) << 0.0 << std::endl;
            continue;
        }

        // customer (rows) products (cols) values
        auto values = std::vector<std::vector<int>>(ncusts);

        // products are columns, but if custs > prods, we need extra null entries
        auto ncols = std::max(ncusts, nprods);

        auto vit = values.begin();
        for (auto c=cbegin; c != cend; c++, vit++) {
            auto &v = *vit;
            v.resize(ncols);

            // fill customer array wit products values
            auto j = 0;
            for (auto p=pbegin; p != pend; p++, j++)
                v[j] = suit_score(p->begin(), p->end(), c->begin(), c->end());

            // fill customer array phantom products with minimum initial value
            for(auto j2=nprods; j2 < ncusts; j2++)
                v[j] = 0;
        }

        auto assigned = auction(values);
        auto maxscore = max_score(assigned, values);
        std::cout << std::fixed << std::setprecision(2) << maxscore << std::endl;
    }
    return 0;
}
