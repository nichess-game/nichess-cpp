#include "nichess/nichess.hpp"
#include "nichess/util.hpp"

using namespace nichess;

int test1() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  std::vector<PlayerAction> legalActions = g.usefulLegalActions();
  if(legalActions.size() == 707) {
    return 0;
  } else {
    return -1;
  }
}

int test2() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  g.boardFromString("0|0-king-140,1-pawn-10,empty,empty,empty,empty,empty,empty,empty,0-pawn-210,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,1-pawn-70,empty,0-pawn-30,empty,empty,empty,0-warrior-80,empty,empty,empty,empty,empty,empty,empty,1-pawn-100,empty,empty,empty,1-king-200,");

  std::vector<PlayerAction> legalActions = g.usefulLegalActions();

  if(legalActions.size() == 320) {
    return 0;
  } else {
    return -1;
  }
}

int test3() {
  GameCache cache = GameCache();
  Game g = Game(cache);
  
  g.boardFromString("0|0-king-200,empty,empty,empty,empty,empty,empty,0-assassin-110,empty,0-pawn-300,empty,0-warrior-500,0-mage-230,0-pawn-300,empty,empty,empty,0-pawn-300,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,1-pawn-300,1-mage-230,1-warrior-500,empty,1-pawn-300,1-pawn-300,1-assassin-110,empty,empty,empty,empty,empty,empty,1-king-200,");

  std::vector<PlayerAction> legalActions = g.usefulLegalActions();

  if(legalActions.size() == 935) {
    return 0;
  } else {
    return -1;
  }
}


int legalactionstest(int argc, char* argv[]) {
  int defaultchoice = 1;
  int choice = defaultchoice;

  if (argc > 1) {
    if(sscanf(argv[1], "%d", &choice) != 1) {
      printf("Couldn't parse that input as a number\n");
      return -1;
    }
  }

  switch(choice) {
  case 1:
    return test1();
  case 2:
    return test2();
  case 3:
    return test3();
  }

  return -1;
}
