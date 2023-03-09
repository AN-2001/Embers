#version 140

uniform sampler2D PieceTexture;

in vec3 fColour;
in vec2 uv;
out vec4 OutCol;
void main(){
    vec3 Colour;
    bool UseTexture = uv.x != -1.f;
    vec3 Tex = texture(PieceTexture, uv).xyz;

    Colour = fColour;

    if (UseTexture && Tex != vec3(1.f, 0.f, 0.f))
        Colour = Tex;


    OutCol = vec4(Colour, 1.f);

}
