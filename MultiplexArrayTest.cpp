#include "catch.hpp"
#include <core\lib\BlockArray.h>
#include <core\memory\DefaultAllocator.h>
#include <core\log\Log.h>
#include <core\Common.h>

TEST_CASE("MultiplexArray_Basic1", "[MultiplexArray]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::MultiplexArray* array = new ds::MultiplexArray(4);
	ID id = array->add();
	array->set(id, 0, v4(10.0f));
	v4 r = array->get(id, 0);
	REQUIRE(r.x == 10.0f);
	REQUIRE(array->size() == 1);
	delete array;
	delete ds::gDefaultMemory;
}

TEST_CASE("MultiplexArray_Basic2", "[MultiplexArray]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::MultiplexArray* array = new ds::MultiplexArray(4);
	ID id = array->add();
	array->set(id, 0, v4(10.0f));
	ID nid = array->add();
	array->set(nid, 0, v4(20.0f));
	array->remove(id);
	bool con = array->contains(id);
	REQUIRE(con == false);
	delete array;
	delete ds::gDefaultMemory;
}

TEST_CASE("MultiplexArray_Basic3", "[MultiplexArray]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::MultiplexArray* array = new ds::MultiplexArray(4);
	ID id = array->add();
	array->set(id, 0, v4(10.0f));
	ID nid = array->add();
	array->set(nid, 0, v4(20.0f));
	array->remove(id);
	bool con = array->contains(id);
	REQUIRE(con == false);
	v4 r = array->get(nid, 0);
	REQUIRE(r.x == 20.0f);
	delete array;
	delete ds::gDefaultMemory;
}

TEST_CASE("MultiplexArray_Basic4", "[MultiplexArray]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::MultiplexArray* array = new ds::MultiplexArray(4);
	ID ids[10];
	for (int i = 0; i < 10; ++i) {
		ids[i] = array->add();
		array->set(ids[i], 0, v4(i * 10.0f));
	}
	for (int i = 0; i < 5; ++i) {
		array->remove(ids[i * 2]);
	}
	v4* p = array->getPtr(0);
	for (int i = 0; i < array->size(); ++i) {
		printf("%d = %g\n", i, p[i].x);
	}
	delete array;
	delete ds::gDefaultMemory;
}