#include "catch.hpp"
#include <core\memory\DefaultAllocator.h>
#include <core\log\Log.h>
#include <core\Common.h>
#include <core\world\World.h>
#include <core\math\math.h>

TEST_CASE("Collision1", "[World]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::World* w = new ds::World();
	ID id = w->create(v2(100, 100), math::buildTexture(0, 0, 32, 32), 1);
	w->attachCollider(id, ds::PST_CIRCLE, v2(20.0f, 0.0f));
	w->moveBy(id, v3(50, 0, 0));
	ID sid = w->create(v2(200, 100), math::buildTexture(0, 0, 32, 32), 2);
	w->attachCollider(sid, ds::PST_CIRCLE, v2(20.0f, 0.0f));
	w->moveBy(sid, v3(-40, 0, 0));
	w->tick(1.0f);
	v3 fp = w->getPosition(id);
	REQUIRE(fp.x == 150.0f);
	v3 sp = w->getPosition(sid);
	REQUIRE(sp.x == 160.0f);
	REQUIRE(w->hasCollisions());
	delete w;
	delete ds::gDefaultMemory;

}

TEST_CASE("Collision2", "[World]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::World* w = new ds::World();
	ID id = w->create(v2(100, 100), math::buildTexture(0, 0, 32, 32), 1);
	w->attachCollider(id, ds::PST_CIRCLE, v2(20.0f, 0.0f));
	w->moveBy(id, v3(10, 0, 0));
	ID sid = w->create(v2(200, 100), math::buildTexture(0, 0, 32, 32), 2);
	w->attachCollider(sid, ds::PST_CIRCLE, v2(20.0f, 0.0f));
	w->moveBy(sid, v3(-10, 0, 0));
	w->tick(1.0f);
	v3 fp = w->getPosition(id);
	REQUIRE(fp.x == 110.0f);
	v3 sp = w->getPosition(sid);
	REQUIRE(sp.x == 190.0f);
	REQUIRE(!w->hasCollisions());
	delete w;
	delete ds::gDefaultMemory;

}

TEST_CASE("Collision3", "[World]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	ds::World* w = new ds::World();
	ID id = w->create(v2(100, 100), math::buildTexture(0, 0, 32, 32), 1);
	v3 v1 = v3(50, 0, 0);
	w->attachCollider(id, ds::PST_CIRCLE, v2(20.0f, 0.0f));
	w->moveBy(id, v1);
	ID sid = w->create(v2(200, 100), math::buildTexture(0, 0, 32, 32), 2);
	w->attachCollider(sid, ds::PST_CIRCLE, v2(20.0f, 0.0f));
	v3 v2 = v3(-40, 0, 0);
	w->moveBy(sid, v2);
	w->tick(1.0f);
	v3 fp = w->getPosition(id);
	REQUIRE(fp.x == 150.0f);
	v3 sp = w->getPosition(sid);
	REQUIRE(sp.x == 160.0f);
	v3 d = sp - fp;
	v3 nd = normalize(d);

	float mdt = dot(v1, nd);
	v3 ref1 = v1 - 2.0f * mdt * nd;
	w->moveBy(id,ref1);
	mdt = dot(v2, nd);
	v3 ref2 = v2 - 2.0f * mdt * nd;
	w->moveBy(sid, ref2);
	v3 np1 = w->getPosition(id) - d * 0.5f;
	w->setPosition(id, np1.xy());
	v3 np2 = w->getPosition(sid) + d * 0.5f;
	w->setPosition(sid, np2.xy());
	w->tick(1.0f);
	REQUIRE(!w->hasCollisions());
	v3 nfp = w->getPosition(id);
	REQUIRE(nfp.x == 95.0f);
	delete w;
	delete ds::gDefaultMemory;

}