#include "nichess/nichess.hpp"
#include "nichess/util.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <tuple>
#include <chrono>

using namespace nichess;

/*
 * Coordinates are not standard. Bottom left is (0,0) and top right is (7,7)
 */
int nichess::coordinatesToBoardIndex(int column, int row) {
  return column + row * NUM_COLUMNS;
}

std::tuple<int, int> nichess::boardIndexToCoordinates(int squareIndex) {
  int x = squareIndex -  (floor(squareIndex / NUM_ROWS) * NUM_ROWS);
  int y = floor(squareIndex / NUM_ROWS);
  return std::tuple<int, int>(x, y);
}

PlayerAction::PlayerAction() { }

PlayerAction::PlayerAction(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx) {
  this->moveSrcIdx = moveSrcIdx;
  this->moveDstIdx = moveDstIdx;
  this->abilitySrcIdx = abilitySrcIdx;
  this->abilityDstIdx = abilityDstIdx;
}

PlayerMove::PlayerMove() { }

PlayerMove::PlayerMove(int moveSrcIdx, int moveDstIdx) {
  this->moveSrcIdx = moveSrcIdx;
  this->moveDstIdx = moveDstIdx;
}

PlayerAbility::PlayerAbility() { }

PlayerAbility::PlayerAbility(int abilitySrcIdx, int abilityDstIdx) {
  this->abilitySrcIdx = abilitySrcIdx;
  this->abilityDstIdx = abilityDstIdx;
}

Piece::Piece(): type(PieceType::NO_PIECE), healthPoints(0), squareIndex(0) { }

Piece::Piece(PieceType type, int healthPoints, int squareIndex):
  type(type),
  healthPoints(healthPoints),
  squareIndex(squareIndex)
{ }

Piece::Piece(const Piece& other): type(other.type), healthPoints(other.healthPoints), squareIndex(other.squareIndex) { }

bool Piece::operator==(const Piece& other) const {
  const auto* other_cs = dynamic_cast<const Piece*>(&other);
  if (other_cs == nullptr) {
    return false;
  }
  return (other_cs->type == type && other_cs->healthPoints == healthPoints && other_cs->squareIndex == squareIndex);
}

bool Piece::operator!=(const Piece& other) const {
  const auto* other_cs = dynamic_cast<const Piece*>(&other);
  if (other_cs == nullptr) {
    return true;
  }
  return (other_cs->type != type || other_cs->healthPoints != healthPoints || other_cs->squareIndex != squareIndex);
}

UndoInfo::UndoInfo() {
  for(int i = 0; i < 9; i++) {
    this->affectedPieces[i] = nullptr;
  }
}

UndoInfo::UndoInfo(int moveSrcIdx, int moveDstIdx, AbilityType abilityType) {
  this->moveSrcIdx = moveSrcIdx;
  this->moveDstIdx = moveDstIdx;
  this->abilityType = abilityType;
  for(int i = 0; i < 9; i++) {
    this->affectedPieces[i] = nullptr;
  }
}

GameCache::GameCache() {
  pieceTypeToSquareIndexToLegalMoves = generateLegalMovesOnAnEmptyBoard();
  pieceTypeToSquareIndexToLegalAbilities = generateLegalAbilitiesOnAnEmptyBoard();
  squareToNeighboringSquares = generateSquareToNeighboringSquares();
}

void Game::reset() {
  moveNumber = 0;
  currentPlayer = Player::PLAYER_1;
  // Create starting position
  board[coordinatesToBoardIndex(0,0)] = new Piece(PieceType::P1_KING, KING_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(0,0));
  board[coordinatesToBoardIndex(0,1)] = new Piece(PieceType::P1_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(0,1));
  board[coordinatesToBoardIndex(1,1)] = new Piece(PieceType::P1_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(1,1));
  board[coordinatesToBoardIndex(7,0)] = new Piece(PieceType::P1_ASSASSIN, ASSASSIN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(7,0));
  board[coordinatesToBoardIndex(3,1)] = new Piece(PieceType::P1_WARRIOR, WARRIOR_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(3,1));
  board[coordinatesToBoardIndex(4,1)] = new Piece(PieceType::P1_MAGE, MAGE_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(4,1));
  board[coordinatesToBoardIndex(5,1)] = new Piece(PieceType::P1_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(5,1));

  board[coordinatesToBoardIndex(7,7)] = new Piece(PieceType::P2_KING, KING_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(7,7));
  board[coordinatesToBoardIndex(7,6)] = new Piece(PieceType::P2_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(7,6));
  board[coordinatesToBoardIndex(6,6)] = new Piece(PieceType::P2_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(6,6));
  board[coordinatesToBoardIndex(0,7)] = new Piece(PieceType::P2_ASSASSIN, ASSASSIN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(0,7));
  board[coordinatesToBoardIndex(4,6)] = new Piece(PieceType::P2_WARRIOR, WARRIOR_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(4,6));
  board[coordinatesToBoardIndex(3,6)] = new Piece(PieceType::P2_MAGE, MAGE_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(3,6));
  board[coordinatesToBoardIndex(2,6)] = new Piece(PieceType::P2_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(2,6));

  board[coordinatesToBoardIndex(0,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(0,2));
  board[coordinatesToBoardIndex(0,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(0,3));
  board[coordinatesToBoardIndex(0,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(0,4));
  board[coordinatesToBoardIndex(0,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(0,5));
  board[coordinatesToBoardIndex(0,6)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(0,6));
  board[coordinatesToBoardIndex(1,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,0));
  board[coordinatesToBoardIndex(1,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,2));
  board[coordinatesToBoardIndex(1,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,3));
  board[coordinatesToBoardIndex(1,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,4));
  board[coordinatesToBoardIndex(1,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,5));
  board[coordinatesToBoardIndex(1,6)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,6));
  board[coordinatesToBoardIndex(1,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,7));
  board[coordinatesToBoardIndex(2,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,0));
  board[coordinatesToBoardIndex(2,1)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,1));
  board[coordinatesToBoardIndex(2,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,2));
  board[coordinatesToBoardIndex(2,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,3));
  board[coordinatesToBoardIndex(2,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,4));
  board[coordinatesToBoardIndex(2,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,5));
  board[coordinatesToBoardIndex(2,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,7));
  board[coordinatesToBoardIndex(3,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,0));
  board[coordinatesToBoardIndex(3,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,2));
  board[coordinatesToBoardIndex(3,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,3));
  board[coordinatesToBoardIndex(3,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,4));
  board[coordinatesToBoardIndex(3,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,5));
  board[coordinatesToBoardIndex(3,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,7));
  board[coordinatesToBoardIndex(4,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,0));
  board[coordinatesToBoardIndex(4,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,2));
  board[coordinatesToBoardIndex(4,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,3));
  board[coordinatesToBoardIndex(4,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,4));
  board[coordinatesToBoardIndex(4,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,5));
  board[coordinatesToBoardIndex(4,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,7));
  board[coordinatesToBoardIndex(5,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,0));
  board[coordinatesToBoardIndex(5,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,2));
  board[coordinatesToBoardIndex(5,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,3));
  board[coordinatesToBoardIndex(5,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,4));
  board[coordinatesToBoardIndex(5,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,5));
  board[coordinatesToBoardIndex(5,6)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,6));
  board[coordinatesToBoardIndex(5,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,7));
  board[coordinatesToBoardIndex(6,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,0));
  board[coordinatesToBoardIndex(6,1)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,1));
  board[coordinatesToBoardIndex(6,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,2));
  board[coordinatesToBoardIndex(6,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,3));
  board[coordinatesToBoardIndex(6,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,4));
  board[coordinatesToBoardIndex(6,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,5));
  board[coordinatesToBoardIndex(6,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,7));
  board[coordinatesToBoardIndex(7,1)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(7,1));
  board[coordinatesToBoardIndex(7,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(7,2));
  board[coordinatesToBoardIndex(7,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(7,3));
  board[coordinatesToBoardIndex(7,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(7,4));
  board[coordinatesToBoardIndex(7,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(7,5));

  // Piece pointers are also kept in an array for faster access
  std::vector<Piece*> p1Pieces{NUM_STARTING_PIECES};
  p1Pieces[KING_PIECE_INDEX] = board[coordinatesToBoardIndex(0,0)];
  p1Pieces[PAWN_1_PIECE_INDEX] = board[coordinatesToBoardIndex(0,1)];
  p1Pieces[PAWN_2_PIECE_INDEX] = board[coordinatesToBoardIndex(1,1)];
  p1Pieces[ASSASSIN_PIECE_INDEX] = board[coordinatesToBoardIndex(7,0)];
  p1Pieces[WARRIOR_PIECE_INDEX] = board[coordinatesToBoardIndex(3,1)];
  p1Pieces[MAGE_PIECE_INDEX] = board[coordinatesToBoardIndex(4,1)];
  p1Pieces[PAWN_3_PIECE_INDEX] = board[coordinatesToBoardIndex(5,1)];
  playerToPieces[Player::PLAYER_1] = p1Pieces;
  p1King = p1Pieces[KING_PIECE_INDEX];

  std::vector<Piece*> p2Pieces{NUM_STARTING_PIECES};
  p2Pieces[KING_PIECE_INDEX] = board[coordinatesToBoardIndex(7,7)];
  p2Pieces[PAWN_1_PIECE_INDEX] = board[coordinatesToBoardIndex(7,6)];
  p2Pieces[PAWN_2_PIECE_INDEX] = board[coordinatesToBoardIndex(6,6)];
  p2Pieces[ASSASSIN_PIECE_INDEX] = board[coordinatesToBoardIndex(0,7)];
  p2Pieces[WARRIOR_PIECE_INDEX] = board[coordinatesToBoardIndex(4,6)];
  p2Pieces[MAGE_PIECE_INDEX] = board[coordinatesToBoardIndex(3,6)];
  p2Pieces[PAWN_3_PIECE_INDEX] = board[coordinatesToBoardIndex(2,6)];
  playerToPieces[Player::PLAYER_2] = p2Pieces;
  p2King = p2Pieces[KING_PIECE_INDEX];
}

Game::Game(GameCache& gameCache) {
  this->gameCache = &gameCache;
  reset();
}

Game::Game(const Game& other) {
  this->moveNumber = other.moveNumber;
  this->currentPlayer = other.currentPlayer;
  this->gameCache = other.gameCache;
  for(int i = 0; i < NUM_SQUARES; i++) {
    board[i] = new Piece(*other.board[i]);
  }

  std::vector<Piece*> otherP1Pieces = other.playerToPieces[PLAYER_1];
  std::vector<Piece*> otherP2Pieces = other.playerToPieces[PLAYER_2];
  std::vector<Piece*> p1Pieces{NUM_STARTING_PIECES};

  Piece* otherP1King = otherP1Pieces[KING_PIECE_INDEX];
  if(otherP1King->healthPoints > 0) {
    p1Pieces[KING_PIECE_INDEX] = board[otherP1King->squareIndex];
  } else {
    p1Pieces[KING_PIECE_INDEX] = new Piece(PieceType::P1_KING, otherP1King->healthPoints, otherP1King->squareIndex);
  }
  p1King = p1Pieces[KING_PIECE_INDEX];

  Piece* otherP1Pawn1 = otherP1Pieces[PAWN_1_PIECE_INDEX];
  if(otherP1Pawn1->healthPoints > 0) {
    p1Pieces[PAWN_1_PIECE_INDEX] = board[otherP1Pawn1->squareIndex];
  } else {
    p1Pieces[PAWN_1_PIECE_INDEX] = new Piece(PieceType::P1_PAWN, otherP1Pawn1->healthPoints, otherP1Pawn1->squareIndex);
  }

  Piece* otherP1Pawn2 = otherP1Pieces[PAWN_2_PIECE_INDEX];
  if(otherP1Pawn2->healthPoints > 0) {
    p1Pieces[PAWN_2_PIECE_INDEX] = board[otherP1Pawn2->squareIndex];
  } else {
    p1Pieces[PAWN_2_PIECE_INDEX] = new Piece(PieceType::P1_PAWN, otherP1Pawn2->healthPoints, otherP1Pawn2->squareIndex);
  }

  Piece* otherP1Assassin = otherP1Pieces[ASSASSIN_PIECE_INDEX];
  if(otherP1Assassin->healthPoints > 0) {
    p1Pieces[ASSASSIN_PIECE_INDEX] = board[otherP1Assassin->squareIndex];
  } else {
    p1Pieces[ASSASSIN_PIECE_INDEX] = new Piece(PieceType::P1_ASSASSIN, otherP1Assassin->healthPoints, otherP1Assassin->squareIndex);
  }

  Piece* otherP1Warrior = otherP1Pieces[WARRIOR_PIECE_INDEX];
  if(otherP1Warrior->healthPoints > 0) {
    p1Pieces[WARRIOR_PIECE_INDEX] = board[otherP1Warrior->squareIndex];
  } else {
    p1Pieces[WARRIOR_PIECE_INDEX] = new Piece(PieceType::P1_WARRIOR, otherP1Warrior->healthPoints, otherP1Warrior->squareIndex);
  }

  Piece* otherP1Mage = otherP1Pieces[MAGE_PIECE_INDEX];
  if(otherP1Mage->healthPoints > 0) {
    p1Pieces[MAGE_PIECE_INDEX] = board[otherP1Mage->squareIndex];
  } else {
    p1Pieces[MAGE_PIECE_INDEX] = new Piece(PieceType::P1_MAGE, otherP1Mage->healthPoints, otherP1Mage->squareIndex);
  }

  Piece* otherP1Pawn3 = otherP1Pieces[PAWN_3_PIECE_INDEX];
  if(otherP1Pawn3->healthPoints > 0) {
    p1Pieces[PAWN_3_PIECE_INDEX] = board[otherP1Pawn3->squareIndex];
  } else {
    p1Pieces[PAWN_3_PIECE_INDEX] = new Piece(PieceType::P1_PAWN, otherP1Pawn3->healthPoints, otherP1Pawn3->squareIndex);
  }

  playerToPieces[Player::PLAYER_1] = p1Pieces;

  std::vector<Piece*> p2Pieces{NUM_STARTING_PIECES};

  Piece* otherP2King = otherP2Pieces[KING_PIECE_INDEX];
  if(otherP2King->healthPoints > 0) {
    p2Pieces[KING_PIECE_INDEX] = board[otherP2King->squareIndex];
  } else {
    p2Pieces[KING_PIECE_INDEX] = new Piece(PieceType::P2_KING, otherP2King->healthPoints, otherP2King->squareIndex);
  }
  p2King = p2Pieces[KING_PIECE_INDEX];

  Piece* otherP2Pawn1 = otherP2Pieces[PAWN_1_PIECE_INDEX];
  if(otherP2Pawn1->healthPoints > 0) {
    p2Pieces[PAWN_1_PIECE_INDEX] = board[otherP2Pawn1->squareIndex];
  } else {
    p2Pieces[PAWN_1_PIECE_INDEX] = new Piece(PieceType::P2_PAWN, otherP2Pawn1->healthPoints, otherP2Pawn1->squareIndex);
  }

  Piece* otherP2Pawn2 = otherP2Pieces[PAWN_2_PIECE_INDEX];
  if(otherP2Pawn2->healthPoints > 0) {
    p2Pieces[PAWN_2_PIECE_INDEX] = board[otherP2Pawn2->squareIndex];
  } else {
    p2Pieces[PAWN_2_PIECE_INDEX] = new Piece(PieceType::P2_PAWN, otherP2Pawn2->healthPoints, otherP2Pawn2->squareIndex);
  }

  Piece* otherP2Assassin = otherP2Pieces[ASSASSIN_PIECE_INDEX];
  if(otherP2Assassin->healthPoints > 0) {
    p2Pieces[ASSASSIN_PIECE_INDEX] = board[otherP2Assassin->squareIndex];
  } else {
    p2Pieces[ASSASSIN_PIECE_INDEX] = new Piece(PieceType::P2_ASSASSIN, otherP2Assassin->healthPoints, otherP2Assassin->squareIndex);
  }

  Piece* otherP2Warrior = otherP2Pieces[WARRIOR_PIECE_INDEX];
  if(otherP2Warrior->healthPoints > 0) {
    p2Pieces[WARRIOR_PIECE_INDEX] = board[otherP2Warrior->squareIndex];
  } else {
    p2Pieces[WARRIOR_PIECE_INDEX] = new Piece(PieceType::P2_WARRIOR, otherP2Warrior->healthPoints, otherP2Warrior->squareIndex);
  }

  Piece* otherP2Mage = otherP2Pieces[MAGE_PIECE_INDEX];
  if(otherP2Mage->healthPoints > 0) {
    p2Pieces[MAGE_PIECE_INDEX] = board[otherP2Mage->squareIndex];
  } else {
    p2Pieces[MAGE_PIECE_INDEX] = new Piece(PieceType::P2_MAGE, otherP2Mage->healthPoints, otherP2Mage->squareIndex);
  }

  Piece* otherP2Pawn3 = otherP2Pieces[PAWN_3_PIECE_INDEX];
  if(otherP2Pawn3->healthPoints > 0) {
    p2Pieces[PAWN_3_PIECE_INDEX] = board[otherP2Pawn3->squareIndex];
  } else {
    p2Pieces[PAWN_3_PIECE_INDEX] = new Piece(PieceType::P2_PAWN, otherP2Pawn3->healthPoints, otherP2Pawn3->squareIndex);
  }

  playerToPieces[Player::PLAYER_2] = p2Pieces;
}

Game::Game(GameCache& gameCache, const std::string encodedBoard) {
  this->gameCache = &gameCache;
  boardFromString(encodedBoard);
}

Game::~Game() {
  for(Piece* p: playerToPieces[PLAYER_1]) {
    if(p->healthPoints <= 0) delete p;
  }
  for(Piece* p: playerToPieces[PLAYER_2]) {
    if(p->healthPoints <= 0) delete p;
  }

  for(int i = 0; i < NUM_SQUARES; i++) {
    delete board[i];
  }
}


/*
 * Assumes that the move and ability are legal.
 * If the ability is not useful (i.e. does not alter the game state), it's converted to
 * AbilityType::NO_ABILITY.
 * Checking whether ability is useful makes the function ~1.5% slower.
 */
UndoInfo Game::makeAction(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx) {
  UndoInfo undoInfo = UndoInfo();
  undoInfo.moveSrcIdx = moveSrcIdx;
  undoInfo.moveDstIdx = moveDstIdx;
  if(moveSrcIdx != MOVE_SKIP) {
    makeMove(moveSrcIdx, moveDstIdx);
  }
  if(abilitySrcIdx != ABILITY_SKIP) {
    Piece* abilitySrcPiece = board[abilitySrcIdx];
    Piece* abilityDstPiece = board[abilityDstIdx];
    Piece* neighboringPiece;
    int neighboringSquare;
    switch(abilitySrcPiece->type) {
      // king does single target damage
      case P1_KING:
        if(player1OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= KING_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::KING_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      // mage damages attacked piece and all enemy pieces that are touching it
      case P1_MAGE:
        if(player1OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= MAGE_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::MAGE_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        for(int i = 0; i < gameCache->squareToNeighboringSquares[abilityDstIdx].size(); i++) {
          neighboringSquare = gameCache->squareToNeighboringSquares[abilityDstIdx][i];
          neighboringPiece = board[neighboringSquare];
          if(player1OrEmpty(neighboringPiece->type)) continue;  // don't damage your own pieces
          neighboringPiece->healthPoints -= MAGE_ABILITY_POINTS;
          // i+1 because 0 is for abilityDstPiece
          undoInfo.affectedPieces[i+1] = neighboringPiece;
          if(neighboringPiece->healthPoints <= 0) {
            board[neighboringSquare] = new Piece(NO_PIECE, 0, neighboringSquare);
          }
        }
        break;
      case P1_PAWN:
        if(player1OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= PAWN_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::PAWN_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      case P1_WARRIOR:
        if(player1OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= WARRIOR_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::WARRIOR_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      case P1_ASSASSIN:
        if(player1OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= ASSASSIN_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::ASSASSIN_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      case P2_KING:
        if(player2OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= KING_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::KING_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      case P2_MAGE:
        if(player2OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= MAGE_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::MAGE_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        for(int i = 0; i < gameCache->squareToNeighboringSquares[abilityDstIdx].size(); i++) {
          neighboringSquare = gameCache->squareToNeighboringSquares[abilityDstIdx][i];
          neighboringPiece = board[neighboringSquare];
          if(player2OrEmpty(neighboringPiece->type)) continue;  // don't damage your own pieces
          neighboringPiece->healthPoints -= MAGE_ABILITY_POINTS;
          // i+1 because 0 is for abilityDstPiece
          undoInfo.affectedPieces[i+1] = neighboringPiece;
          if(neighboringPiece->healthPoints <= 0) {
            board[neighboringSquare] = new Piece(NO_PIECE, 0, neighboringSquare);
          }
        }
        break;
      case P2_PAWN:
        if(player2OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= PAWN_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::PAWN_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      case P2_WARRIOR:
        if(player2OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= WARRIOR_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::WARRIOR_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      case P2_ASSASSIN:
        if(player2OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= ASSASSIN_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::ASSASSIN_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
    }
  } else {
    undoInfo.abilityType = AbilityType::NO_ABILITY;
  } 
  this->moveNumber += 1;
  this->currentPlayer = ~currentPlayer;
  return undoInfo;
}

void Game::undoAction(UndoInfo undoInfo) {
  // undo ability
  switch(undoInfo.abilityType) {
    Piece* affectedPiece;
    case KING_DAMAGE:
      affectedPiece = undoInfo.affectedPieces[0];
      affectedPiece->healthPoints += KING_ABILITY_POINTS;
      if(this->board[affectedPiece->squareIndex]->type == PieceType::NO_PIECE) {
        delete this->board[affectedPiece->squareIndex];
      }
      this->board[affectedPiece->squareIndex] = affectedPiece;
      break;
    case MAGE_DAMAGE:
      for(int i = 0; i < 9; i++){ // 1 attacked square and 8 neighboring
        affectedPiece = undoInfo.affectedPieces[i];
        if(affectedPiece == nullptr) continue;
        affectedPiece->healthPoints += MAGE_ABILITY_POINTS;
        if(this->board[affectedPiece->squareIndex]->type == PieceType::NO_PIECE) {
          delete this->board[affectedPiece->squareIndex];
        }
        this->board[affectedPiece->squareIndex] = affectedPiece;
      }
      break;
    case WARRIOR_DAMAGE:
      affectedPiece = undoInfo.affectedPieces[0];
      affectedPiece->healthPoints += WARRIOR_ABILITY_POINTS;
      if(this->board[affectedPiece->squareIndex]->type == PieceType::NO_PIECE) {
        delete this->board[affectedPiece->squareIndex];
      }
      this->board[affectedPiece->squareIndex] = affectedPiece;
      break;
    case ASSASSIN_DAMAGE:
      affectedPiece = undoInfo.affectedPieces[0];
      affectedPiece->healthPoints += ASSASSIN_ABILITY_POINTS;
      if(this->board[affectedPiece->squareIndex]->type == PieceType::NO_PIECE) {
        delete this->board[affectedPiece->squareIndex];
      }
      this->board[affectedPiece->squareIndex] = affectedPiece;
      break;
    case PAWN_DAMAGE:
      affectedPiece = undoInfo.affectedPieces[0];
      affectedPiece->healthPoints += PAWN_ABILITY_POINTS;
      if(this->board[affectedPiece->squareIndex]->type == PieceType::NO_PIECE) {
        delete this->board[affectedPiece->squareIndex];
      }
      this->board[affectedPiece->squareIndex] = affectedPiece;
      break;
    case NO_ABILITY:
      break;
    default:
      break;
  }
  // undo move
  if(undoInfo.moveSrcIdx != MOVE_SKIP) {
    undoMove(undoInfo.moveSrcIdx, undoInfo.moveDstIdx);
  }
  this->moveNumber -= 1;
  this->currentPlayer = ~currentPlayer;
}

std::string Game::dump() const {
  std::string retval = "";
  retval += std::string("------------------------------------------\n");
  for(int i = NUM_ROWS-1; i >= 0; i--) {
    retval += std::to_string(i) + std::string("   ");
    for(int j = 0; j < NUM_COLUMNS; j++) {
      if(board[coordinatesToBoardIndex(j, i)]->type != PieceType::NO_PIECE) {
        retval += pieceTypeToString(board[coordinatesToBoardIndex(j, i)]->type) + std::to_string(board[coordinatesToBoardIndex(j, i)]->healthPoints) + std::string(" ");
      } else {
        retval += pieceTypeToString(board[coordinatesToBoardIndex(j, i)]->type) + std::string("   ") + std::string(" ");
      }
    }
    retval += std::string("\n");
  } 
  retval += std::string("\n");
  retval += std::string("   ");
  for(int i = 0; i < NUM_COLUMNS; i++) {
    retval += std::string(" ") + std::to_string(i) + std::string("   ");
  }
  retval += std::string("\n------------------------------------------\n");

  return retval;
}

void Game::makeMove(int moveSrcIdx, int moveDstIdx) {
  delete board[moveDstIdx];
  board[moveDstIdx] = board[moveSrcIdx];
  board[moveDstIdx]->squareIndex = moveDstIdx;
  board[moveSrcIdx] = new Piece(PieceType::NO_PIECE, 0, moveSrcIdx);
  return;
}

/*
 * Since move is being reverted, goal here is to move from "destination" to "source".
 */
void Game::undoMove(int moveSrcIdx, int moveDstIdx) {
  delete board[moveSrcIdx];
  board[moveSrcIdx] = board[moveDstIdx];
  board[moveSrcIdx]->squareIndex = moveSrcIdx;
  board[moveDstIdx] = new Piece(PieceType::NO_PIECE, 0, moveDstIdx);
  return;
}

/*
 * Assumes that the game is not over.
 */
std::vector<PlayerMove> Game::legalMovesByPiece(int srcSquareIdx) {
  std::vector<PlayerMove> retval;
  Piece* piece = board[srcSquareIdx];
  if((!pieceBelongsToPlayer(piece->type, currentPlayer)) ||
      piece->healthPoints <= 0) {
    return retval;
  }
  auto legalMovesOnEmptyBoard = gameCache->pieceTypeToSquareIndexToLegalMoves[piece->type][piece->squareIndex];
  for(int i = 0; i < legalMovesOnEmptyBoard.size(); i++) {
    if(board[legalMovesOnEmptyBoard[i].moveDstIdx]->type != NO_PIECE) continue;
    retval.push_back(legalMovesOnEmptyBoard[i]);
  }
  return retval;
}

/*
 * Assumes that the game is not over.
 * Useful abilities are those that change the game state.
 * For example, warrior attacking an empty square is legal but doesn't change the game state.
 */
std::vector<PlayerAbility> Game::usefulLegalAbilitiesByPiece(int srcSquareIdx) {
  std::vector<PlayerAbility> retval;
  Piece* piece = board[srcSquareIdx];
  if((!pieceBelongsToPlayer(piece->type, currentPlayer)) ||
      piece->healthPoints <= 0) {
    return retval;
  }
  auto legalAbilitiesOnEmptyBoard = gameCache->pieceTypeToSquareIndexToLegalAbilities[piece->type][piece->squareIndex];
  for(int l = 0; l < legalAbilitiesOnEmptyBoard.size(); l++) {
    PlayerAbility currentAbility = legalAbilitiesOnEmptyBoard[l];
    Piece* destinationSquarePiece = board[currentAbility.abilityDstIdx];
    // exclude useless abilities, e.g. warrior attacking empty square
    switch(piece->type) {
      // king can only use abilities on enemy pieces
      case P1_KING:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            continue;
          case P1_MAGE:
            continue;
          case P1_PAWN:
            continue;
          case P1_WARRIOR:
            continue;
          case P1_ASSASSIN:
            continue;
          case P2_KING:
            break;
          case P2_MAGE:
            break;
          case P2_PAWN:
            break;
          case P2_WARRIOR:
            break;
          case P2_ASSASSIN:
            break;
          case NO_PIECE:
            continue;
          default:
            break;
        }
      // mage can only use abilities on enemy pieces
      case P1_MAGE:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            continue;
          case P1_MAGE:
            continue;
          case P1_PAWN:
            continue;
          case P1_WARRIOR:
            continue;
          case P1_ASSASSIN:
            continue;
          case P2_KING:
            break;
          case P2_MAGE:
            break;
          case P2_PAWN:
            break;
          case P2_WARRIOR:
            break;
          case P2_ASSASSIN:
            break;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      // pawn can only use abilities on enemy pieces
      case P1_PAWN:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            continue;
          case P1_MAGE:
            continue;
          case P1_PAWN:
            continue;
          case P1_WARRIOR:
            continue;
          case P1_ASSASSIN:
            continue;
          case P2_KING:
            break;
          case P2_MAGE:
            break;
          case P2_PAWN:
            break;
          case P2_WARRIOR:
            break;
          case P2_ASSASSIN:
            break;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      // warrior can only use abilities on enemy pieces
      case P1_WARRIOR:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            continue;
          case P1_MAGE:
            continue;
          case P1_PAWN:
            continue;
          case P1_WARRIOR:
            continue;
          case P1_ASSASSIN:
            continue;
          case P2_KING:
            break;
          case P2_MAGE:
            break;
          case P2_PAWN:
            break;
          case P2_WARRIOR:
            break;
          case P2_ASSASSIN:
            break;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      // assassin can only use abilities on enemy pieces
      case P1_ASSASSIN:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            continue;
          case P1_MAGE:
            continue;
          case P1_PAWN:
            continue;
          case P1_WARRIOR:
            continue;
          case P1_ASSASSIN:
            continue;
          case P2_KING:
            break;
          case P2_MAGE:
            break;
          case P2_PAWN:
            break;
          case P2_WARRIOR:
            break;
          case P2_ASSASSIN:
            break;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;

      // king can only use abilities on enemy pieces
      case P2_KING:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            break;
          case P1_MAGE:
            break;
          case P1_PAWN:
            break;
          case P1_WARRIOR:
            break;
          case P1_ASSASSIN:
            break;
          case P2_KING:
            continue;
          case P2_MAGE:
            continue;
          case P2_PAWN:
            continue;
          case P2_WARRIOR:
            continue;
          case P2_ASSASSIN:
            continue;
          case NO_PIECE:
            continue;
          default:
            break;
        }
      // mage can only use abilities on enemy pieces
      case P2_MAGE:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            break;
          case P1_MAGE:
            break;
          case P1_PAWN:
            break;
          case P1_WARRIOR:
            break;
          case P1_ASSASSIN:
            break;
          case P2_KING:
            continue;
          case P2_MAGE:
            continue;
          case P2_PAWN:
            continue;
          case P2_WARRIOR:
            continue;
          case P2_ASSASSIN:
            continue;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      // pawn can only use abilities on enemy pieces
      case P2_PAWN:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            break;
          case P1_MAGE:
            break;
          case P1_PAWN:
            break;
          case P1_WARRIOR:
            break;
          case P1_ASSASSIN:
            break;
          case P2_KING:
            continue;
          case P2_MAGE:
            continue;
          case P2_PAWN:
            continue;
          case P2_WARRIOR:
            continue;
          case P2_ASSASSIN:
            continue;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      // warrior can only use abilities on enemy pieces
      case P2_WARRIOR:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            break;
          case P1_MAGE:
            break;
          case P1_PAWN:
            break;
          case P1_WARRIOR:
            break;
          case P1_ASSASSIN:
            break;
          case P2_KING:
            continue;
          case P2_MAGE:
            continue;
          case P2_PAWN:
            continue;
          case P2_WARRIOR:
            continue;
          case P2_ASSASSIN:
            continue;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      // assassin can only use abilities on enemy pieces
      case P2_ASSASSIN:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            break;
          case P1_MAGE:
            break;
          case P1_PAWN:
            break;
          case P1_WARRIOR:
            break;
          case P1_ASSASSIN:
            break;
          case P2_KING:
            continue;
          case P2_MAGE:
            continue;
          case P2_PAWN:
            continue;
          case P2_WARRIOR:
            continue;
          case P2_ASSASSIN:
            continue;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      case NO_PIECE:
        break;
      default:
        break;
    }
    retval.push_back(currentAbility);
  }
  return retval;
}

/*
 * Assumes that the game is not over.
 */
std::vector<PlayerAbility> Game::allLegalAbilitiesByPiece(int srcSquareIdx) {
  std::vector<PlayerAbility> retval;
  Piece* piece = board[srcSquareIdx];
  if((!pieceBelongsToPlayer(piece->type, currentPlayer)) ||
      piece->healthPoints <= 0) {
    return retval;
  }
  auto legalAbilitiesOnAnEmptyBoard = gameCache->pieceTypeToSquareIndexToLegalAbilities[piece->type][piece->squareIndex];

  for(PlayerAbility pa: legalAbilitiesOnAnEmptyBoard) {
    Piece* abilityDstPiece = board[pa.abilityDstIdx];
    if(pieceBelongsToPlayer(abilityDstPiece->type, currentPlayer)) continue;
    retval.push_back(pa);
  }
  return retval;
}

/*
 * Useful actions are those whose abilities change the game state.
 * For example, warrior attacking an empty square is legal but doesn't change the game state.
 */
std::vector<PlayerAction> Game::usefulLegalActions() {
  std::vector<PlayerAction> retval;
  // If King is dead, game is over and there are no legal actions
  if(playerToPieces[currentPlayer][KING_PIECE_INDEX]->healthPoints <= 0) {
    return retval;
  }
  for(int i = 0; i < NUM_STARTING_PIECES; i++) {
    Piece* currentPiece = playerToPieces[currentPlayer][i];
    if(currentPiece->healthPoints <= 0) continue; // dead pieces don't move

    auto legalMoves = gameCache->pieceTypeToSquareIndexToLegalMoves[currentPiece->type][currentPiece->squareIndex];
    for(int j = 0; j < legalMoves.size(); j++) {
      PlayerMove currentMove = legalMoves[j];
      // Is p1 pawn trying to jump over another piece?
      if(currentPiece->type == P1_PAWN &&
          currentPiece->squareIndex - currentMove.moveDstIdx == -2 * NUM_COLUMNS 
          ) {
        // checks whether square in front of the p1 pawn is empty
        if(board[currentPiece->squareIndex + NUM_COLUMNS]->type != NO_PIECE) continue;
      }
      // Is p2 pawn trying to jump over another piece?
      if(currentPiece->type == P2_PAWN &&
          currentPiece->squareIndex - currentMove.moveDstIdx == 2 * NUM_COLUMNS 
          ) {
        // checks whether square in front of the p2 pawn is empty
        if(board[currentPiece->squareIndex - NUM_COLUMNS]->type != NO_PIECE) continue;
      }

      if(board[currentMove.moveDstIdx]->type != NO_PIECE) continue;
      makeMove(currentMove.moveSrcIdx, currentMove.moveDstIdx);
      for(int k = 0; k < NUM_STARTING_PIECES; k++) {
        Piece* cp2 = playerToPieces[currentPlayer][k];
        if(cp2->healthPoints <= 0) continue; // no abilities for dead pieces
        auto legalAbilities = gameCache->pieceTypeToSquareIndexToLegalAbilities[cp2->type][cp2->squareIndex];
        for(int l = 0; l < legalAbilities.size(); l++) {
          PlayerAbility currentAbility = legalAbilities[l];
          Piece* destinationSquarePiece = board[currentAbility.abilityDstIdx];
          // exclude useless abilities, e.g. warrior attacking empty square
          switch(cp2->type) {
            // king can only use abilities on enemy pieces
            case P1_KING:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  continue;
                case P1_MAGE:
                  continue;
                case P1_PAWN:
                  continue;
                case P1_WARRIOR:
                  continue;
                case P1_ASSASSIN:
                  continue;
                case P2_KING:
                  break;
                case P2_MAGE:
                  break;
                case P2_PAWN:
                  break;
                case P2_WARRIOR:
                  break;
                case P2_ASSASSIN:
                  break;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
            // mage can only use abilities on enemy pieces
            case P1_MAGE:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  continue;
                case P1_MAGE:
                  continue;
                case P1_PAWN:
                  continue;
                case P1_WARRIOR:
                  continue;
                case P1_ASSASSIN:
                  continue;
                case P2_KING:
                  break;
                case P2_MAGE:
                  break;
                case P2_PAWN:
                  break;
                case P2_WARRIOR:
                  break;
                case P2_ASSASSIN:
                  break;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            // pawn can only use abilities on enemy pieces
            case P1_PAWN:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  continue;
                case P1_MAGE:
                  continue;
                case P1_PAWN:
                  continue;
                case P1_WARRIOR:
                  continue;
                case P1_ASSASSIN:
                  continue;
                case P2_KING:
                  break;
                case P2_MAGE:
                  break;
                case P2_PAWN:
                  break;
                case P2_WARRIOR:
                  break;
                case P2_ASSASSIN:
                  break;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            // warrior can only use abilities on enemy pieces
            case P1_WARRIOR:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  continue;
                case P1_MAGE:
                  continue;
                case P1_PAWN:
                  continue;
                case P1_WARRIOR:
                  continue;
                case P1_ASSASSIN:
                  continue;
                case P2_KING:
                  break;
                case P2_MAGE:
                  break;
                case P2_PAWN:
                  break;
                case P2_WARRIOR:
                  break;
                case P2_ASSASSIN:
                  break;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            // assassin can only use abilities on enemy pieces
            case P1_ASSASSIN:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  continue;
                case P1_MAGE:
                  continue;
                case P1_PAWN:
                  continue;
                case P1_WARRIOR:
                  continue;
                case P1_ASSASSIN:
                  continue;
                case P2_KING:
                  break;
                case P2_MAGE:
                  break;
                case P2_PAWN:
                  break;
                case P2_WARRIOR:
                  break;
                case P2_ASSASSIN:
                  break;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;

            // king can only use abilities on enemy pieces
            case P2_KING:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  break;
                case P1_MAGE:
                  break;
                case P1_PAWN:
                  break;
                case P1_WARRIOR:
                  break;
                case P1_ASSASSIN:
                  break;
                case P2_KING:
                  continue;
                case P2_MAGE:
                  continue;
                case P2_PAWN:
                  continue;
                case P2_WARRIOR:
                  continue;
                case P2_ASSASSIN:
                  continue;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
            // mage can only use abilities on enemy pieces
            case P2_MAGE:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  break;
                case P1_MAGE:
                  break;
                case P1_PAWN:
                  break;
                case P1_WARRIOR:
                  break;
                case P1_ASSASSIN:
                  break;
                case P2_KING:
                  continue;
                case P2_MAGE:
                  continue;
                case P2_PAWN:
                  continue;
                case P2_WARRIOR:
                  continue;
                case P2_ASSASSIN:
                  continue;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            // pawn can only use abilities on enemy pieces
            case P2_PAWN:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  break;
                case P1_MAGE:
                  break;
                case P1_PAWN:
                  break;
                case P1_WARRIOR:
                  break;
                case P1_ASSASSIN:
                  break;
                case P2_KING:
                  continue;
                case P2_MAGE:
                  continue;
                case P2_PAWN:
                  continue;
                case P2_WARRIOR:
                  continue;
                case P2_ASSASSIN:
                  continue;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            // warrior can only use abilities on enemy pieces
            case P2_WARRIOR:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  break;
                case P1_MAGE:
                  break;
                case P1_PAWN:
                  break;
                case P1_WARRIOR:
                  break;
                case P1_ASSASSIN:
                  break;
                case P2_KING:
                  continue;
                case P2_MAGE:
                  continue;
                case P2_PAWN:
                  continue;
                case P2_WARRIOR:
                  continue;
                case P2_ASSASSIN:
                  continue;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            // assassin can only use abilities on enemy pieces
            case P2_ASSASSIN:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  break;
                case P1_MAGE:
                  break;
                case P1_PAWN:
                  break;
                case P1_WARRIOR:
                  break;
                case P1_ASSASSIN:
                  break;
                case P2_KING:
                  continue;
                case P2_MAGE:
                  continue;
                case P2_PAWN:
                  continue;
                case P2_WARRIOR:
                  continue;
                case P2_ASSASSIN:
                  continue;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            case NO_PIECE:
              break;
            default:
              break;
          }
          PlayerAction p = PlayerAction(currentMove.moveSrcIdx, currentMove.moveDstIdx, currentAbility.abilitySrcIdx, currentAbility.abilityDstIdx);
          retval.push_back(p);
        }
      }
      // player can skip the ability
      PlayerAction p = PlayerAction(currentMove.moveSrcIdx, currentMove.moveDstIdx, ABILITY_SKIP, ABILITY_SKIP);
      retval.push_back(p);

      undoMove(currentMove.moveSrcIdx, currentMove.moveDstIdx);
    }
  }
  // player can skip the move
  for(int k = 0; k < NUM_STARTING_PIECES; k++) {
    Piece* cp2 = playerToPieces[currentPlayer][k];
    if(cp2->healthPoints <= 0) continue; // no abilities for dead pieces
    auto legalAbilities = gameCache->pieceTypeToSquareIndexToLegalAbilities[cp2->type][cp2->squareIndex];
    for(int l = 0; l < legalAbilities.size(); l++) {
      Piece* destinationSquarePiece = board[legalAbilities[l].abilityDstIdx];
      // exclude useless abilities
      switch(cp2->type) {
        // king can only use abilities on enemy pieces
        case P1_KING:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              continue;
            case P1_MAGE:
              continue;
            case P1_PAWN:
              continue;
            case P1_WARRIOR:
              continue;
            case P1_ASSASSIN:
              continue;
            case P2_KING:
              break;
            case P2_MAGE:
              break;
            case P2_PAWN:
              break;
            case P2_WARRIOR:
              break;
            case P2_ASSASSIN:
              break;
            case NO_PIECE:
              continue;
            default:
              break;
          }
        // mage can only use abilities on enemy pieces
        case P1_MAGE:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              continue;
            case P1_MAGE:
              continue;
            case P1_PAWN:
              continue;
            case P1_WARRIOR:
              continue;
            case P1_ASSASSIN:
              continue;
            case P2_KING:
              break;
            case P2_MAGE:
              break;
            case P2_PAWN:
              break;
            case P2_WARRIOR:
              break;
            case P2_ASSASSIN:
              break;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        // pawn can only use abilities on enemy pieces
        case P1_PAWN:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              continue;
            case P1_MAGE:
              continue;
            case P1_PAWN:
              continue;
            case P1_WARRIOR:
              continue;
            case P1_ASSASSIN:
              continue;
            case P2_KING:
              break;
            case P2_MAGE:
              break;
            case P2_PAWN:
              break;
            case P2_WARRIOR:
              break;
            case P2_ASSASSIN:
              break;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        // warrior can only use abilities on enemy pieces
        case P1_WARRIOR:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              continue;
            case P1_MAGE:
              continue;
            case P1_PAWN:
              continue;
            case P1_WARRIOR:
              continue;
            case P1_ASSASSIN:
              continue;
            case P2_KING:
              break;
            case P2_MAGE:
              break;
            case P2_PAWN:
              break;
            case P2_WARRIOR:
              break;
            case P2_ASSASSIN:
              break;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        // assassin can only use abilities on enemy pieces
        case P1_ASSASSIN:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              continue;
            case P1_MAGE:
              continue;
            case P1_PAWN:
              continue;
            case P1_WARRIOR:
              continue;
            case P1_ASSASSIN:
              continue;
            case P2_KING:
              break;
            case P2_MAGE:
              break;
            case P2_PAWN:
              break;
            case P2_WARRIOR:
              break;
            case P2_ASSASSIN:
              break;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;

        // king can only use abilities on enemy pieces
        case P2_KING:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              break;
            case P1_MAGE:
              break;
            case P1_PAWN:
              break;
            case P1_WARRIOR:
              break;
            case P1_ASSASSIN:
              break;
            case P2_KING:
              continue;
            case P2_MAGE:
              continue;
            case P2_PAWN:
              continue;
            case P2_WARRIOR:
              continue;
            case P2_ASSASSIN:
              continue;
            case NO_PIECE:
              continue;
            default:
              break;
          }
        // mage can only use abilities on enemy pieces
        case P2_MAGE:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              break;
            case P1_MAGE:
              break;
            case P1_PAWN:
              break;
            case P1_WARRIOR:
              break;
            case P1_ASSASSIN:
              break;
            case P2_KING:
              continue;
            case P2_MAGE:
              continue;
            case P2_PAWN:
              continue;
            case P2_WARRIOR:
              continue;
            case P2_ASSASSIN:
              continue;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        // pawn can only use abilities on enemy pieces
        case P2_PAWN:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              break;
            case P1_MAGE:
              break;
            case P1_PAWN:
              break;
            case P1_WARRIOR:
              break;
            case P1_ASSASSIN:
              break;
            case P2_KING:
              continue;
            case P2_MAGE:
              continue;
            case P2_PAWN:
              continue;
            case P2_WARRIOR:
              continue;
            case P2_ASSASSIN:
              continue;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        // warrior can only use abilities on enemy pieces
        case P2_WARRIOR:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              break;
            case P1_MAGE:
              break;
            case P1_PAWN:
              break;
            case P1_WARRIOR:
              break;
            case P1_ASSASSIN:
              break;
            case P2_KING:
              continue;
            case P2_MAGE:
              continue;
            case P2_PAWN:
              continue;
            case P2_WARRIOR:
              continue;
            case P2_ASSASSIN:
              continue;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        // assassin can only use abilities on enemy pieces
        case P2_ASSASSIN:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              break;
            case P1_MAGE:
              break;
            case P1_PAWN:
              break;
            case P1_WARRIOR:
              break;
            case P1_ASSASSIN:
              break;
            case P2_KING:
              continue;
            case P2_MAGE:
              continue;
            case P2_PAWN:
              continue;
            case P2_WARRIOR:
              continue;
            case P2_ASSASSIN:
              continue;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        case NO_PIECE:
          break;
        default:
          break;
      }

      PlayerAction p = PlayerAction(MOVE_SKIP, MOVE_SKIP, legalAbilities[l].abilitySrcIdx, legalAbilities[l].abilityDstIdx);
      retval.push_back(p);
    }
  }
  // player can skip both move and ability
  PlayerAction p = PlayerAction(MOVE_SKIP, MOVE_SKIP, ABILITY_SKIP, ABILITY_SKIP);
  retval.push_back(p);
  return retval;
}

/*
 * Includes actions with useless abilities (i.e. those that don't alter the game state)
 */
std::vector<PlayerAction> Game::allLegalActions() {
  std::vector<PlayerAction> retval;
  // If King is dead, game is over and there are no legal actions
  if(playerToPieces[currentPlayer][KING_PIECE_INDEX]->healthPoints <= 0) {
    return retval;
  }
  for(int i = 0; i < NUM_STARTING_PIECES; i++) {
    Piece* currentPiece = playerToPieces[currentPlayer][i];
    if(currentPiece->healthPoints <= 0) continue; // dead pieces don't move

    auto legalMoves = gameCache->pieceTypeToSquareIndexToLegalMoves[currentPiece->type][currentPiece->squareIndex];
    for(int j = 0; j < legalMoves.size(); j++) {
      PlayerMove currentMove = legalMoves[j];
      // Is p1 pawn trying to jump over another piece?
      if(currentPiece->type == P1_PAWN &&
          currentPiece->squareIndex - currentMove.moveDstIdx == -2 * NUM_COLUMNS 
          ) {
        // checks whether square in front of the p1 pawn is empty
        if(board[currentPiece->squareIndex + NUM_COLUMNS]->type != NO_PIECE) continue;
      }
      // Is p2 pawn trying to jump over another piece?
      if(currentPiece->type == P2_PAWN &&
          currentPiece->squareIndex - currentMove.moveDstIdx == 2 * NUM_COLUMNS 
          ) {
        // checks whether square in front of the p2 pawn is empty
        if(board[currentPiece->squareIndex - NUM_COLUMNS]->type != NO_PIECE) continue;
      }

      if(board[currentMove.moveDstIdx]->type != NO_PIECE) continue;
      makeMove(currentMove.moveSrcIdx, currentMove.moveDstIdx);
      for(int k = 0; k < NUM_STARTING_PIECES; k++) {
        Piece* cp2 = playerToPieces[currentPlayer][k];
        if(cp2->healthPoints <= 0) continue; // no abilities for dead pieces
        auto legalAbilities = gameCache->pieceTypeToSquareIndexToLegalAbilities[cp2->type][cp2->squareIndex];
        for(int l = 0; l < legalAbilities.size(); l++) {
          PlayerAbility currentAbility = legalAbilities[l];
          Piece* destinationSquarePiece = board[currentAbility.abilityDstIdx];
          if(pieceBelongsToPlayer(destinationSquarePiece->type, this->currentPlayer)) continue;

          PlayerAction p = PlayerAction(currentMove.moveSrcIdx, currentMove.moveDstIdx, currentAbility.abilitySrcIdx, currentAbility.abilityDstIdx);
          retval.push_back(p);
        }
      }
      // player can skip the ability
      PlayerAction p = PlayerAction(currentMove.moveSrcIdx, currentMove.moveDstIdx, ABILITY_SKIP, ABILITY_SKIP);
      retval.push_back(p);

      undoMove(currentMove.moveSrcIdx, currentMove.moveDstIdx);
    }
  }
  // player can skip the move
  for(int k = 0; k < NUM_STARTING_PIECES; k++) {
    Piece* cp2 = playerToPieces[currentPlayer][k];
    if(cp2->healthPoints <= 0) continue; // no abilities for dead pieces
    auto legalAbilities = gameCache->pieceTypeToSquareIndexToLegalAbilities[cp2->type][cp2->squareIndex];
    for(int l = 0; l < legalAbilities.size(); l++) {
      Piece* destinationSquarePiece = board[legalAbilities[l].abilityDstIdx];
      if(pieceBelongsToPlayer(destinationSquarePiece->type, this->currentPlayer)) continue;
      PlayerAction p = PlayerAction(MOVE_SKIP, MOVE_SKIP, legalAbilities[l].abilitySrcIdx, legalAbilities[l].abilityDstIdx);
      retval.push_back(p);
    }
  }
  // player can skip both move and ability
  PlayerAction p = PlayerAction(MOVE_SKIP, MOVE_SKIP, ABILITY_SKIP, ABILITY_SKIP);
  retval.push_back(p);
  return retval;
}

/*
 * Checks whether values are in the right range.
 */
bool isActionValid(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx) {
  bool moveValid = false;
  bool abilityValid = false;
  if(moveSrcIdx == MOVE_SKIP && moveDstIdx == MOVE_SKIP) {
    moveValid = true;
  } else if((!isOffBoard(moveSrcIdx)) && (!isOffBoard(moveDstIdx))) {
    moveValid = true;
  }
  if(abilitySrcIdx == ABILITY_SKIP && abilityDstIdx == ABILITY_SKIP) {
    abilityValid = true;
  } else if((!isOffBoard(abilitySrcIdx)) && (!isOffBoard(abilityDstIdx))) {
    abilityValid = true;
  }
  return moveValid && abilityValid;
}

bool Game::isActionLegal(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx) {
  // It's important for this method to not have many exit points because it's altering the game
  // state. If a return statement is between makeMove and undoMove, game state will remain changed
  // which shouldn't happen in a method that checks action legality.
  bool validInput = isActionValid(moveSrcIdx, moveDstIdx, abilitySrcIdx, abilityDstIdx);
  if(!validInput) return false;

  bool moveLegal = false;
  bool abilityLegal = false;
  bool movePieceBelongsToCurrentPlayerOrMoveSkip = false;
  bool abilityPieceBelongsToCurrentPlayerOrAbilitySkip = false;
  bool movePieceIsAliveOrMoveSkip = false;
  bool abilityPieceIsAliveOrAbilitySkip = false;
  bool abilityDstPieceBelongsToCurrentPlayer = false; // are you trying to attack your own piece?
  bool currentPlayersKingIsAlive = false;
  Piece* movePiece;
  Piece* abilityPiece;
  Piece* abilityDstPiece;

  if(moveSrcIdx == MOVE_SKIP && moveDstIdx == MOVE_SKIP) {
    moveLegal = true;
    movePieceBelongsToCurrentPlayerOrMoveSkip = true;
    movePieceIsAliveOrMoveSkip = true;
  } else {
    movePiece = board[moveSrcIdx];
    if(pieceBelongsToPlayer(movePiece->type, currentPlayer)) {
      movePieceBelongsToCurrentPlayerOrMoveSkip = true;
    }
    if(movePiece->healthPoints > 0) {
      movePieceIsAliveOrMoveSkip = true;
    }
    auto legalMovesOnEmptyBoard = gameCache->pieceTypeToSquareIndexToLegalMoves[movePiece->type][movePiece->squareIndex];
    for(int i = 0; i < legalMovesOnEmptyBoard.size(); i++) {
      PlayerMove currentMove = legalMovesOnEmptyBoard[i];
      // Is p1 pawn trying to jump over another piece?
      if(movePiece->type == P1_PAWN &&
          movePiece->squareIndex - currentMove.moveDstIdx == -2 * NUM_COLUMNS 
          ) {
        // checks whether square in front of the p1 pawn is empty
        if(board[movePiece->squareIndex + NUM_COLUMNS]->type != NO_PIECE) continue;
      }

      // Is p2 pawn trying to jump over another piece?
      if(movePiece->type == P2_PAWN &&
          movePiece->squareIndex - currentMove.moveDstIdx == 2 * NUM_COLUMNS 
          ) {
        // checks whether square in front of the p2 pawn is empty
        if(board[movePiece->squareIndex - NUM_COLUMNS]->type != NO_PIECE) continue;
      }

      if(board[currentMove.moveDstIdx]->type == NO_PIECE && 
          currentMove.moveDstIdx == moveDstIdx) {
        moveLegal = true;
        makeMove(moveSrcIdx, moveDstIdx);
        break;
      }
    }
  }
  if(abilitySrcIdx == ABILITY_SKIP && abilityDstIdx == ABILITY_SKIP) {
    abilityLegal = true;
    abilityPieceBelongsToCurrentPlayerOrAbilitySkip = true;
    abilityPieceIsAliveOrAbilitySkip = true;
  } else {
    abilityPiece = board[abilitySrcIdx];
    abilityDstPiece = board[abilityDstIdx];
    if(pieceBelongsToPlayer(abilityPiece->type, currentPlayer)) {
      abilityPieceBelongsToCurrentPlayerOrAbilitySkip = true;
    }
    if(abilityPiece->healthPoints > 0) {
      abilityPieceIsAliveOrAbilitySkip = true;
    }
    if(pieceBelongsToPlayer(abilityDstPiece->type, this->currentPlayer)) {
      abilityDstPieceBelongsToCurrentPlayer = true;
    }
    auto legalAbilitiesOnEmptyBoard = gameCache->pieceTypeToSquareIndexToLegalAbilities[abilityPiece->type][abilityPiece->squareIndex];
    for(int i = 0; i < legalAbilitiesOnEmptyBoard.size(); i++) {
      PlayerAbility currentAbility = legalAbilitiesOnEmptyBoard[i];
      if(currentAbility.abilityDstIdx == abilityDstIdx) {
        abilityLegal = true;
        break;
      }
    }
  }

  if(moveSrcIdx != MOVE_SKIP && moveLegal) {
    undoMove(moveSrcIdx, moveDstIdx);
  }

  currentPlayersKingIsAlive = playerToPieces[currentPlayer][KING_PIECE_INDEX]->healthPoints > 0;
  
  if(moveLegal && abilityLegal && movePieceBelongsToCurrentPlayerOrMoveSkip &&
      abilityPieceBelongsToCurrentPlayerOrAbilitySkip && movePieceIsAliveOrMoveSkip &&
      abilityPieceIsAliveOrAbilitySkip && currentPlayersKingIsAlive &&
      !(abilityDstPieceBelongsToCurrentPlayer)) {
    return true;
  } else {
    return false;
  }
}

Player Game::getCurrentPlayer() {
  return currentPlayer;
}

Piece Game::getPieceByCoordinates(int x, int y) {
  return *board[coordinatesToBoardIndex(x, y)]; 
}

Piece Game::getPieceBySquareIndex(int squareIndex) {
  return *board[squareIndex]; 
}

bool Game::gameOver() {
  Piece* p1King = playerToPieces[PLAYER_1][KING_PIECE_INDEX];
  Piece* p2King = playerToPieces[PLAYER_2][KING_PIECE_INDEX];
  if(p1King->healthPoints <= 0 || p2King->healthPoints <= 0) {
    return true;
  } else {
    return false;
  }
}

std::optional<Player> Game::winner() {
  Piece* p1King = playerToPieces[PLAYER_1][KING_PIECE_INDEX];
  Piece* p2King = playerToPieces[PLAYER_2][KING_PIECE_INDEX];
  if(p1King->healthPoints <= 0) {
    return PLAYER_2;
  } else if(p2King->healthPoints <= 0) {
    return PLAYER_1;
  }
  return std::nullopt;
}

std::string Game::boardToString() {
  std::stringstream retval;
  retval << currentPlayer << "|";
  Piece* currentPiece;
  for(int i = 0; i < NUM_SQUARES; i++) {
    currentPiece = board[i];
    if(currentPiece->type == NO_PIECE) {
      retval << "empty,";
    } else {
      switch(currentPiece->type) {
        case P1_KING:
          retval << "0-king-";
          break;
        case P1_MAGE:
          retval << "0-mage-";
          break;
        case P1_PAWN:
          retval << "0-pawn-";
          break;
        case P1_WARRIOR:
          retval << "0-warrior-";
          break;
        case P1_ASSASSIN:
          retval << "0-assassin-";
          break;
        case P2_KING:
          retval << "1-king-";
          break;
        case P2_MAGE:
          retval << "1-mage-";
          break;
        case P2_PAWN:
          retval << "1-pawn-";
          break;
        case P2_WARRIOR:
          retval << "1-warrior-";
          break;
        case P2_ASSASSIN:
          retval << "1-assassin-";
          break;
      }        
      retval << currentPiece->healthPoints << ",";
    }
  }
  return retval.str();
}

void Game::boardFromString(std::string encodedBoard) {
  currentPlayer = (Player)(std::stoi(encodedBoard.substr(0, encodedBoard.find("|"))));
  moveNumber = 0;
  // pieces need to exist in the piece array even if they're dead
  // first all pieces are initialized as dead, then they're replaced if found in the encodedBoard
  std::vector<Piece*> p1Pieces(NUM_STARTING_PIECES);
  p1Pieces[KING_PIECE_INDEX] = new Piece(PieceType::P1_KING, 0, 0);
  p1Pieces[PAWN_1_PIECE_INDEX] = new Piece(PieceType::P1_PAWN, 0, 0);
  p1Pieces[PAWN_2_PIECE_INDEX] = new Piece(PieceType::P1_PAWN, 0, 0);
  p1Pieces[PAWN_3_PIECE_INDEX] = new Piece(PieceType::P1_PAWN, 0, 0);
  p1Pieces[ASSASSIN_PIECE_INDEX] = new Piece(PieceType::P1_ASSASSIN, 0, 0);
  p1Pieces[MAGE_PIECE_INDEX] = new Piece(PieceType::P1_MAGE, 0, 0);
  p1Pieces[WARRIOR_PIECE_INDEX] = new Piece(PieceType::P1_WARRIOR, 0, 0);

  p1King = p1Pieces[KING_PIECE_INDEX];

  std::vector<Piece*> p2Pieces(NUM_STARTING_PIECES);
  p2Pieces[KING_PIECE_INDEX] = new Piece(PieceType::P2_KING, 0, 0);
  p2Pieces[PAWN_1_PIECE_INDEX] = new Piece(PieceType::P2_PAWN, 0, 0);
  p2Pieces[PAWN_2_PIECE_INDEX] = new Piece(PieceType::P2_PAWN, 0, 0);
  p2Pieces[PAWN_3_PIECE_INDEX] = new Piece(PieceType::P2_PAWN, 0, 0);
  p2Pieces[ASSASSIN_PIECE_INDEX] = new Piece(PieceType::P2_ASSASSIN, 0, 0);
  p2Pieces[MAGE_PIECE_INDEX] = new Piece(PieceType::P2_MAGE, 0, 0);
  p2Pieces[WARRIOR_PIECE_INDEX] = new Piece(PieceType::P2_WARRIOR, 0, 0);

  p2King = p2Pieces[KING_PIECE_INDEX];

  std::string b1 = encodedBoard.substr(2);
  std::string delimiter1 = ",";
  std::string delimiter2 = "-";
  std::string token1;
  std::string s, tmp;
  size_t pos = 0;
  int boardIdx = 0;
  while((pos = b1.find(delimiter1)) != std::string::npos) {
    token1 = b1.substr(0, pos);
    if(token1 == "empty") {
      board[boardIdx] = new Piece(PieceType::NO_PIECE, 0, boardIdx);
    } else {
      std::stringstream ss(token1);
      std::vector<std::string> words;
      while(std::getline(ss, tmp, '-')) {
        words.push_back(tmp);
      }
      int healthPoints = std::stoi(words[2]);
      s = words[0] + words[1];
      if(s == "0king") {
        board[boardIdx] = new Piece(PieceType::P1_KING, healthPoints, boardIdx);
        p1Pieces[KING_PIECE_INDEX] = board[boardIdx];
        p1King = board[boardIdx];
      } else if(s == "0pawn") {
        board[boardIdx] = new Piece(PieceType::P1_PAWN, healthPoints, boardIdx);
        if(p1Pieces[PAWN_1_PIECE_INDEX]->healthPoints <= 0) {
          p1Pieces[PAWN_1_PIECE_INDEX] = board[boardIdx];
        } else if(p1Pieces[PAWN_2_PIECE_INDEX]->healthPoints <= 0) {
          p1Pieces[PAWN_2_PIECE_INDEX] = board[boardIdx];
        } else if(p1Pieces[PAWN_3_PIECE_INDEX]->healthPoints <= 0) {
          p1Pieces[PAWN_3_PIECE_INDEX] = board[boardIdx];
        } else {
          throw "Already found 3 living PLAYER_1 Pawns";
        }
      } else if(s == "0mage") {
        board[boardIdx] = new Piece(PieceType::P1_MAGE, healthPoints, boardIdx);
        p1Pieces[MAGE_PIECE_INDEX] = board[boardIdx];
      } else if(s == "0assassin") {
        board[boardIdx] = new Piece(PieceType::P1_ASSASSIN, healthPoints, boardIdx);
        p1Pieces[ASSASSIN_PIECE_INDEX] = board[boardIdx];
      } else if(s == "0warrior") {
        board[boardIdx] = new Piece(PieceType::P1_WARRIOR, healthPoints, boardIdx);
        p1Pieces[WARRIOR_PIECE_INDEX] = board[boardIdx];
      } else if(s == "1king") {
        board[boardIdx] = new Piece(PieceType::P2_KING, healthPoints, boardIdx);
        p2Pieces[KING_PIECE_INDEX] = board[boardIdx];
        p2King = board[boardIdx];
      } else if(s == "1pawn") {
        board[boardIdx] = new Piece(PieceType::P2_PAWN, healthPoints, boardIdx);
        if(p2Pieces[PAWN_1_PIECE_INDEX]->healthPoints <= 0) {
          p2Pieces[PAWN_1_PIECE_INDEX] = board[boardIdx];
        } else if(p2Pieces[PAWN_2_PIECE_INDEX]->healthPoints <= 0) {
          p2Pieces[PAWN_2_PIECE_INDEX] = board[boardIdx];
        } else if(p2Pieces[PAWN_3_PIECE_INDEX]->healthPoints <= 0) {
          p2Pieces[PAWN_3_PIECE_INDEX] = board[boardIdx];
        } else {
          throw "Already found 3 living PLAYER_2 Pawns";
        }
      } else if(s == "1mage") {
        board[boardIdx] = new Piece(PieceType::P2_MAGE, healthPoints, boardIdx);
        p2Pieces[MAGE_PIECE_INDEX] = board[boardIdx];
      } else if(s == "1assassin") {
        board[boardIdx] = new Piece(PieceType::P2_ASSASSIN, healthPoints, boardIdx);
        p2Pieces[ASSASSIN_PIECE_INDEX] = board[boardIdx];
      } else if(s == "1warrior") {
        board[boardIdx] = new Piece(PieceType::P2_WARRIOR, healthPoints, boardIdx);
        p2Pieces[WARRIOR_PIECE_INDEX] = board[boardIdx];
      }
    }
    b1.erase(0, pos + delimiter1.length());
    boardIdx += 1;
  }

  playerToPieces[PLAYER_1] = p1Pieces;
  playerToPieces[PLAYER_2] = p2Pieces;
}

std::vector<Piece*> Game::getAllPiecesByPlayer(Player player) {
  return playerToPieces[player];
}

/* 
 * performance test - https://www.chessprogramming.org/Perft
 * with bulk counting
 */
unsigned long long nichess::perft(Game& game, int depth) {
  unsigned long long nodes = 0;
  std::vector<PlayerAction> legalActions = game.usefulLegalActions();
  int numLegalActions = legalActions.size();
  if(depth == 1) {
    return (unsigned long long) numLegalActions;
  }

  UndoInfo ui;
  PlayerAction pa;
  for(int i = 0; i < numLegalActions; i++) {
    PlayerAction pa = legalActions[i];
    ui = game.makeAction(pa.moveSrcIdx, pa.moveDstIdx, pa.abilitySrcIdx, pa.abilityDstIdx);
    nodes += perft(game, depth-1);
    game.undoAction(ui);
  }
  return nodes;
}
