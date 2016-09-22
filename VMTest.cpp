#include "catch.hpp"
#include <core\script\vm.h>
#include <core\io\FileUtils.h>
#include <core\memory\DefaultAllocator.h>
#include <core\log\Log.h>
#include <core\Common.h>

TEST_CASE("VM_Basic", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);	
	//char* txt = ds::file::loadTextFile("test.script");
	char* txt = "R1 = R2 / 4.0;";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	const ds::vm::Line& line = m.lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_DIV);
	delete ctx;
	//delete[] txt;
	delete ds::gDefaultMemory;
	
}

TEST_CASE("VM_Basic1", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0;";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	const ds::vm::Line& line = m.lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_NONE);
	ctx->execute();
	REQUIRE(4.0f == ctx->getRegister(1).x);
	delete ctx;
	delete ds::gDefaultMemory;

}

TEST_CASE("VM_Basic2", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 + 8.0;";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	const ds::vm::Line& line = m.lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_PLUS);
	ctx->execute();
	REQUIRE(12.0f == ctx->getRegister(1).x);
	delete ctx;
	//delete[] txt;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic3", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 + 8.0;\nR2 = R1 / 2.0;";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(2 == m.lines.size());
	const ds::vm::Line& line = m.lines[1];
	REQUIRE(2 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_DIV);
	ctx->execute();
	REQUIRE(6.0f == ctx->getRegister(2).x);
	delete ctx;
	//delete[] txt;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic4", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 + DT;";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->set(SID("DT"), v4(1, 2, 3, 4));
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	const ds::vm::Line& line = m.lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_PLUS);
	ctx->execute();
	REQUIRE(5.0f == ctx->getRegister(1).x);
	delete ctx;
	//delete[] txt;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic5", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 + SIN(DT);";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->set(SID("DT"), v4(HALF_PI));
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	const ds::vm::Line& line = m.lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_PLUS);
	ctx->execute();
	REQUIRE(5.0f == ctx->getRegister(1).x);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic6", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0;\nR2 = 10.0;\nR3 = 1.0 + LRP(R1,R2,0.5);\n";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(3 == m.lines.size());
	const ds::vm::Line& line = m.lines[2];
	REQUIRE(3 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_PLUS);
	ctx->execute();
	REQUIRE(8.0f == ctx->getRegister(3).x);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic7", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 * SIN(DT);";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->registerVar(SID("DT"));
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	const ds::vm::Line& line = m.lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_MUL);
	ctx->set(0, v4(HALF_PI));
	ctx->execute();
	REQUIRE(4.0f == ctx->getRegister(1).x);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic8", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 * COS(TWO_PI);";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	const ds::vm::Line& line = m.lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_MUL);
	ctx->execute();
	REQUIRE(4.0f == ctx->getRegister(1).x);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic9", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 + SIN(TWO_PI);";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	ctx->set(0, v4(HALF_PI));
	ctx->execute();
	REQUIRE(4.0f == ctx->getRegister(1).x);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic10", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 10.0;\nR1 = SAT(R1);\nR2 = R1 * 4.0;";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	ctx->execute(m);
	REQUIRE(4.0f == ctx->getRegister(2).x);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Function1", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "function wiggle() {\nR1 = 4.0 + 8.0;\n}";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("wiggle"));
	ctx->execute(SID("wiggle"));
	REQUIRE(12.0f == ctx->getRegister(1).x);
	delete ctx;
	delete ds::gDefaultMemory;
}
TEST_CASE("VM_Function2", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "function wiggle() {\nR1 = 4.0 + 8.0;\n}\nfunction next() {\nR2 = 1.0 + 2.0;\n}";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("next"));
	ctx->execute(SID("next"));
	REQUIRE(3.0f == ctx->getRegister(2).x);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Function3", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "function wiggle() {\nR1 = 4.0;\nR2 = CLM(R1,0.0,2.0);\nR3 = R2 * 5.0\n}";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	ctx->execute(SID("wiggle"));
	REQUIRE(10.0f == ctx->getRegister(3).x);
	delete ctx;
	delete ds::gDefaultMemory;
}