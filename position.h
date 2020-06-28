// Copyright Tetsuro Tanaka, 2020
#ifndef POSITION_H
#define POSITION_H
#include <vector>
#include <utility>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <string>
#include <sstream>
#include <bit>
#include <cstdint>
typedef std::pair<int, int> II;

template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v) {
  os << "[";
  for (size_t i = 0; i < v.size(); i++) {
    os << (i == 0 ? "" : ", ") << v[i];
  }
  return os << "]";
}

int popcount(uint64_t x) {
  int r = 0;
  while (x != 0) {
    x &= (x - 1);
    r++;
  }
  return r;
}


template<typename S, typename T>
std::ostream& operator<<(std::ostream& os, std::pair<S, T> const& v) {
  return os << "<" << v.first << ", " << v.second << ">";
}



/*
  4 bit per square
  bit 0 - possible king
  bit 1 - possible diag
  bit 2 - possible side
  bit 3 - turn : 1 if the next player's piece
  0000 for empty square
 */
struct Position {
  const uint64_t Mx0 = 0xf00'f00'f00'f00'f00ull;
  const uint64_t Mx1 = 0x0f0'0f0'0f0'0f0'0f0ull;
  const uint64_t Mx2 = 0x00f'00f'00f'00f'00full;
  const uint64_t My4 = 0xfff'000'000'000'000ull;
  const uint64_t My3 = 0xfff'000'000'000ull;
  const uint64_t My2 = 0xfff'000'000ull;
  const uint64_t My1 = 0xfff'000ull;
  const uint64_t My0 = 0xfffull;
  const uint64_t Mb0 = 0x111'111'111'111'111ull;
  const uint64_t Mb1 = 0x222'222'222'222'222ull;
  const uint64_t Mb2 = 0x444'444'444'444'444ull;
  const uint64_t Mb3 = 0x888'888'888'888'888ull;
  const int W = 3;
  const int H = 5;
  using vP = std::vector<Position>;
  uint64_t v;
  explicit Position(uint64_t v_) :v(v_) {}
//  explicit Position(Position const& p) :v(p.v) {}
  explicit Position(std::string s) :v(0) {
    if (s.size() != W * H * 2) {
      std::cerr << "format error" << std::endl;
      return;
    }
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 3; x++) {
        int i = xy2i(x, y);
        int sq = 0;
        if (s[i * 2] == '+') sq = s[i * 2 + 1] - '0' + 8;
        else if (s[i * 2] == '-') sq = s[i * 2 + 1] - '0';
        else if (s[i * 2] != ' ' || s[i * 2 + 1] != '.') {
          std::cerr << "format error" << std::endl;
          return;
        }
        v |= (uint64_t(sq) << (i * 4));
      }
    }
  }
  static Position init_pos() {
    return Position(
      "-7-7-7"
      " . . ."
      " . . ."
      " . . ."
      "+7+7+7");
  }
  static int xy2i(int x, int y) {
    return y * 3 + x;
  }
  static II i2xy(int i) {
    return II(i % 3, i / 3);
  }
  int get(int x, int y) const {
    int i = xy2i(x, y);
    return (v >> (i * 4)) & 0xf;
  }
  void set(int x, int y, int sq) {
    int i = xy2i(x, y);
    uint64_t m = 0xfull << (i * 4);
    v = (v & ~m) | (uint64_t(sq) << (i * 4));
  }
  Position flipH() const {
    uint64_t v_ =
      ((v & Mx0) >> 8) | (v & Mx1) |((v & Mx2) << 8);
    return Position(v_);
  }
  Position flipV() const {
    uint64_t v_ =
      ((v & My4) >> 48) | ((v & My3) >> 24) | (v & My2) |
      ((v & My1) << 24) | ((v & My0) << 48);
    return Position(v_);
  }
  Position normalize() const {
    Position fh = flipH();
    return Position(std::min(fh.v, v));
  }
  uint64_t pieceMask() const {
    uint64_t pm =
      ((v & Mb0) << 3) |
      ((v & Mb1) << 2) |
      ((v & Mb2) << 1);
    return pm;
  }
  Position flipTurn() const {
    uint64_t v_ = v ^ pieceMask();
    return Position(v_).flipV();
  }
  bool isLose() const {
    uint64_t myMask = (v & Mb3);
    if ((~(myMask >> 3) & v & Mb0 & My4) != 0) return true;
    return false;
  }
  bool isWin() const {
    Next ns = next_positions();
    if (ns.first > 0) return true;
    return false;
  }
  using Next = std::pair<int, vP>;
  bool can_move_on(int x2, int y2) const {
    if (x2 < 0 || x2 >= W || y2 < 0) return false;
    int sq1 = get(x2, y2);
    if (sq1 >= 8) return false;
    return true;
  }
  std::vector<int> counts() const {
    std::vector<int> r(16);
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 3; x++) {
        int sq = get(x, y);
        r[sq & 0xf]++;
      }
    }
    return r;
  }
  bool valid_piece_pos() const { // assert piece valiation is valid
//    std::cerr << std::hex << v << std::dec << std::endl;
    if (((v >> 3) & (v & Mb0) & My0) != 0) return false;
    if ((((~(v & Mb3)>> 3)) & (v & Mb0) & My4) != 0) return false;
    return true;
  }
  void eliminate_my_kings() {
    uint64_t my_kings = (v >> 3) & (v & Mb0);
    v ^= my_kings;
  }
  bool determine_op_kings() {
    uint64_t op_kings = (~(v >> 3)) & (v & Mb0);
    if (op_kings == 0) return true;  // win
    if ((op_kings & (op_kings - 1)) == 0) {  // one-bit
      for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 3; x++) {
          int sq = get(x, y);
          if (sq < 8 && (sq & 1) != 0) {
            set(x, y, 1);
            return false;
          }
        }
      }
    }
    return false;
  }
  Position move(int x1, int y1, int x2, int y2, int orig_sq) const {
    Position n(*this);
    int sq = orig_sq;
    if (x1 != x2) {
      if (y1 == y2) {
        sq &= ~2;
        uint64_t my_b1 = (v >> 2) & (v & Mb1);
        if (orig_sq != sq && popcount(my_b1) == 2 && popcount(v & Mb3) == 3) {
          for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 3; x++) {
              int sq = get(x, y);
              if ((sq & 0b1010) == 0b1010) n.set(x, y, 0b1010);
            }
          }
        }
      } else {
        sq &= ~4;
        uint64_t my_b2 = (v >> 1) & (v & Mb2);
        if (orig_sq != sq && popcount(my_b2) == 2 && popcount(v & Mb3) == 3) {
          for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 3; x++) {
              int sq = get(x, y);
              if ((sq & 0b1100) == 0b1100) n.set(x, y, 0b1100);
            }
          }
        }
      }
      if (sq == 9 && orig_sq != 9) {
        n.eliminate_my_kings();
      }
    }
    n.set(x2, y2, sq);
    n.set(x1, y1, 0);
    return n;
  }
  bool check_and_move(vP* r, vP* winr, int x1, int y1, int x2, int y2, int sq) const {
    if (can_move_on(x2, y2)) {
      Position p = move(x1, y1, x2, y2, sq);
      // std::cerr << "check_and_move(x2=" << x2 << ",y2=" << y2 << ",sq=" << sq << ",p=" << p.to_string() << std::endl;
      if (p.determine_op_kings() || (y2 == 0 && (sq & 1) != 0)) {
        winr->push_back(p.flipTurn());
        return true;
      }
      r->push_back(p.flipTurn());
    }
    return false;
  }
  Next next_positions() const {
    if (isLose()) return Next(-1, vP());
    vP r, wr;
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 3; x++) {
        int sq = get(x, y);
        if (sq >= 8) {
          if (check_and_move(&r, &wr, x, y, x, y - 1, sq))
            return Next(1, wr);
          if ((sq & 3) != 0) {
            if (check_and_move(&r, &wr, x, y, x - 1, y - 1, sq))
              return Next(1, wr);
            if (check_and_move(&r, &wr, x, y, x + 1, y - 1, sq))
              return Next(1, wr);
          }
          if ((sq & 5) != 0) {
            if (check_and_move(&r, &wr, x, y, x - 1, y, sq))
              return Next(1, wr);
            if (check_and_move(&r, &wr, x, y, x + 1, y, sq))
              return Next(1, wr);
          }
        }
      }
    }
    return Next(0, r);
  }
  std::string to_string(bool isLong = false) const {
    std::ostringstream os;
    if (isLong) os << "\n";
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 3; x++) {
        int sq = get(x, y);
        if (sq == 0) {
          os << " .";
        } else {
          os << (sq > 7 ? '+' : '-') << (sq & 7);
        }
      }
      if (isLong) os << "\n";
    }
    return os.str();
  }
};
bool operator==(Position const& p1, Position const& p2) {
  return p1.v == p2.v;
}
using Next = Position::Next;

std::ostream& operator<<(std::ostream& os, Position const& p) {
  return os << p.to_string();
}
#endif
