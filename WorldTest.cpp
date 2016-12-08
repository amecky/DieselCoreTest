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

struct MoreAddDataTest {
	int first;
	float second;
	int third;
	float fourth;
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
	REQUIRE(tmp->value == 5);
	utils::shutdown(w);
}

TEST_CASE("World_FindByType", "[World]") {
	ds::World* w = utils::initialize();
	ID ids[10];
	for (int i = 0; i < 10; ++i) {
		int type = i % 4;
		ids[i] = w->create(v2(100 + i * 20, 200), math::buildTexture(i * 10, 100 + i * 20, 32, 32), type);
	}
	ID ret[10];
	int num = w->find_by_type(2, ret, 10);
	REQUIRE(num == 2);
	REQUIRE(ret[0] == 2);
	REQUIRE(ret[1] == 6);
	utils::shutdown(w);
}

TEST_CASE("World_FindByType2", "[World]") {
	ds::World* w = utils::initialize();
	ID ids[10];
	for (int i = 0; i < 10; ++i) {
		int type = i % 4;
		ids[i] = w->create(v2(100 + i * 20, 200), math::buildTexture(i * 10, 100 + i * 20, 32, 32), type);
	}
	w->remove(ids[2]);
	ID ret[10];
	int num = w->find_by_type(2, ret, 10);
	REQUIRE(num == 1);
	REQUIRE(ret[0] == 6);
	utils::shutdown(w);
}

TEST_CASE("World_AddData2", "[World]") {
	ds::World* w = utils::initialize();
	ID ids[10];
	for (int i = 0; i < 10; ++i) {
		int type = i % 4;
		ids[i] = w->create(v2(100 + i * 20, 200), math::buildTexture(i * 10, 100 + i * 20, 32, 32), type);
		if (i % 2 == 0) {
			AddDataTest* at = (AddDataTest*)w->attach_data(ids[i], sizeof(AddDataTest), 1);
			at->value = 100 + i * 10;
		}
		else {
			MoreAddDataTest* mt = (MoreAddDataTest*)w->attach_data(ids[i], sizeof(MoreAddDataTest), 2);
			mt->first = 1 + i * 10;
			mt->second = 2.0f;
			mt->third = 3;
			mt->fourth = 4.0f;
		}
	}
	w->remove(ids[2]);
	ID ret[10];
	int num = w->find_by_type(2, ret, 10);
	REQUIRE(num == 1);
	REQUIRE(ret[0] == 6);
	MoreAddDataTest* tmp = (MoreAddDataTest*)w->get_data(ids[1]);
	REQUIRE(tmp->first == 11);
	utils::shutdown(w);
}

TEST_CASE("World_AddData3", "[World]") {
	LOG << "=====> World_AddData3";
	ds::World* w = utils::initialize();
	ID ids[10];
	for (int i = 0; i < 10; ++i) {
		int type = i % 4;
		ids[i] = w->create(v2(100 + i * 20, 200), math::buildTexture(i * 10, 100 + i * 20, 32, 32), type);
		if (i % 2 == 0) {
			AddDataTest* at = (AddDataTest*)w->attach_data(ids[i], sizeof(AddDataTest), 1);
			at->value = 100 + i * 10;
		}
		else {
			MoreAddDataTest* mt = (MoreAddDataTest*)w->attach_data(ids[i], sizeof(MoreAddDataTest), 2);
			mt->first = 1 + i * 10;
			mt->second = 2.0f;
			mt->third = 3;
			mt->fourth = 4.0f;
		}
	}
	w->remove(ids[2]);
	ID nid = w->create(v2(500,500), math::buildTexture(0, 100, 32, 32), 2);
	REQUIRE(nid == 2);
	MoreAddDataTest* mt = (MoreAddDataTest*)w->attach_data(nid, sizeof(MoreAddDataTest), 2);
	mt->first = 111;
	mt->second = 2.1f;
	mt->third = 13;
	mt->fourth = 4.2f;
	ID ret[10];
	int num = w->find_by_type(2, ret, 10);
	REQUIRE(num == 2);
	REQUIRE(ret[0] == 2);
	MoreAddDataTest* tmp = (MoreAddDataTest*)w->get_data(ids[1]);
	REQUIRE(tmp->first == 11);
	ds::ReportWriter writer("World.html");
	w->saveReport(writer);
	utils::shutdown(w);
}