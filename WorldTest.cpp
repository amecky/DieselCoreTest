#include "catch.hpp"
#include <core\io\FileUtils.h>
#include <core\memory\DefaultAllocator.h>
#include <core\log\Log.h>
#include <core\Common.h>
#include <core\world\World.h>
#include <core\math\math.h>
#include <core\profiler\Profiler.h>
#include "utils.h"

struct AddDataTest {
	int value;
};

TEST_CASE("World_Basic1", "[World]") {
	ds::World* w = utils::initialize();
	ID id = w->create();
	w->scale(id, v3(0.0f), v3(1.0f), 1.0f, 0);
	w->tick(0.25f);
	ds::ChannelArray* array = w->getChannelArray();
	v3* c = (v3*)array->get_ptr(ds::WEC_SCALE);
	REQUIRE(0.25f == c[0].x);
	utils::shutdown(w);
}

TEST_CASE("World_Basic2", "[World]") {
	ds::World* w = utils::initialize();
	ID ids[10];
	for (int i = 0; i < 10; ++i) {
		ids[i] = w->create(v2(100 + i * 20,200),math::buildTexture(i * 10,100 + i * 20,32,32),i);
	}
	ds::ChannelArray* array = w->getChannelArray();
	v3* c = (v3*)array->get_ptr(ds::WEC_POSITION);
	ds::Texture* t = (ds::Texture*)array->get_ptr(ds::WEC_TEXTURE);
	int* tp = (int*)array->get_ptr(ds::WEC_TYPE);
	for (int i = 0; i < array->size; ++i) {
		REQUIRE(100 + i * 20 == c[i].x);
		REQUIRE(i * 10 == t[i].rect.top);
		REQUIRE(i == tp[i]);
	}
	utils::shutdown(w);
}

TEST_CASE("World_AddData", "[World]") {
	ds::World* w = utils::initialize();
	ID ids[10];
	for (int i = 0; i < 10; ++i) {
		ids[i] = w->create(v2(100 + i * 20, 200), math::buildTexture(i * 10, 100 + i * 20, 32, 32), i);
		AddDataTest* t = (AddDataTest*)w->attach_data(ids[i], sizeof(AddDataTest), 100);
		t->value = i;
	}
	ds::ChannelArray* array = w->getChannelArray();
	v3* c = (v3*)array->get_ptr(ds::WEC_POSITION);
	ds::Texture* t = (ds::Texture*)array->get_ptr(ds::WEC_TEXTURE);
	int* tp = (int*)array->get_ptr(ds::WEC_TYPE);
	for (int i = 0; i < array->size; ++i) {
		REQUIRE(100 + i * 20 == c[i].x);
		REQUIRE(i * 10 == t[i].rect.top);
		REQUIRE(i == tp[i]);
	}
	AddDataTest* tmp = (AddDataTest*)w->get_data(ids[5]);
	REQUIRE(tmp->value == 100);
	utils::shutdown(w);
}