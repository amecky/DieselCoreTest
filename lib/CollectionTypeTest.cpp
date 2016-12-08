#include "..\catch.hpp"
#include <core\lib\collection_types.h>

TEST_CASE("ArrayBasic1", "[Array]") {
	ds::Array<int> ar;
	ar.push_back(100);
	ar.push_back(200);
	REQUIRE(ar.size() == 2);
}

TEST_CASE("ArrayBasicIter", "[Array]") {
	ds::Array<int> ar;
	ar.push_back(100);
	ar.push_back(200);
	ar.push_back(300);
	int v[] = { 100, 200, 300 };
	ds::Array<int>::iterator it = ar.begin();
	int cnt = 0;
	while (it != ar.end()) {
		REQUIRE(*it == v[cnt]);
		++cnt;
		++it;
	}
}

TEST_CASE("ArrayBasicIter2", "[Array]") {
	ds::Array<int> ar;
	ar.push_back(100);
	ar.push_back(200);
	ar.push_back(300);
	ar.push_back(200);
	ar.push_back(400);
	ar.push_back(200);
	ar.push_back(500);
	ds::Array<int>::iterator nit = ar.begin();
	while (nit != ar.end()) {
		if (*nit == 200) {
			nit = ar.remove(nit);
		}
		else {
			++nit;
		}
	}
	int v[] = { 100, 500, 300 , 400};
	ds::Array<int>::iterator it = ar.begin();
	int cnt = 0;
	while (it != ar.end()) {
		REQUIRE(*it == v[cnt]);
		++cnt;
		++it;
	}
}

TEST_CASE("ArrayBasicRemoveAll", "[Array]") {
	ds::Array<int> ar;
	ar.push_back(100);
	ar.push_back(200);
	ar.push_back(300);
	ar.push_back(200);
	ar.push_back(400);
	ar.push_back(200);
	ar.push_back(500);
	int num = ar.remove_all(200);
	REQUIRE(ar.size() == 4);	
}