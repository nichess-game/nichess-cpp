#include "nichess.hpp"

using namespace nichess;

std::string playerToString(Player p);
std::string pieceTypeToString(PieceType pt);
bool player1OrEmpty(PieceType pt);
bool player2OrEmpty(PieceType pt);
bool pieceBelongsToPlayer(PieceType pt, Player player);
bool isOffBoard(int x, int y);
bool isOffBoard(int squareIndex);
std::vector<std::vector<std::vector<PlayerMove>>> generateLegalMovesOnAnEmptyBoard();
std::vector<std::vector<std::vector<PlayerAbility>>> generateLegalAbilitiesOnAnEmptyBoard();
std::vector<std::vector<int>> generateSquareToNeighboringSquares();
