/******************************************************************************\
*  embers.h                                                                    *
*                                                                              * 
*  The core file of embers, houses the game init code and handles the game     *
*  loop.                                                                       * 
*                                                                              *
*              Written by Abed Naran                          September 2022   *
*                                                                              *
\******************************************************************************/

#ifndef EMBERS_H
#define EMBERS_H
#include <stdio.h>
#include "config.h"


/* Status enum for basic return values.                                       */
typedef enum {
	EMBERS_FAILURE = 0,
	EMBERS_SUCCESS,
	EMBERS_CANT_OPEN_WINDOW,
	EMBERS_BAD_GLAD,
	EMBERS_BAD_GLFW,
	EMBERS_CANT_COMPILE_SHADER,
	EMBERS_CANT_LINK_PROGRAM,
    EMBERS_OUT_OF_MEMORY,
    EMBERS_CANT_OPEN_FILE,
	EMBERS_GL_ERROR = 0x100,
} EmbersStatus;

/******************************************************************************\
* EmbersInit                                                                   *
*                                                                              * 
*  Initializes Embers.                                                         * 
*                                                                              * 
* Return:                                                                      *
*                                                                              * 
*  -int: EMEBRS_TRUE on success, EMBERS_FALSE on failure.                      *
*                                                                              *
\******************************************************************************/
int EmbersInit();

/******************************************************************************\
* EmbersLoop                                                                   *
*                                                                              * 
*  Runs the main game loop which handles the game logic And hte main render    *
*  calls.                                                                      *  
*                                                                              * 
* Return                                                                       *
*                                                                              * 
*   void.                                                                      * 
*                                                                              *
\******************************************************************************/
void EmbersLoop();

#endif /* EMBERS_H */
