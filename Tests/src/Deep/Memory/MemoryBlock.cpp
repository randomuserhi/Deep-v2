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

TEST(MemoryBlock, EmptyStates) {
	Deep::MemoryBlock<int> empty;
	Deep::MemoryBlock<int> zero{ 0 };
	EXPECT_EQ(empty.m_Size(), 0);
	EXPECT_EQ(empty.m_Ptr(), nullptr);
	EXPECT_EQ(zero.m_Size(), 0);
	EXPECT_EQ(zero.m_Ptr(), nullptr);
	Deep::MemoryBlock<int> copy{ empty };
	Deep::MemoryBlock<int> moved{ std::move(zero) };
	EXPECT_EQ(copy.m_Ptr(), nullptr);
	EXPECT_EQ(moved.m_Ptr(), nullptr);
	EXPECT_EQ(zero.m_Size(), 0);
	EXPECT_TRUE(empty == nullptr);
	EXPECT_TRUE(nullptr == empty);
}

TEST(MemoryBlock, CopyAssignmentAndIndependence) {
	Deep::MemoryBlock<int> source{ 4 };
	Deep::MemoryBlock<int> destination{ 2 };
	for (size_t i = 0; i < 4; ++i)
		source[i] = static_cast<int>(i) + 10;
	destination = source;
	EXPECT_EQ(destination.m_Size(), 4);
	EXPECT_NE(destination.m_Ptr(), source.m_Ptr());
	for (size_t i = 0; i < 4; ++i)
		EXPECT_EQ(destination[i], source[i]);
	destination[size_t{ 0 }] = -1;
	EXPECT_EQ(source[size_t{ 0 }], 10);
	const auto& alias = destination;
	int* pointer = destination.m_Ptr();
	destination = alias;
	EXPECT_EQ(destination.m_Ptr(), pointer);
	EXPECT_EQ(destination[size_t{ 0 }], -1);
	destination = Deep::MemoryBlock<int>{};
	EXPECT_EQ(destination.m_Size(), 0);
	EXPECT_EQ(destination.m_Ptr(), nullptr);
	Deep::MemoryBlock<int> empty;
	source = empty;
	EXPECT_EQ(source.m_Size(), 0);
	EXPECT_EQ(source.m_Ptr(), nullptr);
}

TEST(MemoryBlock, AssignmentDestroysPreviousElements) {
	NonTrivialType::s_numDestructorCalls = 0;
	{
		Deep::MemoryBlock<NonTrivialType> source{ 3 };
		Deep::MemoryBlock<NonTrivialType> destination{ 2 };
		source[size_t{ 1 }].m_value = 42;
		destination = source;
		EXPECT_EQ(NonTrivialType::s_numDestructorCalls, 2);
		EXPECT_EQ(destination[size_t{ 1 }].m_value, 42);
		auto* pointer = source.m_Ptr();
		destination = std::move(source);
		EXPECT_EQ(NonTrivialType::s_numDestructorCalls, 5);
		EXPECT_EQ(destination.m_Ptr(), pointer);
		EXPECT_EQ(source.m_Ptr(), nullptr);
		EXPECT_EQ(source.m_Size(), 0);
		auto& alias = destination;
		destination = std::move(alias);
		EXPECT_EQ(destination.m_Ptr(), pointer);
		EXPECT_EQ(destination.m_Size(), 3);
		EXPECT_EQ(NonTrivialType::s_numDestructorCalls, 5);
	}
	EXPECT_EQ(NonTrivialType::s_numDestructorCalls, 8);
}

TEST(MemoryBlock, AdoptAndPointerAccess) {
	int* pointer = Deep::RawAllocator<int>::s_Malloc(4);
	for (size_t i = 0; i < 4; ++i)
		new (pointer + i) int(static_cast<int>(i));
	Deep::MemoryBlock<int> block{ pointer, 4 };
	EXPECT_EQ(block.m_Ptr(), pointer);
	EXPECT_EQ(block.m_Size(), 4);
	EXPECT_EQ(block + size_t{ 2 }, pointer + 2);
	EXPECT_EQ(size_t{ 2 } + block, pointer + 2);
	const auto& view = block;
	EXPECT_EQ(view + size_t{ 3 }, pointer + 3);
	EXPECT_EQ(size_t{ 3 } + view, pointer + 3);
	EXPECT_EQ(view[size_t{ 3 }], 3);
	EXPECT_EQ(static_cast<const int*>(view), pointer);
	EXPECT_EQ(static_cast<int*>(block), pointer);
}

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
