#include <ag_debug.h>
#include <ag_layout.h>

void AG_Init_libantargisgui()
{
	TRACE;

	AGLayout::registerLayouts();
}

