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
	REQUIRE(1 == line.register_index.index);
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
	REQUIRE(1 == line.register_index.index);
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
	REQUIRE(1 == line.register_index.index);
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
	REQUIRE(2 == line.register_index.index);
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
	REQUIRE(1 == line.register_index.index);
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
	REQUIRE(1 == line.register_index.index);
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
	REQUIRE(3 == line.register_index.index);
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
	REQUIRE(1 == line.register_index.index);
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
	REQUIRE(1 == line.register_index.index);
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

TEST_CASE("VM_Function4", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "function wiggle() {\nR1 = 4.0;\nR2 = TWN(0,1.0,2.0,0.5,1.0);\n}";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	ctx->execute(SID("wiggle"));
	REQUIRE(1.5f == ctx->getRegister(2).x);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Function5", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "function wiggle {\nR1 = 4.0;\nR2 = TWN(0,1.0,2.0,0.5,1.0);\n}";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ds::vm::ParserStatus ret = ctx->parse(txt);
	REQUIRE(ret == ds::vm::ParserStatus::PS_INVALID_FUNCTION);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic11", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = (1.0,2.0,3.0,4.0);";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	ctx->execute(m);
	REQUIRE(1.0f == ctx->getRegister(1).x);
	REQUIRE(2.0f == ctx->getRegister(1).y);
	REQUIRE(3.0f == ctx->getRegister(1).z);
	REQUIRE(4.0f == ctx->getRegister(1).w);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic12", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = (1.0,2.0,3.0,4.0) + (10.0,20.0,30.0,40.0);";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	ctx->execute(m);
	REQUIRE(11.0f == ctx->getRegister(1).x);
	REQUIRE(22.0f == ctx->getRegister(1).y);
	REQUIRE(33.0f == ctx->getRegister(1).z);
	REQUIRE(44.0f == ctx->getRegister(1).w);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic13", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "OUT = (1.0,2.0,3.0,4.0) + (10.0,20.0,30.0,40.0);";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	v4 r = ctx->execute(m);
	REQUIRE(11.0f == r.x);
	REQUIRE(22.0f == r.y);
	REQUIRE(33.0f == r.z);
	REQUIRE(44.0f == r.w);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic14", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1[0] = (1.0,2.0,3.0,4.0) + (10.0,20.0,30.0,40.0);";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->parse(txt);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	ctx->execute(m);
	REQUIRE(11.0f == ctx->getRegister(1).x);
	REQUIRE(0.0f == ctx->getRegister(1).y);
	REQUIRE(0.0f == ctx->getRegister(1).z);
	REQUIRE(0.0f == ctx->getRegister(1).w);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic15", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 (1.0,2.0,3.0,4.0) + (10.0,20.0,30.0,40.0);";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ds::vm::ParserStatus status = ctx->parse(txt);
	REQUIRE(status == ds::vm::ParserStatus::PS_MISSING_ASSIGNMENT);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic16", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "ABC = (1.0,2.0,3.0,4.0) + (10.0,20.0,30.0,40.0);";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ds::vm::ParserStatus status = ctx->parse(txt);
	REQUIRE(status == ds::vm::ParserStatus::PS_UNKNOWN_REGISTER_TYPE);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic17", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = (1.0,2.0,3.0;";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ds::vm::ParserStatus status = ctx->parse(txt);
	REQUIRE(status == ds::vm::ParserStatus::PS_WRONG_V4_DEFINITION);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic18", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "vars {\nDT = (1.0,2.0,3.0,4.0);\n}\nR1 = 4.0 * DT;";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ds::vm::ParserStatus status = ctx->parse(txt);
	REQUIRE(status == ds::vm::ParserStatus::PS_OK);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	ctx->execute(m);
	REQUIRE(4.0f == ctx->getRegister(1).x);
	REQUIRE(8.0f == ctx->getRegister(1).y);
	REQUIRE(12.0f == ctx->getRegister(1).z);
	REQUIRE(16.0f == ctx->getRegister(1).w);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic19", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "vars {\nDT = (1.0,2.0,3.0,4.0);\nGG = 4.0;\n}\nR1 = DT + GG;";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ds::vm::ParserStatus status = ctx->parse(txt);
	REQUIRE(status == ds::vm::ParserStatus::PS_OK);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	ctx->execute(m);
	REQUIRE(5.0f == ctx->getRegister(1).x);
	REQUIRE(6.0f == ctx->getRegister(1).y);
	REQUIRE(7.0f == ctx->getRegister(1).z);
	REQUIRE(8.0f == ctx->getRegister(1).w);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic20", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "vars {\nDT = CLR(255.0,128.0,64.0,32.0);\n}\nR1 = DT;";
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ds::vm::ParserStatus status = ctx->parse(txt);
	REQUIRE(status == ds::vm::ParserStatus::PS_OK);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	ctx->execute(m);
	REQUIRE(1.0f == ctx->getRegister(1).x);
	REQUIRE(0.5f == ctx->getRegister(1).y);
	REQUIRE(0.25f == ctx->getRegister(1).z);
	REQUIRE(0.125f == ctx->getRegister(1).w);
	delete ctx;
	delete ds::gDefaultMemory;
}

TEST_CASE("VM_Basic21", "[VM]") {
	init_logger(LogTypes::LT_FILE, 0, 0);
	ds::gDefaultMemory = new ds::DefaultAllocator(64 * 1024 * 1024);
	char* txt = "R1 = PTH(0,0.5);";
	ds::V3Path path;
	path.add(0.0f, v3(0.1f, 0.1f, 0.0f));
	path.add(0.5f, v3(1.2f, 1.2f, 0.0f));
	path.add(0.75f, v3(0.75f, 0.75f, 0.0f));
	path.add(1.0f, v3(1.0f, 1.0f, 0.0f));
	ds::vm::Script* ctx = new ds::vm::Script("Test");
	ctx->addPath(&path);
	ds::vm::ParserStatus status = ctx->parse(txt);
	REQUIRE(status == ds::vm::ParserStatus::PS_OK);
	const ds::vm::Method& m = ctx->getMethod(SID("default"));
	REQUIRE(1 == m.lines.size());
	ctx->execute(m);
	REQUIRE(1.2f == ctx->getRegister(1).x);
	REQUIRE(1.2f == ctx->getRegister(1).y);
	REQUIRE(0.0f == ctx->getRegister(1).z);
	REQUIRE(0.0f == ctx->getRegister(1).w);
	delete ctx;
	delete ds::gDefaultMemory;
}