#pragma once 

#include "constants.hpp"

#include <vector>
#include <optional>
#include <tuple>

namespace nichess {

enum Player: int {
  PLAYER_1, PLAYER_2
};

constexpr Player operator~(Player p) {
  return Player(p ^ 1);
}

enum PieceType: int {
  P1_KING, P1_MAGE, P1_WARRIOR, P1_ASSASSIN, P1_PAWN,
  P2_KING, P2_MAGE, P2_WARRIOR, P2_ASSASSIN, P2_PAWN,
  NO_PIECE
};

enum AbilityType: int {
  KING_DAMAGE, MAGE_DAMAGE, WARRIOR_DAMAGE, ASSASSIN_DAMAGE, PAWN_DAMAGE, NO_ABILITY
};

class Piece {
  public:
    PieceType type;
    int healthPoints;
    int squareIndex;
    Piece();
    Piece(PieceType type, int healthPoints, int squareIndex);
    Piece(const Piece& other);
    bool operator==(const Piece& other) const;
    bool operator!=(const Piece& other) const;
};

class PlayerMove {
  public:
    int moveSrcIdx, moveDstIdx;
    PlayerMove();
    PlayerMove(int moveSrcIdx, int moveDstIdx);
};

class PlayerAbility {
  public:
    int abilitySrcIdx, abilityDstIdx;
    PlayerAbility();
    PlayerAbility(int abilitySrcIdx, int abilityDstIdx);
};

class PlayerAction {
  public:
    int moveSrcIdx, moveDstIdx, abilitySrcIdx, abilityDstIdx;
    PlayerAction();
    PlayerAction(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx);
};

class UndoInfo {
  public:
    Piece* affectedPieces[9];
    int moveSrcIdx, moveDstIdx;
    AbilityType abilityType;
    UndoInfo();
    UndoInfo(int moveSrcIdx, int moveDstIdx, AbilityType abilityType);
};

/*
 * Used for faster generation and validation of actions.
 */
class GameCache {
  public:
    std::vector<std::vector<std::vector<PlayerMove>>> pieceTypeToSquareIndexToLegalMoves;
    std::vector<std::vector<std::vector<PlayerAbility>>> pieceTypeToSquareIndexToLegalAbilities;
    std::vector<std::vector<int>> squareToNeighboringSquares;
    
    GameCache(); 
};

class Game {
  private:
    Game();
  public:
    Piece* board[NUM_ROWS * NUM_COLUMNS];
    Piece* p1King;
    Piece* p2King;
    std::vector<std::vector<Piece*>> playerToPieces{NUM_PLAYERS};
    Player currentPlayer;
    int moveNumber;
    GameCache *gameCache;

    Game(GameCache &gameCache);
    Game(const Game& other);
    Game(GameCache &gameCache, const std::string encodedBoard);
    ~Game();
    void makeMove(int moveSrcIdx, int moveDstIdx);
    void undoMove(int moveSrcIdx, int moveDstIdx);
    bool isActionLegal(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx);
    UndoInfo makeAction(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx);
    void undoAction(UndoInfo undoInfo);
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
    std::string dump() const;
    void reset();
};

int coordinatesToBoardIndex(int column, int row);
std::tuple<int, int> boardIndexToCoordinates(int squareIndex);
unsigned long long perft(Game& game, int depth);

} // namespace nichess
