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
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "shader.h"
#include "program.h"
#include "vec3.h"
#include "mat4.h"
#include "chess.h"
#include <math.h>


/**ERROR HANDLING**************************************************************/
int EmbersExit = EMBERS_FALSE;
EmbersStatus EmbersErrno = EMBERS_SUCCESS;
int EmbersErrLn = 0;
const char *EmbersErrFl = NULL;
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
int cpx = -1,
    cpy = -1;

static void UpdateState(int Tick, EMBERS_REAL Delta)
{
    double MouseDeltaX, MouseDeltaY,
           OldMouseX = MouseX,
           OldMouseY = MouseY;

    glfwPollEvents();
    glfwGetCursorPos(EmbersWindow, &MouseX, &MouseY);


    double Tx = MouseX / Zoom - (x + ((EMBERS_WIDTH / 2.f) / Zoom) ),
           Ty = MouseY / Zoom - (y + ((EMBERS_HEIGHT / 2.f) / Zoom) );

    int x_ = Tx / (int)(EMBERS_WIDTH / 8),
        y_ = Ty / (int)(EMBERS_HEIGHT / 8);

    static int OldP = 0,
               P = glfwGetMouseButton(EmbersWindow, GLFW_MOUSE_BUTTON_1);

    P = glfwGetMouseButton(EmbersWindow, GLFW_MOUSE_BUTTON_1);
    if (x_ >= 0 && x_ < 8 && y_ >= 0 && y_ < 8) {
        unsigned char &Flags = Board(x_, y_);

        if (P && !OldP) {
            if (cpy != -1 && (cpy != y_ || cpx != x_)) {
                Flags = Board(cpx, cpy);
                Board(cpx, cpy) = 0;

                Flags ^= CHESS_FLAG_HIGHLITED;
                cpx = cpy = -1;

            } else if (Flags & (CHESS_FLAG_WHITE | CHESS_FLAG_BLACK)) {
                Flags |= CHESS_FLAG_HIGHLITED;
                cpx = x_;
                cpy = y_;
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
        EmbersExit |= glfwWindowShouldClose(EmbersWindow);                    
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
    "Invalid enum",
    "Invalid value",
    "Invalid operation",
    "stack overflow",
    "stack underflow",
    "out of memory",
    "invalid framebuffer operation",
    "context lost"
};

static const char 
*Formattings[] = {
    "%s\n",
    "%s %s %s\n",
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

    /* Determine the type of error then query the right table and fetch the   */
    /* right formatting.                                                      */
    const char * const *Table = Tables[EMBERS_IS_GL(EmbersErrno)];
    int IsSucc = EmbersErrno == EMBERS_SUCCESS;
    const char *Formatting = Formattings[!IsSucc];
    int Index = EmbersErrno & (~EMBERS_GL_ERROR);
    char Buff[EMBERS_BUFFER_SIZE];

    sprintf(Buff,
            Formatting,
            IsSucc ? Table[Index] : EmbersErrFl,
            EmbersErrLn,
            Table[Index]);

    EMBERS_LOG_INFO(Buff);
    EmbersErrno = EMBERS_SUCCESS;
}

static void WriteReport()
{
#ifndef EMBERS_DEBUG
    return;
#endif

    char Buff[EMBERS_BUFFER_SIZE];
    sprintf(Buff,
            "FPS: %3d   TPS: %3d",
            CurrentFPS,
            CurrentTPS);

    EMBERS_LOG_INFO(Buff);
}
