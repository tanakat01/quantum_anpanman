/*
size=57750184
end of read
step=1, wCount=28795629,lCount=0
step=2, wCount=0,lCount=5410151
step=3, wCount=5877753,lCount=0
step=4, wCount=0,lCount=1640950
step=5, wCount=3101659,lCount=0
step=6, wCount=0,lCount=898325
step=7, wCount=2753033,lCount=0
step=8, wCount=0,lCount=809952
step=9, wCount=2259965,lCount=0
step=10, wCount=0,lCount=746688
step=11, wCount=1550015,lCount=0
step=12, wCount=0,lCount=513003
step=13, wCount=779120,lCount=0
step=14, wCount=0,lCount=280632
step=15, wCount=344818,lCount=0
step=16, wCount=0,lCount=172075
step=17, wCount=160166,lCount=0
step=18, wCount=0,lCount=89368
step=19, wCount=70010,lCount=0
step=20, wCount=0,lCount=43592
step=21, wCount=34735,lCount=0
step=22, wCount=0,lCount=20462
step=23, wCount=15669,lCount=0
step=24, wCount=0,lCount=7398
step=25, wCount=6421,lCount=0
step=26, wCount=0,lCount=3156
step=27, wCount=1991,lCount=0
step=28, wCount=0,lCount=1110
step=29, wCount=820,lCount=0
step=30, wCount=0,lCount=392
step=31, wCount=275,lCount=0
step=32, wCount=0,lCount=106
step=33, wCount=96,lCount=0
step=34, wCount=0,lCount=51
step=35, wCount=20,lCount=0
step=36, wCount=0,lCount=11
step=37, wCount=3,lCount=0
step=38, wCount=0,lCount=0
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
#if 0
      if (wCount % 100000 == 0) {
        std::cerr << "isWin :" << p.to_string(true) << std::endl;
      }
#endif
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
#if 0
  size_t k1 = ps.find(l1.normalize());
  assert(k1 >= 0);
#endif
  std::cerr << "step=" << 1 << ", wCount=" << wCount << ",lCount=" << lCount << std::endl;
  for (int step = 2;; step++) {
    wCount = lCount = 0;
    vC ncount(count);
    for (size_t i = 0; i < ps.size(); i++) {
#if 0
      bool debug = (i == k1);
#else
      bool debug = false;
#endif
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
	if (k < 0) {
	  std::cerr << p.to_string(true) << std::endl;
	  std::cerr << "->\n";
	  std::cerr << j.to_string(true) << std::endl;
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
