/******************************************************************************\
*  shader.h                                                                    *
*                                                                              * 
*  Handles OpenGL shaders.                                                     *
*                                                                              * 
*              Written by Abed Naran                          September 2022   *
*                                                                              *
\******************************************************************************/
#ifndef EMBERS_SHADER_H
#define EMBERS_SHADER_H
#include <GL/gl.h>

typedef struct EmbersShader {
	int Handle;
	GLenum Type;
} EmbersShader;

/******************************************************************************\
* LoadShader                                                                   *
*                                                                              *
*  Loads the OpenGL shader in Path, compiles it and returns the Shader struct. *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Path: The path of the shader file.                                         *
*  -Type: the type of the shader.                                              *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -EmbersShader: the loaded shader.                                                 *
*                                                                              *
\******************************************************************************/
EmbersShader EmbersLoadShader(const char *Path, GLenum Type);

/******************************************************************************\
* EmbersFreeShader                                                           *
*                                                                              *
*  Free Shader from the OpenGL context.                                        *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Shader: the shader to be freed.                                            *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  Void.                                                                       *
*                                                                              *
\******************************************************************************/
void EmbersFreeShader(EmbersShader Shader);

#endif /* EMBERS_SHADER_H */
