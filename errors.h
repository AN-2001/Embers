/******************************************************************************\
*  errors.h                                                                    *
*                                                                              * 
*  Handles errors throughout the program.                                      *
*                                                                              *
*              Written by Abed Naran                          September 2022   *
*                                                                              *
\******************************************************************************/

#ifndef EMBERS_ERRORS_H
#define EMBERS_ERRORS_H
#include "embers.h"

/* Externs from embers.c for error handling                                   *
 * EmbersExit can be set to request an exit.                                  */
extern int EmbersExit;
extern EmbersStatus EmbersErrno;
extern int EmbersErrLn;
extern const char *EmbersErrFl;
extern const char *EmbersErrFun;
#define GL_TO_EMBERS(Err) ((EmbersStatus) ((Err - 0x500) | EMBERS_GL_ERROR))

/* Check whether Err is an OpenGL Error.                                      */
#define EMBERS_IS_GL(Err) ((Err & EMBERS_GL_ERROR) != 0)
#ifdef EMBERS_DEBUG
/* Checked gl call to make error handling easier.                             */
#define EMBERS_GL(Function) do {                                               \
                                Function;                                      \
                                GLenum Err = glGetError();                     \
                                if (Err)                                       \
									EMBERS_ERROR(GL_TO_EMBERS(Err));           \
							} while(0)                     
#else

/* Work normally if we're not in debug.                                       */
#define EMBERS_GL(Function) Function;
#endif 

/* Abstract stride and offset calculation for a given type.                   */
#define EMBERS_OFFSET(T, M) ((unsigned char*)&(((T*)NULL) -> M) -              \
                             (unsigned char*)((T*)NULL))                       \

#define EMBERS_STRIDE(T) (sizeof(T))

/* Set errno with the proper line and file info.                              */
#define EMBERS_ERROR(Error) do{                                                \
                              EmbersErrno = Error;                             \
                              EmbersErrFl = __FILE__;                          \
							  EmbersErrLn = __LINE__;                          \
							  EmbersErrFun = __FUNCTION__;                     \
                              EmbersExit = EMBERS_TRUE;                        \
                            } while(0);                                        \


/* Returns true if Embers is in a bad state and should exit.                  */
#define EMBERS_IS_BAD_STATE() (EmbersExit)

#endif /* EMBERS_ERRORS_H */
