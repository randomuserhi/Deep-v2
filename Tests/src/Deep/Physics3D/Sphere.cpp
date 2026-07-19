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
			EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
			EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
		}

		TEST_CASE(NoHit) {
			Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
			Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { -1, 0, 0 } };
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
		}
	}

	TEST_CASE(Inside) {
		Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
		Deep::Ray3D ray{ .m_origin = { 0, 0, 0 }, .m_direction = { 1, 0, 0 } };
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
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
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
			}

			TEST_CASE(WithEdge) {
				Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
				Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0.5f }, .m_direction = { -1, 0, 0 } };
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
			}
		}

		TEST_CASE(NoHit) {
			Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
			Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0 }, .m_direction = { 1, 0, 0 } };
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
		}
	}
}
