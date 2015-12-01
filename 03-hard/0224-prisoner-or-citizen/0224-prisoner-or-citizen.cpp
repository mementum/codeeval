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

/*
  This is a "Point in a Polygon" problem with boundaries counting as part of
  the polygon

  References
    - http://www.ics.uci.edu/~eppstein/161/960307.html
    - http://geomalgorithms.com/a03-_inclusion.html
    - http://geomalgorithms.com/a01-_area.html

  To be investigated for improvement:
    - http://www.inf.usi.ch/hormann/papers/Hormann.2001.TPI.pdf
    - https://www.engr.colostate.edu/~dga/dga/papers/point_in_polygon.pdf

  Although Raycasting is also a potential solution, for a problem with the
  given constraints the Winding Number Algorithm should be better because it
  provides accuracy by using only integer operations.
*/

// Headers for test case
#include <fstream>
#include <iostream>
#include <string>

// Headers for the implementation
#include <algorithm>
#include <iterator>
#include <vector>
#include <forward_list>


class Point_t {
public:
    size_t x;
    size_t y;

    Point_t(const size_t &x, const size_t &y) : x{x}, y{y} {}
};

using Vertex_t = Point_t;


template <typename Seq>
auto
is_in_poly(const Seq &vs, const Point_t &p)
//    -> bool
{
    /*
      Input:
        - vs: A sequence of vertex containing the definition of the polygon
        - p: The point to be tested
      Output:
        - bool: true -> It's inside polygon, false -> not in the polygon

      Because being on the "edges" of the polygon is considered being in the
      polygon, the algorithm exits immediately if such condition is detected at
      any stage
    */

    ssize_t wn = 0;    // the  winding number counter

    // loop through all edges of the polygon
    for (auto vs0=vs.begin(), vs1=std::next(vs0); vs1 != vs.end(); vs0++, vs1++) {
        auto v0 = *vs0;
        auto v1 = *vs1;

        auto OnLineX = [&v0, &v1, &p] () -> bool {
            // p is within the x coordinates of v0/v1
            if(v0.x <= v1.x)
                return v0.x <= p.x && p.x <= v1.x;
            return v0.x >= p.x && p.x >= v1.x;
        };

        auto LeftOrRight = [&v0, &v1, &p] () -> ssize_t {
            // Must set return type or else "unsigned" will be auto chosen
            // Left/Right -  < 0 Right, 0 = On Line, > 0 Left
            return ((v1.x - v0.x) * (p.y - v0.y)) - ((v1.y - v0.y) * (p.x - v0.x));
        };

        if(v1.y >= p.y && p.y > v0.y) {   // upwards crossing
            auto lor = LeftOrRight();
            if(lor > 0)
                wn++;
            else if(lor == 0)
                return true;
        } else if(v0.y >= p.y && p.y > v1.y) {   // downwards crossing
            auto lor = LeftOrRight();
            if(lor < 0)  // Right of line
                wn--;
            else if(lor == 0)
                return true;
        } else if(v0.y == p.y && OnLineX())
            return true;
    }
    return wn != 0;
}


template<typename T, typename C>
auto
tokenize(const T &str, const C &delim, bool empty=true)
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


int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;

    while(std::getline(stream, line)) {
        auto coords = tokenize(line, '|');
        auto vcoords = tokenize(coords[0], ',');

        std::forward_list<Point_t> vertex;
        auto iter = vertex.before_begin();
        for(auto vcoord: vcoords) {
            auto xy = tokenize(vcoord, ' ', false);
            iter = vertex.insert_after(iter, Point_t(std::stoi(xy[0]), std::stoi(xy[1])));
        }

        // Close the polygon by adding the 1st vertex to the back
        vertex.insert_after(iter, vertex.front());

        auto xy = tokenize(coords[1], ' ', false);
        Point_t poc(std::stoi(xy[0]), std::stoi(xy[1]));

        std::cout << (is_in_poly(vertex, poc) ? "Prisoner" : "Citizen") << std::endl;
    }
    return 0;
}
