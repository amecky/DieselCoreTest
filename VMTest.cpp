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
	ds::vm::VMContext* ctx = new ds::vm::VMContext("Test");
	ds::vm::parse(txt,ctx);
	REQUIRE(1 == ctx->lines.size());
	const ds::vm::Line& line = ctx->lines[0];
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
	ds::vm::VMContext* ctx = new ds::vm::VMContext("Test");
	ds::vm::parse(txt, ctx);
	REQUIRE(1 == ctx->lines.size());
	const ds::vm::Line& line = ctx->lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_NONE);
	ds::vm::execute(ctx);
	REQUIRE(4.0f == ctx->data[1].x);
	delete ctx;
	delete ds::gDefaultMemory;

}

TEST_CASE("VM_Basic2", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 + 8.0;";
	ds::vm::VMContext* ctx = new ds::vm::VMContext("Test");
	ds::vm::parse(txt, ctx);
	REQUIRE(1 == ctx->lines.size());
	const ds::vm::Line& line = ctx->lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_PLUS);
	ds::vm::execute(ctx);
	REQUIRE(12.0f == ctx->data[1].x);
	delete ctx;
	//delete[] txt;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic3", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 + 8.0;\nR2 = R1 / 2.0;";
	ds::vm::VMContext* ctx = new ds::vm::VMContext("Test");
	ds::vm::parse(txt, ctx);
	REQUIRE(2 == ctx->lines.size());
	const ds::vm::Line& line = ctx->lines[1];
	REQUIRE(2 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_DIV);
	ds::vm::execute(ctx);
	REQUIRE(6.0f == ctx->data[2].x);
	delete ctx;
	//delete[] txt;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic4", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 + DT;";
	ds::vm::VMContext* ctx = new ds::vm::VMContext("Test");
	ctx->set(SID("DT"), v4(1, 2, 3, 4));
	ds::vm::parse(txt, ctx);
	REQUIRE(1 == ctx->lines.size());
	const ds::vm::Line& line = ctx->lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_PLUS);
	ds::vm::execute(ctx);
	REQUIRE(5.0f == ctx->data[1].x);
	delete ctx;
	//delete[] txt;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic5", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 + SIN(DT);";
	ds::vm::VMContext* ctx = new ds::vm::VMContext("Test");
	ctx->set(SID("DT"), v4(HALF_PI));
	ds::vm::parse(txt, ctx);
	REQUIRE(1 == ctx->lines.size());
	const ds::vm::Line& line = ctx->lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_PLUS);
	ds::vm::execute(ctx);
	REQUIRE(5.0f == ctx->data[1].x);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic6", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0;\nR2 = 10.0;\nR3 = 1.0 + LRP(R1,R2,0.5);\n";
	ds::vm::VMContext* ctx = new ds::vm::VMContext("Test");
	ds::vm::parse(txt, ctx);
	REQUIRE(3 == ctx->lines.size());
	const ds::vm::Line& line = ctx->lines[2];
	REQUIRE(3 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_PLUS);
	ds::vm::execute(ctx);
	REQUIRE(8.0f == ctx->data[3].x);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic7", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = 4.0 * SIN(DT);";
	ds::vm::VMContext* ctx = new ds::vm::VMContext("Test");
	ctx->registerVar(SID("DT"));
	ds::vm::parse(txt, ctx);
	REQUIRE(1 == ctx->lines.size());
	const ds::vm::Line& line = ctx->lines[0];
	REQUIRE(1 == line.register_index);
	REQUIRE(line.operation == ds::vm::Operation::OP_MUL);
	ctx->set(0, v4(HALF_PI));
	ds::vm::execute(ctx);
	REQUIRE(4.0f == ctx->data[1].x);
	delete ctx;
	delete ds::gDefaultMemory;
}