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
#include <sstream>

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

        auto OnLineX = [&] () -> bool {
            // p is within the x coordinates of v0/v1
            if(v0.x <= v1.x)
                return v0.x <= p.x and p.x <= v1.x;
            return v0.x >= p.x and p.x >= v1.x;
        };

        auto LeftOrRight = [&] () -> ssize_t {
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


struct SeparatorReader: std::ctype<char>
{
    SeparatorReader(const std::string &seps):
        std::ctype<char>(get_table(seps), true) {}

    std::ctype_base::mask const *get_table(const std::string &seps) {
        auto rc = new std::ctype_base::mask[std::ctype<char>::table_size]();
        for(auto &sep: seps)
            rc[sep] = std::ctype_base::space;
        return &rc[0];
    }
};


int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;

    std::stringstream sscoord;
    sscoord.imbue(std::locale(sscoord.getloc(), new SeparatorReader(" ,|\n")));

    while (std::getline(stream, line)) {
        sscoord.str(line); sscoord.clear();  // set str & clear any eol/eof

        // Parse all coordinates (vertex and point)
        std::vector<Point_t> coords;
        for (int x=0, y=0; sscoord >> x and sscoord >> y;)
            coords.push_back(Point_t(x, y));

        // extract point and replace with initial vertex to close polygon
        auto pt = coords.back();
        coords[coords.size() - 1] = coords.front();

        std::cout << (is_in_poly(coords, pt) ? "Prisoner" : "Citizen") << std::endl;
    }
    return 0;
}
