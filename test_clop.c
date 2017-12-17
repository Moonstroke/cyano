
#include "clop.h"
#include "log.h"

int main(const int argc, const char *const argv[]) {

	unsigned int w = 0, h = 0, c = 0;
	setvars(&w, &h, &c);

	if(!getvals(argc, argv, OPTSTRING, LONGOPTS))
		error("An error happened");

	debug("\nw = %u, h = %u, c = %u", w, h, c);


	return 0;
}
