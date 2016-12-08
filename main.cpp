//#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <core\log\Log.h>
#include <core\profiler\Profiler.h>

int main(int argc, char* const argv[]) {
	// global setup...	
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	init_logger();
	perf::init();
	Catch::Session session;
	int result = session.run(argc, argv);
	perf::shutdown();
	// global clean-up...
	delete ds::gDefaultMemory;
	return result;
}