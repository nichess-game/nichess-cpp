#include "nichess/nichess.hpp"
#include "nichess/util.hpp"

using namespace nichess;

int copyTest1() {
  GameCache cache = GameCache();
  Game g1 = Game(cache);
  Game g2 = Game(g1);

  std::string b1 = g1.boardToString();
  std::string b2 = g2.boardToString();

  if(b1 == b2) {
    return 0;
  } else {
    return -1;
  }
}

int copyTest2() {
  GameCache cache = GameCache();
  Game g1 = Game(cache);
  Game g2 = Game(g1);
  std::vector<PlayerAction> legalActions = g1.usefulLegalActions();
  PlayerAction pa = legalActions[0];

  g1.makeAction(pa.moveSrcIdx, pa.moveDstIdx, pa.abilitySrcIdx, pa.abilityDstIdx);
  g2.makeAction(pa.moveSrcIdx, pa.moveDstIdx, pa.abilitySrcIdx, pa.abilityDstIdx);
  std::string b1 = g1.boardToString();
  std::string b2 = g2.boardToString();

  if(b1 == b2) {
    return 0;
  } else {
    return -1;
  }
}

int othertest(int argc, char* argv[]) {
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
    return copyTest1();
  case 2:
    return copyTest2();
  default:
    printf("\nInvalid test number.\n");
    return -1;
  }

  return -1;
}
