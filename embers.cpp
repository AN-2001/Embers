/******************************************************************************\
 *  embers.h                                                                    *
 *                                                                              * 
 *  The core file of embers, houses the game init code and handles the game     *
 *  loop.                                                                       * 
 *                                                                              *
 *              Written by Abed Naran                          September 2022   *
 *                                                                              *
 \******************************************************************************/
#include "config.h"
#include "glad/glad.h"
#include "embers.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "shader.h"
#include "program.h"
#include "vec3.h"
#include "mat4.h"
#include "chess.h"
#include <math.h>
#include "moves.h"

/**ERROR HANDLING**************************************************************/
int EmbersExit = EMBERS_FALSE;
EmbersStatus EmbersErrno = EMBERS_SUCCESS;
int EmbersErrLn = 0;
const char *EmbersErrFl = NULL;
const char *EmbersErrFun = NULL;
/******************************************************************************/

/* The embers window.                                                         */
static GLFWwindow *EmbersWindow = NULL;

/* FPS and TPS data                                                           */
static int CurrentFPS;
static int CurrentTPS;

/* perform setup operations like loading resources etc...                     */
static void SetupState();

/* updates the embers logic state.                                            */
/* Gets glfw events and runs inner game logic.                                */ 
static void UpdateState(int Tick, EMBERS_REAL Delta); 

/* Write the current status to Out                                            */
static void WriteStatus();

/* Write a report of the current state.                                       */
static void WriteReport();

/* Update the render state.                                                   */
/* Basically does everything render related.                                  */ 
static void RenderFrame(unsigned Frame, EMBERS_REAL Delta);

/* Clean up the state after exiting the game loop.                            */
static void CleanupState();

/* Exit embers.                                                               */
static void Exit();

int EmbersInit()
{
    /* Init GLFW.                                                             */
    if (!glfwInit()){
        EMBERS_ERROR(EMBERS_BAD_GLFW);
        return EMBERS_FALSE;
    }

    /* Window hints for OpenGL context creation.                              */
    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    /* Create the window and set the OpenGL context.                          */
    EmbersWindow = glfwCreateWindow(EMBERS_WIDTH,
            EMBERS_HEIGHT,
            "Embers",
            NULL,
            NULL);

    if (!EmbersWindow) {
        EMBERS_ERROR(EMBERS_CANT_OPEN_WINDOW);
        glfwTerminate();
        return EMBERS_FALSE;
    }

    glfwMakeContextCurrent(EmbersWindow);

    /* Import OpenGL functions.                                               */
    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
        EMBERS_ERROR(EMBERS_BAD_GLAD);  
        glfwDestroyWindow(EmbersWindow);
        glfwTerminate();
        return EMBERS_FALSE;
    }

    return EMBERS_TRUE;
}

static EMBERS_REAL x = -EMBERS_WIDTH / 2.f,
                   y = -EMBERS_HEIGHT / 2.f,
                   Zoom = 1;
void ZoomUpdater(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset > 0) 
        Zoom *= 1.1f;

    if (yoffset < 0)
        Zoom /= 1.1f;

    Zoom = std::max(std::min(Zoom, 3.f), 0.2f);

}

static void SetupState()
{
    ChessInit();
    glfwSetScrollCallback(EmbersWindow, ZoomUpdater);
}


static double MouseX, MouseY;
static int cpx = -1,
           cpy = -1,
           OldP = 0;

static int LegalMoves[64][2];

static inline int InBounds(int x, int y)
{
    return (x >= 0 && y >= 0 && x < 8 && y < 8);
}
static inline int GetTeam(int f) 
{
    return (f & CHESS_FLAG_WHITE ? -1 : 1) * (f != 0);
}

static void GenerateLegalMoves(int x, int y)
{
    int j, k, a, b, Iters, NumMoves, Team,
        i = 0;

    unsigned char 
        &CellVal = Board(x, y),
         Type = (CellVal & 0xf0) >> 4;

    if (!CellVal)
        goto GEN_END;


    NumMoves = MovesSizes[Type];
    Iters = MovesIterate[Type] ? 8 : 1;
    Team = GetTeam(CellVal);

    if (Type << 4 == CHESS_FLAG_PAWN) {
        if (InBounds(x, y + Team) && !Board(x, y + Team)) {
            LegalMoves[i][0] = x;
            LegalMoves[i][1] = y + Team;
            i++;
        }


        if (InBounds(x + Team, y + Team) &&
                Board(x + Team, y + Team) &&
                GetTeam(Board(x + Team, y + Team)) != Team) {

            LegalMoves[i][0] = x + Team;
            LegalMoves[i][1] = y + Team;
            i++;
        }

        if (InBounds(x - Team, y + Team) &&
                Board(x - Team, y + Team) &&
                GetTeam(Board(x - Team, y + Team)) != Team) {

            LegalMoves[i][0] = x - Team;
            LegalMoves[i][1] = y + Team;
            i++;
        }

        if (i && InBounds(x, y + Team * 2) && 
                !Board(x, y + Team*2) &&
                y == (Team == -1 ? 6 : 1)) {

            LegalMoves[i][0] = x;
            LegalMoves[i][1] = y + Team * 2;
            i++;
        }

        goto GEN_END;
    }


    for (j = 0; j < NumMoves; j++) {
        for (k = 0; k < Iters; k++) {
            a = MovesTemplates[Type][j][0] * (k + 1);
            b = MovesTemplates[Type][j][1] * (k + 1);
            if (!InBounds(x + a, y + b) || GetTeam(Board(x + a, y + b)) == Team) 
                break;

            LegalMoves[i][0] = x + a;
            LegalMoves[i][1] = y + b;
            i++;

            if (Board(x + a, y + b) && GetTeam(Board(x + a, y + b)) != Team)
                break;
        }
    }


GEN_END:
    LegalMoves[i][0] = -1;
    LegalMoves[i][1] = -1;

}

static void ChessHandle(int x, int y)
{

    for (int i = 0; LegalMoves[i][0] != -1; i++) {
        if (x != LegalMoves[i][0] || y != LegalMoves[i][1])
            continue;

        Board(x, y) = Board(cpx, cpy);
        Board(cpx, cpy) = 0;
        return;
    }
    
}

static void UpdateState(int Tick, EMBERS_REAL Delta)
{
    double MouseDeltaX, MouseDeltaY,
           OldMouseX = MouseX,
           OldMouseY = MouseY;

    glfwPollEvents();
    glfwGetCursorPos(EmbersWindow, &MouseX, &MouseY);

    int Tx = MouseX / Zoom - (x + ((EMBERS_WIDTH / 2.f) / Zoom) ),
        Ty = MouseY / Zoom - (y + ((EMBERS_HEIGHT / 2.f) / Zoom) );

    Tx = Tx / (int)(EMBERS_WIDTH / 8),
    Ty = Ty / (int)(EMBERS_HEIGHT / 8);

    int P = glfwGetMouseButton(EmbersWindow, GLFW_MOUSE_BUTTON_1);

    if (Tx >= 0 && Tx < 8 && Ty >= 0 && Ty < 8) {
        if (P && !OldP) {
            if (cpx != -1 && cpy != -1) {
                Board(cpx, cpy) ^= CHESS_FLAG_HIGHLITED;

                for (int i = 0; LegalMoves[i][0] != -1; i++) {
                    Board(LegalMoves[i][0], LegalMoves[i][1]) ^= CHESS_FLAG_HIGHLITED;
                }

                ChessHandle(Tx, Ty);
            }

            if (cpx == -1 && cpx == - 1 &&
                 Board(Tx, Ty) & (CHESS_FLAG_WHITE | CHESS_FLAG_BLACK)) {

                GenerateLegalMoves(Tx, Ty);

                for (int i = 0; LegalMoves[i][0] != -1; i++) {
                    Board(LegalMoves[i][0], LegalMoves[i][1]) |= CHESS_FLAG_HIGHLITED;
                }

                Board(Tx, Ty) |= CHESS_FLAG_HIGHLITED;
                cpx = Tx;
                cpy = Ty;
            } else {
                cpx = cpy = -1;
            }

        }
            

        OldP = P;
    }


    MouseDeltaX = MouseX - OldMouseX;
    MouseDeltaY = MouseY - OldMouseY;
    if (glfwGetMouseButton(EmbersWindow, 1)) {
        x += MouseDeltaX / Zoom;
        y += MouseDeltaY / Zoom;
    }
    
    ChessSetCamera(x, y, Zoom);
    ChessUploadBoard();
}

static void RenderFrame(unsigned Frame, EMBERS_REAL Delta)
{
    EMBERS_GL(glClearColor(186 / 255.f,202 / 255.f,68 / 255.f, 0.f));
    EMBERS_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    ChessSetCamera(x, y, Zoom);
    ChessDraw();
}

static void CleanupState() 
{
    ChessShutdown();
}

void EmbersLoop()
{
    /* Exit if the user ignored errors.                                       */
    if (EMBERS_IS_BAD_STATE())
        return;

    unsigned
        Frame = 0,
        Tick = 0,
        TotalFrames = 0;

    /* Delta1 is for logic updates and resets after one tick.                 */
    /* Delta2 is for render updates and resets after one second.              */
    EMBERS_REAL
        Delta1 = 0,
        Delta2 = 0,
        CT = glfwGetTime(),
        LT = glfwGetTime(),
        DT = 0,
        SecondPerTick = 1.f / EMBERS_TPS;

    SetupState();
    if (EMBERS_IS_BAD_STATE()) {
        Exit();
        return;
    }

    while (!EmbersExit) {
        CT = glfwGetTime(); 
        DT = CT - LT;
        Delta1 += DT;
        Delta2 += DT;

        /* One tick passed.                                                       */
        if (Delta1 >= SecondPerTick) {
            UpdateState(Tick, Delta1);
            Tick++;
            Delta1 = 0;
        }

        RenderFrame(TotalFrames, DT);
        Frame++;
        TotalFrames++;

        /* One second passed                                                      */
        if (Delta2 >= 1.f) {
            CurrentFPS = Frame;
            CurrentTPS = Tick;
            Delta2 = 0;
            Frame = 0;
            Tick = 0;
            WriteReport();
        }

        LT = CT;
        EmbersExit |= glfwWindowShouldClose(EmbersWindow) |
                      EMBERS_IS_BAD_STATE();

        glfwSwapBuffers(EmbersWindow);
    }

    CleanupState();
    Exit();
}

void Exit()
{
    WriteStatus();
    glfwDestroyWindow(EmbersWindow);
    glfwTerminate();
}

/* Error tables to be used in WriteStatus                                     */
static const char
*EmbersErrorTable[] = {
    "Unexpected Error.",
    "Exited successfully.",
    "Couldn't create the game window.",
    "Couldn't init GLAD.",
    "Couldn't init GLFW.",
    "Couldn't compile shader.",
    "Couldn't link program.",
    "Out of memory.",
    "Couldn't open a file.",
    "Hit an openGL error."
};

static const char
*EmbersGLErrorTable[] = {
    "Invalid enum.",
    "Invalid value.",
    "Invalid operation.",
    "Stack overflow.",
    "Stack underflow.",
    "Out of memory.",
    "Invalid framebuffer operation.",
    "Context lost."
};

static const char *
*Tables[] = {
    EmbersErrorTable,
    EmbersGLErrorTable
};

void WriteStatus()
{
#ifndef EMBERS_DEBUG
    return;
#endif
    if (EmbersErrno == EMBERS_SUCCESS) {
        EMBERS_LOG_INFO("Exiting with no errors.");
        return;
    }

    /* Determine the type of error then query the right table and fetch the   */
    /* right formatting.                                                      */
    const char * const *Table = Tables[EMBERS_IS_GL(EmbersErrno)];
    int Index = EmbersErrno & (~EMBERS_GL_ERROR);
    char Buff[EMBERS_BUFFER_SIZE];
    snprintf(Buff,
            EMBERS_BUFFER_SIZE,
            "[%s():%s:%d] %s",
            EmbersErrFun,
            EmbersErrFl,
            EmbersErrLn,
            Table[Index]);

    EMBERS_LOG_ERROR(Buff);
}

static void WriteReport()
{
#ifndef EMBERS_DEBUG
    return;
#endif

    char Buff[EMBERS_BUFFER_SIZE];
    snprintf(Buff,
             EMBERS_BUFFER_SIZE,
             "FPS: %3d   TPS: %3d",
             CurrentFPS,
             CurrentTPS);

    EMBERS_LOG_INFO(Buff);
}
