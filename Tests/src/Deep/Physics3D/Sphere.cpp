#include "Tests.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Ray.h"
#include "Deep/Physics3D/Colliders.h"
#include "Deep/Physics3D/Colliders/Sphere.h"

// TODO(randomuserhi): Add grazing tests (when box edge == second box edge, or point lies on edge or ray origin starts on
//                     edge)
// TODO(randomuserhi): Standardize grazing behaviour:
//                     - for overlap, graze counts as true
//                     - for raycasts, graze counts as outside the collider, ray moving away has 0 hits, ray going towards
//                       gives 2 hits for RaycastAll and returns the ray origin as the hit point for regular raycast

TEST(Sphere, IsOverlapping) {
	TEST_CASE(Point) {
		TEST_CASE(ShouldOverlap) {
			Deep::Vec3 point{ 0.25f, 0.25f, 0.25f };
			Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };

			EXPECT_TRUE(Deep::IsOverlapping(point, a));
		}

		TEST_CASE(NoOverlap) {
			Deep::Vec3 point{ 1.25f, 0.25f, 0.25f };
			Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };

			EXPECT_FALSE(Deep::IsOverlapping(point, a));
		}
	}

	TEST_CASE(Sphere) {
		TEST_CASE(ShouldOverlap) {
			Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
			Deep::Sphere b{ .m_center = { 0.25f, 0.25f, 0.25f }, .m_radius = 0.5f };

			EXPECT_TRUE(Deep::IsOverlapping(a, b));
		}

		TEST_CASE(NoOverlap) {
			Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
			Deep::Sphere b{ .m_center = { 1.0f, 1.0f, 1.0f }, .m_radius = 0.5f };

			EXPECT_FALSE(Deep::IsOverlapping(a, b));
		}
	}
}

TEST(Sphere, Raycast) {
	TEST_CASE(Outside) {
		TEST_CASE(ShouldHit) {
			Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
			Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { 1, 0, 0 } };
			EXPECT_TRUE(Deep::Raycast<Deep::RaycastType3D::e_startsOutside>(ray, a));
			EXPECT_TRUE(Deep::Raycast<Deep::RaycastType3D::e_startsInside>(ray, a));
		}

		TEST_CASE(NoHit) {
			Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
			Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { -1, 0, 0 } };
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType3D::e_startsOutside>(ray, a));
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType3D::e_startsInside>(ray, a));
		}
	}

	TEST_CASE(Inside) {
		Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
		Deep::Ray3D ray{ .m_origin = { 0, 0, 0 }, .m_direction = { 1, 0, 0 } };
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType3D::e_startsOutside>(ray, a));
		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType3D::e_startsInside>(ray, a));
	}
}

TEST(Sphere, Grazing) {
	TEST_CASE(IsOverlapping) {
		TEST_CASE(Point) {
			Deep::Vec3 point{ 0.5f, 0.0f, 0.0f };
			Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };

			EXPECT_FALSE(Deep::IsOverlapping(point, a));
		}

		TEST_CASE(Sphere) {
			Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
			Deep::Sphere b{ .m_center = { 1.0f, 0.0f, 0.0f }, .m_radius = 0.5f };

			EXPECT_FALSE(Deep::IsOverlapping(a, b));
		}
	}

	TEST_CASE(Raycast) {
		TEST_CASE(ShouldHit) {
			TEST_CASE(OnEdge) {
				Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
				Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0 }, .m_direction = { -1, 0, 0 } };
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType3D::e_startsOutside>(ray, a));
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType3D::e_startsInside>(ray, a));
			}

			TEST_CASE(WithEdge) {
				Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
				Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0.5f }, .m_direction = { -1, 0, 0 } };
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType3D::e_startsOutside>(ray, a));
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType3D::e_startsInside>(ray, a));
			}
		}

		TEST_CASE(NoHit) {
			Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
			Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0 }, .m_direction = { 1, 0, 0 } };
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType3D::e_startsOutside>(ray, a));
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType3D::e_startsInside>(ray, a));
		}
	}
}

TEST(Sphere, RaycastHits) {
	Deep::Sphere sphere{ .m_center = { 2, 3, 4 }, .m_radius = 2 };
	struct Case {
		Deep::Vec3 m_origin;
		Deep::Vec3 m_direction;
		Deep::int32 m_outsideCount;
		Deep::int32 m_insideCount;
		Deep::float32 m_enter;
		Deep::float32 m_exit;
	};
	const Case cases[] = {
		{ { -2, 3, 4 }, { 1, 0, 0 }, 2, 2, 2, 6 }, // Through center
		{ { 2, -1, 4 }, { 0, 1, 0 }, 2, 2, 2, 6 },  { { 2, 3, 8 }, { 0, 0, -1 }, 2, 2, 2, 6 },
		{ { 2, 3, 4 }, { 1, 0, 0 }, 0, 1, 2, 2 }, // At center
		{ { 3, 3, 4 }, { 1, 0, 0 }, 0, 1, 1, 1 }, // Inside, moving away from center
		{ { 3, 3, 4 }, { -1, 0, 0 }, 0, 1, 3, 3 },  { { 0, 3, 4 }, { 1, 0, 0 }, 2, 2, 0, 4 }, // Surface, moving inward
		{ { 0, 3, 4 }, { -1, 0, 0 }, 0, 0, 0, 0 },                                            // Surface, moving outward
		{ { -2, 5, 4 }, { 1, 0, 0 }, 2, 2, 4, 4 },  // Tangent entry and exit, as with Aabb
		{ { -2, 6, 4 }, { 1, 0, 0 }, 0, 0, 0, 0 },  // Miss
		{ { -2, 3, 4 }, { -1, 0, 0 }, 0, 0, 0, 0 }, // Both roots behind origin
	};
	auto check = [&]<Deep::RaycastType3D in_queryType>() {
		for (const Case& test : cases) {
			Deep::Ray3D ray{ test.m_origin, test.m_direction };
			Deep::int32 expected =
				in_queryType == Deep::RaycastType3D::e_startsOutside ? test.m_outsideCount : test.m_insideCount;
			Deep::RayHit3D sentinel{ .m_point = { 9, 8, 7 }, .m_normal = { 6, 5, 4 }, .m_distance = 123 };
			Deep::RayHit3D hit = sentinel;
			Deep::RayHit3D hits[3]{ sentinel, sentinel, sentinel };
			EXPECT_TRUE(Deep::Raycast<in_queryType>(ray, sphere) == (expected > 0));
			EXPECT_TRUE(Deep::Raycast<in_queryType>(ray, sphere, &hit) == (expected > 0));
			EXPECT_EQ(Deep::RaycastAll<in_queryType>(ray, sphere, hits), expected);
			if (expected > 0) {
				EXPECT_EQ(hit.m_distance, test.m_enter);
				EXPECT_EQ(hit.m_point, hits[0].m_point);
				EXPECT_EQ(hit.m_Normal(), hits[0].m_Normal());
				for (Deep::int32 i = 0; i < expected; ++i) {
					Deep::float32 distance = i == 0 ? test.m_enter : test.m_exit;
					EXPECT_EQ(hits[i].m_distance, distance);
					EXPECT_EQ(hits[i].m_point, ray.m_origin + ray.m_direction * distance);
					EXPECT_TRUE(hits[i].m_Normal().IsNormalized());
					EXPECT_EQ(hits[i].m_Normal(), (hits[i].m_point - sphere.m_Center()) / sphere.m_radius);
				}
			} else {
				EXPECT_EQ(hit.m_point, sentinel.m_point);
				EXPECT_EQ(hit.m_Normal(), sentinel.m_Normal());
				EXPECT_EQ(hit.m_distance, sentinel.m_distance);
			}
			for (Deep::int32 i = expected; i < 3; ++i) {
				EXPECT_EQ(hits[i].m_point, sentinel.m_point);
				EXPECT_EQ(hits[i].m_Normal(), sentinel.m_Normal());
				EXPECT_EQ(hits[i].m_distance, sentinel.m_distance);
			}
		}
	};
	check.operator()<Deep::RaycastType3D::e_startsOutside>();
	check.operator()<Deep::RaycastType3D::e_startsInside>();
}

TEST(Sphere, ContactAndPackedCenter) {
	Deep::Sphere a{ .m_center = { 2, 3, 4 }, .m_radius = 2 };
	Deep::Sphere b{ .m_center = { 5, 3, 4 }, .m_radius = 2 };
	Deep::ContactInfo3D contact;
	EXPECT_EQ(Deep::IsOverlapping(a, b, &contact), 1);
	EXPECT_EQ(contact.m_Normal(), Deep::Vec3::k_right);
	EXPECT_EQ(contact.m_penetrationDistance, 1.0f);
	EXPECT_EQ(Deep::IsOverlapping(b, a, &contact), 1);
	EXPECT_EQ(contact.m_Normal(), Deep::Vec3::k_left);
	EXPECT_EQ(contact.m_penetrationDistance, 1.0f);
	auto sentinel = contact;
	b.m_Center({ 6, 3, 4 });
	EXPECT_EQ(b.m_radius, 2.0f);
	EXPECT_EQ(Deep::IsOverlapping(a, b, &contact), 0);
	EXPECT_EQ(contact.m_Normal(), sentinel.m_Normal());
	EXPECT_EQ(contact.m_penetrationDistance, sentinel.m_penetrationDistance);
	EXPECT_EQ(Deep::IsOverlapping(a, a, &contact), 1);
	EXPECT_EQ(contact.m_Normal(), Deep::Vec3::k_up);
	EXPECT_EQ(contact.m_penetrationDistance, 4.0f);
}

TEST(Sphere, ObliqueRaycast) {
	Deep::Sphere sphere{ .m_center = { 2, -3, 4 }, .m_radius = 2 };
	Deep::Vec3 direction{ 0.6f, 0.8f, 0.0f };
	Deep::Ray3D ray{ .m_origin = sphere.m_Center() - direction * 5.0f, .m_direction = direction };
	Deep::RayHit3D hits[2];
	ASSERT_TRUE(Deep::Raycast(ray, sphere));
	ASSERT_TRUE(Deep::Raycast(ray, sphere, hits));
	EXPECT_TRUE(Deep::Abs(hits[0].m_distance - 3.0f) < 0.00001f);
	EXPECT_EQ(Deep::RaycastAll(ray, sphere, hits), 2);
	EXPECT_TRUE(Deep::Abs(hits[0].m_distance - 3.0f) < 0.00001f);
	EXPECT_TRUE(Deep::Abs(hits[1].m_distance - 7.0f) < 0.00001f);
	for (size_t i = 0; i < 3; ++i) {
		EXPECT_TRUE(Deep::Abs(hits[0].m_Normal()[i] + direction[i]) < 0.00001f);
		EXPECT_TRUE(Deep::Abs(hits[1].m_Normal()[i] - direction[i]) < 0.00001f);
	}
}
