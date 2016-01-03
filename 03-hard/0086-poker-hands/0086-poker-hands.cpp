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
#include <array>
#include <iterator>
// #include <map>
#include <set>
#include <string>
#include <vector>


// Even with simplifications it fails to compile under 10 seconds


const auto NUMCARDS = 5;

using CardT = std::array<char, 2>;
using HandT = std::array<CardT, NUMCARDS>;
#if 0
using KindsT = std::array<char, NUMCARDS>;
using SuitsT = KindsT;
#endif


int
hand_eval(const HandT &hand)
{
    const static auto SCORE_STRAIGHT_FLUSH = 8000000;
    const static auto SCORE_FOUR_OF_A_KIND = 7000000;
    const static auto SCORE_FULL_HOUSE = 6000000;
    const static auto SCORE_FLUSH = 5000000;
    const static auto SCORE_STRAIGHT = 4000000;
    const static auto SCORE_THREE_OF_A_KIND = 3000000;
    const static auto SCORE_TWO_PAIR = 2000000;
    const static auto SCORE_PAIR = 1000000;
    const static auto SCORE_HIGH_CARD = 0;

    const static std::string DECK{"23456789TJQKA"};
    const static std::string DECK_STRAIGHT{"2345A23456789TJQKA"};

    using RankT = std::vector<std::pair<int, int>>;

#if 0
    KindsT kinds;
    SuitsT suits;
#else
    char kinds[5];
    char *kend = kinds + NUMCARDS;
    char suits[5];
#endif

    for(auto i=0; i < NUMCARDS; i++) {
        kinds[i] = hand[i][0];
        suits[i] = hand[i][1];
    }

    // Sort the kinds according to face value (and not ascii char)
    // keep the asci chars to compare against the DECK strings
    std::sort(kinds, kend,
              [](char l, char r) -> bool { return DECK.find(l) < DECK.find(r); });

    // Calculate a flush by seeing if all suits are equal
#if 0
    auto flush = std::all_of(
        suits.begin(), suits.end(),
        [&suits](SuitsT::value_type &s) { return s == suits[0]; });
#else
    int slen = 0;
    for(auto i=1; i < NUMCARDS; i++)
        slen += suits[0] == suits[1];
    bool flush = slen == (NUMCARDS - 1);
#endif

    // Find the straight by comparing the sorted kinds to the master DECK_STRAIGHT
    auto straight = DECK_STRAIGHT.find(kinds, 0, NUMCARDS);

    auto score = 0;
    if(straight != DECK_STRAIGHT.npos) {
        score = flush ? SCORE_STRAIGHT_FLUSH : SCORE_STRAIGHT;
        score += straight;
    } else {
        // Count distinct card groupins (1s, 2s, 3s and 4s)
        // keep count and associated face value
        RankT rank;
        for(auto &&k: std::set<char>(kinds, kend)) {
            auto r = std::make_pair(std::count(kinds, kend, k), DECK.find(k));
            rank.push_back(r);
        }

        // Sort to make it a real rank (it's pair so {1, 2} is before {1, 3}
        std::sort(rank.begin(), rank.end());

        // Best rank and best card
        auto rank1 = rank.rbegin();
        auto hrank = rank1->first;
        auto hcard = rank1->second;

        // Best next rank (don't need the card)
        auto rank2 = std::next(rank1, 1);
        auto lrank = rank2->first;

        if(hrank == 4) {
            // Only 1 four of a kind for a given card. 2 4s -> highest card wins
            score = SCORE_FOUR_OF_A_KIND;
            score += hcard;
        } else if(hrank == 3) {
            // Only 1 three of a kind for a card. 2 3s -> highest card wins
            score = lrank == 2 ? SCORE_FULL_HOUSE : SCORE_THREE_OF_A_KIND;
            score += hcard;
        } else if(hrank == 2) {
            // For either two pair / pair value distinct cards following rank
            // 1 -> 1, 2 -> 2s for two pair - 3 -> 1s, 1 -> 2 for pair
            score = lrank == 2 ? SCORE_TWO_PAIR : SCORE_PAIR;
            auto i = 0;
            for(auto &&p: rank)
                score += std::pow(13, i++) * p.second;
        } else {
            // For either flush / high card value all cards following rank
            score = flush ? SCORE_FLUSH : SCORE_HIGH_CARD;
            auto i = 0;
            for(auto &&p: rank)
                score += std::pow(13, i++) * p.second;
        }
    }
    return score;
}



///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////

int
main(int argc, char *argv[]) {
    // if "const" then [] cannot be used
#if 0
    std::map<int, const char *> results{
        {1, "left"},
        {-1, "right"},
        {0, "none"} };
#endif
    std::ifstream stream(argv[1]);

    HandT lhand, rhand;

    auto in1 = std::istream_iterator<char>(stream);
    auto in2 = std::istream_iterator<char>();
    while(stream) {
        // Get left hand by filling (Value/Suit)
        for(auto card=lhand.begin(); in1 != in2 && card != lhand.end(); card++) {
            (*card)[0] = *in1++;
            (*card)[1] = *in1++;
        }

        // Get right hand by filling the cards (Value/Suit)
        for(auto card=rhand.begin(); in1 != in2 && card != rhand.end(); card++) {
            (*card)[0] = *in1++;
            (*card)[1] = *in1++;
        }

        // Evaluate the hands, compare
        auto lscore = hand_eval(lhand);
        auto rscore = hand_eval(rhand);
        auto cmped = (lscore > rscore) - (rscore > lscore);

#if 0
        // print the string
        std::cout << results[cmped] << std::endl;
#else
        if(cmped == 1) std::cout << "left";
        else if(cmped == -1) std::cout << "right";
        else std::cout << "none";
        std::cout << std::endl;
#endif
    }

    return 0;
}
