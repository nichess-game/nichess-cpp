#pragma once 

#include <vector>
#include <optional>
#include <tuple>

namespace nichess {

const int NUM_ROWS = 8;
const int NUM_COLUMNS = 8;
const int NUM_STARTING_PIECES = 7;
const int ABILITY_SKIP = -1;
const int MOVE_SKIP = -1;

// TODO: move this into a config file (and add starting coordinates, and move/ability offsets to it)
const int KING_STARTING_HEALTH_POINTS = 200;
const int MAGE_STARTING_HEALTH_POINTS = 230;
const int PAWN_STARTING_HEALTH_POINTS = 300;
const int WARRIOR_STARTING_HEALTH_POINTS = 500;
const int ASSASSIN_STARTING_HEALTH_POINTS = 110;
const int WALL_STARTING_HEALTH_POINTS = 100;

const int KING_ABILITY_POINTS = 60;
const int MAGE_ABILITY_POINTS = 80;
const int PAWN_ABILITY_POINTS = 30;
const int WARRIOR_ABILITY_POINTS = 100;
const int ASSASSIN_ABILITY_POINTS = 120;
const int WALL_ABILITY_POINTS = 0;

const int NUM_PLAYERS = 2;
const int NUM_PIECE_TYPE = 13;

// piece index is not the same thing as board(square) index
// it is used as an array index for faster access to a specific piece
const int KING_PIECE_INDEX = 0;
const int ASSASSIN_PIECE_INDEX = 1;
const int MAGE_PIECE_INDEX = 2;
const int WARRIOR_PIECE_INDEX = 3;
const int PAWN_1_PIECE_INDEX = 4;
const int PAWN_2_PIECE_INDEX = 5;
const int PAWN_3_PIECE_INDEX = 6;

enum Player: int {
  PLAYER_1, PLAYER_2
};

constexpr Player operator~(Player p) {
  return Player(p ^ 1);
}

enum PieceType: int {
  P1_KING, P1_MAGE, P1_WARRIOR, P1_ASSASSIN, P1_PAWN, P1_WALL,
  P2_KING, P2_MAGE, P2_WARRIOR, P2_ASSASSIN, P2_PAWN, P2_WALL,
  NO_PIECE
};

enum AbilityType: int {
  KING_DAMAGE, MAGE_DAMAGE, WARRIOR_DAMAGE, ASSASSIN_DAMAGE, PAWN_DAMAGE, PAWN_MAKE_WALL, PAWN_DESTROY_WALL, NO_ABILITY
};

class Piece {
  public:
    PieceType type;
    int healthPoints;
    int squareIndex;
    Piece();
    Piece(PieceType type, int healthPoints, int squareIndex);
    void print();
};

class PlayerMove {
  public:
    int moveSrcIdx, moveDstIdx;
    PlayerMove();
    PlayerMove(int moveSrcIdx, int moveDstIdx);
    void print();
};

class PlayerAbility {
  public:
    int abilitySrcIdx, abilityDstIdx;
    PlayerAbility();
    PlayerAbility(int abilitySrcIdx, int abilityDstIdx);
    void print();
};

class PlayerAction {
  public:
    int moveSrcIdx, moveDstIdx, abilitySrcIdx, abilityDstIdx;
    PlayerAction();
    PlayerAction(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx);
    void print();
};

class UndoInfo {
  public:
    Piece* affectedPieces[9];
    int moveSrcIdx, moveDstIdx;
    AbilityType abilityType;
    UndoInfo();
    UndoInfo(int moveSrcIdx, int moveDstIdx, AbilityType abilityType);
};

class Game {
  private:
    Piece* board[NUM_ROWS * NUM_COLUMNS];
    Piece* p1King;
    Piece* p2King;
    std::vector<std::vector<Piece*>> playerToPieces{NUM_PLAYERS};
    Player currentPlayer;
    UndoInfo moveNumberToUndoInfo[300];
    int moveNumber;
    std::vector<std::vector<std::vector<PlayerMove>>> pieceTypeToSquareIndexToLegalMoves;
    std::vector<std::vector<std::vector<PlayerAbility>>> pieceTypeToSquareIndexToLegalAbilities;
    std::vector<std::vector<int>> squareToNeighboringSquares;

  public:
    Game();
    void makeMove(int moveSrcIdx, int moveDstIdx);
    void undoMove(int moveSrcIdx, int moveDstIdx);
    bool isActionLegal(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx);
    void makeAction(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx);
    void undoLastAction();
    std::vector<PlayerAction> usefulLegalActions();
    std::vector<PlayerAction> allLegalActions();
    std::vector<PlayerMove> legalMovesByPiece(int srcSquareIdx);
    std::vector<PlayerAbility> usefulLegalAbilitiesByPiece(int srcSquareIdx);
    std::vector<PlayerAbility> allLegalAbilitiesByPiece(int srcSquareIdx);
    Player getCurrentPlayer();
    Piece getPieceByCoordinates(int x, int y);
    Piece getPieceBySquareIndex(int squareIndex);
    std::vector<Piece*> getAllPiecesByPlayer(Player player);
    std::string boardToString();
    void boardFromString(std::string encodedBoard);
    bool gameOver();
    std::optional<Player> winner();
    void print();
    void reset();
};

int coordinatesToBoardIndex(int column, int row);

std::tuple<int, int> boardIndexToCoordinates(int squareIndex);

unsigned long long perft(Game& game, int depth);

} // namespace nichess
