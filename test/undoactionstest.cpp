#include "nichess/nichess.hpp"
#include "nichess/util.hpp"

using namespace nichess;

int undoActionTest1() {
  auto pieceTypeToSquareIndexToLegalMoves = generateLegalMovesOnAnEmptyBoard();
  auto pieceTypeToSquareIndexToLegalAbilities = generateLegalAbilitiesOnAnEmptyBoard();
  auto squareToNeighboringSquares = generateSquareToNeighboringSquares();
  Game g = Game(
    pieceTypeToSquareIndexToLegalMoves,
    pieceTypeToSquareIndexToLegalAbilities,
    squareToNeighboringSquares
  );

  std::vector<PlayerAction> legalActions = g.usefulLegalActions();
  PlayerAction pa = legalActions[0];
  std::string b1 = g.boardToString();
  UndoInfo ui = g.makeAction(pa.moveSrcIdx, pa.moveDstIdx, pa.abilitySrcIdx, pa.abilityDstIdx);
  g.undoAction(ui);
  std::string b2 = g.boardToString();

  if(b1 == b2) {
    return 0;
  } else {
    return -1;
  }
}

int undoactionstest(int argc, char* argv[]) {
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
    return undoActionTest1();
  }

  return -1;
}
