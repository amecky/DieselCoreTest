#pragma comment(lib, "DieselCore.lib")
#include <stdio.h>
#include <core\string\StaticHash.h>
#include "catch.hpp"

TEST_CASE("Hash generation", "[StaticHash]") {
	StaticHash h = SID("Hello world");
	REQUIRE(h.get() == 1498229191);
}
