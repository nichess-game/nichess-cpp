namespace nichess {

const int NUM_ROWS = 8;
const int NUM_COLUMNS = 8;
const int NUM_SQUARES = NUM_ROWS * NUM_COLUMNS;
const int NUM_STARTING_PIECES = 7;
const int ABILITY_SKIP = -1;
const int MOVE_SKIP = -1;

const int KING_STARTING_HEALTH_POINTS = 200;
const int MAGE_STARTING_HEALTH_POINTS = 230;
const int PAWN_STARTING_HEALTH_POINTS = 300;
const int WARRIOR_STARTING_HEALTH_POINTS = 500;
const int ASSASSIN_STARTING_HEALTH_POINTS = 110;

const int KING_ABILITY_POINTS = 60;
const int MAGE_ABILITY_POINTS = 80;
const int PAWN_ABILITY_POINTS = 30;
const int WARRIOR_ABILITY_POINTS = 100;
const int ASSASSIN_ABILITY_POINTS = 120;

const int NUM_PLAYERS = 2;
const int NUM_PIECE_TYPE = 11;

// piece index is not the same thing as board(square) index
// it is used as an array index for faster access to a specific piece
const int MAGE_PIECE_INDEX = 0;
const int ASSASSIN_PIECE_INDEX = 1;
const int WARRIOR_PIECE_INDEX = 2;
const int PAWN_1_PIECE_INDEX = 3;
const int PAWN_2_PIECE_INDEX = 4;
const int PAWN_3_PIECE_INDEX = 5;
const int KING_PIECE_INDEX = 6;

} // namespace nichess
