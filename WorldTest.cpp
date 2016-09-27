#include "catch.hpp"
#include <core\io\FileUtils.h>
#include <core\memory\DefaultAllocator.h>
#include <core\log\Log.h>
#include <core\Common.h>
#include <core\world\World.h>

TEST_CASE("World_Basic1", "[World]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::World* w = new ds::World(4);
	ID id = w->create();
	w->scale(id, 0, v4(0.0f), v4(1.0f), 1.0f, 0);
	w->tick(0.25f);
	v4* c = w->getChannel(0);
	REQUIRE(0.25f == c[0].x);
	delete w;
	delete ds::gDefaultMemory;

}