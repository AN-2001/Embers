/******************************************************************************\
*  program.h                                                                   *
*                                                                              * 
*  Handles OpenGL programs                                                     *
*                                                                              *
*              Written by Abed Naran                          September 2022   *
*                                                                              *
\******************************************************************************/

#ifndef EMBERS_PROGRAM_H
#define EMBERS_PROGRAM_H

#include "shader.h"
#include <mat4.h>
#include <vec3.h>

typedef struct EmbersProgram {
	int Handle;
} EmbersProgram;

/******************************************************************************\
* EmbersCreateProgram                                                          *
*                                                                              *
*  Create an OpenGL program.                                                   *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Program: creates a new program.                                            *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  EmbersProgram: the new program.                                             *
*                                                                              *
\******************************************************************************/
EmbersProgram EmbersCreateProgram();

/******************************************************************************\
* EmbersSetActiveProgram                                                       *
*                                                                              *
*  Set Program to the active OpenGL program.                                   *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Program: the program to activate.                                          *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  void.                                                                       *
*                                                                              *
\******************************************************************************/
void EmbersSetActiveProgram(EmbersProgram Program);

/******************************************************************************\
* EmbersFreeProgram                                                            *
*                                                                              *
*  Frees the program.                                                          *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Program: the program to free.                                              *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  void.                                                                       *
*                                                                              *
\******************************************************************************/
void EmbersFreeProgram(EmbersProgram Program);


/******************************************************************************\
* EmbersLinkProgram                                                            *
*                                                                              *
*  Links the program.                                                          *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Program: the program to link.                                              *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  int: the status of the link.                                                *
*                                                                              *
\******************************************************************************/
int EmbersLinkProgram(EmbersProgram Program);

/******************************************************************************\
* EmbersBindShader                                                             *
*                                                                              *
*  Binds the shader to the program.                                            *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Program: The program to bind to.                                           *
*  -Shader: The sahder to use.                                                 *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  void.                                                                       *
*                                                                              *
\******************************************************************************/
void EmbersBindShader(EmbersProgram Program, EmbersShader Shader);

/******************************************************************************\
* EmbersSetAttribLocation                                                      *
*                                                                              *
*  Set the location of a program attribute.                                    *
*  NOTE: This should be called BEFORE linking.                                 *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -Program: The program.                                                      *
*  -ID: The location of the attribute.                                         *
*  -Attrib: The name of the attribute.                                         *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Void.                                                                      *
*                                                                              *
\******************************************************************************/
void EmbersSetAttribLocation(EmbersProgram Program, int ID, const char *Attrib);

/******************************************************************************\
* EmbersSetUniformM4                                                           *
*                                                                              *
*  Sets a mat4 uniform.                                                        *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  void.                                                                       *
*                                                                              *
\******************************************************************************/
void EmbersSetUniformM4(EmbersProgram Program, const char *Name, Mat4 Value);

/******************************************************************************\
* EmbersSetUniformV3                                                           *
*                                                                              *
*  Sets a Vec3 uniform.                                                        *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  void.                                                                       *
*                                                                              *
\******************************************************************************/
void EmbersSetUniformV3(EmbersProgram Program, const char *Name, Vec3 Value);

/******************************************************************************\
* EmbersSetUniformF                                                            *
*                                                                              *
*  Sets a float uniform.                                                       *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  void.                                                                       *
*                                                                              *
\******************************************************************************/
void EmbersSetUniformF(EmbersProgram Program, const char *Name, float Value);

/******************************************************************************\
* EmbersSetUniformF                                                            *
*                                                                              *
*  Sets an int uniform.                                                        *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  void.                                                                       *
*                                                                              *
\******************************************************************************/
void EmbersSetUniformI(EmbersProgram Program, const char *Name, int Value);

/******************************************************************************\
* EmbersUnsetProgram                                                           *
*                                                                              *
*  Sets the currently active program to 0.                                     *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  void.                                                                       *
*                                                                              *
\******************************************************************************/
void EmbersUnsetProgram();

#endif /* EMBERS_PROGRAM_H */

