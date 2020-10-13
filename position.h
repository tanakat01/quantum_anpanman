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
#include <cstdint>
#include <bitset>
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
#if defined(__GNUC__) || defined(__clang__)
  return __builtin_popcountll(x);
#else
  int r = 0;
  while (x != 0) {
    x &= (x - 1);
    r++;
  }
  return r;
#endif
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
  explicit Position(std::string s) :v(0) {
    if (s.size() != size_t(W * H * 2)) {
      std::cerr << "format error" << std::endl;
      return;
    }
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 3; x++) {
        int i = xy2i(x, y);
        int sq = 0;
        if (s[i * 2] == '+') {
          sq = s[i * 2 + 1] - '0' + 8;
        } else if (s[i * 2] == '-') {
          sq = s[i * 2 + 1] - '0';
        } else if (s[i * 2] != ' ' || s[i * 2 + 1] != '.') {
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
    uint64_t v1 = ((v & Mx0) >> 8) | (v & Mx1) |((v & Mx2) << 8);
    return Position(v1);
  }
  Position flipV() const {
    uint64_t v1 = ((v & My4) >> 48) | ((v & My3) >> 24) | (v & My2) |
      ((v & My1) << 24) | ((v & My0) << 48);
    return Position(v1);
  }
  Position normalize() const {
    return Position(std::min(flipH().v, v));
  }
  uint64_t pieceMask() const {
    return ((v & Mb0) << 3) | ((v & Mb1) << 2) | ((v & Mb2) << 1);
  }
  uint64_t myPieceMask() const {
    return v & pieceMask();
  }
  uint64_t opPieceMask() const {
    return ~v & pieceMask();
  }
  Position flipTurn() const {
    return Position(v ^ pieceMask()).flipV();
  }
  uint64_t myKings() const {
    return (v >> 3) & v & Mb0;
  }
  uint64_t opKings() const {
    return (~v >> 3) & v & Mb0;
  }
  bool isLose() const {
    return (opKings() & My4) != 0;
  }
  bool isWin() const {
    Next ns = next_positions();
    if (ns.first > 0) return true;
    return false;
  }
  using Next = std::pair<int, vP>;
  bool can_move_on(int x2, int y2) const {
    if (x2 < 0 || x2 >= W || y2 < 0 || y2 >= H) return false;
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
  bool valid_piece_pos() const {  // assert piece valiation is valid
    if ((myKings() & My0) != 0) return false;
    if ((opKings() & My4) != 0) return false;
    return true;
  }
  void eliminate_my_kings() {
    v ^= myKings();
  }
  bool determine_op_kings() {
    uint64_t op_kings = opKings();
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
  void set_mask(uint64_t mask, int sq) {
    for (size_t i = 0; i < 15; i++) {
      uint64_t mask_i = 0xfull << (i * 4);
      if ((mask & mask_i) != 0) {
        v = (v & ~mask_i) | (uint64_t(sq) << (i * 4));
        return;
      }
    }
  }
  Position move(int x1, int y1, int x2, int y2, int orig_sq) const;
  bool check_and_move(vP* r, vP* winr, int x1, int y1,
                      int x2, int y2, int sq) const {
    if (can_move_on(x2, y2)) {
      Position p = move(x1, y1, x2, y2, sq);
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
Position Position::move(int x1, int y1, int x2, int y2, int orig_sq) const {
    Position n(*this);
    int sq = orig_sq;
    if (x1 != x2) {
      if (y1 == y2) {
        sq &= ~2;
      } else {
        sq &= ~4;
      }
    }
    int capture_sq = n.get(x2, y2);
    n.set(x2, y2, sq);
    n.set(x1, y1, 0);
    if (sq != orig_sq) {  // normalize my piece
      uint64_t mask = n.myPieceMask();
      uint64_t old_mask = myPieceMask();
      uint64_t old_b0_mask = (v & (old_mask >> 3));
      uint64_t old_b1_mask = (v & (old_mask >> 2));
      uint64_t old_b2_mask = (v & (old_mask >> 1));
      uint64_t b0_mask = (n.v & (mask >> 3));
      uint64_t b1_mask = (n.v & (mask >> 2));
      uint64_t b2_mask = (n.v & (mask >> 1));
        while (b0_mask != old_b0_mask || b1_mask != old_b1_mask || b2_mask != old_b2_mask) {
        uint64_t b0_only_mask = b0_mask & (~b1_mask >> 1) & (~b2_mask >> 2);
        uint64_t b1_only_mask = (~b0_mask << 1) & b1_mask & (~b2_mask >> 1);
        uint64_t b2_only_mask = (~b0_mask << 2) & (~b1_mask << 1) & b2_mask;
        if (b0_only_mask == 0 && popcount(b0_mask) == 1) {
          n.set_mask(b0_mask, 0b1001);
        } else if (b0_only_mask != 0 && b0_only_mask != b0_mask) {
          n.v ^= b0_mask ^ b0_only_mask;
        } else if (b1_only_mask == 0 && popcount(b1_mask) == 1) {
          n.set_mask(b1_mask, 0b1010);
        } else if (b1_only_mask != 0 && b1_only_mask != b1_mask) {
          n.v ^= b1_mask ^ b1_only_mask;
        } else if (b2_only_mask == 0 && popcount(b2_mask) == 1) {
          n.set_mask(b2_mask, 0b1100);
        } else if (b2_only_mask != 0 && b2_only_mask != b2_mask) {
          n.v ^= b2_mask ^ b2_only_mask;
        }
        old_b0_mask = b0_mask;
        old_b1_mask = b1_mask;
        old_b2_mask = b2_mask;
        b0_mask = (n.v & (mask >> 3));
        b1_mask = (n.v & (mask >> 2));
        b2_mask = (n.v & (mask >> 1));
      }
    }
    if (capture_sq != 0) {  // normalize opp piece
      uint64_t mask = n.opPieceMask();
      if (capture_sq == 3) {
        n.v &= ~(mask >> 2);
      } else if (capture_sq == 5) {
        n.v &= ~(mask >> 1);
      }
      uint64_t old_mask = opPieceMask();
      uint64_t old_b0_mask = (v & (old_mask >> 3));
      uint64_t old_b1_mask = (v & (old_mask >> 2));
      uint64_t old_b2_mask = (v & (old_mask >> 1));
      uint64_t b0_mask = (n.v & (mask >> 3));
      uint64_t b1_mask = (n.v & (mask >> 2));
      uint64_t b2_mask = (n.v & (mask >> 1));
      while (b0_mask != old_b0_mask || b1_mask != old_b1_mask || b2_mask != old_b2_mask) {
	//std::cerr << "n=" << n << std::endl;
        uint64_t b0_only_mask = b0_mask & (~b1_mask >> 1) & (~b2_mask >> 2);
        uint64_t b1_only_mask = (~b0_mask << 1) & b1_mask & (~b2_mask >> 1);
        uint64_t b2_only_mask = (~b0_mask << 2) & (~b1_mask << 1) & b2_mask;
#if 0
	std::cerr << "   mask=" << std::bitset<60>(mask) << std::endl;
	std::cerr << "b0_mask=" << std::bitset<60>(b0_mask) << std::endl;
	std::cerr << "b1_mask=" << std::bitset<60>(b1_mask) << std::endl;
	std::cerr << "b2_mask=" << std::bitset<60>(b2_mask) << std::endl;
	std::cerr << "o0_mask=" << std::bitset<60>(b0_only_mask) << std::endl;
	std::cerr << "o1_mask=" << std::bitset<60>(b1_only_mask) << std::endl;
	std::cerr << "o2_mask=" << std::bitset<60>(b2_only_mask) << std::endl;
#endif
        if (b0_only_mask == 0 && popcount(b0_mask) == 1) {
	  //	  std::cerr << "1" << std::endl;
          n.set_mask(b0_mask, 0b0001);
        } else if (b0_only_mask != 0 && b0_only_mask != b0_mask) {
	  //	  std::cerr << "2" << std::endl;
          n.v ^= b0_mask ^ b0_only_mask;
        } else if (b1_only_mask == 0 && popcount(b1_mask) == 1 && popcount(mask) == 3) {
	  //	  std::cerr << "3" << std::endl;
          n.set_mask(b1_mask, 0b0010);
        } else if (b1_only_mask != 0 && b1_only_mask != b1_mask) {
	  //std::cerr << "4" << std::endl;
          n.v ^= b1_mask ^ b1_only_mask;
        } else if (b2_only_mask == 0 && popcount(b2_mask) == 1 && popcount(mask) == 3) {
	  //	  std::cerr << "5" << std::endl;
          n.set_mask(b2_mask, 0b0100);
        } else if (b2_only_mask != 0 && b2_only_mask != b2_mask) {
	  //	  std::cerr << "6" << std::endl;
          n.v ^= b2_mask ^ b2_only_mask;
        }
        old_b0_mask = b0_mask;
        old_b1_mask = b1_mask;
        old_b2_mask = b2_mask;
        b0_mask = (n.v & (mask >> 3));
        b1_mask = (n.v & (mask >> 2));
        b2_mask = (n.v & (mask >> 1));
        //        std::cerr << "op: n=" << n.to_string(true) << std::endl;
      }
    }
    return n;
  }

#endif
