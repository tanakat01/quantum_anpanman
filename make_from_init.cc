// Copyright Tetsuro Tanaka, 2020
#include "./position.h"
#include <unordered_set>
#include <vector>
#include <fstream>

using sP = std::unordered_set<Position>;
using vP = std::vector<Position>;

int main() {
  std::unordered_set<uint64_t> all;
  vP q;
  q.push_back(Position::init_pos());
  all.insert(Position::init_pos().normalize().v);
  for (int i = 0; q.size() > 0; i++) {
    vP newq;
    int w = 0, l = 0, d = 0;
    for (auto p : q) {
      Next n = p.next_positions();
      if (n.first < 0) l++;
      else if (n.first > 0) w++;
      else {
        d++;
        for (auto p1 : n.second) {
          std::vector<int> counts = p1.counts();
          if (counts[2] > 1 || counts[4] > 1 || counts[10] > 1 || counts[12] > 1) {
            std::cerr << "----\n" << p.to_string(true) << "\n->\n" << p1.to_string(true) << std::endl;
          }
          Position p2 = p1.normalize();
          if (all.find(p2.v) == all.end()) {
            all.insert(p2.v);
            newq.push_back(p2);
          }
        }
      }
    }
    std::cerr << "i = " << i << ", all.size() = " << all.size() << std::endl;
    std::cerr << "w = " << w << ", l = " << l << ", d = " << d << std::endl;
    q.swap(newq);
  }
  std::vector<uint64_t> all_v(all.begin(), all.end());
  std::sort(all_v.begin(), all_v.end());
  std::ofstream f("all_init.bin");
  f.write((char *)(&all_v[0]), (char *)(&all_v[all_v.size()]) - (char *)(&all_v[0]));
  f.close();

}

