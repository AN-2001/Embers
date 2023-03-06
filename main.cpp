/******************************************************************************\
*  main.c                                                                      *
*                                                                              * 
*  It all starts here!!                                                        *
*                                                                              * 
*              Written by Abed Naran                          September 2022   *
*                                                                              *
\******************************************************************************/

#include "config.h"
#include "embers.h"
int main(int argc, const char *argv[])
{

    EMBERS_LOG_INFO(EMBERS_SPLASH_MSG);

	if (EmbersInit() != EMBERS_TRUE)
		return 1;

	EmbersLoop();
	return 0;
}
