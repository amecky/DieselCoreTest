#pragma comment(lib, "DieselCore.lib")
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <stdio.h>
#include <string\StaticHash.h>

TEST_CASE("Hash generation", "[StaticHash]") {
	StaticHash h = SID("Hello world");
	REQUIRE(h.get() == 1498229191);
}
