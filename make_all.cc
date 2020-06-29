// Copyright Tetsuro Tanaka, 2020
#include "./position.h"
#include <unordered_set>
#include <ios>
#include <fstream>
typedef std::vector<int> vI;
typedef std::vector<vI> vvI;

/*
  [3, 4] is invalid
  [3, 6] is invalid
 */
bool isValid(vI const& v) {
  // has king
  int k_count = 0;
  int b1_count = 0;
  int b2_count = 0;
  vI count(8, 0);
  for (auto sq : v) {
    k_count += (sq & 1);
    b1_count += ((sq >> 1) & 1);
    b2_count += ((sq >> 2) & 1);
    count[sq]++;
  }
  if (k_count == 0) return false;
  if (count[1] > 1 || count[2] > 1 || count[4] > 1) return false;
  if (k_count == 1 && count[1] != 1) return false;
  if (v.size() == 3) {
    if (b1_count == 0) return false;
    if (b2_count == 0) return false;
    if (b1_count == 1 && count[2] != 1) return false;
    if (b2_count == 1 && count[4] != 1) return false;
  } else if (v.size() == 2) {
    if (count[2] == 1 && count[1] != 1) return false;
    if (count[4] == 1 && count[1] != 1) return false;
  }
  if (count[1] == 1 && k_count != 1) return false;
  if (count[2] == 1 && b1_count != 1) return false;
  if (count[4] == 1 && b2_count != 1) return false;
  return true;
}

vvI make_team(int n) {
  if (n == 1) return vvI(1, vI(1, 1));
  else if (n == 2) {
    vvI r;
    for (int i = 1; i < 8; i++) {
      for (int j = 1; j < 8; j++) {
        vI v(2);
        v[0] = j;
        v[1] = i;
        if (isValid(v)) r.push_back(v);
      }
    }
    return r;
  }
  vvI r;
  for (int i = 1; i < 8; i++) {
    for (int j = 1; j < 8; j++) {
      for (int k = 1; k < 8; k++) {
        vI v(3);
        v[0] = k;
        v[1] = j;
        v[2] = i;
        if (isValid(v)) r.push_back(v);
      }
    }
  }
  return r;
}

vvI make_permutations(int n) {
  vvI r;
  for (int i0 = 0; i0 < 15; i0++) {
    for (int i1 = 0; i1 < 15; i1++) {
      if (i0 == i1) continue;
      if (n == 2) {
        r.push_back(vI{i0, i1});
        continue;
      }
      for (int i2 = 0; i2 < 15; i2++) {
        if (i2 == i0 || i2 == i1) continue;
        if (n == 3) {
          r.push_back(vI{i0, i1, i2});
          continue;
        }
        for (int i3 = 0; i3 < 15; i3++) {
          if (i3 == i0 || i3 == i1 || i3 == i2) continue;
          if (n == 4) {
            r.push_back(vI{i0, i1, i2, i3});
            continue;
          }
          for (int i4 = 0; i4 < 15; i4++) {
            if (i4 == i0 || i4 == i1 || i4 == i2 || i4 == i3) continue;
            if (n == 5) {
              r.push_back(vI{i0, i1, i2, i3, i4});
              continue;
            }
            for (int i5 = 0; i5 < 15; i5++) {
              if (i5 == i0 || i5 == i1 || i5 == i2 || i5 == i3 || i5 == i4) continue;
              r.push_back(vI{i0, i1, i2, i3, i4, i5});
            }
          }
        }
      }
    }
  }
  return r;
}

int main() {
#if 0
  vvI teams = make_team(3);
  for (auto t : teams) {
    std::cerr << t << std::endl;
  }
#endif
  Position testpos(
"+6+4-3"
"+1-3 ."
" . . ."
" . . ."
" . . ."
		   );
  std::unordered_set<uint64_t> all;
  for (int mynum = 1; mynum < 4; mynum++) {
    vvI my_teams = make_team(mynum);
    for (int opnum = 1; opnum < 4; opnum++) {
      vvI op_teams = make_team(opnum);
      vvI permutations = make_permutations(mynum + opnum);
      std::cerr << "mynum = " << mynum << ",my_teams.size()=" << my_teams.size() << ",opnum=" << opnum << ",op_team.size()= " << op_teams.size() << ",perm.size()=" << permutations.size() << ",all.size()=" << all.size() << std::endl;
      for (size_t k = 0; k < permutations.size(); k++) {
        for (size_t j = 0; j < op_teams.size(); j++) {
#if 0
          if (mynum == 3 && opnum == 3) {
            std::cerr << "j=" << j << std::endl;
          }
#endif
          for (size_t i = 0; i < my_teams.size(); i++) {
            uint64_t v = 0;
            for (int ii = 0; ii < mynum; ii++) {
              v |= uint64_t(my_teams[i][ii] | 0x8) << (permutations[k][ii] * 4);
#if 0
              if (k == 100) {
                std::cerr << "ii=" << ii << std::hex << ",v=" << v << std::dec << std::endl;
                std::cerr << "my_teams[i][ii]=" << my_teams[i][ii] << ",permutations[k][ii] = " << permutations[k][ii];
              }
#endif
            }
            for (int ii = 0; ii < opnum; ii++) {
              v |= uint64_t(op_teams[j][ii]) << (permutations[k][ii + mynum] * 4);
#if 0
              if (k == 100) std::cerr << "ii=" << ii << std::hex << ",v=" << v << std::dec << std::endl;
#endif
            }
            Position p = Position(v).normalize();
#if 0
	    if (p.v == testpos.v) {
	      std::cerr << "my_teams[" << i << "]=" << my_teams[i] << std::endl;
	      std::cerr << "op_teams[" << j << "]=" << op_teams[j] << std::endl;
	      std::cerr << "permutations[" << k << "]=" << permutations[k] << std::endl;
	      std::cerr << std::hex << "v=" << v << std::dec << std::endl;
	    }
#endif
#if 0
            if (k == 100) {
              std::cerr << "p=" << p.to_string(true) << std::endl;
            }
#endif
            if (p.valid_piece_pos()) {
              all.insert(p.v);
            }
          }
        }
      }
    }
  }
  std::vector<uint64_t> all_v(all.begin(), all.end());
  std::sort(all_v.begin(), all_v.end());
  std::ofstream f("all.bin");
  f.write((char *)(&all_v[0]), (char *)(&all_v[all_v.size()]) - (char *)(&all_v[0]));
  f.close();
}
