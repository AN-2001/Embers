/******************************************************************************\
*  chess.h                                                                     *
*                                                                              *
*  Implements the chess opengl backend and the chess logic.                    *
*                                                                              *
*              Written by Abed Na'ran                          March 2023      *
*                                                                              *
\******************************************************************************/
#ifndef CHESS_H
#define CHESS_H
#include "config.h"

enum {
    CHESS_FLAG_HIGHLITED = 0x01,
    CHESS_FLAG_WHITE = 0x02,
    CHESS_FLAG_BLACK = 0x04,
    CHESS_FLAG_KING = 0x00,
    CHESS_FLAG_QUEEN = 0x10,
    CHESS_FLAG_ROOK = 0x20,
    CHESS_FLAG_KNIGHT = 0x30,
    CHESS_FLAG_BISHOP = 0x40,
    CHESS_FLAG_PAWN = 0x50,
};

/******************************************************************************\
* ChessInit                                                                    *
*                                                                              *
*  Initialize the chess board.                                                 *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -void.                                                                      *
*                                                                              *
\******************************************************************************/
void ChessInit();

/******************************************************************************\
* ChessDraw                                                                    *
*                                                                              *
*  Draw the chess board.                                                       *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -void.                                                                      *
*                                                                              *
\******************************************************************************/
void ChessDraw();

/******************************************************************************\
* Board                                                                *
*                                                                              *
*  Get a reference to the cell type at (x, y)                                  *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -x, y: The cell position.                                                   *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -unsigned char&: A reference to the cell type at the position.              *
*                                                                              *
\******************************************************************************/
unsigned char &Board(int x, int y);

/******************************************************************************\
* ChessUploadBoard                                                             *
*                                                                              *
*  Upload the chess board to the GPU.                                          *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Void.                                                                      *
*                                                                              *
\******************************************************************************/
void ChessUploadBoard();

/******************************************************************************\
* ChessSetCamera                                                               *
*                                                                              *
*  Set the camera state of the chess game.                                     *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -x, y, Zoom: The settings.                                                  *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Void.                                                                      *
*                                                                              *
\******************************************************************************/
void ChessSetCamera(EMBERS_REAL x, EMBERS_REAL y, EMBERS_REAL Zoom);

/******************************************************************************\
* ChessShutdown                                                                *
*                                                                              *
*  Gracefully shutdown the game by freeing all the used resources.             *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -void.                                                                      *
*                                                                              *
\******************************************************************************/
void ChessShutdown();

#endif /* CHESS_H */
