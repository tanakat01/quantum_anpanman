#include "./position.h"
#include "./position_set.h"
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

using vC = std::vector<int8_t>;

std::string pieceStr(Position const& p, int turn) {
  std::vector<int> ps;
  for (size_t y = 0; y < 5; y++) {
    for (size_t x = 0; x < 3; x++) {
      int v = p.get(x, y);
      if (v != 0 and ((v >> 3) & 1) == turn) {
	ps.push_back(v & 7);
      }
    }
  }
  std::sort(ps.begin(), ps.end());
  std::string s;
  for (size_t i = 0; i < ps.size(); i++) {
    s += (i == 0 ? "" : ":");
    s += std::to_string(ps[i]);
  }
  return s;
}

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
  std::ifstream f((all_positions ? "count.bin" : "count_init.bin"));
  f.read((char *)(&count[0]), (char *)(&count[count.size()]) - (char *)(&count[0]));
  f.close();
  std::vector<uint64_t> hist(256);
  std::set<std::string> psset;
  for (size_t i = 0; i < ps.size(); i++) {
    Position p = ps[i];
    psset.insert(pieceStr(p, 0));
    psset.insert(pieceStr(p, 1));
    uint8_t v = count[i];
    hist[v]++;
  }
  for (int i = -128; i < 128; i++) {
    if (hist[i & 255] > 0) {
      std::cout << i << ", " << hist[i & 255] << std::endl;
    }
  }
  std::vector<std::string> pslist(psset.begin(), psset.end());
  std::sort(pslist.begin(), pslist.end(), [](std::string const& a, std::string const&b) {if (a.size() != b.size()) return a.size() < b.size(); else return a < b;});
  for (auto s : pslist) {
    std::cout << s << std::endl;
  }
}
  
