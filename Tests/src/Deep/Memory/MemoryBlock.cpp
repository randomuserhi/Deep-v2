#include "Tests.h"

#include "Deep/Memory/MemoryBlock.h"

TEST(MemoryBlock, Trivial) {
	TEST_CASE(Move) {
		Deep::MemoryBlock<int> a{ 10 };
		int* ptr = a.m_Ptr();

		Deep::MemoryBlock<int> b = std::move(a);

		EXPECT_EQ(a.m_Ptr(), nullptr);
		EXPECT_EQ(b.m_Ptr(), ptr);
	}

	TEST_CASE(Copy) {
		Deep::MemoryBlock<int> a{ 10 };
		for (size_t i = 0; i < 10; ++i) {
			a[i] = i;
		}

		Deep::MemoryBlock<int> b = a;
		for (size_t i = 0; i < 10; ++i) {
			EXPECT_EQ(a[i], b[i]);
		}

		EXPECT_NE(a.m_Ptr(), b.m_Ptr());
	}
}

struct NonTrivialType {
	NonTrivialType() noexcept {}
	NonTrivialType(const NonTrivialType& in_other) noexcept {
		m_value = in_other.m_value;
	}
	~NonTrivialType() {
		++s_numDestructorCalls;
	}

	int m_value = 0;

	static size_t s_numDestructorCalls;
};
size_t NonTrivialType::s_numDestructorCalls = 0;

TEST(MemoryBlock, NonTrivial) {
	TEST_CASE(Move) {
		NonTrivialType::s_numDestructorCalls = 0;
		{
			Deep::MemoryBlock<NonTrivialType> a{ 10 };
			NonTrivialType* ptr = a.m_Ptr();

			Deep::MemoryBlock<NonTrivialType> b = std::move(a);

			EXPECT_EQ(a.m_Ptr(), nullptr);
			EXPECT_EQ(b.m_Ptr(), ptr);
			EXPECT_EQ(NonTrivialType::s_numDestructorCalls, 0);
		}
		EXPECT_EQ(NonTrivialType::s_numDestructorCalls, 10);
	}

	TEST_CASE(Copy) {
		NonTrivialType::s_numDestructorCalls = 0;
		{
			Deep::MemoryBlock<NonTrivialType> a{ 10 };
			for (size_t i = 0; i < 10; ++i) {
				a[i].m_value = i;
			}

			Deep::MemoryBlock<NonTrivialType> b = a;
			for (size_t i = 0; i < 10; ++i) {
				EXPECT_EQ(a[i].m_value, b[i].m_value);
			}

			EXPECT_NE(a.m_Ptr(), b.m_Ptr());
			EXPECT_EQ(NonTrivialType::s_numDestructorCalls, 0);
		}
		EXPECT_EQ(NonTrivialType::s_numDestructorCalls, 20);
	}
}
