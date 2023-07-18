#include "nichess/nichess.hpp"
#include "nichess/util.hpp"

using namespace nichess;

int legalActionsTest1() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  std::vector<PlayerAction> usefulLegalActions = g.usefulLegalActions();
  if(usefulLegalActions.size() == 42) {
    return 0;
  } else {
    return -1;
  }
}

int legalActionsTest2() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  g.boardFromString("0|0-king-140,1-pawn-10,empty,empty,empty,empty,empty,empty,empty,0-pawn-210,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,1-pawn-70,empty,0-pawn-30,empty,empty,empty,0-warrior-80,empty,empty,empty,empty,empty,empty,empty,1-pawn-100,empty,empty,empty,1-king-200,");

  std::vector<PlayerAction> usefulLegalActions = g.usefulLegalActions();

  if(usefulLegalActions.size() == 84) {
    return 0;
  } else {
    return -1;
  }
}

int legalActionsTest3() {
  GameCache cache = GameCache();
  Game g = Game(cache);
  
  g.boardFromString("0|0-king-200,empty,empty,empty,empty,empty,empty,0-assassin-110,empty,0-pawn-300,empty,0-warrior-500,0-mage-230,0-pawn-300,empty,empty,empty,0-pawn-300,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,1-pawn-300,1-mage-230,1-warrior-500,empty,1-pawn-300,1-pawn-300,1-assassin-110,empty,empty,empty,empty,empty,empty,1-king-200,");

  std::vector<PlayerAction> usefulLegalActions = g.usefulLegalActions();

  if(usefulLegalActions.size() == 46) {
    return 0;
  } else {
    return -1;
  }
}

int legalActionsTest4() {
  GameCache cache = GameCache();
  Game g = Game(cache);
  std::vector<PlayerAction> allLegalActions = g.allLegalActions();

  if(allLegalActions.size() == 1886) {
    return 0;
  } else {
    return -1;
  }
}

int legalActionsTest5() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  if(g.isActionLegal(0, 1, ABILITY_SKIP, ABILITY_SKIP)) {
    return 0;
  } else {
    return -1;
  }
}

int legalActionsTest6() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  if(g.isActionLegal(MOVE_SKIP, MOVE_SKIP, ABILITY_SKIP, ABILITY_SKIP)) {
    return 0;
  } else {
    return -1;
  }
}

/*
 * Move to the same square piece is already at
 */
int legalActionsTest7() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  if(g.isActionLegal(0, 0, ABILITY_SKIP, ABILITY_SKIP)) {
    return -1;
  } else {
    return 0;
  }
}

/*
 * Only set moveSrcIdx to MOVE_SKIP (should be moveDstIdx too in order to skip the move)
 */
int legalActionsTest8() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  if(g.isActionLegal(MOVE_SKIP, 0, ABILITY_SKIP, ABILITY_SKIP)) {
    return -1;
  } else {
    return 0;
  }
}

/*
 * Move outside the board
 */
int legalActionsTest9() {
  GameCache cache = GameCache();
  Game g = Game(cache);
  g.makeAction(MOVE_SKIP, MOVE_SKIP, ABILITY_SKIP, ABILITY_SKIP);

  if(g.isActionLegal(63, 64, ABILITY_SKIP, ABILITY_SKIP)) {
    return -1;
  } else {
    return 0;
  }
}

/*
 * Move to an already occupied square
 */
int legalActionsTest10() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  if(g.isActionLegal(0, 8, ABILITY_SKIP, ABILITY_SKIP)) {
    return -1;
  } else {
    return 0;
  }
}

/*
 * Use ability on an empty square
 */
int legalActionsTest11() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  if(g.isActionLegal(MOVE_SKIP, MOVE_SKIP, 0, 1)) {
    return 0;
  } else {
    return -1;
  }
}

/*
 * Use ability on a square occupied by your own piece
 */
int legalActionsTest12() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  if(g.isActionLegal(MOVE_SKIP, MOVE_SKIP, 0, 8)) {
    return -1;
  } else {
    return 0;
  }
}

int legalActionsTest13() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  if(g.isActionLegal(0, 2, ABILITY_SKIP, ABILITY_SKIP)) {
    return -1;
  } else {
    return 0;
  }
}

int legalActionsTest14() {
  GameCache cache = GameCache();
  Game g = Game(cache);

  if(g.isActionLegal(MOVE_SKIP, MOVE_SKIP, 0, 2)) {
    return -1;
  } else {
    return 0;
  }
}

int legalActionsTest15() {
  GameCache cache = GameCache();
  Game g = Game(cache);
  auto a = g.allLegalAbilitiesByPiece(0);

  if(a.size() == 1) {
    return 0;
  } else {
    return -1;
  }
}

int legalActionsTest16() {
  GameCache cache = GameCache();
  Game g = Game(cache);
  auto a = g.allLegalAbilitiesByPiece(12);

  if(a.size() == 17) {
    return 0;
  } else {
    return -1;
  }
}

int legalActionsTest17() {
  GameCache cache = GameCache();
  Game g = Game(cache);
  auto a = g.usefulLegalAbilitiesByPiece(0);

  if(a.size() == 0) {
    return 0;
  } else {
    return -1;
  }
}

int legalActionsTest18() {
  GameCache cache = GameCache();
  Game g = Game(cache);
  auto a = g.usefulLegalAbilitiesByPiece(12);

  if(a.size() == 0) {
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
    return legalActionsTest1();
  case 2:
    return legalActionsTest2();
  case 3:
    return legalActionsTest3();
  case 4:
    return legalActionsTest4();
  case 5:
    return legalActionsTest5();
  case 6:
    return legalActionsTest6();
  case 7:
    return legalActionsTest7();
  case 8:
    return legalActionsTest8();
  case 9:
    return legalActionsTest9();
  case 10:
    return legalActionsTest10();
  case 11:
    return legalActionsTest11();
  case 12:
    return legalActionsTest12();
  case 13:
    return legalActionsTest13();
  case 14:
    return legalActionsTest14();
  case 15:
    return legalActionsTest15();
  case 16:
    return legalActionsTest16();
  case 17:
    return legalActionsTest17();
  case 18:
    return legalActionsTest18();
  default:
    printf("\nInvalid test number.\n");
    return -1;
  }

  return -1;
}
