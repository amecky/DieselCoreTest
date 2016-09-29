#include "catch.hpp"
#include <core\io\FileUtils.h>
#include <core\memory\DefaultAllocator.h>
#include <core\log\Log.h>
#include <core\Common.h>
#include <core\world\World.h>
#include <core\math\math.h>

TEST_CASE("World_Basic1", "[World]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::World* w = new ds::World();
	ID id = w->create();
	w->scale(id, v3(0.0f), v3(1.0f), 1.0f, 0);
	w->tick(0.25f);
	ds::ChannelArray* array = w->getChannelArray();
	v3* c = (v3*)array->get_ptr(ds::WEC_SCALE);
	REQUIRE(0.25f == c[0].x);
	delete w;
	delete ds::gDefaultMemory;

}

TEST_CASE("World_Basic2", "[World]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::World* w = new ds::World();
	ID ids[10];
	for (int i = 0; i < 4; ++i) {
		// const v2& pos, const Texture& texture, int type
		ids[i] = w->create(v2(100 + i * 20,200),math::buildTexture(i * 10,100 + i * 20,32,32),1);
	}
	
	ds::ChannelArray* array = w->getChannelArray();
	v3* c = (v3*)array->get_ptr(ds::WEC_POSITION);
	ds::Texture* t = (ds::Texture*)array->get_ptr(ds::WEC_TEXTURE);
	for (int i = 0; i < array->size; ++i) {
		printf("%d = %g %g %g %g\n", i, t[i].rect.top, t[i].rect.left, t[i].rect.bottom, t[i].rect.right);
		REQUIRE(100 + i * 20 == c[i].x);
	}
	delete w;
	delete ds::gDefaultMemory;

}