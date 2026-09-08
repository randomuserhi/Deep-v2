#include "Tests.h"

#include "Deep/Physics2D/Colliders/Aabb.h"

TEST(Aabb2D, OverlapBoundaries) {
	Deep::Aabb2D box{ .m_center = { 2, 3 }, .m_extents = { 1, 2 } };
	EXPECT_TRUE(Deep::IsOverlapping(Deep::Vec2{ 2, 3 }, box));
	const Deep::Vec2 outside[] = { { 1, 3 }, { 3, 3 }, { 2, 1 }, { 2, 5 }, { 0, 3 }, { 2, 6 } };
	for (const auto& point : outside)
		EXPECT_FALSE(Deep::IsOverlapping(point, box));
	EXPECT_TRUE(Deep::IsOverlapping(box, box));
	Deep::Aabb2D touching{ .m_center = { 4, 3 }, .m_extents = { 1, 1 } };
	EXPECT_FALSE(Deep::IsOverlapping(box, touching));
	EXPECT_FALSE(Deep::IsOverlapping(touching, box));
	Deep::ContactInfo2D contact{ .m_normal = { 7, 8 }, .m_penetrationDistance = 9 };
	EXPECT_EQ(Deep::IsOverlapping(box, touching, &contact), 0);
	EXPECT_EQ(contact.m_normal, (Deep::Vec2{ 7, 8 }));
	EXPECT_EQ(contact.m_penetrationDistance, 9.0f);
}

TEST(Aabb2D, ContactAxes) {
	Deep::Aabb2D a{ .m_center = { 2, 3 }, .m_extents = { 1, 1 } };
	for (size_t axis = 0; axis < 2; ++axis) {
		for (float sign : { -1.0f, 1.0f }) {
			auto b = a;
			b.m_center[axis] += sign * 1.5f;
			Deep::ContactInfo2D contact;
			ASSERT_TRUE(Deep::IsOverlapping(a, b, &contact) == 1);
			EXPECT_EQ(contact.m_penetrationDistance, 0.5f);
			EXPECT_EQ(contact.m_normal[axis], sign);
			EXPECT_EQ(contact.m_normal[1 - axis], 0.0f);
			ASSERT_TRUE(Deep::IsOverlapping(b, a, &contact) == 1);
			EXPECT_EQ(contact.m_normal[axis], -sign);
			EXPECT_EQ(contact.m_penetrationDistance, 0.5f);
		}
	}
}

TEST(Aabb2D, RaycastQueries) {
	Deep::Aabb2D box{ .m_center = { 2, 3 }, .m_extents = { 1, 1 } };
	struct Case {
		Deep::Vec2 m_origin;
		Deep::Vec2 m_direction;
		int m_outsideCount;
		int m_insideCount;
		float m_first;
		float m_last;
	};
	const Case cases[] = {
		{ { -1, 3 }, { 1, 0 }, 2, 2, 2, 4 },  { { 2, 6 }, { 0, -1 }, 2, 2, 2, 4 },
		{ { 2, 3 }, { 1, 0 }, 0, 1, 1, 1 },   { { 2.5f, 3 }, { 1, 0 }, 0, 1, 0.5f, 0.5f },
		{ { 1, 3 }, { 1, 0 }, 2, 2, 0, 2 },   { { 1, 3 }, { -1, 0 }, 0, 0, 0, 0 },
		{ { -1, 3 }, { -1, 0 }, 0, 0, 0, 0 }, { { -1, 5 }, { 1, 0 }, 0, 0, 0, 0 },
	};
	auto check = [&]<Deep::RaycastType2D in_queryType>() {
		for (const Case& test : cases) {
			Deep::Ray2D ray{ test.m_origin, test.m_direction };
			int expected = in_queryType == Deep::RaycastType2D::e_startsOutside ? test.m_outsideCount : test.m_insideCount;
			Deep::RayHit2D sentinel{ .m_point = { 7, 8 }, .m_normal = { 9, 10 }, .m_distance = 11 };
			Deep::RayHit2D hit = sentinel;
			Deep::RayHit2D hits[3]{ sentinel, sentinel, sentinel };
			EXPECT_TRUE(Deep::Raycast<in_queryType>(ray, box) == (expected > 0));
			EXPECT_TRUE(Deep::Raycast<in_queryType>(ray, box, &hit) == (expected > 0));
			EXPECT_EQ(Deep::RaycastAll<in_queryType>(ray, box, hits), expected);
			if (expected > 0) {
				EXPECT_EQ(hit.m_distance, test.m_first);
				EXPECT_EQ(hit.m_point, hits[0].m_point);
				EXPECT_EQ(hit.m_normal, hits[0].m_normal);
				for (int i = 0; i < expected; ++i) {
					EXPECT_EQ(hits[i].m_distance, i == 0 ? test.m_first : test.m_last);
					EXPECT_EQ(hits[i].m_point, ray.m_origin + ray.m_direction * hits[i].m_distance);
					EXPECT_TRUE(hits[i].m_normal.IsNormalized());
					float dot = Deep::Vec2::s_Dot(hits[i].m_normal, ray.m_direction);
					EXPECT_EQ(dot, expected == 2 && i == 0 ? -1.0f : 1.0f);
				}
			} else {
				EXPECT_EQ(hit.m_point, sentinel.m_point);
				EXPECT_EQ(hit.m_normal, sentinel.m_normal);
				EXPECT_EQ(hit.m_distance, sentinel.m_distance);
			}
			for (int i = expected; i < 3; ++i) {
				EXPECT_EQ(hits[i].m_point, sentinel.m_point);
				EXPECT_EQ(hits[i].m_normal, sentinel.m_normal);
				EXPECT_EQ(hits[i].m_distance, sentinel.m_distance);
			}
		}
	};
	check.operator()<Deep::RaycastType2D::e_startsOutside>();
	check.operator()<Deep::RaycastType2D::e_startsInside>();
}
