/******************************************************************************\
*  moves.h                                                                     *
*                                                                              *
*  A data store of all the valid move for each piece type.                     *
*                                                                              *
*                                                                              *
*              Written by Abed Na'ran                          March 2023      *
*                                                                              *
\******************************************************************************/
#ifndef MOVES_H
#define MOVES_H

#define CHESS_END_MOVES (0xffff)

/* The pawn is handled differently as it has ALOT more rules.                 */

static char 
    MovesSizes[5] =
        {
            8, /* King.                                                       */
            8, /* Queen.                                                      */
            4, /* Rook.                                                       */
            8, /* Knight.                                                     */
            4, /* Bishop.                                                     */
        };

static char
    MovesIterate[5] =
        {
            0, /* King.                                                       */
            1, /* Queen.                                                      */
            1, /* Rook.                                                       */
            0, /* Knight.                                                     */
            1, /* Bishop.                                                     */
        };

static char
    MovesTemplates[5][8][2] =
        { 
            {{1, 0}, {-1, 0},
             {0, 1}, {0, -1},
             {1, 1}, {-1, 1},
             {1, -1}, {-1, -1}},

            {{1, 0}, {-1, 0},
             {0, 1}, {0, -1},
             {1, 1}, {-1, 1},
             {1, -1}, {-1, -1}},

            {{1, 0}, {-1, 0},
             {0, 1}, {0, -1}},

            {{1, 2}, {-1, 2},
             {1, -2}, {-1, -2},
             {2, 1},  {2, -1},
             {-2, 1}, {-2, -1}},

            {{1, 1}, {-1, 1},
             {1, -1}, {-1, -1}},

        };

#endif /* MOVES_H */
