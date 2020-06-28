// Copyright Tetsuro Tanaka, 2020
#ifndef POSITION_SET_H
#define POSITION_SET_H
#include "position.h"
#include <fstream>

class PositionSet {
private:
  std::vector<uint64_t> all_v;
public:
  PositionSet(const char *s) {
    std::ifstream fs(s, std::ios::binary);
    size_t b_pos = fs.tellg();
    fs.seekg(0, std::fstream::end);
    size_t e_pos = fs.tellg();
    fs.clear();
    fs.seekg(0, std::fstream::beg);
    uint64_t size = (e_pos - b_pos) / sizeof(uint64_t);
    std::cerr << "size=" << size << std::endl;
    all_v.resize(size);
    fs.read((char *)(&all_v[0]), e_pos - b_pos);
    fs.close();
    std::cerr << "end of read" <<  std::endl;
  }
  size_t size() const {
    return all_v.size();
  }
  int64_t find(uint64_t v) const {
    int64_t l = 0, h = size();
    while (h - l > 1) {
      int64_t m = l + (h - l) / 2;
      if (all_v[m] == v) {
        return m;
      } else if (all_v[m] > v) {
        h = m;
      } else {
        l = m + 1;
      }
    }
    if (all_v[l] == v) return l;
    return -1;
  }
  int64_t find(Position const& p) const {
    return find(p.v);
  }
  const Position operator[](size_t i) const {
    return Position(all_v[i]);
  }
};

#endif // POSITION_SET_H
