#include "Tests.h"

TEST_SUPPRESS_WARNINGS_STD_BEGIN
#include <string_view>
#include <unordered_map>
#include <vector>
#include <chrono>
TEST_SUPPRESS_WARNINGS_STD_END

namespace Test {
struct CStrHash {
    size_t operator()(const char* s) const {
        return std::hash<std::string_view>{}(s);
    }
};

TEST_SUPPRESS_WARNING_PUSH
TEST_CLANG_SUPPRESS_WARNING("-Wunsafe-buffer-usage-in-libc-call")
struct CStrEqual {
    bool operator()(const char* a, const char* b) const {
        return std::strcmp(a, b) == 0;
    }
};
TEST_SUPPRESS_WARNING_POP

bool internal::g_testFailed = false;
// NOTE(randomuserhi): Required to be wrapped in a getter with static storage as the initialization order of globals is
//                     only guaranteed top-to-bottom within a single file. Thus other files accessing `tests` dictionary
//                     to create tests may access it before it has been constructed.
//
//                     This getter prevents this issue entirely.
static std::unordered_map<const char*, std::vector<const TestInfo*>, CStrHash, CStrEqual>& GetTests(void) {
    // intentionally leaked singleton
    static std::unordered_map<const char*, std::vector<const TestInfo*>, CStrHash, CStrEqual>* s_tests =
        new std::unordered_map<const char*, std::vector<const TestInfo*>, CStrHash, CStrEqual>{};
    return *s_tests;
}

const TestInfo* RegisterTest(const char* roTestGroup, const char* roTestName, const char* roFile, int roLine,
                             const TestBase* roTestObj) {
    std::vector<const TestInfo*>& info = GetTests()[roTestGroup];
    info.emplace_back(new TestInfo(roTestGroup, roTestName, roFile, roLine, roTestObj));
    return info.back();
}

void RunAllTests() {
    int totalTestsFailed = 0;

    for (const auto& kv : GetTests()) {
        const std::vector<const TestInfo*>& units = kv.second;

        int testFailCount = 0;

        std::cout << "[" << kv.first << "]\n\n";

        for (const TestInfo* info : units) {
            /*if (info.testObj == nullptr) {
                std::cout << "Could not find test object for '" << info.testName << "'\n";
                continue;
            }*/

            internal::g_testFailed = false;

            auto start = std::chrono::high_resolution_clock::now();

            std::cout << info->m_testName;
            info->m_testObj->TestBody();

            if (internal::g_testFailed == true) {
                std::cout << "\n";
                ++testFailCount;
            } else {
                std::cout << " - ";
            }

            auto end = std::chrono::high_resolution_clock::now();

            auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            if (duration_us.count() < 1000) {
                std::cout << duration_us.count() << " microseconds";
            } else {
                auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                std::cout << duration_ms.count() << " milliseconds";
            }

            if (internal::g_testFailed == true) {
                std::cout << '\n';
            }

            std::cout << '\n';

            // Teardown test object after running test
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
}
} // namespace Test
