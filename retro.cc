#include "./position.h"
#include "./position_set.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;

using vC = std::vector<int8_t>;

int main(int ac, char **ag) {
  bool all_positions;
    po::options_description options("all_options");
  options.add_options()
    ("all,a",
     po::value<bool>(&all_positions),
     "all positions")
    ;
  po::variables_map vm;
  try
    {
      po::store(po::parse_command_line(ac, ag, options), vm);
      po::notify(vm);
    }
  catch (std::exception& e)
    {
      std::cerr << "error in parsing options" << std::endl
		<< e.what() << std::endl;
      std::cerr << options << std::endl;
      return 1;
    }

  PositionSet ps((all_positions ? "all.bin" : "all_init.bin"));
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
  std::ofstream f((all_positions ? "count.bin" : "count_init.bin"));
  f.write((char *)(&count[0]), (char *)(&count[count.size()]) - (char *)(&count[0]));
  f.close();

}
