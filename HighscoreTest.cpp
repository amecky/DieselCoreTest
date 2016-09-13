#include "catch.hpp"
#include <stdio.h>
#include <core\data\HighscoreService.h>

struct Highscore {

	int minutes;
	int seconds;
	int mode;

	Highscore() : seconds(99), minutes(99), mode(-1) {}

	Highscore(int m, int s, int md) : seconds(s), minutes(m), mode(md) {}

	int compare(const Highscore& other) const {
		int time = minutes * 60 + seconds;
		int other_time = other.minutes * 60 + other.seconds;
		if (time < other_time) {
			return -1;
		}
		if (time > other_time) {
			return 1;
		}
		return 0;
	}

};

TEST_CASE("HighscoreService_Basic", "[HighscoreService]") {
	ds::HighscoreService<Highscore> service;
	Highscore h(10, 10, 0);
	int ret = service.add(h);
	REQUIRE(ret == 0);
	Highscore n(12, 10, 0);
	int next = service.add(n);
	REQUIRE(next == 1);
}

TEST_CASE("HighscoreService_Second", "[HighscoreService]") {
	ds::HighscoreService<Highscore> service;
	Highscore h(10, 10, 0);
	int ret = service.add(h);
	REQUIRE(ret == 0);
	Highscore n(9, 10, 0);
	int next = service.add(n);
	REQUIRE(next == 0);
}

TEST_CASE("HighscoreService_Third", "[HighscoreService]") {
	ds::HighscoreService<Highscore> service;
	for (int i = 0; i < 12; ++i) {
		Highscore h(100 - i, 10, 0);
		int ret = service.add(h);
		REQUIRE(ret == 0);
	}
	REQUIRE(10 == service.size());
}

TEST_CASE("HighscoreService_Load", "[HighscoreService]") {
	ds::HighscoreService<Highscore> service;
	service.load("Test.scr");
	REQUIRE(10 == service.size());
}

TEST_CASE("HighscoreService_LoadInsert", "[HighscoreService]") {
	ds::HighscoreService<Highscore> service;
	service.load("Test.scr");
	for (int i = 0; i < service.size(); ++i) {
		const Highscore& current = service.get(i);
		printf("%d = %d:%d\n", i, current.minutes, current.seconds);
	}
	REQUIRE(10 == service.size());
	Highscore h(90, 20, 0);
	int ret = service.add(h);
	REQUIRE(ret == 2);
	Highscore n(100, 20, 0);
	int next = service.add(n);
	REQUIRE(next == -1);
}