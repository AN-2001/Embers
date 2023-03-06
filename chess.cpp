#include "glad.h"
#include "errors.h"
#include "shader.h"
#include "program.h"
#include "config.h"
#include "chess.h"
#include "stdlib.h"
#include "image.h"

#define BOARD_WIDTH (8)
#define BOARD_HEIGHT (8)
#define BOARD_SIZE (64)
#define GRID_SIZE ((int)(EMBERS_WIDTH / BOARD_WIDTH))
#define CHESS_PROG_ID (0)
#define CHESS_PROG_POS (1)
#define CHESS_PROG_CORNER (2)

#define CHESS_TEXTURE_PIECES (0)
#define CHESS_TEXTURE_DATA (1)

enum Corners {
    CORNER_TOP_LEFT = 0,
    CORNER_TOP_RIGHT,
    CORNER_BOTTOM_LEFT,
    CORNER_BOTTOM_RIGHT
};

enum TextureType {
    TEXTURE_THREE_CHANNELS,
    TEXTURE_ONE_CHANNEL
};

typedef struct GridCell {
    int Corner;
    int ID;
    EMBERS_REAL Pos[2];
} GridCell;

static struct Game {
    GridCell *Inner; /* The inner board mesh.                                 */
    unsigned char FlagsTexture[64]; /* Describes the game state.              */
    unsigned char Trash; /* Used as a reference when given illegal values.    */
    GLuint Textures[2]; /* 0 for pieces, 1 for data.                          */

    /* Opengl buffers.                                                        */
    GLuint VBO;
    GLuint VAO;
    EmbersProgram Program;
} *Game;

static inline void PopulateBoard();
static inline void PrepBoardBuffers();
static inline void PrepareTexture(int Tex,
                                  void *Data,
                                  int Width,
                                  int Height,
                                  TextureType Type = TEXTURE_THREE_CHANNELS);

void PopulateBoard()
{
    Image *Positions;
    int i, j, x, y;
    Game = (struct Game*)malloc(sizeof(*Game)); 

    if (!Game) {
        EMBERS_ERROR(EMBERS_OUT_OF_MEMORY);
        return;
    }

    Game -> Inner = (GridCell*)malloc(sizeof(*Game -> Inner) * BOARD_SIZE * 6);
    if (!Game -> Inner)
        return;

    /* Load the cached starter position.                                      */
    Positions = ImageRead("./assets/StarterPosition.ppm");
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            Board(i, j) = ImageGetPixel(Positions, i, j).r;

    ImageFree(Positions);

    /* Calculate the mesh.                                                    */
    for (i = 0; i < BOARD_SIZE; i++) {
        x = i % BOARD_WIDTH, y = i / BOARD_WIDTH;
        j = i * 6; 

        Game -> Inner[j + 0].Corner = CORNER_TOP_LEFT;
        Game -> Inner[j + 1].Corner = CORNER_TOP_RIGHT;
        Game -> Inner[j + 2].Corner = CORNER_BOTTOM_LEFT;
        Game -> Inner[j + 3].Corner = CORNER_TOP_RIGHT;
        Game -> Inner[j + 4].Corner = CORNER_BOTTOM_LEFT;
        Game -> Inner[j + 5].Corner = CORNER_BOTTOM_RIGHT;

        Game -> Inner[j + 0].ID = i; Game -> Inner[j + 1].ID = i;
        Game -> Inner[j + 2].ID = i; Game -> Inner[j + 3].ID = i;
        Game -> Inner[j + 4].ID = i; Game -> Inner[j + 5].ID = i;

        Game -> Inner[j + 0].Pos[0] = x * GRID_SIZE; 
        Game -> Inner[j + 0].Pos[1] = y * GRID_SIZE;

        Game -> Inner[j + 1].Pos[0] = x * GRID_SIZE + GRID_SIZE;
        Game -> Inner[j + 1].Pos[1] = y * GRID_SIZE;

        Game -> Inner[j + 2].Pos[0] = x * GRID_SIZE;
        Game -> Inner[j + 2].Pos[1] = y * GRID_SIZE + GRID_SIZE;

        Game -> Inner[j + 3].Pos[0] = x * GRID_SIZE + GRID_SIZE;
        Game -> Inner[j + 3].Pos[1] = y * GRID_SIZE;

        Game -> Inner[j + 4].Pos[0] = x * GRID_SIZE;
        Game -> Inner[j + 4].Pos[1] = y * GRID_SIZE + GRID_SIZE;

        Game -> Inner[j + 5].Pos[0] = x * GRID_SIZE + GRID_SIZE;
        Game -> Inner[j + 5].Pos[1] = y * GRID_SIZE + GRID_SIZE;

    }

}

void PrepBoardBuffers()
{
    Image *Pieces = ImageRead("./assets/pieces.ppm");

    EMBERS_GL(glGenBuffers(1, &(Game -> VBO)));
    EMBERS_GL(glGenVertexArrays(1, &(Game -> VAO)));

    EMBERS_GL(glBindVertexArray(Game -> VAO));
    EMBERS_GL(glBindBuffer(GL_ARRAY_BUFFER, Game -> VBO));

    EMBERS_GL(glBufferData(GL_ARRAY_BUFFER,
                           sizeof(*Game -> Inner) * BOARD_SIZE * 6,
                           Game -> Inner,
                           GL_STATIC_DRAW));

    EMBERS_GL(glGenTextures(2, Game -> Textures));
    PrepareTexture(Game -> Textures[CHESS_TEXTURE_PIECES],
                   Pieces -> Pixels,
                   Pieces -> Width,
                   Pieces -> Height);

    PrepareTexture(Game -> Textures[CHESS_TEXTURE_DATA],
                   Game -> FlagsTexture,
                   BOARD_WIDTH,
                   BOARD_HEIGHT,
                   TEXTURE_ONE_CHANNEL);

    Game -> Program = EmbersCreateProgram();
    EmbersShader VertexShader = EmbersLoadShader("vert.glsl", GL_VERTEX_SHADER),
                 FragmentShader = EmbersLoadShader("frag.glsl", GL_FRAGMENT_SHADER);

    EmbersBindShader(Game -> Program, VertexShader);
    EmbersBindShader(Game -> Program, FragmentShader);

    EmbersSetAttribLocation(Game -> Program, CHESS_PROG_CORNER, "Corner");
    EmbersSetAttribLocation(Game -> Program, CHESS_PROG_ID, "ID");
    EmbersSetAttribLocation(Game -> Program, CHESS_PROG_POS, "Position");

    EmbersLinkProgram(Game -> Program);
    EmbersSetActiveProgram(Game -> Program);

    EmbersFreeShader(VertexShader);
    EmbersFreeShader(FragmentShader);
    ImageFree(Pieces);
}

void ChessInit()
{
    PopulateBoard();
    PrepBoardBuffers();

    EMBERS_GL(glVertexAttribPointer(CHESS_PROG_CORNER,
                          1,
                          GL_FLOAT,
                          GL_FALSE,
                          EMBERS_STRIDE(GridCell),
                          (const void*)EMBERS_OFFSET(GridCell, Corner)));

    EMBERS_GL(glVertexAttribPointer(CHESS_PROG_ID,
                          1,
                          GL_FLOAT,
                          GL_FALSE,
                          EMBERS_STRIDE(GridCell),
                          (const void*)EMBERS_OFFSET(GridCell, ID)));

    EMBERS_GL(glVertexAttribPointer(CHESS_PROG_POS,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          EMBERS_STRIDE(GridCell),
                          (const void*)EMBERS_OFFSET(GridCell, Pos)));

    EMBERS_GL(glEnableVertexAttribArray(CHESS_PROG_CORNER));
    EMBERS_GL(glEnableVertexAttribArray(CHESS_PROG_ID));
    EMBERS_GL(glEnableVertexAttribArray(CHESS_PROG_POS));

    EmbersSetUniformM4(Game -> Program,
                       "World",
                       Mat4::Identity());

    EmbersSetUniformM4(Game -> Program,
                       "Projection",
                       Mat4::Ortho(-EMBERS_WIDTH / 2.f,
                                    EMBERS_WIDTH / 2.f,
                                   -EMBERS_HEIGHT / 2.f,
                                    EMBERS_HEIGHT / 2.f,
                                    10, 1000));

    EmbersSetUniformM4(Game -> Program,
                       "View",
                       Mat4::LookAt({EMBERS_WIDTH / 2.f, EMBERS_HEIGHT / 2.f, 100},
                                    {EMBERS_WIDTH / 2.f, EMBERS_HEIGHT / 2.f, 0},
                                    {0, 1, 0}));

    EmbersSetUniformI(Game -> Program, "PieceTexture", CHESS_TEXTURE_PIECES);
    EmbersSetUniformI(Game -> Program, "DataTexture", CHESS_TEXTURE_DATA);

}

void ChessShutdown()
{
    free(Game -> Inner);
    EmbersFreeProgram(Game -> Program);
    EMBERS_GL(glDeleteBuffers(1, &Game -> VBO));
    EMBERS_GL(glDeleteVertexArrays(1, &Game -> VAO));
    EMBERS_GL(glDeleteTextures(2, Game -> Textures));
}

void ChessDraw()
{

    EmbersSetActiveProgram(Game -> Program);
    EMBERS_GL(glBindBuffer(GL_ARRAY_BUFFER, Game -> VBO));

    EMBERS_GL(glActiveTexture(GL_TEXTURE0 + CHESS_TEXTURE_PIECES));
    EMBERS_GL(glBindTexture(GL_TEXTURE_2D, Game -> Textures[CHESS_TEXTURE_PIECES]));

    EMBERS_GL(glActiveTexture(GL_TEXTURE0 + CHESS_TEXTURE_DATA));
    EMBERS_GL(glBindTexture(GL_TEXTURE_2D, Game -> Textures[CHESS_TEXTURE_DATA]));

    EMBERS_GL(glBindVertexArray(Game -> VAO));

    glDrawArrays(GL_TRIANGLES, 0, BOARD_SIZE * 6);
}

unsigned char &Board(int x, int y)
{
    if (x < 0 || y < 0 || x >= 8 || y >= 8)
        return Game -> Trash;

    return Game -> FlagsTexture[x + y * BOARD_WIDTH];
}

void ChessUploadBoard()
{
    /* Just change the data on the GPU.                                       */
    EMBERS_GL(glBindTexture(GL_TEXTURE_2D, Game -> Textures[CHESS_TEXTURE_DATA]));
    EMBERS_GL(glTexSubImage2D(GL_TEXTURE_2D,
                              0,
                              0,
                              0,
                              BOARD_WIDTH,
                              BOARD_HEIGHT,
                              GL_RED,
                              GL_UNSIGNED_BYTE,
                              Game -> FlagsTexture));

}

void ChessSetCamera(EMBERS_REAL x, EMBERS_REAL y, EMBERS_REAL Zoom)
{
    EmbersSetUniformM4(Game -> Program,
                       "World",
                       Mat4::Translate({EMBERS_WIDTH / 2.f,
                                        EMBERS_HEIGHT / 2.f, 0.f}) *
                       Mat4::Scale(Zoom) *
                       Mat4::Translate({x, y, 0}));

}

void PrepareTexture(int Tex,
                    void *Data,
                    int Width,
                    int Height,
                    TextureType Type)
{
    EMBERS_GL(glBindTexture(GL_TEXTURE_2D, Tex));
    EMBERS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    EMBERS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    EMBERS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    EMBERS_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    EMBERS_GL(glTexImage2D(GL_TEXTURE_2D,
                           0,
                           Type == TEXTURE_THREE_CHANNELS ? GL_RGB : GL_R8,
                           Width,
                           Height,
                           0,
                           Type == TEXTURE_THREE_CHANNELS ? GL_RGB : GL_RED,
                           GL_UNSIGNED_BYTE,
                           Data));

    EMBERS_GL(glGenerateMipmap(GL_TEXTURE_2D));
}
