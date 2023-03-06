/******************************************************************************\
*  program.c                                                                   *
*                                                                              * 
*  Handles OpenGL programs                                                     *
*                                                                              *
*              Written by Abed Naran                          September 2022   *
*                                                                              *
\******************************************************************************/
#include "config.h"
#include "embers.h"
#include "glad/glad.h"
#include "program.h"
#include "errors.h"

EmbersProgram EmbersCreateProgram()
{
	EmbersProgram Program;
	Program.Handle = glCreateProgram();

	/* Check for failure.                                                     */
	if (Program.Handle == -1) {
		EMBERS_ERROR(EMBERS_FAILURE);	
		return Program;
	}
	return Program;
}

void EmbersSetAttribLocation(EmbersProgram Program, int ID, const char *Attrib)
{
    glBindAttribLocation(Program.Handle, ID, Attrib);
}

int EmbersLinkProgram(EmbersProgram Program)
{
	/* Try to link.                                                           */
	EMBERS_GL(glLinkProgram(Program.Handle));
	/* Get the status of the link.                                            */
	int Status;
	EMBERS_GL(glGetProgramiv(Program.Handle, GL_LINK_STATUS, &Status));

	/* If we failed output the link error then exit the function.             */
	if (!Status) {
		char Msg[EMBERS_BUFFER_SIZE];
		EMBERS_GL(glGetProgramInfoLog(Program.Handle, EMBERS_BUFFER_SIZE, NULL, Msg));
		sprintf(Msg, "Link error:\n %s", Msg);
        EMBERS_LOG_ERROR(Msg);
		return EMBERS_FAILURE;
	}
	return EMBERS_TRUE;
}

void EmbersSetActiveProgram(EmbersProgram Program)
{
	EMBERS_GL(glUseProgram(Program.Handle));
}

void EmbersFreeProgram(EmbersProgram Program)
{
	EMBERS_GL(glDeleteProgram(Program.Handle));
}

void EmbersBindShader(EmbersProgram Program, EmbersShader Shader)
{
	EMBERS_GL(glAttachShader(Program.Handle, Shader.Handle));
}

void EmbersUnsetProgram()
{
	EMBERS_GL(glUseProgram(0));
}

void EmbersSetUniformM4(EmbersProgram Program, const char *Name, Mat4 Value)
{

    int Uniform = glGetUniformLocation(Program.Handle, Name);
    float mat[4][4];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] = Value[i][j];

    glUniformMatrix4fv(Uniform, 1, GL_FALSE, (const float*)mat);

}

void EmbersSetUniformV3(EmbersProgram Program, const char *Name, Vec3 Value)
{
    int Uniform = glGetUniformLocation(Program.Handle, Name);
    float vec[3] = {(float)Value.x, (float)Value.y, (float)Value.z};
    EMBERS_GL(glUniform3fv(Uniform, 1, vec));

}

void EmbersSetUniformI(EmbersProgram Program, const char *Name, int Value)
{
    int Uniform = glGetUniformLocation(Program.Handle, Name);
    EMBERS_GL(glUniform1i(Uniform, Value));
}

void EmbersSetUniformV4(EmbersProgram Program, const char *Name, float Value)
{

}
