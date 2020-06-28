#include "./position.h"
#include "./position_set.h"

using vC = std::vector<int8_t>;

void show1(PositionSet const& ps, vC const& count, size_t i, bool turn=true) {
  int c = count[i];
  Next ns = ps[i].next_positions();
  std::cout << "--------\n";
  if (turn)
    std::cout << ps[i].to_string(true) << std::endl;
  else
    std::cout << ps[i].flipTurn().to_string(true) << std::endl;
  if (ns.first != 0) return;
  int nextc = (c >= 0 ? -c + 1 : -c - 1);
  std::cout << "c=" << c << ",nextc=" << nextc << std::endl;
  for (auto p1 : ns.second) {
    int j = ps.find(p1.normalize());
    if (count[j] == nextc) {
      show1(ps, count, j, !turn);
      return;
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
  int v = 12;
  if (ac > 1) v = std::stoi(ag[1]);
  int c = 20;
  if (ac > 2) c = std::stoi(ag[2]);
  PositionSet ps("all.bin");
  vC count(ps.size(), 0);
  std::ifstream f("count.bin");
  f.read((char *)(&count[0]), (char *)(&count[count.size()]) - (char *)(&count[0]));
  f.close();
  for (size_t i = 0; i < ps.size(); i++) {
    if (count[i] == v) {
      c--;
      if (c >= 0)
        show1(ps, count, i, true);
    }
  }
}
  
