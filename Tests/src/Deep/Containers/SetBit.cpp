#include "Tests.h"

#define DEEP_TESTABLE
#include "Deep.h"
#include "Deep/Bit/IntegerBitMask.h"
#include "Deep/Containers/SetBit.h"

TEST(SetBit, Archetype) {
	TEST_CASE(Iteration) {
		struct A {
			int32 m_value;
		};
		struct B {
			int32 m_value;
		};

		using Archetype = Deep::FixedSizeSetBitArchetype<Deep::IntegerBitMask<uint16>, A, B>;
		static_assert(Archetype::k_maxCapacity == 16, "With a `uint16` mask, only 16 items can be made max.");

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

		using Archetype = Deep::FixedSizeSetBitArchetype<Deep::IntegerBitMask<uint16>, A, B>;
		static_assert(Archetype::k_maxCapacity == 16, "With a `uint16` mask, only 16 items can be made max.");

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

	using Archetype = Deep::FixedSizeSetBitArchetype<Deep::IntegerBitMask<uint16>, A, B>;
	static_assert(Archetype::k_maxCapacity == 16, "With a `uint16` mask, only 16 items can be made max.");

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

TEST(SetBit, Copy) {
	struct A {
		A(const A&) = default;
		A(A&&) = default;
		A(int32 in_value, size_t& in_destructorCalls) :
			m_value{ in_value }, m_destructorCalls{ in_destructorCalls } {}

		~A() {
			++m_destructorCalls;
		}

		int32 m_value;
		size_t& m_destructorCalls;
	};
	struct B {
		int32 m_value;
	};

	using Archetype = Deep::FixedSizeSetBitArchetype<Deep::IntegerBitMask<uint16>, A, B>;
	static_assert(Archetype::k_maxCapacity == 16, "With a `uint16` mask, only 16 items can be made max.");

	auto isComponentPtrEq = []<typename T>(Archetype& in_a, Archetype& in_b) {
		return in_a.GetComponentPtr<T>() == in_b.GetComponentPtr<T>();
	};

	size_t destructorCalls = 0;
	TEST_CASE(Constructor) {
		Archetype a{ 10 };
		for (size_t i = 0; i < 10; ++i) {
			if (i % 2 == 0) {
				a.ConstructEntity(i, Deep::ConstructWith<A>(static_cast<int32>(i), destructorCalls));
			}
		}
		Archetype b{ a };

		EXPECT_EQ(a.m_activeMask, b.m_activeMask);
		EXPECT_EQ(a.m_capacity, b.m_capacity);

		EXPECT_FALSE(isComponentPtrEq.template operator()<A>(a, b));
		EXPECT_FALSE(isComponentPtrEq.template operator()<B>(a, b));

		{
			auto aIter = a.View<const A>().begin();
			auto bIter = b.View<const A>().begin();
			for (; aIter != Deep::Sentinel{}; ++aIter, ++bIter) {
				auto [aA] = *aIter;
				auto [bA] = *bIter;
				EXPECT_EQ(aA.m_value, bA.m_value);
			}
		}
	}
	EXPECT_EQ(destructorCalls, 10);

	destructorCalls = 0;
	TEST_CASE(Assignment) {
		Archetype a{ 10 };
		Archetype b{ 10 };
		for (size_t i = 0; i < 10; ++i) {
			if (i % 2 == 0) {
				a.ConstructEntity(i, Deep::ConstructWith<A>(static_cast<int32>(i), destructorCalls));
			} else {
				b.ConstructEntity(i, Deep::ConstructWith<A>(static_cast<int32>(i), destructorCalls));
			}
		}

		EXPECT_FALSE(isComponentPtrEq.template operator()<A>(a, b));
		EXPECT_FALSE(isComponentPtrEq.template operator()<B>(a, b));

		b = a;

		EXPECT_EQ(destructorCalls, 5);

		EXPECT_FALSE(isComponentPtrEq.template operator()<A>(a, b));
		EXPECT_FALSE(isComponentPtrEq.template operator()<B>(a, b));

		{
			auto aIter = a.View<const A>().begin();
			auto bIter = b.View<const A>().begin();
			for (; aIter != Deep::Sentinel{}; ++aIter, ++bIter) {
				auto [aA] = *aIter;
				auto [bA] = *bIter;
				EXPECT_EQ(aA.m_value, bA.m_value);
			}
		}
	}
	EXPECT_EQ(destructorCalls, 15);
}

TEST(SetBit, Move) {
	struct A {
		A(const A&) = default;
		A(A&&) = default;
		A(int32 in_value, size_t& in_destructorCalls) :
			m_value{ in_value }, m_destructorCalls{ in_destructorCalls } {}

		~A() {
			++m_destructorCalls;
		}

		int32 m_value;
		size_t& m_destructorCalls;
	};
	struct B {
		int32 m_value;
	};

	using Archetype = Deep::FixedSizeSetBitArchetype<Deep::IntegerBitMask<uint16>, A, B>;
	static_assert(Archetype::k_maxCapacity == 16, "With a `uint16` mask, only 16 items can be made max.");

	auto isComponentNullPtr = []<typename T>(Archetype& in_arch) {
		return in_arch.GetComponentPtr<T>() == nullptr;
	};

	size_t destructorCalls = 0;
	TEST_CASE(Constructor) {
		Archetype a{ 10 };
		for (size_t i = 0; i < 10; ++i) {
			if (i % 2 == 0) {
				a.ConstructEntity(i, Deep::ConstructWith<A>(static_cast<int32>(i), destructorCalls));
			}
		}
		Archetype b{ std::move(a) };

		EXPECT_EQ(destructorCalls, 0);

		EXPECT_TRUE(isComponentNullPtr.template operator()<A>(a));
		EXPECT_TRUE(isComponentNullPtr.template operator()<B>(a));

		EXPECT_FALSE(isComponentNullPtr.template operator()<A>(b));
		EXPECT_FALSE(isComponentNullPtr.template operator()<B>(b));

		int32 index = 0;
		for (auto [a] : b.View<const A>()) {
			EXPECT_EQ(a.m_value, index * 2);
			++index;
		}
	}
	EXPECT_EQ(destructorCalls, 5);

	destructorCalls = 0;
	TEST_CASE(Assignment) {
		Archetype a{ 10 };
		Archetype b{ 10 };
		for (size_t i = 0; i < 10; ++i) {
			if (i % 2 == 0) {
				a.ConstructEntity(i, Deep::ConstructWith<A>(static_cast<int32>(i), destructorCalls));
			} else {
				b.ConstructEntity(i, Deep::ConstructWith<A>(static_cast<int32>(i), destructorCalls));
			}
		}
		b = std::move(a);

		EXPECT_EQ(destructorCalls, 5);

		EXPECT_TRUE(isComponentNullPtr.template operator()<A>(a));
		EXPECT_TRUE(isComponentNullPtr.template operator()<B>(a));

		EXPECT_FALSE(isComponentNullPtr.template operator()<A>(b));
		EXPECT_FALSE(isComponentNullPtr.template operator()<B>(b));

		int32 index = 0;
		for (auto [a] : b.View<const A>()) {
			EXPECT_EQ(a.m_value, index * 2);
			++index;
		}
	}
	EXPECT_EQ(destructorCalls, 10);
}
