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
    BOOST_CHECK_EQUAL(s, p.to_string());
  }
}

BOOST_AUTO_TEST_CASE(testFlipH)
{
  {
    std::string s(
"+4+2-3"
"+1-3 ."
" . . ."
" . . ."
" . . .");
    Position p(s);
    std::string s1(
"-3+2+4"
" .-3+1"
" . . ."
" . . ."
" . . .");
    Position p1(s1);
    Position p2 = p.flipH();
    BOOST_CHECK_MESSAGE(p1.v == p2.v, p2.to_string(true));
    //    std::cerr << p1.to_string(true) << std::endl;
  }
}

BOOST_AUTO_TEST_CASE(testFlipTurn)
{
  {
    std::string s(
"+4+2-3"
"+1-3 ."
" . . ."
" . . ."
" . . .");
    Position p(s);
    std::string s1(
" . . ."
" . . ."
" . . ."
"-1+3 ."
"-4-2+3");
    Position p1(s1);
    Position p2 = p.flipTurn();
    BOOST_CHECK_MESSAGE(p1.v == p2.v, p2.to_string(true));
    //    std::cerr << p1.to_string(true) << std::endl;
  }
}

BOOST_AUTO_TEST_CASE(testFlipV)
{
  {
    std::string s(
"+4+2-3"
"+1-3 ."
" . . ."
" . . ."
" . . .");
    Position p(s);
    std::string s1(
" . . ."
" . . ."
" . . ."
"+1-3 ."
"+4+2-3");
    Position p1(s1);
    Position p2 = p.flipV();
    BOOST_CHECK_MESSAGE(p1.v == p2.v, p2.to_string(true));
    //    std::cerr << p1.to_string(true) << std::endl;
  }
}

BOOST_AUTO_TEST_CASE(testMove)
{
  {
    std::string s(
"+5+2-3"
"+5-3 ."
" . . ."
" . . ."
" . . .");
    Position p(s);
    std::string s1(
"+5+2-1"
" .+5 ."
" . . ."
" . . ."
" . . .");
    Position p1(s1);
    Position p2 = p.move(0, 1, 1, 1, 13);
    BOOST_CHECK_MESSAGE(p1.v == p2.v, p2.to_string(true));
    //    std::cerr << p1.to_string(true) << std::endl;
  }
  {
    std::string s(
"+6+6-4"
"+1 .-1"
" . . ."
" . . ."
" . . ."
      );
    Position p(s);
    std::string s1(
"+2 .+4"
"+1 .-1"
" . . ."
" . . ."
" . . ."
      );
    Position p1(s1);
    Position p2 = p.move(1, 0, 2, 0, 14);
    BOOST_CHECK_MESSAGE(p1.v == p2.v, p2.to_string(true));
    //    std::cerr << p1.to_string(true) << std::endl;
  }
  {
    std::string s(
"+6+6-3"
"+1-3 ."
" . . ."
" . . ."
" . . ."
      );
    Position p(s);
    std::string s1(
"+2 .+4"
"+1-1 ."
" . . ."
" . . ."
" . . ."
      );
    Position p1(s1);
    Position p2 = p.move(1, 0, 2, 0, 14);
    BOOST_CHECK_MESSAGE(p1.v == p2.v, p2.to_string(true));
    //    std::cerr << p1.to_string(true) << std::endl;
  }
  {
    std::string s(
      " . . ."
      " . . ."
      "-7-5-7"
      " .+1 ."
      " . . ."
      );
    Position p(s);
    std::string s1(
      " . . ."
      " . . ."
      "-3+1-3"
      " . . ."
      " . . ."
      );
    Position p1(s1);
    Position p2 = p.move(1, 3, 1, 2, 9);
    BOOST_CHECK_MESSAGE(p1.v == p2.v, p2.to_string(true));
    //    std::cerr << p1.to_string(true) << std::endl;
  }
  {
    std::string s(
      "+4+2-3"
      "+1-3 ."
      " . . ."
      " . . ."
      " . . ."
      );
    Position p(s);
    std::string s1(
      "+4+2-1"
      " .+1 ."
      " . . ."
      " . . ."
      " . . ."
      );
    Position p1(s1);
    Position p2 = p.move(0, 1, 1, 1, 9);
    BOOST_CHECK_MESSAGE(p1.v == p2.v, p2.to_string(true));
    //    std::cerr << p1.to_string(true) << std::endl;
  }
  {
    std::string s(
      "+4+2-3"
      "+1-3 ."
      " . . ."
      " . . ."
      " . . ."
      );
    Position p(s);
    std::string s1(
      "+4+2-1"
      " .+1 ."
      " . . ."
      " . . ."
      " . . ."
      );
    Position p1(s1);
    Position p2 = p.move(0, 1, 1, 1, 9);
    BOOST_CHECK_MESSAGE(p1.v == p2.v, p2.to_string(true));
    //    std::cerr << p1.to_string(true) << std::endl;
  }
  {
    std::string s(
		  "+6 .-1"
		  " . . ."
		  "+5+3 ."
		  " . . ."
		  " . . ."
      );
    Position p(s);
    std::string s1(
		  "+4 .-1"
		  " .+1 ."
		  " .+2 ."
		  " . . ."
		  " . . ."
      );
    Position p1(s1);
    Position p2 = p.move(0, 2, 1, 1, 13);
    BOOST_CHECK_MESSAGE(p1.v == p2.v, p2.to_string(true));
    //    std::cerr << p1.to_string(true) << std::endl;
  }
  {
    std::string s(
		  "+6 .-1"
		  " . . ."
		  "+5+3 ."
		  " . . ."
		  " . . ."
      );
    Position p(s);
    std::string s1(
		  " .+4-1"
		  " . . ."
		  "+1+2 ."
		  " . . ."
		  " . . ."
      );
    Position p1(s1);
    Position p2 = p.move(0, 0, 1, 0, 14);
    BOOST_CHECK_MESSAGE(p1.v == p2.v, p2.to_string(true));
    //    std::cerr << p1.to_string(true) << std::endl;
  }
}
BOOST_AUTO_TEST_CASE(testNext)
{
  {
   std::string s(
		  "+6 .-1"
		  " . . ."
		  "+5+3 ."
		  " . . ."
		  " . . ."
      );
    Position p(s);
    Next ns = p.next_positions();
#if 0
    for (auto p1 : ns.second) {
      std::cerr << p1.to_string(true) << std::endl;
    }
#endif
    {
     std::string s1(
		  " .+4-1"
		  " . . ."
		  "+1+2 ."
		  " . . ."
		  " . . ."
		     );
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) != ns.second.end());
    }
  }
  {
    std::string s(
      "+4+2-3"
      "+1-3 ."
      " . . ."
      " . . ."
      " . . ."
      );
    Position p(s);
    Next ns = p.next_positions();
#if 0
    for (auto p : ns.second) {
      std::cerr << p.to_string(true) << std::endl;
    }
#endif
    {
      std::string s1(
		     "+4+2-1"
		     " .+1 ."
		     " . . ."
		     " . . ."
		     " . . ."
		     );
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) != ns.second.end());
    }
  }
  {
    std::string s(
      "+4+2-3"
      "+1-3 ."
      " . . ."
      " . . ."
      " . . ."
      );
    Position p(s);
    Next ns = p.next_positions();
#if 0
    for (auto p : ns.second) {
      std::cerr << p.to_string(true) << std::endl;
    }
#endif
    {
      std::string s1(
		     "+4+2-1"
		     " .+1 ."
		     " . . ."
		     " . . ."
		     " . . ."
		     );
      Position p1 = Position(s1).flipTurn();
      BOOST_CHECK(std::find(ns.second.begin(), ns.second.end(), p1) != ns.second.end());
    }
  }
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
#if 0
    std::cerr << ns << std::endl;
#endif
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
#if 0
    std::cerr << ns << std::endl;
#endif
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
#if 0
    for (auto p : ns.second) {
      std::cerr << p.to_string(true) << std::endl;
    }
#endif
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
  {
    std::string s(
      " . . ."
      " . . ."
      "-7-5-7"
      " .+1 ."
      " . . ."
      );
    Position p(s);
    Next ns = p.next_positions();
#if 0
    for (auto p : ns.second) {
      std::cerr << p.to_string(true) << std::endl;
    }
#endif
    {
      std::string s1(
        " . . ."
        " . . ."
        "-3+1-3"
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
