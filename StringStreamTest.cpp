#include "catch.hpp"
#include <core\lib\collection_types.h>

TEST_CASE("StringStream1", "[StringStream]") {
	ds::StringStream s;
	uint32_t i = 100;
	s << i;
	REQUIRE(strcmp("100",s.c_str()) == 0);
}

TEST_CASE("StringStreamV2", "[StringStream]") {
	ds::StringStream s;
	v2 v(100, 200);
	s << v;
	REQUIRE(strcmp("x: 100 y: 200", s.c_str()) == 0);
}

TEST_CASE("StringStreamV3", "[StringStream]") {
	ds::StringStream s;
	v3 v(100, 200, 300);
	s << v;
	REQUIRE(strcmp("x: 100 y: 200 z: 300", s.c_str()) == 0);
}

TEST_CASE("StringStreamP2I", "[StringStream]") {
	ds::StringStream s;
	p2i v(11, 22);
	s << v;
	REQUIRE(strcmp("x: 11 y: 22", s.c_str()) == 0);
}

TEST_CASE("StringStreamSeveral", "[StringStream]") {
	ds::StringStream s;
	int i = 100;
	p2i v(11, 22);
	v3 nv(1, 2, 3);
	s << "v: " << v << " i: " << i << " nv: " << nv;
	REQUIRE(strcmp("v: x: 11 y: 22 i: 100 nv: x: 1 y: 2 z: 3", s.c_str()) == 0);
}