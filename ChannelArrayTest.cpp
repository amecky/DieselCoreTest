#include "catch.hpp"
#include <core\lib\BlockArray.h>
#include <core\memory\DefaultAllocator.h>
#include <core\log\Log.h>
#include <core\Common.h>

TEST_CASE("ChannelArray_Basic1", "[ChannelArray]") {
	init_logger();
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::ChannelArray* array = new ds::ChannelArray();
	int sizes[] = { sizeof(int), sizeof(int), sizeof(float) };
	array->init(sizes, 3);
	ID id = array->add();
	array->set(id, 0, 100);
	array->set(id, 1, 200);
	array->set(id, 2, 1.23f);
	int r = array->get<int>(id, 0);
	REQUIRE(r == 100);
	REQUIRE(array->get<int>(id, 1) == 200);
	REQUIRE(array->get<float>(id, 2) == 1.23f);
	delete array;
	delete ds::gDefaultMemory;
}

TEST_CASE("ChannelArray_Basic2", "[ChannelArray]") {
	init_logger();
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::ChannelArray* array = new ds::ChannelArray();
	int sizes[] = { sizeof(int), sizeof(int), sizeof(float) };
	array->init(sizes, 3);
	ID ids[10];
	for (int i = 0; i < 10; ++i) {
		ids[i] = array->add();
		array->set(ids[i], 0, i * 10);
	}
	int* d1 = (int*)array->get_ptr(0);
	for (int i = 0; i < 5; ++i) {
		array->remove(ids[i * 2]);
	}
	ID tm = array->add();
	array->set(tm, 0, 200);
	int expected[] = { 90, 10, 50, 30, 70, 200 };
	int* d = (int*)array->get_ptr(0);
	for (int i = 0; i < array->size; ++i) {
		REQUIRE(expected[i] ==  d[i]);
	}
	delete array;
	delete ds::gDefaultMemory;
}

TEST_CASE("ChannelArray_Basic3", "[ChannelArray]") {
	init_logger();
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::ChannelArray* array = new ds::ChannelArray();
	int sizes[] = { sizeof(int), sizeof(int), sizeof(float) };
	array->init(sizes, 3);
	ID ids[3];
	for (int i = 0; i < 3; ++i) {
		ids[i] = array->add();
		array->set(ids[i], 0, i * 10);
		array->set(ids[i], 1, i * 100);
		array->set(ids[i], 2, i * 1.2f);
	}
	for (int i = 0; i < 10; ++i) {
		array->add();
	}
	REQUIRE(array->get<int>(ids[1], 0) == 10);
	REQUIRE(array->get<int>(ids[1], 1) == 100);
	REQUIRE(array->get<float>(ids[1], 2) == 1.2f);
	delete array;
	delete ds::gDefaultMemory;
}

TEST_CASE("ChannelArray_Basic4", "[ChannelArray]") {
	init_logger();
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::ChannelArray* array = new ds::ChannelArray();
	int sizes[] = { sizeof(int), sizeof(ds::Rect), sizeof(int) };
	array->init(sizes, 3);
	ID ids[10];
	for (int i = 0; i < 10; ++i) {
		ids[i] = array->add();
		array->set(ids[i], 0, i * 10);
		array->set(ids[i], 1, ds::Rect(10.0f + i * 20.0f,100.0f + i * 20.0f,32.0f,32.0f));
		array->set(ids[i], 2, 100 + i * 10);
	}
	int expected[] = { 90, 10, 50, 30, 70, 200 };
	int* d = (int*)array->get_ptr(0);
	ds::Rect* t = (ds::Rect*)array->get_ptr(1);
	for (int i = 0; i < array->size; ++i) {
		printf("%d = %d - %g %g %g %g\n", i, d[i], t[i].top, t[i].left, t[i].bottom, t[i].right);
		//REQUIRE(expected[i] == d[i]);
	}
	delete array;
	delete ds::gDefaultMemory;
}
