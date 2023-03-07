/******************************************************************************\
*  shader.c                                                                    *
*                                                                              * 
*  Handles OpenGL shaders.                                                     *
*                                                                              * 
*              Written by Abed Naran                          September 2022   *
*                                                                              *
\******************************************************************************/

#include "config.h"
#include "embers.h"
#include "glad/glad.h"
#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

/* Tries to compile the shader, returns accordingly.                          */
static int TryCompile(int Handle, const char *Text);

/* Extracts all the data from the file in Path.                               */
static char *ReadFile(const char *Path);

static int TryCompile(int Handle, const char *Path)
{
	char *Text = ReadFile(Path);
	EMBERS_GL(glShaderSource(Handle,
				             1,
				             (const char *const *)&Text,
				             NULL));

	/* We don't need the shader text anymore.                                 */
	free(Text);
	int Success;
	EMBERS_GL(glCompileShader(Handle));
	glGetShaderiv(Handle, GL_COMPILE_STATUS, &Success);
	if (!Success){
		char Msg0[EMBERS_BUFFER_SIZE],
		     Msg1[EMBERS_BUFFER_SIZE_BIG];

		glGetShaderInfoLog(Handle, EMBERS_BUFFER_SIZE, NULL, Msg0);
		snprintf(Msg1,
                 EMBERS_BUFFER_SIZE_BIG, 
                 "Syntax Error in %s\n %s\n",
                 Path,
                 Msg0);

        EMBERS_LOG_ERROR(Msg1);
		EMBERS_ERROR(EMBERS_CANT_COMPILE_SHADER);

		return EMBERS_FALSE;
	}

	return EMBERS_TRUE;
}

static char *ReadFile(const char *Path)
{
    FILE *File;
    size_t Size;
    char *Ret;

	File = fopen(Path, "r");

	fseek(File, 0, SEEK_END);
	Size = ftell(File);
	fseek(File, 0, SEEK_SET);

	Ret = (char*)malloc(Size + 1);

	fread(Ret, Size, 1, File);
	Ret[Size] = '\0';

	fclose(File);
	return Ret;
}

EmbersShader EmbersLoadShader(const char *Path, GLenum Type)
{
	EmbersShader Shader;
	Shader.Handle = glCreateShader(Type);
	if (Shader.Handle == -1) {
		/* Unexpected error if we can't create a shader.                      */
		EMBERS_ERROR(EMBERS_FAILURE);
		return Shader;
	}

	Shader.Type = Type;
	/* Try to compile the shader in Path.                                     */
	if (TryCompile(Shader.Handle, Path) != EMBERS_TRUE) {
	/* Make the shader invalid if we failed.                                  */
        EmbersFreeShader(Shader);
		return Shader;
	}

	return Shader;
}


void EmbersFreeShader(EmbersShader Shader)
{
	EMBERS_GL(glDeleteShader(Shader.Handle));
}

