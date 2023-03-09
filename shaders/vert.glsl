#version 140

#define TEXTURE_WIDTH (360.f)
#define TEXTURE_HEIGHT (120.f)
#define CELL_WIDTH (60.f)
#define CELL_HEIGHT (60.f)

#define BOARD_WIDTH (8.f)
#define BOARD_HEIGHT (8.f)

#define FLAG_HIGHLIGHTED (0x1)
#define FLAG_WHITE (0x2)
#define FLAG_BLACK (0x4)

#define GRID_COL1 vec3(118,150,86)
#define GRID_COL2 vec3(238,238,210)

uniform sampler2D DataTexture;

in int ID;
in int Corner;
in vec2 Position;

out vec3 fColour;
out vec2 uv;

uniform mat4 World;
uniform mat4 Projection;
uniform mat4 View;


bool CheckFlag(int Type, int Flags)
{
    Type = Type & Flags;

    return Type != 0;
}

void SetUVs(int Piece, int Team)
{
    vec2 
        Offset = vec2(CELL_WIDTH / TEXTURE_WIDTH, CELL_HEIGHT / TEXTURE_HEIGHT),
        TopLeft = vec2(Piece, Team) * Offset;

    if (Corner == 0)
        uv = TopLeft;

    if (Corner == 1)
        uv = TopLeft + Offset * vec2(1.f, 0.f);

    if (Corner == 2)
        uv = TopLeft + Offset * vec2(0.f, 1.f);

    if (Corner == 3)
        uv = TopLeft + Offset * vec2(1.f, 1.f);
}

void main(){
    int x = ID % int(BOARD_WIDTH),
        y = ID / int(BOARD_HEIGHT),
        Diag =  x + y,
        Tst = int(texture(DataTexture, vec2(x / BOARD_WIDTH, y / BOARD_HEIGHT)).r * 255);


    vec3 WorldPosition = (vec4(Position, 0.f, 1.f) * World).xyz;
    fColour = Diag % 2 == 0 ? GRID_COL1 / 255.f : GRID_COL2 / 255.f;

    if (CheckFlag(Tst, FLAG_HIGHLIGHTED))
        fColour *= vec3(0.3, 0.1, 0.85);

    if (CheckFlag(Tst, FLAG_WHITE)) {
        SetUVs(Tst >> 4, 1);
    } else if (CheckFlag(Tst, FLAG_BLACK)) {
        SetUVs(Tst >> 4, 0);
    } else
        uv = vec2(-1.f);

	gl_Position = vec4(WorldPosition, 1.f) * View * Projection;
}
