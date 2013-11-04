#include <freevic.h>

extern int global_debug;

void debug_printf(char *fmt, ...) {
	
	va_list args;

	if(global_debug == 0)
		return;

	va_start(args, fmt);
	vprintf(fmt, args);
	
	va_end(args);
}
