#include "Tests.h"
#include <ostream>
#include <sstream>

TEST_SUPPRESS_WARNINGS_STD_BEGIN
#include <string_view>
#include <unordered_map>
#include <vector>
#include <cstring>
TEST_SUPPRESS_WARNINGS_STD_END

namespace {

class ScopedCoutCapture {
public:
	ScopedCoutCapture(std::streambuf& in_buf) :
		m_oldBuf{ std::cout.rdbuf(&in_buf) } {}

	~ScopedCoutCapture() {
		std::cout.rdbuf(m_oldBuf);
	}

private:
	std::streambuf* m_oldBuf;
};

} // namespace

namespace Test {

IndentBuf::IndentBuf(std::streambuf* in_dest) :
	m_dest{ in_dest }, m_depth{ 0 }, m_atLineStart{ true } {}

int IndentBuf::overflow(int in_ch) {
	if (in_ch == traits_type::eof()) return m_dest->sputc(in_ch);

	if (m_atLineStart) {
		for (size_t i = 0; i < m_depth; ++i) {
			m_dest->sputc('\t');
		}
		m_atLineStart = false;
	}

	m_dest->sputc(in_ch);

	if (in_ch == '\n') m_atLineStart = true;

	return in_ch;
}

void TestBase::Init() {
	m_failed = false;
	m_out.m_depth = 2;
}

TestCase::TestCase(const char* in_caseName, TestBase& in_testObj) :
	m_caseName{ in_caseName },
	m_testObj{ in_testObj },
	m_parentBuf{ m_testObj.m_out.m_dest },
	m_parentDepth{ m_testObj.m_out.m_depth } {

	// Write case name
	m_testObj.m_out.m_atLineStart = true;
	std::cout << "[" << in_caseName;

	// Swap out write buffer to this test case object's one
	// All writes that occure will now be under this test case
	m_testObj.m_out.m_dest = m_buf.rdbuf();
	m_testObj.m_out.m_depth = 1;
	std::cout << '\n';

	m_start = std::chrono::high_resolution_clock::now();
}

TestCase::~TestCase() {
	m_end = std::chrono::high_resolution_clock::now();

	// Restore the write buffer to what it was before entering this test case
	m_testObj.m_out.m_dest = m_parentBuf;
	m_testObj.m_out.m_depth = m_parentDepth;

	// Write duration to the test output
	m_testObj.m_out.m_atLineStart = false;
	std::cout << " - ";

	auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(m_end - m_start);
	if (duration_us.count() < 1000) {
		std::cout << duration_us.count() << " microseconds]";
	} else {
		auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start);
		std::cout << duration_ms.count() << " milliseconds]";
	}

	// Write the test case output to test output if there was any
	std::cout << m_buf.str();
}

struct CStrHash {
	size_t operator()(const char* s) const {
		return std::hash<std::string_view>{}(s);
	}
};

TEST_SUPPRESS_WARNING_PUSH
TEST_CLANG_SUPPRESS_WARNING("-Wunsafe-buffer-usage")
struct CStrEqual {
	bool operator()(const char* in_a, const char* in_b) const {
		return std::strcmp(in_a, in_b) == 0;
	}
};
TEST_SUPPRESS_WARNING_POP

// NOTE(randomuserhi): Required to be wrapped in a getter with static storage as the initialization order of globals is
//                     only guaranteed top-to-bottom within a single file. Thus other files accessing `tests` dictionary
//                     to create tests may access it before it has been constructed.
//
//                     This getter prevents this issue entirely.
static std::unordered_map<const char*, std::vector<TestInfo*>, CStrHash, CStrEqual>& GetTests(void) {
	// intentionally leaked singleton
	static std::unordered_map<const char*, std::vector<TestInfo*>, CStrHash, CStrEqual>* s_tests =
		new std::unordered_map<const char*, std::vector<TestInfo*>, CStrHash, CStrEqual>{};
	return *s_tests;
}

const TestInfo* RegisterTest(const char* in_testGroup, const char* in_testName, const char* in_file, int in_line,
                             TestBase* in_testObj) {
	std::vector<TestInfo*>& info = GetTests()[in_testGroup];
	info.emplace_back(new TestInfo(in_testGroup, in_testName, in_file, in_line, in_testObj));
	return info.back();
}

int RunAllTests() {
	int totalTestsFailed = 0;

	for (const auto& kv : GetTests()) {
		const std::vector<TestInfo*>& units = kv.second;

		int testFailCount = 0;

		std::cout << "[" << kv.first << "]\n";

		for (const TestInfo* info : units) {
			/*if (info.testObj == nullptr) {
			    std::cout << "Could not find test object for '" << info.testName << "'\n";
			    continue;
			}*/

			std::cout << "\t[" << info->m_testName;

			TestBase& testObj = *(info->m_testObj);
			testObj.Init();

			// TODO(randomuserhi): Timing includes how long it took to write to standard output (test case names, fail
			//                     messages etc...) Ideally should collect messages into a `std::ostream` then print at the
			//                     end
			auto start = std::chrono::high_resolution_clock::now();
			{
				ScopedCoutCapture capture{ testObj.m_out };
				testObj.TestBody();
			}
			auto end = std::chrono::high_resolution_clock::now();

			std::cout << " - ";

			auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			if (duration_us.count() < 1000) {
				std::cout << duration_us.count() << " microseconds]";
			} else {
				auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
				std::cout << duration_ms.count() << " milliseconds]";
			}

			std::cout << '\n' << testObj.m_buf.str();

			if (testObj.m_failed == true) {
				++testFailCount;
			}

			// Teardown test object after running test
			// NOTE(randomuserhi): Isn't needed as of right now as test lifetime matches program lifetime
			/*delete info.testObj;
			info.testObj = nullptr;*/
		}

		if (testFailCount > 0) {
			std::cout << "\nTest group '" << kv.first << "' failed " << testFailCount << " tests!\n";
		}

		totalTestsFailed += testFailCount;

		std::cout << '\n';
	}

	if (totalTestsFailed == 0) {
		std::cout << "All tests passed!" << std::endl;
	} else {
		std::cout << totalTestsFailed << " tests failed!" << std::endl;
	}

	return static_cast<int>(totalTestsFailed > 0);
}
} // namespace Test
