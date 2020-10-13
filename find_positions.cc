#include "./position.h"
#include "./position_set.h"
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

using vC = std::vector<int8_t>;

void show1(PositionSet const& ps, vC const& count, size_t i, bool turn=true) {
  int c = count[i];
  Next ns = ps[i].next_positions();
  std::cout << "--------\n";
  if (turn)
    std::cout << ps[i].to_string(true) << std::endl;
  else
    std::cout << ps[i].flipTurn().to_string(true) << std::endl;
  if (ns.first != 0) {std::cerr << "ns=" << ns << std::endl; return;}
  int nextc = (c >= 0 ? -c + 1 : -c - 1);
  std::cout << "c=" << c << ",nextc=" << nextc << std::endl;
  if (c != 0) {
    for (auto p1 : ns.second) {
      int j = ps.find(p1.normalize());
      if (count[j] == nextc) {
	show1(ps, count, j, !turn);
	return;
      }
    }
  }
  for (auto p1 : ns.second) {
    std::cout << "--------\n";
    if (turn)
      std::cout << p1.flipTurn().to_string(true) << std::endl;
    else
      std::cout << p1.to_string(true) << std::endl;
    int j = ps.find(p1.normalize());
    if (j < 0) {
      std::cout << "not found\n";
    } else {
      std::cout << "count[j] = " << int(count[j]) << std::endl;
    }
  }
}

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
  int number_of_moves;
  int print_count;
  bool all_positions;
  po::options_description options("all_options");
  options.add_options()
    ("all,a",
     po::value<bool>(&all_positions),
     "all positions")
    ("number-of-moves,m",
     po::value<int>(&number_of_moves)->default_value(-1),
     "select states whose move counts matches the number")
    ("print-count,c",
     po::value<int>(&print_count)->default_value(-1),
     "how many states must be printed")
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
  int show_count = 0;
  std::cerr << "nubmer_of_moves=" << number_of_moves << std::endl;
  for (size_t i = 0; i < ps.size(); i++) {
    if (abs(count[i]) == number_of_moves && (print_count < 0 ||  ++show_count <= print_count)) {
      show1(ps, count, i);
    }
  }
}
  
