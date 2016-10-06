#include "..\catch.hpp"
#include <core\memory\DefaultAllocator.h>
#include <core\log\Log.h>
#include <core\Common.h>
#include <core\physics\ColliderArray.h>
#include <core\math\math.h>

TEST_CASE("Physics1", "[Physics]") {
	v2 p1(100, 100);
	v2 p2(160, 100);
	ds::Collision c;
	bool ret = ds::physics::testCircleIntersection(p1, 40.0f, p2, 40.0f, &c);
	REQUIRE(ret);
	REQUIRE(c.distance == 0.75f);
	REQUIRE(c.norm.x == 1.0f);
	REQUIRE(c.norm.y == 0.0f);
}

TEST_CASE("Physics2", "[Physics]") {
	v2 p1(100, 100);
	v2 p2(160, 100);
	ds::Collision c;
	bool ret = ds::physics::testCircleIntersection(p1, 20.0f, p2, 10.0f, &c);
	REQUIRE(!ret);
}

TEST_CASE("Physics_Quad1", "[Physics]") {
	v2 p1(100, 100);
	v2 p2(160, 100);
	v2 e1(80, 80);
	v2 e2(80, 80);
	ds::Collision c;
	bool ret = ds::physics::testBoxIntersection(p1, e1, p2, e2, &c);
	REQUIRE(ret);
	REQUIRE(c.norm.x == 1.0f);
	REQUIRE(c.norm.y == 0.0f);
}