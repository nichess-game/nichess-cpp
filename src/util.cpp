#include "nichess/util.hpp"

std::string playerToString(Player p) {
  switch(p) {
    case PLAYER_1:
      return "PLAYER_1";
    case PLAYER_2:
      return "PLAYER_2";
    default:
      return "default";
  }
}

std::string pieceTypeToString(PieceType pt) {
  switch(pt) {
    case P1_KING:
      return "K";
    case P1_MAGE:
      return "M";
    case P1_PAWN:
      return "P";
    case P1_WARRIOR:
      return "W";
    case P1_WALL:
      return "L";
    case P1_ASSASSIN:
      return "A";
    case P2_KING:
      return "k";
    case P2_MAGE:
      return "m";
    case P2_PAWN:
      return "p";
    case P2_WARRIOR:
      return "w";
    case P2_WALL:
      return "l";
    case P2_ASSASSIN:
      return "a";
    case NO_PIECE:
      return ".";
    default:
      return "default";
  }
}

bool player1OrEmpty(PieceType pt) {
  switch(pt) {
    case P1_KING:
      return true;
    case P1_MAGE:
      return true;
    case P1_PAWN:
      return true;
    case P1_WARRIOR:
      return true;
    case P1_WALL:
      return true;
    case P1_ASSASSIN:
      return true;
    case P2_KING:
      return false;
    case P2_MAGE:
      return false;
    case P2_PAWN:
      return false;
    case P2_WARRIOR:
      return false;
    case P2_WALL:
      return false;
    case P2_ASSASSIN:
      return false;
    case NO_PIECE:
      return true;
  }
  return false;
}

bool player2OrEmpty(PieceType pt) {
  switch(pt) {
    case P1_KING:
      return false;
    case P1_MAGE:
      return false;
    case P1_PAWN:
      return false;
    case P1_WARRIOR:
      return false;
    case P1_WALL:
      return false;
    case P1_ASSASSIN:
      return false;
    case P2_KING:
      return true;
    case P2_MAGE:
      return true;
    case P2_PAWN:
      return true;
    case P2_WARRIOR:
      return true;
    case P2_WALL:
      return true;
    case P2_ASSASSIN:
      return true;
    case NO_PIECE:
      return true;
  }
  return false;
}

bool pieceBelongsToPlayer(PieceType pt, Player player) {
  switch(pt) {
    case P1_KING:
        return player == PLAYER_1;
      case P1_MAGE:
        return player == PLAYER_1;
      case P1_PAWN:
        return player == PLAYER_1;
      case P1_WARRIOR:
        return player == PLAYER_1;
      case P1_WALL:
        return player == PLAYER_1;
      case P1_ASSASSIN:
        return player == PLAYER_1;
      case P2_KING:
        return player == PLAYER_2;
      case P2_MAGE:
        return player == PLAYER_2;
      case P2_PAWN:
        return player == PLAYER_2;
      case P2_WARRIOR:
        return player == PLAYER_2;
      case P2_WALL:
        return player == PLAYER_2;
      case P2_ASSASSIN:
        return player == PLAYER_2;
      case NO_PIECE:
        return false;
    }
  return false;
}

bool isOffBoard(int x, int y) {
  if(x >= NUM_COLUMNS || x < 0 || y >= NUM_ROWS || y < 0)
    return true;
  else
    return false;
}

bool isOffBoard(int squareIndex) {
  if(squareIndex < 0 || squareIndex > NUM_ROWS * NUM_COLUMNS - 1)
    return true;
  else
    return false;
}

/*
 * For each Piece type, for each square, generates legal moves as if there were no other
 * pieces on the board. Elsewhere, occupied squares will be discarded from the legal moves.
 */
std::vector<std::vector<std::vector<PlayerMove>>> generateLegalMovesOnAnEmptyBoard() {
  std::vector<std::vector<std::vector<PlayerMove>>> pieceTypeToSquareToLegalMoves{NUM_PIECE_TYPE};

  // p1 king moves
  std::vector<std::vector<PlayerMove>> squareToP1KingMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP1KingMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_KING] = squareToP1KingMoves;

  // p1 mage moves
  std::vector<std::vector<PlayerMove>> squareToP1MageMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP1MageMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_MAGE] = squareToP1MageMoves;

  // p1 pawn moves
  std::vector<std::vector<PlayerMove>> squareToP1PawnMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      // pawn can also go 2 squares forward
      int move_dst_x = move_column;
      int move_dst_y = move_row + 2;
      if(!isOffBoard(move_dst_x, move_dst_y)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
        playerMoves.push_back(pm);
      }
      squareToP1PawnMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_PAWN] = squareToP1PawnMoves;

  // p1 warrior moves
  std::vector<std::vector<PlayerMove>> squareToP1WarriorMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP1WarriorMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_WARRIOR] = squareToP1WarriorMoves;

  // p1 wall moves
  // walls can't move
  std::vector<std::vector<PlayerMove>> squareToP1WallMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      squareToP1WallMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_WALL] = squareToP1WallMoves;

  // p1 assassin moves
  std::vector<std::vector<PlayerMove>> squareToP1AssassinMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -2; dx < 3; dx++) {
        for(int dy = -2; dy < 3; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      // 4 extra moves:
      int move_dst_x1 = move_column + 3;
      int move_dst_y1 = move_row + 3;
      int move_dst_x2 = move_column + 3;
      int move_dst_y2 = move_row - 3;
      int move_dst_x3 = move_column - 3;
      int move_dst_y3 = move_row + 3;
      int move_dst_x4 = move_column - 3;
      int move_dst_y4 = move_row - 3;
      if(!isOffBoard(move_dst_x1, move_dst_y1)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x1, move_dst_y1));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x2, move_dst_y2)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x2, move_dst_y2));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x3, move_dst_y3)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x3, move_dst_y3));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x4, move_dst_y4)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x4, move_dst_y4));
        playerMoves.push_back(pm);
      }

      squareToP1AssassinMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_ASSASSIN] = squareToP1AssassinMoves;

  // p2 king moves
  std::vector<std::vector<PlayerMove>> squareToP2KingMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP2KingMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_KING] = squareToP2KingMoves;

  // p2 mage moves
  std::vector<std::vector<PlayerMove>> squareToP2MageMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP2MageMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_MAGE] = squareToP2MageMoves;

  // p2 pawn moves
  std::vector<std::vector<PlayerMove>> squareToP2PawnMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      // pawn can also go 2 squares forward(for p2 this means -2 on y axis)
      int move_dst_x = move_column;
      int move_dst_y = move_row - 2;
      if(!isOffBoard(move_dst_x, move_dst_y)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
        playerMoves.push_back(pm);
      }
      squareToP2PawnMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_PAWN] = squareToP2PawnMoves;

  // p2 warrior moves
  std::vector<std::vector<PlayerMove>> squareToP2WarriorMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP2WarriorMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_WARRIOR] = squareToP2WarriorMoves;

  // p2 wall moves
  // walls can't move
  std::vector<std::vector<PlayerMove>> squareToP2WallMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      squareToP2WallMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_WALL] = squareToP2WallMoves;

  // p2 assassin moves
  std::vector<std::vector<PlayerMove>> squareToP2AssassinMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -2; dx < 3; dx++) {
        for(int dy = -2; dy < 3; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      // 4 extra moves:
      int move_dst_x1 = move_column + 3;
      int move_dst_y1 = move_row + 3;
      int move_dst_x2 = move_column + 3;
      int move_dst_y2 = move_row - 3;
      int move_dst_x3 = move_column - 3;
      int move_dst_y3 = move_row + 3;
      int move_dst_x4 = move_column - 3;
      int move_dst_y4 = move_row - 3;
      if(!isOffBoard(move_dst_x1, move_dst_y1)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x1, move_dst_y1));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x2, move_dst_y2)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x2, move_dst_y2));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x3, move_dst_y3)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x3, move_dst_y3));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x4, move_dst_y4)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x4, move_dst_y4));
        playerMoves.push_back(pm);
      }

      squareToP2AssassinMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_ASSASSIN] = squareToP2AssassinMoves;

  // NO_PIECE moves (shouldn't be used, added for completeness)
  std::vector<std::vector<PlayerMove>> squareToNoPieceMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      squareToNoPieceMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[NO_PIECE] = squareToNoPieceMoves;

  return pieceTypeToSquareToLegalMoves;
}

/*
 * For each Piece type, for each square, generates legal abilities as if there were no other
 * pieces on the board. Elsewhere, abilities will be filtered by the actual board position.
 */
std::vector<std::vector<std::vector<PlayerAbility>>> generateLegalAbilitiesOnAnEmptyBoard() {
  std::vector<std::vector<std::vector<PlayerAbility>>> pieceTypeToSquareToLegalAbilities{NUM_PIECE_TYPE};

  // p1 king abilities
  std::vector<std::vector<PlayerAbility>> squareToP1KingAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP1KingAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_KING] = squareToP1KingAbilities;

  // p1 mage abilities
  std::vector<std::vector<PlayerAbility>> squareToP1MageAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -2; dx < 3; dx++) {
        for(int dy = -2; dy < 3; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP1MageAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_MAGE] = squareToP1MageAbilities;

  // p1 pawn abilities
  std::vector<std::vector<PlayerAbility>> squareToP1PawnAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP1PawnAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_PAWN] = squareToP1PawnAbilities;

  // p1 warrior abilities
  std::vector<std::vector<PlayerAbility>> squareToP1WarriorAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP1WarriorAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_WARRIOR] = squareToP1WarriorAbilities;

  // p1 wall abilities
  // walls can't ability
  std::vector<std::vector<PlayerAbility>> squareToP1WallAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      squareToP1WallAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_WALL] = squareToP1WallAbilities;

  // p1 assassin abilities
  std::vector<std::vector<PlayerAbility>> squareToP1AssassinAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP1AssassinAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_ASSASSIN] = squareToP1AssassinAbilities;

  // p2 king abilities
  std::vector<std::vector<PlayerAbility>> squareToP2KingAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP2KingAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_KING] = squareToP2KingAbilities;

  // p2 mage abilities
  std::vector<std::vector<PlayerAbility>> squareToP2MageAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -2; dx < 3; dx++) {
        for(int dy = -2; dy < 3; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP2MageAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_MAGE] = squareToP2MageAbilities;

  // p2 pawn abilities
  std::vector<std::vector<PlayerAbility>> squareToP2PawnAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP2PawnAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_PAWN] = squareToP2PawnAbilities;

  // p2 warrior abilities
  std::vector<std::vector<PlayerAbility>> squareToP2WarriorAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP2WarriorAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_WARRIOR] = squareToP2WarriorAbilities;

  // p2 wall abilities
  // walls can't ability
  std::vector<std::vector<PlayerAbility>> squareToP2WallAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      squareToP2WallAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_WALL] = squareToP2WallAbilities;

  // p2 assassin abilities
  std::vector<std::vector<PlayerAbility>> squareToP2AssassinAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP2AssassinAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_ASSASSIN] = squareToP2AssassinAbilities;

  // NO_PIECE abilities
  std::vector<std::vector<PlayerAbility>> squareToNoPieceAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      squareToNoPieceAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[NO_PIECE] = squareToNoPieceAbilities;

  return pieceTypeToSquareToLegalAbilities;
}

/*
 * For index of each square, generates indices of squares that are touching it.
 * Used for mage ability.
 */
std::vector<std::vector<int>> generateSquareToNeighboringSquares() {
  std::vector<std::vector<int>> squareToNeighboringSquares{NUM_ROWS * NUM_COLUMNS};
  for(int srcY = 0; srcY < NUM_ROWS; srcY++) {
    for(int srcX = 0; srcX < NUM_COLUMNS; srcX++) {
      std::vector<int> neighboringSquares;
      int srcIndex = coordinatesToBoardIndex(srcX, srcY);
      for(int k = -1; k < 2; k++) {
        for(int l = -1; l < 2; l++) {
          if(k == 0 && l == 0) continue;
          int newX = srcX + k;
          int newY = srcY + l;
          if(isOffBoard(newX, newY)) continue;
          int newIndex = coordinatesToBoardIndex(newX, newY);
          neighboringSquares.push_back(newIndex);
        }
      }
      squareToNeighboringSquares[srcIndex] = neighboringSquares;  
    }
  }
  return squareToNeighboringSquares;
}
