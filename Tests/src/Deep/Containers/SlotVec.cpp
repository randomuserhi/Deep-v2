#include "Tests.h"

#include "Deep/Containers/SlotVec.h"

#include <memory>

static_assert(Deep::SlotVec<int>::k_maxCapacity == (Deep::uint64{ 1 } << 48));
static_assert(sizeof(Deep::SlotVec<int>::Handle) == 8);

TEST(SlotVec, ClearPreservesVersions) {
	Deep::SlotVec<int> items;
	const auto a = items.PushBack(10);
	const auto b = items.PushBack(20);
	const auto c = items.PushBack(30);
	items.Remove(b);
	items.Clear();
	items.Clear();
	for (int i = 0; i < 3; ++i) {
		const auto id = items.PushBack(i);
		EXPECT_TRUE(items.IsActive(id));
		EXPECT_FALSE(items.IsActive(a));
		EXPECT_FALSE(items.IsActive(b));
		EXPECT_FALSE(items.IsActive(c));
		EXPECT_EQ(items.GetId(items.GetSlot(id)), id);
	}
	EXPECT_FALSE(items.IsActive(items.GetId(0) ^ 1));
	EXPECT_FALSE(items.IsActive(Deep::uint64{ 1 } << 63));
	EXPECT_FALSE(items.IsActive(~Deep::uint64{ 0 }));
	items.Clear();
	Deep::SlotVec<int> copy{ items };
	const auto id = copy.PushBack(99);
	EXPECT_NE(id, a);
	EXPECT_NE(id, b);
	EXPECT_NE(id, c);
	EXPECT_EQ(copy[id], 99);
}

TEST(SlotVec, VersionWrapDoesNotChangeIndex) {
	Deep::SlotVec<int> items;
	const auto survivor = items.PushBack(10);
	const auto original = items.PushBack(20);
	auto id = original;
	for (Deep::uint64 version = 1; version <= 65536; ++version) {
		items.Remove(id);
		EXPECT_FALSE(items.IsActive(id));
		const auto next = items.PushBack(20);
		EXPECT_NE(next, id);
		EXPECT_EQ(next >> 16, original >> 16);
		EXPECT_EQ(next & 65535, version & 65535);
		EXPECT_EQ(items.GetSlot(next), 1);
		EXPECT_EQ(items[survivor], 10);
		id = next;
	}
	// A 16-bit version repeats after 65,536 reuses of the same index.
	EXPECT_EQ(id, original);
}

static_assert(std::is_same_v<decltype(std::declval<Deep::SlotVec<int>&>()[0]), int&>);
static_assert(std::is_same_v<decltype(std::declval<const Deep::SlotVec<int>&>()[0]), const int&>);
static_assert(std::is_same_v<decltype(*std::declval<Deep::SlotVec<int>&>().begin()), int&>);
static_assert(std::is_same_v<decltype(*std::declval<const Deep::SlotVec<int>&>().begin()), const int&>);

TEST(SlotVec, EmptyAndReserve) {
	Deep::SlotVec<int> items;
	EXPECT_EQ(items.Size(), 0);
	EXPECT_FALSE(items.IsActive(0));
	EXPECT_TRUE(items.begin() == items.end());
	const auto& view = items;
	EXPECT_TRUE(view.begin() == view.end());
	items.Clear();
	items.Reserve(0);
	items.Reserve(8);
	EXPECT_EQ(items.Size(), 0);
	const Deep::uint64 a = items.PushBack(10);
	const Deep::uint64 b = items.PushBack(20);
	items.Remove(a);
	items.Reserve(32);
	EXPECT_EQ(items.Size(), 1);
	EXPECT_EQ(items[b], 20);
	EXPECT_FALSE(items.IsActive(a));
	items.Reserve(1);
	const auto replacement = items.PushBack(30);
	EXPECT_EQ(replacement, a + 1);
	EXPECT_EQ(items[replacement], 30);
	items.Clear();
	EXPECT_FALSE(items.IsActive(a));
	EXPECT_FALSE(items.IsActive(b));
	EXPECT_TRUE(items.begin() == items.end());
}

TEST(SlotVec, MultipleHolesAndSparseCopy) {
	Deep::SlotVec<int> items;
	for (int i = 0; i < 16; ++i)
		items.PushBack(i * 10);
	for (size_t i = 0; i < 16; i += 2)
		items.Remove(static_cast<Deep::uint64>(i) << 16);

	Deep::SlotVec<int> copy{ items };
	for (size_t i = 0; i < 16; ++i) {
		EXPECT_TRUE(copy.IsActive(static_cast<Deep::uint64>(i) << 16) == (i % 2 != 0));
		if (i % 2 != 0) EXPECT_EQ(copy[static_cast<Deep::uint64>(i) << 16], static_cast<int>(i * 10));
	}
	bool seen[16]{};
	for (size_t slot = 0; slot < copy.Size(); ++slot) {
		const Deep::uint64 id = copy.GetId(slot);
		EXPECT_TRUE((id >> 16) < 16);
		if ((id >> 16) >= 16) continue;
		EXPECT_FALSE(seen[id >> 16]);
		seen[id >> 16] = true;
		EXPECT_EQ(copy[id], *(copy.begin() + slot));
	}
	for (int i = 0; i < 8; ++i) {
		const Deep::uint64 id = copy.PushBack(100 + i);
		EXPECT_TRUE((id >> 16) < 16 && (id >> 16) % 2 == 0);
		EXPECT_FALSE(items.IsActive(id));
		EXPECT_EQ(copy[id], 100 + i);
	}
	EXPECT_EQ(copy.Size(), 16);
	for (size_t i = 0; i < 16; ++i)
		EXPECT_TRUE(copy.IsActive(copy.GetId(i)));
	const Deep::uint64 newId = copy.PushBack(999);
	EXPECT_EQ(newId, Deep::uint64{ 16 } << 16);
	EXPECT_EQ(copy[newId], 999);
}

TEST(SlotVec, EmplaceAndObjectLifetime) {
	using Item = std::pair<std::unique_ptr<int>, int>;
	Deep::SlotVec<Item> pairs;
	const Deep::uint64 first = pairs.Emplace(std::make_unique<int>(42), 7);
	const Deep::uint64 second = pairs.Emplace(std::make_unique<int>(84), 9);
	EXPECT_EQ(*pairs[first].first, 42);
	EXPECT_EQ(pairs[first].second, 7);
	pairs.Remove(first);
	EXPECT_EQ(*pairs[second].first, 84);
	EXPECT_EQ(pairs[second].second, 9);

	std::weak_ptr<int> removed;
	std::weak_ptr<int> cleared;
	std::weak_ptr<int> destroyed;
	{
		Deep::SlotVec<std::shared_ptr<int>> items;
		const Deep::uint64 a = items.PushBack(std::make_shared<int>(1));
		removed = items[a];
		const Deep::uint64 b = items.PushBack(std::make_shared<int>(2));
		cleared = items[b];
		items.Remove(a);
		EXPECT_TRUE(removed.expired());
		EXPECT_FALSE(cleared.expired());
		EXPECT_EQ(*items[b], 2);
		items.Clear();
		EXPECT_TRUE(cleared.expired());
		const Deep::uint64 c = items.PushBack(std::make_shared<int>(3));
		destroyed = items[c];
	}
	EXPECT_TRUE(destroyed.expired());
}

TEST(SlotVec, SwapAndSelfAssignment) {
	Deep::SlotVec<int> a;
	a.PushBack(10);
	const Deep::uint64 removed = a.PushBack(20);
	const Deep::uint64 live = a.PushBack(30);
	a.Remove(removed);
	Deep::SlotVec<int> b;
	b.PushBack(99);
	a.Swap(b);
	EXPECT_EQ(a.Size(), 1);
	EXPECT_EQ(a[0], 99);
	EXPECT_EQ(b.Size(), 2);
	EXPECT_EQ(b[live], 30);
	EXPECT_FALSE(b.IsActive(removed));
	auto& alias = b;
	b = alias;
	b = std::move(alias);
	b.Swap(alias);
	EXPECT_EQ(b.Size(), 2);
	EXPECT_EQ(b[live], 30);
	EXPECT_EQ(b.PushBack(40), removed + 1);
	Deep::SlotVec<int> empty;
	a = empty;
	EXPECT_EQ(a.Size(), 0);
	EXPECT_FALSE(a.IsActive(0));
	EXPECT_EQ(a.PushBack(5), 0);
}

TEST(SlotVec, StableIdsAndIteration) {
	Deep::SlotVec<int> items;
	const Deep::uint64 a = items.PushBack(10);
	const Deep::uint64 b = items.PushBack(20);
	const Deep::uint64 c = items.PushBack(30);
	EXPECT_EQ(items.GetSlot(c), 2);
	items.Remove(b);
	EXPECT_EQ(items.Size(), 2);
	EXPECT_FALSE(items.IsActive(b));
	EXPECT_EQ(items[a], 10);
	EXPECT_EQ(items[c], 30);
	EXPECT_EQ(items.GetId(1), c);
	EXPECT_EQ(items.GetSlot(c), 1);
	EXPECT_FALSE(items.IsActive(size_t(-1)));

	for (int& item : items)
		item += 1;
	const auto& view = items;
	int sum = 0;
	for (const int& item : view)
		sum += item;
	EXPECT_EQ(sum, 42);
	EXPECT_EQ(view[c], 31);
	EXPECT_EQ(view.GetSlot(a), 0);
	EXPECT_EQ(view.end() - view.begin(), 2);

	const auto replacement = items.PushBack(40);
	EXPECT_EQ(replacement, b + 1);
	EXPECT_FALSE(items.IsActive(b));
	EXPECT_EQ(items[replacement], 40);
	items.Remove(replacement); // Last slot.
	items.Remove(a);           // First slot.
	EXPECT_EQ(items[c], 31);
	items.Remove(c); // Only slot.
	EXPECT_TRUE(items.begin() == items.end());
	EXPECT_FALSE(items.IsActive(c));
	EXPECT_EQ(items.PushBack(50), c + 1);
}

TEST(SlotVec, GrowthAndAliasing) {
	Deep::SlotVec<int> items;
	for (int i = 0; i < 257; ++i) {
		EXPECT_EQ(items.PushBack(i), (static_cast<Deep::uint64>(i) << 16));
	}
	for (size_t i = 0; i < items.Size(); ++i)
		EXPECT_EQ(items[items.GetId(i)], static_cast<int>(i));

	Deep::SlotVec<int> alias{ 1 };
	const Deep::uint64 first = alias.PushBack(42);
	const Deep::uint64 second = alias.PushBack(alias[first]);
	EXPECT_EQ(alias[first], 42);
	EXPECT_EQ(alias[second], 42);
	alias.Clear();
	EXPECT_TRUE(alias.begin() == alias.end());
	EXPECT_FALSE(alias.IsActive(first));
	EXPECT_EQ(alias.PushBack(7), first + 1);
	EXPECT_FALSE(alias.IsActive(first));
}

TEST(SlotVec, CopyMoveAndReuse) {
	Deep::SlotVec<int> source;
	const Deep::uint64 a = source.PushBack(1);
	const Deep::uint64 b = source.PushBack(2);
	const Deep::uint64 c = source.PushBack(3);
	source.Remove(b);
	Deep::SlotVec<int> copy{ source };
	copy[a] = 10;
	EXPECT_EQ(source[a], 1);
	EXPECT_EQ(copy.PushBack(20), b + 1);
	EXPECT_EQ(copy[c], 3);
	Deep::SlotVec<int> assigned;
	assigned = source;
	EXPECT_EQ(assigned.PushBack(30), b + 1);
	Deep::SlotVec<int> moved{ std::move(source) };
	EXPECT_EQ(source.Size(), 0);
	EXPECT_EQ(source.PushBack(99), 0);
	EXPECT_EQ(moved[c], 3);
	assigned = std::move(moved);
	EXPECT_EQ(moved.Size(), 0);
	EXPECT_EQ(moved.PushBack(100), 0);
	EXPECT_EQ(assigned.PushBack(40), b + 1);
	EXPECT_EQ(assigned[a], 1);
}

TEST(SlotVec, MoveOnly) {
	Deep::SlotVec<std::unique_ptr<int>> items;
	const Deep::uint64 a = items.PushBack(std::make_unique<int>(10));
	const Deep::uint64 b = items.PushBack(std::make_unique<int>(20));
	items.Remove(a);
	EXPECT_EQ(*items[b], 20);
	const auto replacement = items.PushBack(std::make_unique<int>(30));
	EXPECT_EQ(replacement, a + 1);
	EXPECT_FALSE(items.IsActive(a));
	EXPECT_EQ(*items[replacement], 30);
}

TEST(SlotVec, RepeatedRemovalAndReuse) {
	Deep::SlotVec<int> items;
	int expected[64]{};
	Deep::uint64 ids[64]{};
	for (size_t i = 0; i < 64; ++i)
		ids[i] = items.PushBack(0);
	for (int step = 1; step <= 1000; ++step) {
		const size_t index = static_cast<size_t>(step * 17) % 64;
		const auto id = ids[index];
		items.Remove(id);
		EXPECT_FALSE(items.IsActive(id));
		ids[index] = items.PushBack(step);
		EXPECT_NE(ids[index], id);
		EXPECT_FALSE(items.IsActive(id));
		expected[index] = step;
		for (size_t i = 0; i < 64; ++i) {
			EXPECT_EQ(items[ids[i]], expected[i]);
			EXPECT_EQ(items[items.GetId(i)], *(items.begin() + i));
			EXPECT_EQ(items.GetSlot(items.GetId(i)), i);
		}
	}
}
