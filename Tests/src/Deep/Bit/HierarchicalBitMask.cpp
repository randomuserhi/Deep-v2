#include "Tests.h"

#define DEEP_TESTABLE

#include "Deep/Types.h"
#include "Deep/Bit/HierarchicalBitMask.h"

TEST(HierarchicalBitMask, DefaultConstructionAndClear) {
	Deep::HierarchicalBitMask<32> mask{};

	EXPECT_FALSE(mask.Any());
	EXPECT_EQ(mask.NumSetBits(), 0);

	for (size_t i = 0; i < 32; ++i) {
		EXPECT_FALSE(mask.Test(i));
	}
}

TEST(HierarchicalBitMask, SetAndTest) {
	Deep::HierarchicalBitMask<32> mask{};

	mask.Set<true>(0);
	EXPECT_TRUE(mask.Test(0));
	EXPECT_TRUE(mask.Any());

	mask.Set<true>(5);
	EXPECT_TRUE(mask.Test(5));

	mask.Set<true>(31);
	EXPECT_TRUE(mask.Test(31));

	EXPECT_FALSE(mask.Test(1));
	EXPECT_FALSE(mask.Test(4));
	EXPECT_FALSE(mask.Test(30));

	EXPECT_EQ(mask.NumSetBits(), 3);

	mask.Set<false>(5);

	EXPECT_FALSE(mask.Test(5));
	EXPECT_TRUE(mask.Test(0));
	EXPECT_TRUE(mask.Test(31));
	EXPECT_EQ(mask.NumSetBits(), 2);
}

TEST(HierarchicalBitMask, RuntimeSet) {
	Deep::HierarchicalBitMask<32> mask{};

	mask.Set(7, true);

	EXPECT_TRUE(mask.Test(7));
	EXPECT_TRUE(mask.Any());

	mask.Set(7, false);

	EXPECT_FALSE(mask.Test(7));
	EXPECT_FALSE(mask.Any());
}

TEST(HierarchicalBitMask, InverseSingleChunk) {
	using Mask = Deep::HierarchicalBitMask<32>;

	Mask mask{};

	EXPECT_EQ(mask.m_chunks[0], 0);

	mask.Inverse();

	EXPECT_EQ(mask.m_chunks[0], static_cast<Mask::ChunkType>(~uint32{ 0 }));

	EXPECT_TRUE(mask.Any());
	EXPECT_EQ(mask.NumSetBits(), 32);

	mask.Inverse();

	EXPECT_FALSE(mask.Any());
	EXPECT_EQ(mask.NumSetBits(), 0);
}

TEST(HierarchicalBitMask, InverseMasksUnusedBits) {
	Deep::HierarchicalBitMask<35> mask{};

	mask.Inverse();

	EXPECT_EQ(mask.NumSetBits(), 35);

	for (size_t i = 0; i < 35; ++i) {
		EXPECT_TRUE(mask.Test(i));
	}

	// Inverse must not leave padding bits logically set.
	mask.Inverse();

	EXPECT_FALSE(mask.Any());
	EXPECT_EQ(mask.NumSetBits(), 0);
}

TEST(HierarchicalBitMask, BitwiseOr) {
	Deep::HierarchicalBitMask<32> a{};
	Deep::HierarchicalBitMask<32> b{};

	a.Set<true>(1);
	a.Set<true>(10);

	b.Set<true>(10);
	b.Set<true>(20);

	const auto result = a | b;

	EXPECT_TRUE(result.Test(1));
	EXPECT_TRUE(result.Test(10));
	EXPECT_TRUE(result.Test(20));

	EXPECT_EQ(result.NumSetBits(), 3);

	// Inputs should remain unchanged.
	EXPECT_TRUE(a.Test(1));
	EXPECT_TRUE(a.Test(10));
	EXPECT_FALSE(a.Test(20));

	EXPECT_FALSE(b.Test(1));
	EXPECT_TRUE(b.Test(10));
	EXPECT_TRUE(b.Test(20));
}

TEST(HierarchicalBitMask, BitwiseOrAssign) {
	Deep::HierarchicalBitMask<32> a{};
	Deep::HierarchicalBitMask<32> b{};

	a.Set<true>(3);
	b.Set<true>(17);

	a |= b;

	EXPECT_TRUE(a.Test(3));
	EXPECT_TRUE(a.Test(17));
	EXPECT_EQ(a.NumSetBits(), 2);
}

TEST(HierarchicalBitMask, BitwiseAnd) {
	Deep::HierarchicalBitMask<32> a{};
	Deep::HierarchicalBitMask<32> b{};

	a.Set<true>(1);
	a.Set<true>(10);
	a.Set<true>(20);

	b.Set<true>(10);
	b.Set<true>(20);
	b.Set<true>(25);

	const auto result = a & b;

	EXPECT_FALSE(result.Test(1));
	EXPECT_TRUE(result.Test(10));
	EXPECT_TRUE(result.Test(20));
	EXPECT_FALSE(result.Test(25));

	EXPECT_EQ(result.NumSetBits(), 2);
}

TEST(HierarchicalBitMask, BitwiseAndAssign) {
	Deep::HierarchicalBitMask<32> a{};
	Deep::HierarchicalBitMask<32> b{};

	a.Set<true>(4);
	a.Set<true>(12);

	b.Set<true>(12);
	b.Set<true>(18);

	a &= b;

	EXPECT_FALSE(a.Test(4));
	EXPECT_TRUE(a.Test(12));
	EXPECT_FALSE(a.Test(18));

	EXPECT_EQ(a.NumSetBits(), 1);
}

TEST(HierarchicalBitMask, BitwiseXor) {
	Deep::HierarchicalBitMask<32> a{};
	Deep::HierarchicalBitMask<32> b{};

	a.Set<true>(1);
	a.Set<true>(10);
	a.Set<true>(20);

	b.Set<true>(10);
	b.Set<true>(25);

	const auto result = a ^ b;

	EXPECT_TRUE(result.Test(1));
	EXPECT_FALSE(result.Test(10));
	EXPECT_TRUE(result.Test(20));
	EXPECT_TRUE(result.Test(25));

	EXPECT_EQ(result.NumSetBits(), 3);
}

TEST(HierarchicalBitMask, BitwiseXorAssign) {
	Deep::HierarchicalBitMask<32> a{};
	Deep::HierarchicalBitMask<32> b{};

	a.Set<true>(5);
	a.Set<true>(6);

	b.Set<true>(6);
	b.Set<true>(7);

	a ^= b;

	EXPECT_TRUE(a.Test(5));
	EXPECT_FALSE(a.Test(6));
	EXPECT_TRUE(a.Test(7));

	EXPECT_EQ(a.NumSetBits(), 2);
}

TEST(HierarchicalBitMask, BitwiseNot) {
	Deep::HierarchicalBitMask<32> a{};

	a.Set<true>(3);
	a.Set<true>(9);

	const auto result = ~a;

	EXPECT_FALSE(result.Test(3));
	EXPECT_FALSE(result.Test(9));

	for (size_t i = 0; i < 32; ++i) {
		if (i == 3 || i == 9) continue;

		EXPECT_TRUE(result.Test(i));
	}

	EXPECT_EQ(result.NumSetBits(), 30);
}

TEST(HierarchicalBitMask, Equality) {
	Deep::HierarchicalBitMask<32> a{};
	Deep::HierarchicalBitMask<32> b{};

	EXPECT_TRUE(a == b);

	a.Set<true>(10);

	EXPECT_FALSE(a == b);

	b.Set<true>(10);

	EXPECT_TRUE(a == b);

	a.Set<true>(20);

	EXPECT_FALSE(a == b);

	b.Set<true>(20);

	EXPECT_TRUE(a == b);
}

TEST(HierarchicalBitMask, NumSetBits) {
	Deep::HierarchicalBitMask<128> mask{};

	EXPECT_EQ(mask.NumSetBits(), 0);

	constexpr size_t indices[] = {
		0, 1, 31, 32, 33, 63, 64, 95, 96, 127,
	};

	for (const size_t index : indices) {
		mask.Set(index, true);
	}

	EXPECT_EQ(mask.NumSetBits(), std::size(indices));

	mask.Set<false>(32);
	mask.Set<false>(96);

	EXPECT_EQ(mask.NumSetBits(), std::size(indices) - 2);
}

TEST(HierarchicalBitMask, IndexOfLowestSetBit) {
	Deep::HierarchicalBitMask<128> mask{};

	mask.Set<true>(93);
	EXPECT_EQ(mask.IndexOfLowestSetBit(), 93);

	mask.Set<true>(64);
	EXPECT_EQ(mask.IndexOfLowestSetBit(), 64);

	mask.Set<true>(17);
	EXPECT_EQ(mask.IndexOfLowestSetBit(), 17);

	mask.Set<true>(0);
	EXPECT_EQ(mask.IndexOfLowestSetBit(), 0);
}

TEST(HierarchicalBitMask, ClearLowestSetBit) {
	Deep::HierarchicalBitMask<128> mask{};

	mask.Set<true>(5);
	mask.Set<true>(37);
	mask.Set<true>(64);
	mask.Set<true>(127);

	EXPECT_EQ(mask.IndexOfLowestSetBit(), 5);

	mask.ClearLowestSetBit();

	EXPECT_FALSE(mask.Test(5));
	EXPECT_EQ(mask.IndexOfLowestSetBit(), 37);

	mask.ClearLowestSetBit();

	EXPECT_FALSE(mask.Test(37));
	EXPECT_EQ(mask.IndexOfLowestSetBit(), 64);

	mask.ClearLowestSetBit();

	EXPECT_FALSE(mask.Test(64));
	EXPECT_EQ(mask.IndexOfLowestSetBit(), 127);

	mask.ClearLowestSetBit();

	EXPECT_FALSE(mask.Any());
	EXPECT_EQ(mask.NumSetBits(), 0);
}

TEST(HierarchicalBitMask, PopLowestSetBit) {
	Deep::HierarchicalBitMask<128> mask{};

	mask.Set<true>(100);
	mask.Set<true>(2);
	mask.Set<true>(63);
	mask.Set<true>(64);

	EXPECT_EQ(mask.PopLowestSetBit(), 2);
	EXPECT_FALSE(mask.Test(2));

	EXPECT_EQ(mask.PopLowestSetBit(), 63);
	EXPECT_FALSE(mask.Test(63));

	EXPECT_EQ(mask.PopLowestSetBit(), 64);
	EXPECT_FALSE(mask.Test(64));

	EXPECT_EQ(mask.PopLowestSetBit(), 100);
	EXPECT_FALSE(mask.Test(100));

	EXPECT_FALSE(mask.Any());
}

TEST(HierarchicalBitMask, ChunkBoundaryOperations) {
	using Mask = Deep::HierarchicalBitMask<128>;

	constexpr size_t bitsPerChunk = Mask::k_bitsPerChunk;

	Mask mask{};

	mask.Set<true>(bitsPerChunk - 1);
	mask.Set<true>(bitsPerChunk);
	mask.Set<true>(bitsPerChunk + 1);

	EXPECT_TRUE(mask.Test(bitsPerChunk - 1));
	EXPECT_TRUE(mask.Test(bitsPerChunk));
	EXPECT_TRUE(mask.Test(bitsPerChunk + 1));

	EXPECT_EQ(mask.NumSetBits(), 3);
	EXPECT_EQ(mask.IndexOfLowestSetBit(), bitsPerChunk - 1);
}

TEST(HierarchicalBitMask, SummaryUpdatedWhenLeafBecomesOccupied) {
	using Mask = Deep::HierarchicalBitMask<512>;

	Mask mask{};

	EXPECT_FALSE(mask.Any());

	mask.Set<true>(400);

	EXPECT_TRUE(mask.Any());
	EXPECT_TRUE(mask.Test(400));

	// Every level above the leaf should now contain occupancy
	// information for the path to bit 400.
	for (size_t level = 1; level < Mask::k_numLevels; ++level) {
		bool levelHasBits = false;

		for (size_t i = 0; i < Mask::k_levelSizes[level]; ++i) {
			if (mask.m_chunks[Mask::k_levelOffsets[level] + i] != 0) {
				levelHasBits = true;
				break;
			}
		}

		EXPECT_TRUE(levelHasBits);
	}
}

TEST(HierarchicalBitMask, SummaryRemainsSetWhenLeafStillOccupied) {
	using Mask = Deep::HierarchicalBitMask<512>;

	Mask mask{};

	mask.Set<true>(100);
	mask.Set<true>(101);

	EXPECT_TRUE(mask.Any());

	mask.Set<false>(100);

	EXPECT_FALSE(mask.Test(100));
	EXPECT_TRUE(mask.Test(101));

	// The hierarchy must still report occupancy.
	EXPECT_TRUE(mask.Any());

	mask.Set<false>(101);

	EXPECT_FALSE(mask.Any());
}

TEST(HierarchicalBitMask, SummaryPropagatesToRootWhenCleared) {
	using Mask = Deep::HierarchicalBitMask<512>;

	Mask mask{};

	mask.Set<true>(511);

	EXPECT_TRUE(mask.Any());

	mask.ClearLowestSetBit();

	EXPECT_FALSE(mask.Any());

	const size_t rootOffset = Mask::k_levelOffsets[Mask::k_numLevels - 1];

	EXPECT_EQ(mask.m_chunks[rootOffset], 0);
}

TEST(HierarchicalBitMask, MultipleHierarchyBranches) {
	using Mask = Deep::HierarchicalBitMask<512>;

	Mask mask{};

	// Deliberately place bits in widely separated leaf chunks.
	mask.Set<true>(1);
	mask.Set<true>(100);
	mask.Set<true>(255);
	mask.Set<true>(400);
	mask.Set<true>(511);

	EXPECT_EQ(mask.NumSetBits(), 5);

	EXPECT_EQ(mask.PopLowestSetBit(), 1);
	EXPECT_EQ(mask.PopLowestSetBit(), 100);
	EXPECT_EQ(mask.PopLowestSetBit(), 255);
	EXPECT_EQ(mask.PopLowestSetBit(), 400);
	EXPECT_EQ(mask.PopLowestSetBit(), 511);

	EXPECT_FALSE(mask.Any());
}

TEST(HierarchicalBitMask, OrMaintainsHierarchy) {
	using Mask = Deep::HierarchicalBitMask<512>;

	Mask a{};
	Mask b{};

	a.Set<true>(10);
	a.Set<true>(200);

	b.Set<true>(100);
	b.Set<true>(400);

	a |= b;

	EXPECT_TRUE(a.Any());

	EXPECT_TRUE(a.Test(10));
	EXPECT_TRUE(a.Test(100));
	EXPECT_TRUE(a.Test(200));
	EXPECT_TRUE(a.Test(400));

	EXPECT_EQ(a.NumSetBits(), 4);

	EXPECT_EQ(a.PopLowestSetBit(), 10);
	EXPECT_EQ(a.PopLowestSetBit(), 100);
	EXPECT_EQ(a.PopLowestSetBit(), 200);
	EXPECT_EQ(a.PopLowestSetBit(), 400);

	EXPECT_FALSE(a.Any());
}

TEST(HierarchicalBitMask, AndRebuildsHierarchyCorrectly) {
	using Mask = Deep::HierarchicalBitMask<512>;

	Mask a{};
	Mask b{};

	a.Set<true>(5);
	a.Set<true>(200);
	a.Set<true>(400);

	b.Set<true>(10);
	b.Set<true>(200);
	b.Set<true>(450);

	a &= b;

	EXPECT_TRUE(a.Any());
	EXPECT_EQ(a.NumSetBits(), 1);

	EXPECT_TRUE(a.Test(200));

	EXPECT_EQ(a.IndexOfLowestSetBit(), 200);

	a.ClearLowestSetBit();

	EXPECT_FALSE(a.Any());
}

TEST(HierarchicalBitMask, XorRebuildsHierarchyCorrectly) {
	using Mask = Deep::HierarchicalBitMask<512>;

	Mask a{};
	Mask b{};

	a.Set<true>(5);
	a.Set<true>(200);
	a.Set<true>(400);

	b.Set<true>(5);
	b.Set<true>(200);
	b.Set<true>(450);

	a ^= b;

	EXPECT_FALSE(a.Test(5));
	EXPECT_FALSE(a.Test(200));

	EXPECT_TRUE(a.Test(400));
	EXPECT_TRUE(a.Test(450));

	EXPECT_EQ(a.NumSetBits(), 2);

	EXPECT_EQ(a.PopLowestSetBit(), 400);
	EXPECT_EQ(a.PopLowestSetBit(), 450);

	EXPECT_FALSE(a.Any());
}

TEST(HierarchicalBitMask, InverseRebuildsHierarchyCorrectly) {
	using Mask = Deep::HierarchicalBitMask<512>;

	Mask mask{};

	mask.Set<true>(0);
	mask.Set<true>(255);
	mask.Set<true>(511);

	mask.Inverse();

	EXPECT_FALSE(mask.Test(0));
	EXPECT_FALSE(mask.Test(255));
	EXPECT_FALSE(mask.Test(511));

	EXPECT_EQ(mask.NumSetBits(), 509);

	EXPECT_TRUE(mask.Any());

	EXPECT_EQ(mask.IndexOfLowestSetBit(), 1);
}

TEST(HierarchicalBitMask, RepeatedPopProducesSortedIndices) {
	using Mask = Deep::HierarchicalBitMask<512>;

	Mask mask{};

	constexpr size_t indices[] = {
		3, 17, 63, 64, 65, 127, 128, 255, 256, 400, 511,
	};

	for (const size_t index : indices) {
		mask.Set(index, true);
	}

	for (const size_t expected : indices) {
		ASSERT_TRUE(mask.Any());
		EXPECT_EQ(mask.PopLowestSetBit(), expected);
	}

	EXPECT_FALSE(mask.Any());
}
