#include <ag_debug.h>

#include <minimap.h>

AGEXPORT void AG_Init_libantargisgame()
{
	TRACE;

	registerMinimapCreator();
}

