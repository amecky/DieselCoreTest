#include "..\catch.hpp"
#include <core\string\StringUtils.h>

TEST_CASE("StringFind", "[StringUtils]") {
	int d = ds::string::find("Hello world", "wor");
	REQUIRE(d == 6);
	d = ds::string::find("Hello world", "abc");
	REQUIRE(d == -1);
	d = ds::string::find("Hello world", "Hello world");
	REQUIRE(d == 0);
	d = ds::string::find("Hello", "Hello world");
	REQUIRE(d == -1);
}