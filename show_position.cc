#include "./position.h"
#include "./position_set.h"
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
  std::string s;
  while (s.size() < 30) {
    std::string l;
    std::getline(std::cin, l);
    s += l;
  }
  std::cerr << s << std::endl;
  Position p(s);
  std::cerr << p.to_string(true) << std::endl;
  std::cerr << "valid=" << p.valid_piece_pos() << std::endl;
  int i = ps.find(p.normalize().v);
  std::cerr << i << std::endl;
  show1(ps, count, i, true);
}
  
