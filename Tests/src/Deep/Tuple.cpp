#include "Tests.h"

#include "Deep.h"
#include "Deep/Memory.h"
#include "Deep/Tuple.h"

TEST(Tuple, Primitives) {
	TEST_CASE(Constructor) {
		Deep::Tuple<int32, int32> tuple{ 5, 10 };

		EXPECT_EQ(std::get<0>(tuple), 5);
		EXPECT_EQ(std::get<1>(tuple), 10);
	}

	TEST_CASE(MakeTuple) {
		Deep::Tuple<int32, int32> tuple = Deep::MakeTuple(5, 10);

		EXPECT_EQ(std::get<0>(tuple), 5);
		EXPECT_EQ(std::get<1>(tuple), 10);
	}
}

TEST(Tuple, Tie) {
	int a = 5;
	int b = 10;
	auto tie = Deep::Tie(a, b);

	TEST_CASE(Bind) {
		EXPECT_EQ(std::get<0>(tie), 5);
		EXPECT_EQ(std::get<1>(tie), 10);
	}

	TEST_CASE(ReassignVars) {
		a = 15;
		b = 25;

		EXPECT_EQ(std::get<0>(tie), 15);
		EXPECT_EQ(std::get<1>(tie), 25);
	}

	TEST_CASE(ReassignTuple) {
		std::get<0>(tie) = 5;
		std::get<1>(tie) = 10;

		EXPECT_EQ(std::get<0>(tie), 5);
		EXPECT_EQ(std::get<1>(tie), 10);
	}
}

TEST(Tuple, PlainOldData) {
	struct A {
		int32 a;
		int32 b;
	};

	TEST_CASE(Constructor) {
		Deep::Tuple<A, A> tuple{ A{ 5, 10 }, A{ 2, 3 } };

		EXPECT_EQ(std::get<0>(tuple).a, 5);
		EXPECT_EQ(std::get<0>(tuple).b, 10);

		EXPECT_EQ(std::get<1>(tuple).a, 2);
		EXPECT_EQ(std::get<1>(tuple).b, 3);
	}

	TEST_CASE(MakeTuple) {
		Deep::Tuple<A, A> tuple = Deep::MakeTuple(A{ 5, 10 }, A{ 2, 3 });

		EXPECT_EQ(std::get<0>(tuple).a, 5);
		EXPECT_EQ(std::get<0>(tuple).b, 10);

		EXPECT_EQ(std::get<1>(tuple).a, 2);
		EXPECT_EQ(std::get<1>(tuple).b, 3);
	}
}

TEST(Tuple, NonTrivial) {
	struct A {
		A() = delete;
		A(const A&) = delete;
		A(size_t in_size, size_t& in_ref) :
			m_ref(in_ref) {
			m_buffer = Deep::Malloc(in_size);
			size = in_size;
		};
		A(A&& in_other) :
			m_ref(in_other.m_ref) {
			m_buffer = in_other.m_buffer;
			size = in_other.size;

			in_other.m_buffer = nullptr;
			in_other.size = 0;
		}
		~A() {
			Deep::Free(m_buffer);
			size = 0;
			++m_ref;
		}

		size_t& m_ref;
		void* m_buffer;
		size_t size;
	};

	size_t destructorCount = 0;
	TEST_CASE(TupleConstructor) {
		Deep::Tuple<A, A> tuple{ A{ 5, destructorCount }, A{ 10, destructorCount } };

		EXPECT_EQ(destructorCount, 2); // From destructing the 2 tempory A objects that were moved into the tuple
		destructorCount = 0;

		EXPECT_EQ(std::get<0>(tuple).size, 5);
		EXPECT_NE(std::get<0>(tuple).m_buffer, nullptr);

		EXPECT_EQ(std::get<1>(tuple).size, 10);
		EXPECT_NE(std::get<1>(tuple).m_buffer, nullptr);
	}
	EXPECT_EQ(destructorCount, 2); // Destruction of tuple

	destructorCount = 0;
	TEST_CASE(MakeTuple) {
		Deep::Tuple<A, A> tuple = Deep::MakeTuple(A{ 5, destructorCount }, A{ 10, destructorCount });

		EXPECT_EQ(destructorCount, 2); // From destructing the 2 tempory A objects that were moved into the tuple
		destructorCount = 0;

		EXPECT_EQ(std::get<0>(tuple).size, 5);
		EXPECT_NE(std::get<0>(tuple).m_buffer, nullptr);

		EXPECT_EQ(std::get<1>(tuple).size, 10);
		EXPECT_NE(std::get<1>(tuple).m_buffer, nullptr);
	}
	EXPECT_EQ(destructorCount, 2); // Destruction of tuple
}
