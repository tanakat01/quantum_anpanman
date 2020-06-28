// Copyright Tetsuro Tanaka, 2020
#include "./position.h"

# define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testIO)
{
  {
    std::string s(
      "-7-7-7"
      " . . ."
      " . . ."
      " . . ."
      "+7+7+7"
      );
    Position p(s);
    std::cerr << s << "\\n\n";
    std::cerr << p.to_string() << "\\n\n";
    BOOST_CHECK_EQUAL(s, p.to_string());
  }
}
BOOST_AUTO_TEST_CASE(testNext)
{
  {
    std::string s(
      " . . ."
      " .-7-7"
      "+5+3 ."
      " . . ."
      " . . ."
      );
    Position p(s);
    Next ns = p.next_positions();
    BOOST_CHECK_EQUAL(ns.second.size(), 6);
    BOOST_CHECK(!ns.first);
    {
      std::string s1(
        " . . ."
        "+5-7-7"
        " .+3 ."
        " . . ."
        " . . ."
        );
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) != ns.second.end());
    }
    {
      std::string s1(
        " . . ."
        "+3-7-7"
        "+5 . ."
        " . . ."
        " . . ."
        );
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) != ns.second.end());
    }
    {
      std::string s1(
        " . . ."
        " .+1-1"
        " .+2 ."
        " . . ."
        " . . ."
        );
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) != ns.second.end());
    }
    {
      std::string s1(
        " . . ."
        " .+3-1"
        "+5 . ."
        " . . ."
        " . . ."
        );
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) != ns.second.end());
    }
    {
      std::string s1(
        " . . ."
        " .-1+3"
        "+5 . ."
        " . . ."
        " . . ."
        );
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) != ns.second.end());
    }
    {
      std::string s1(
        " . . ."
        " .-7-7"
        "+4 .+1"
        " . . ."
        " . . ."
        );
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) != ns.second.end());
    }
    std::cerr << ns << std::endl;
  }
  {
    std::string s(
      "-7-7-7"
      " . . ."
      " . .+7"
      "+7+5 ."
      " . . ."
      );
    Position p(s);
    Next ns = p.next_positions();
    std::cerr << ns << std::endl;
    {
      std::string s1(
        "-7-7-7"
        " . . ."
        " .+5 ."
        "+2+5 ."
        " . . ."
        );
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) != ns.second.end());
    }
  }
  {
    std::string s(
      "+4 . ."
      "-1 .+4"
      "-4 . ."
      " . .+1"
      "-2 . .");
    Position p(s);
    Next ns = p.next_positions();
#if 0
    for (auto p1 : ns.second) {
      std::cerr << "-------\n" << p1.flipTurn().to_string(true) << std::endl;
    }
    std::cerr << ns << std::endl;
#endif
    {
      std::string s1(
        "+4 . ."
        "-1 . ."
        "+4 . ."
        " . .+1"
        "-2 . .");
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) == ns.second.end());
    }
  }
  {
    std::string s(
      " . . ."
      " .-7 ."
      "-7 .-7"
      " .+1 ."
      " . . ."
      );
    Position p(s);
    Next ns = p.next_positions();
    for (auto p : ns.second) {
      std::cerr << p.to_string(true) << std::endl;
    }
    {
      std::string s1(
        " . . ."
        " .-7 ."
        "-7+1-7"
        " . . ."
        " . . ."
        );
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) != ns.second.end());
    }
  }
}

BOOST_AUTO_TEST_CASE(testWin)
{
  {
    std::string s(
      " . . ."
      "+3 . ."
      " . . ."
      " .-7-7"
      " . . ."
      );
    Position p(s);
    BOOST_CHECK_MESSAGE(p.isWin(), p.to_string());
  }
  {
    std::string s(
      "-7 . ."
      "+3 . ."
      " . . ."
      " .-7-7"
      " . . ."
      );
    Position p(s);
    BOOST_CHECK_MESSAGE(p.isWin(), p.to_string());
  }
  {
    std::string s(
      "+2 . ."
      "+3 . ."
      " . . ."
      " .-7-7"
      " . . ."
      );
    Position p(s);
    BOOST_CHECK_MESSAGE(p.isWin(), p.to_string());
  }
  {
    std::string s(
      "+2+4 ."
      "+3 . ."
      " . . ."
      " .-7-7"
      " . . ."
      );
    Position p(s);
    BOOST_CHECK_MESSAGE(!p.isWin(), p.to_string());
  }
  {
    std::string s(
      " . . ."
      "-2-1-4"
      " . .+2"
      " .+1 ."
      "+4 . ."
      );
    Position p(s);
    BOOST_CHECK_MESSAGE(p.isWin(), p.to_string());
  }
}
