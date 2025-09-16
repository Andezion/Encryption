#pragma once

#include <cstdint>
#include <vector>
#include <forward_list>
#include <string>

using namespace std;

inline vector<uint8_t> encode(const string& source) {
    forward_list<uint8_t> symbolSet;
    for (int i = 0; i < 256; i++) {
        symbolSet.push_front(static_cast<uint8_t>(i));
    }

    vector<uint8_t> encoding;
    for (uint8_t c : source) {
        int index = 0;
        auto prev = symbolSet.before_begin();
        for (auto it = symbolSet.begin(); it != symbolSet.end(); ++it, ++index) {
            if (*it == c) {
                encoding.push_back(index);
                symbolSet.erase_after(prev);
                symbolSet.push_front(c);
                break;
            }
            prev = it;
        }
    }
    return encoding;
}

inline string decode(const vector<uint8_t>& encoding) {
    forward_list<uint8_t> symbolSet;
    for (int i = 0; i < 256; i++) {
        symbolSet.push_front(static_cast<uint8_t>(i));
    }

    string source;
    source.reserve(encoding.size());

    for (const uint8_t idx : encoding) {
        auto prev = symbolSet.before_begin();
        auto it = symbolSet.begin();
        for (int i = 0; i < idx; i++) {
            prev = it;
            ++it;
        }
        uint8_t symbol = *it;
        source.push_back(symbol);
        symbolSet.erase_after(prev);
        symbolSet.push_front(symbol);
    }
    return source;
}
