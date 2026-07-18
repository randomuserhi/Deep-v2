#include "Tests.h"

#define DEEP_TESTABLE
#include "Deep.h"
#include "Deep/Containers/SetBit.h"

TEST(SetBit, Archetype) {
	TEST_CASE(Iteration) {
		struct A {
			int32 m_value;
		};
		struct B {
			int32 m_value;
		};

		using Archetype = Deep::SetBit::FixedSizeArchetype<uint16, A, B>;

		Archetype arch{ 10 };
		for (size_t i = 0; i < 10; ++i) {
			if (i % 2 == 0) {
				arch.ConstructEntity(i, Deep::ConstructWith<A>(static_cast<int32>(i)),
				                     Deep::ConstructWith<B>(static_cast<int32>(i) + 1));
			}
		}

		EXPECT_EQ(arch.Size(), 5);

		int32 index = 0;
		for (auto [a, b] : arch) {
			EXPECT_EQ(a.m_value, index * 2);
			EXPECT_EQ(b.m_value, index * 2 + 1);
			++index;
		}
	}

	TEST_CASE(NonTrivial) {
		struct A {
			A(size_t& in_destructorCalls) :
				m_destructorCalls{ in_destructorCalls } {}

			~A() {
				++m_destructorCalls;
			}

			int32 m_value;
			size_t& m_destructorCalls;
		};
		struct B {
			int32 m_value;
		};

		using Archetype = Deep::SetBit::FixedSizeArchetype<uint16, A, B>;

		size_t destructorCalls = 0;
		{
			Archetype arch{ 10 };
			for (size_t i = 0; i < 10; ++i) {
				if (i % 2 == 0) {
					arch.ConstructEntity(i, Deep::ConstructWith<A>(destructorCalls));
				}
			}

			auto [a, b] = arch.GetComponents<A, B>(0);
			a.m_value = 10;
			b.m_value = 20;

			EXPECT_EQ(arch.GetComponent<A>(0).m_value, 10);
			EXPECT_EQ(arch.GetComponent<B>(0).m_value, 20);
		}
		EXPECT_EQ(destructorCalls, 5);
	}
}

TEST(SetBit, ArchetypeView) {
	struct A {
		int32 m_value;
	};
	struct B {
		int32 m_value;
	};

	using Archetype = Deep::SetBit::FixedSizeArchetype<uint16, A, B>;

	Archetype arch{ 10 };
	for (size_t i = 0; i < 10; ++i) {
		if (i % 2 == 0) {
			arch.ConstructEntity(i, Deep::ConstructWith<A>(static_cast<int32>(i)));
		}
	}

	int32 index = 0;
	for (auto [a] : arch.View<const A>()) {
		EXPECT_EQ(a.m_value, index * 2);
		++index;
	}

	for (auto [a] : arch.View<A>()) {
		a.m_value += 1;
	}

	index = 0;
	for (auto [a] : arch.View<const A>()) {
		EXPECT_EQ(a.m_value, index * 2 + 1);
		++index;
	}
}
