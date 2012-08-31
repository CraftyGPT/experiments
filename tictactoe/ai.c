/*
 * Implement a negamax AI for tictactoe
 */

#include <math.h>
#include "tictactoe.h"

#define T3_AI_MAXDEPTH 9
#define MAX(x,y) x > y ? x : y
#define MIN(x,y) x < y ? x : y

short t3_ai_candidate_moves(T3Board * board, short moves[]) {
   short letter, row, i = 0;
   for(row = 0; row < 3; row++) {
      for(letter = 0; letter < 3; letter++) {
          if(t3_get_ch(board, 'A' + letter, '1' + row) == ' ') {
              moves[i] = row*3 + letter;
              i += 1;
          }
      }
   }
   return i;
}

float t3_ai_buildtree(T3Board * board, char player, short * best_move, int depth) {

   /*
    * Recursively construct and go down a game tree.
    * Each node is a possible game state.
    * Each node is scored as either
    * +/- INFINITY for win/loose or 0 for Draw
    */

    if(depth > T3_AI_MAXDEPTH) return 0;

    T3Status status = t3_gamestatus(board);
    T3Board * board_clone;
    float score, subscore;
    short moves[9], moves_count, i;
    char  other_player, active_player = t3_get_turn(board);

    if(status == X_WINS) {
       if(active_player == 'X') return INFINITY;
       else return -INFINITY;
    } else if(status == O_WINS) {
       if(active_player == 'O') return INFINITY;
       else return -INFINITY;
    } else if(status == DRAW) {
       return 0;
    }

    if(player == 'X') other_player = 'O';
    else other_player = 'X';

    if(player == active_player) score = -INFINITY;
    else score = INFINITY;

    moves_count = t3_ai_candidate_moves(board, moves);
    for(i = 0; i < moves_count; i++) {
        board_clone = t3_clone(board);
        t3_set_pos(board_clone, moves[i], player);
        subscore = -t3_ai_buildtree(board_clone, other_player, best_move, depth + 1);

        if(score < subscore) score = subscore;
        if(depth == 0) *best_move = moves[i];

        t3_free(board_clone);
    }

    return score;
}

short t3_ai_bestmove(T3Board * board) {
    short best_move;
    t3_ai_buildtree(board, t3_get_turn(board), &best_move, 0);
    return best_move;
}
