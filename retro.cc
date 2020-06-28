/*
size=54484899
end of read
step=1, wCount=27244955,lCount=0
step=2, wCount=0,lCount=1608722
step=3, wCount=2967199,lCount=0
step=4, wCount=0,lCount=542642
step=5, wCount=2207327,lCount=0
step=6, wCount=0,lCount=371195
step=7, wCount=2293822,lCount=0
step=8, wCount=0,lCount=364653
step=9, wCount=2732362,lCount=0
step=10, wCount=0,lCount=315140
step=11, wCount=1806324,lCount=0
step=12, wCount=0,lCount=183161
step=13, wCount=974893,lCount=0
step=14, wCount=0,lCount=62839
step=15, wCount=301172,lCount=0
step=16, wCount=0,lCount=22406
step=17, wCount=97479,lCount=0
step=18, wCount=0,lCount=7442
step=19, wCount=30977,lCount=0
step=20, wCount=0,lCount=2120
step=21, wCount=9224,lCount=0
step=22, wCount=0,lCount=695
step=23, wCount=1468,lCount=0
step=24, wCount=0,lCount=244
step=25, wCount=205,lCount=0
step=26, wCount=0,lCount=4
step=27, wCount=0,lCount=0
 */
#include "./position.h"
#include "./position_set.h"

using vC = std::vector<int8_t>;

int main(int ac, char **ag) {
  PositionSet ps("all.bin");
  vC count(ps.size(), 0);
  size_t wCount = 0, lCount = 0;
  for (size_t i = 0; i < ps.size(); i++) {
    Position p = ps[i];
    if (p.isLose()) {
      count[i] = -1;
      lCount++;
    } else if (p.isWin()) {
      if (wCount % 100000 == 0) {
        std::cerr << "isWin :" << p.to_string(true) << std::endl;
      }
      count[i] = 1;
      wCount++;
    }
  }
  Position l1(
    " . . ."
    " .-7 ."
    "-7 .-7"
    " .+1 ."
    " . . ."
    );
  size_t k1 = ps.find(l1.normalize());
  assert(k1 >= 0);
  std::cerr << "step=" << 1 << ", wCount=" << wCount << ",lCount=" << lCount << std::endl;
  for (int step = 2;; step++) {
    wCount = lCount = 0;
    vC ncount(count);
    for (size_t i = 0; i < ps.size(); i++) {
      bool debug = (i == k1);
      if (count[i] != 0) continue;
      Position p = ps[i];
      if (debug) {
        std::cerr << p.to_string(true) << std::endl;
      }
      Next ns = p.next_positions();
      assert(ns.first == 0);
      bool isWin = false, hasZero = false;
      for (auto j : ns.second) {
        int64_t k = ps.find(j.normalize());
        if (debug) {
          std::cerr << "j=" << j.to_string(true) << ",k=" << k << std::endl;
        }
        assert(k >= 0);
        if (count[k] < 0)  {
          isWin = true;
          break;
        } else if (count[k] == 0) {
          hasZero = true;
        }
      }
      if (isWin) {
        ncount[i] = step;
        wCount++;
      } else if (!hasZero) {
        ncount[i] = -step;
        lCount++;
      }
    }
    ncount.swap(count);
    std::cerr << "step=" << step << ", wCount=" << wCount << ",lCount=" << lCount << std::endl;
    if (wCount == 0 && lCount == 0) break;
  }
  std::ofstream f("count.bin");
  f.write((char *)(&count[0]), (char *)(&count[count.size()]) - (char *)(&count[0]));
  f.close();

}
