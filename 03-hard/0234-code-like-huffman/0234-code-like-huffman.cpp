/*
  Copyright (C) 2016 Daniel Rodriguez

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
#include <array>
#include <iterator>
#include <tuple>
#include <vector>


template <typename T>
struct istream_iterator_until {
    std::istream_iterator<T> iter;
    std::istream_iterator<T> last;
    T until;
    bool at_end;

    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

    istream_iterator_until() : at_end(true) {}

    istream_iterator_until(std::istream &is, const T &until) :
        iter(is), last(), until(until), at_end(not is or *iter == until) {}

    T operator *() const { return *iter; }

    istream_iterator_until& operator ++() {
        ++iter;
        at_end = iter == last or *iter == until;
        return *this;
    }

    istream_iterator_until operator ++(int) {
        istream_iterator_until tmp = *this;
        ++(*this);
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


struct HuffmanEncoder {
    const char ALPHA = 'a';
    const char OMEGA = 'z';
    const int MAXSYMBOLS = OMEGA - ALPHA + 1;  // +1 both are included
    const int TREESIZE = 2 * MAXSYMBOLS - 1;

    static const int PRIO = 0, NAME = 1, LEFT = 2, RIGHT = 3;
    static const int NBITS = LEFT, NCOUNT = RIGHT;

    using TNode = std::tuple<int, int, int, int>;
    using Tree = std::vector<TNode>;
    Tree nodes;
    Tree::iterator nbegin, nmid, nend;

    HuffmanEncoder() :
        nodes(Tree(TREESIZE, std::make_tuple(0, 0, 0, 0))),
        nbegin(nodes.begin()),
        nmid(std::next(nbegin, MAXSYMBOLS)),
        nend(nodes.end()) {}

    template <typename Iter>
    auto
    encode(Iter first, Iter last) -> HuffmanEncoder & {
        // Build the tree by doing the following using a dual queue algorithm
        // using & moving front iterators forward equals popping the element
        // using & moving back iterators forward equals appeending to the queue
        // there are no pops/inserts effectively removing any
        // allocation/reallocation effect, because existing nodes are being modified

        // Reset the leaf nodes
        for(auto i=0; i < MAXSYMBOLS; i++) {
            std::get<PRIO>(nodes[i]) = 0;  // index 0 is weight
            std::get<NAME>(nodes[i]) = i + ALPHA;  // index 1 is character
            std::get<LEFT>(nodes[i]) = 0;  // index 2 is left
            std::get<RIGHT>(nodes[i]) = 0;  // index 3 is right
        }

        // iterators to leaf nodes (codes) and internal nodes queues
        auto codefirst = nbegin;
        auto codelast = nmid;
        auto nodefirst = std::next(codelast);
        auto nodelast = nodefirst;

        for(auto in=first; in != last; in++) // count code frequency
            std::get<PRIO>(nodes[*in - ALPHA]) += 1;

        std::sort(codefirst, codelast);  // Sort the alphabet according to priority
        // Find the 1st alphabet code with a weight (the rest play no role)
        codefirst = std::find_if(
            codefirst, codelast,
            [] (const TNode &n) { return std::get<PRIO>(n) > 0; });


        // Generate the tree-like structure
        auto csize = std::distance(codefirst, codelast);
        auto nsize = std::distance(nodefirst, nodelast);
        while(true) {
            if(csize + nsize < 2)
                break;  // tree completed

            int lr[2];
            for(auto i=0; i < 2; i++) {
                if(csize and nsize) {  // both queues have something
                    if(std::get<PRIO>(*nodefirst) <= std::get<PRIO>(*codefirst)) {
                        lr[i] = std::distance(nbegin, nodefirst);
                        nodefirst++;  // move forward -> pop element
                        nsize--;
                    } else {
                        lr[i] = std::distance(nbegin, codefirst);
                        codefirst++;  // move forward -> pop element
                        csize--;
                    }
                } else if(csize) {
                    lr[i] = std::distance(nbegin, codefirst);
                    codefirst++;  // move forward -> pop element
                    csize--;
                } else {  // nsize
                    lr[i] = std::distance(nbegin, nodefirst);
                    nodefirst++;  // move forward -> pop element
                    nsize--;
                }
            }

            auto &nlast = *nodelast;
            std::get<PRIO>(nlast) = std::get<PRIO>(nodes[lr[0]]) + std::get<PRIO>(nodes[lr[1]]);
            std::get<LEFT>(nlast) = lr[0];  // index to left node
            std::get<RIGHT>(nlast) = lr[1];  // index to right node
            nodelast++;  // move away from head - add to back of the queue
            nsize++;
        }

        huffman(std::distance(nbegin, nodefirst));
        std::sort(nbegin, nmid);  // re-sort alphabet
        return *this;
    }

    auto
    huffman(int nidx, int ncount=0, int nbits=0) -> void {
        // Recurse until a leaf node is met
        // Reuse the left/node empty fields in the leafs to store the BITS (in
        // bits and bit count) to avoid pushing to vectors or extending strings
        // The leaf nodes get the priority reset to allow alphabetic sorting of
        // the nodes again (with a plain sort rather than with predicate)
        int left, right;
        std::tie(std::ignore, std::ignore, left, right) = nodes[nidx];  // unpack the nodes

        if(left) {  // subnodes present
            huffman(left, ncount + 1, nbits << 1);
            huffman(right, ncount + 1, nbits << 1 | 0x01);
        } else {  // terminal node
            // Reuse the fields for the bit count
            auto &n = nodes[nidx];
            std::get<PRIO>(n) = 0;  // remove priority to allow alphabetic sorting
            // std::get<1>(n); <- leaf name (code), keep intact for sorting
            std::get<NBITS>(n) = nbits;  // store bits
            std::get<NCOUNT>(n) = ncount;  // store bitcount
        }
    }

    friend
    std::ostream &operator <<(std::ostream &os, const HuffmanEncoder &h) {
        char name; int nbits, ncount;
        auto j = 0;
        for(auto n=h.nbegin; n < h.nmid; n++, j++) {
            std::tie(std::ignore, name, nbits, ncount) = *n;

            if(not ncount)  // not used in alphabet
                continue;

            os << (j ? " " : "") << name << ": ";
            for(auto i=0; i < ncount; i++)
                os << (nbits >> (ncount - i - 1) & 0x01);
            os << ';';
        }
        return os;
    }
};


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////

int
main(int argc, char *argv[]) {

    std::ifstream stream(argv[1]);
    // Ensure '\n' is not ws and can be intercepted during reading
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader("")));
    auto in2 = istream_iterator_until<char>();

    auto hencoder = HuffmanEncoder();
    while(stream) {
        auto in1 = istream_iterator_until<char>(stream, '\n');
        std::cout << hencoder.encode(in1, in2) << std::endl;
    }
    return 0;
}
