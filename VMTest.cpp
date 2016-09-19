#include "catch.hpp"
#include <core\script\vm.h>
#include <core\io\FileUtils.h>
#include <core\memory\DefaultAllocator.h>
#include <core\log\Log.h>

TEST_CASE("VM_Basic", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);	
	//char* txt = ds::file::loadTextFile("test.script");
	char* txt = "R1 = R2 / 4.0;";
	ds::vm::parse(txt);
	//delete[] txt;
	delete ds::gDefaultMemory;
	
}