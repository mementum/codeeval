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

// Headers for the implementation
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>


// Version which removes most of C++ 11 (and even non C++ 11 but STL) things to
// try to decrease compilations times and get over the 10sec compilation
// barrier

// Although it seems that the most effective thing is to complain at the
// CodeEval feedback site and suddenly C++ compiles


const auto NUMCARDS = 5;

const auto SCORE_STRAIGHT_FLUSH = 8000000;
const auto SCORE_FOUR_OF_A_KIND = 7000000;
const auto SCORE_FULL_HOUSE = 6000000;
const auto SCORE_FLUSH = 5000000;
const auto SCORE_STRAIGHT = 4000000;
const auto SCORE_THREE_OF_A_KIND = 3000000;
const auto SCORE_TWO_PAIR = 2000000;
const auto SCORE_PAIR = 1000000;
const auto SCORE_HIGH_CARD = 0;

const static std::string DECK{"23456789TJQKA"};
const static std::string DECK_STRAIGHT{"2345A23456789TJQKA"};

int
hand_eval(const char hand[NUMCARDS][2])
{
    char kinds[5];
    char *kend = kinds + NUMCARDS;

    for(int i=0; i < NUMCARDS; i++)
        kinds[i] = hand[i][0];

    // Sort the kinds according to face value (and not ascii char)
    // keep the asci chars to compare against the DECK strings
    std::sort(kinds, kend,
              [](const char l, const char r) -> bool
              { return DECK.find(l) < DECK.find(r); });

    // Calculate a flush by seeing if all suits are equal
    // alternative is to separate suits to char[5] and check with all_of
    int slen = 1;
    for(int i=1; i < NUMCARDS; i++)
        slen += hand[0][1] == hand[i][1];
    bool flush = slen == NUMCARDS;

    // Find the straight by comparing the sorted kinds to the master DECK_STRAIGHT
    auto straight = DECK_STRAIGHT.find(kinds, 0, NUMCARDS);

    int score = 0;
    if(straight != DECK_STRAIGHT.npos) {
        score = flush ? SCORE_STRAIGHT_FLUSH : SCORE_STRAIGHT;
        score += straight;
    } else {
        // Count distinct card groupins (1s, 2s, 3s and 4s)
        // keep count and associated face value
        std::vector<std::pair<int, int>> rank;
        for(auto i=0; i < NUMCARDS; i++) {
            int c = std::count(kinds, kend, kinds[i]);
            i += c - 1;  // skip repeated face values
            rank.push_back(std::make_pair(c, DECK.find(kinds[i])));
        }

        // Sort to make it a real rank (it's pair so {1, 2} is before {1, 3}
        std::sort(rank.begin(), rank.end());

        // Best rank and best card
        auto rank1 = rank.rbegin();
        int hrank = rank1->first;
        int hcard = rank1->second;

        // Best next rank (don't need the card)
        auto rank2 = std::next(rank1, 1);
        int lrank = rank2->first;

        if(hrank == 4) {
            // Only 1 four of a kind for a given card. 2 4s -> highest card wins
            score = SCORE_FOUR_OF_A_KIND;
            score += hcard;
        } else if(hrank == 3) {
            // Only 1 three of a kind for a card. 2 3s -> highest card wins
            score = (lrank == 2) ? SCORE_FULL_HOUSE : SCORE_THREE_OF_A_KIND;
            score += hcard;
        } else if(hrank == 2) {
            // For either two pair / pair value distinct cards following rank
            // 1 -> 1, 2 -> 2s for two pair - 3 -> 1s, 1 -> 2 for pair
            score = (lrank == 2) ? SCORE_TWO_PAIR : SCORE_PAIR;
            auto i = 0;
            for(auto &&p: rank)
                score += static_cast<int>(std::pow(13, i++)) * p.second;
        } else {
            // For either flush / high card value all cards following rank
            score = flush ? SCORE_FLUSH : SCORE_HIGH_CARD;
            auto i = 0;
            for(auto &&p: rank)
                score += static_cast<int>(std::pow(13, i++)) * p.second;
        }
    }
    return score;
}



///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////

int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::istream_iterator<char> in1(stream);

    const char *results[3] = {"right", "none", "left"};
    char lhand[NUMCARDS][2];
    char rhand[NUMCARDS][2];

    while(stream) {
        // Get left hand by filling (Value/Suit)
        for(int i=0; i < NUMCARDS; i++) {
            lhand[i][0] = *in1++;
            lhand[i][1] = *in1++;
        }

        // Get right hand by filling the cards (Value/Suit)
        for(int i=0; i < NUMCARDS; i++) {
            rhand[i][0] = *in1++;
            rhand[i][1] = *in1++;
        }

        // Evaluate the hands, compare
        int lscore = hand_eval(lhand);
        int rscore = hand_eval(rhand);
        int cmped = (lscore > rscore) - (rscore > lscore);

        // print the string
        std::cout << results[cmped + 1] << std::endl;
    }

    return 0;
}
