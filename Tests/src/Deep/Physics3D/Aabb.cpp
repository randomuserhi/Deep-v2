#include "Tests.h"

#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Ray.h"
#include "Deep/Physics3D/Colliders.h"

TEST(Aabb3D, IsOverlapping) {
	TEST_CASE(Point) {
		TEST_CASE(ShouldOverlap) {
			Deep::Vec3 point{ 0.25f, 0.25f, 0.25f };
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };

			EXPECT_TRUE(Deep::IsOverlapping(point, a));
		}

		TEST_CASE(NoOverlap) {
			Deep::Vec3 point{ 1.0f, 1.0f, 1.0f };
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };

			EXPECT_FALSE(Deep::IsOverlapping(point, a));
		}
	}

	TEST_CASE(Aabb3D) {
		TEST_CASE(ShouldOverlap) {
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
			Deep::Aabb3D b{ .m_center = { 0.25f, 0.25f, 0.25f }, .m_extents = { 0.5f, 0.5f, 0.5f } };

			EXPECT_TRUE(Deep::IsOverlapping(a, b));
		}

		TEST_CASE(NoOverlap) {
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
			Deep::Aabb3D b{ .m_center = { 1.0f, 1.0f, 1.0f }, .m_extents = { 0.5f, 0.5f, 0.5f } };

			EXPECT_FALSE(Deep::IsOverlapping(a, b));
		}
	}
}

TEST(Aabb3D, Raycast) {
	TEST_CASE(Outside) {
		TEST_CASE(ShouldHit) {
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
			Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { 1, 0, 0 } };
			EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
			EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
		}

		TEST_CASE(NoHit) {
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
			Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { -1, 0, 0 } };
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
		}
	}

	TEST_CASE(Inside) {
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { 0, 0, 0 }, .m_direction = { 1, 0, 0 } };
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
	}
}

TEST(Aabb3D, RaycastHit) {
	TEST_CASE(Outside) {
		TEST_CASE(ShouldHit) {
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
			Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { 1, 0, 0 } };
			Deep::RayHit3D hits[2];
			EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a, hits));
			EXPECT_EQ(hits->m_normal, Deep::Vec3::k_left);
			EXPECT_EQ(hits->m_point, (Deep::Vec3{ -0.5f, 0.0f, 0.0f }));

			EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a, hits));
			EXPECT_EQ(hits->m_normal, Deep::Vec3::k_left);
			EXPECT_EQ(hits->m_point, (Deep::Vec3{ -0.5f, 0.0f, 0.0f }));
			EXPECT_EQ(hits->m_distance, 0.5f);

			EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsOutside>(ray, a, hits), 2);
			EXPECT_EQ(hits[0].m_normal, Deep::Vec3::k_left);
			EXPECT_EQ(hits[0].m_point, (Deep::Vec3{ -0.5f, 0.0f, 0.0f }));
			EXPECT_EQ(hits[0].m_distance, 0.5f);
			EXPECT_EQ(hits[1].m_normal, Deep::Vec3::k_right);
			EXPECT_EQ(hits[1].m_point.x, 0.5f);
			EXPECT_EQ(hits[1].m_point.y, 0.0f);
			EXPECT_EQ(hits[1].m_point.z, 0.0f);
			EXPECT_EQ(hits[1].m_distance, 1.5f);

			EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsInside>(ray, a, hits), 2);
			EXPECT_EQ(hits[0].m_normal, Deep::Vec3::k_left);
			EXPECT_EQ(hits[0].m_point, (Deep::Vec3{ -0.5f, 0.0f, 0.0f }));
			EXPECT_EQ(hits[0].m_distance, 0.5f);
			EXPECT_EQ(hits[1].m_normal, Deep::Vec3::k_right);
			EXPECT_EQ(hits[1].m_point, (Deep::Vec3{ 0.5f, 0.0f, 0.0f }));
			EXPECT_EQ(hits[1].m_distance, 1.5f);
		}

		TEST_CASE(NoHit) {
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
			Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { -1, 0, 0 } };
			Deep::RayHit3D hits[2];
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a, hits));
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a, hits));

			EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsOutside>(ray, a, hits), 0);
			EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsInside>(ray, a, hits), 0);
		}
	}

	TEST_CASE(Inside) {
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { 0, 0, 0 }, .m_direction = { 1, 0, 0 } };
		Deep::RayHit3D hits[2];
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a, hits));

		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a, hits));
		EXPECT_EQ(hits->m_normal, Deep::Vec3::k_right);
		EXPECT_EQ(hits->m_point, (Deep::Vec3{ 0.5f, 0.0f, 0.0f }));
		EXPECT_EQ(hits->m_distance, 0.5f);

		EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsOutside>(ray, a, hits), 0);

		EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsInside>(ray, a, hits), 1);
		EXPECT_EQ(hits->m_normal, Deep::Vec3::k_right);
		EXPECT_EQ(hits->m_point, (Deep::Vec3{ 0.5f, 0.0f, 0.0f }));
		EXPECT_EQ(hits->m_distance, 0.5f);
	}
}

TEST(Aabb3D, Grazing) {
	TEST_CASE(IsOverlap) {
		TEST_CASE(Point) {
			Deep::Vec3 point{ 0.5f, 0.0f, 0.0f };
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };

			EXPECT_FALSE(Deep::IsOverlapping(point, a));
		}

		TEST_CASE(Aabb3D) {
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
			Deep::Aabb3D b{ .m_center = { 1.0f, 0.0f, 0.0f }, .m_extents = { 0.5f, 0.5f, 0.5f } };

			EXPECT_FALSE(Deep::IsOverlapping(a, b));
		}
	}

	TEST_CASE(Raycast) {
		TEST_CASE(ShouldHit) {
			TEST_CASE(OnEdge) {
				Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
				Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0 }, .m_direction = { -1, 0, 0 } };
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
			}

			TEST_CASE(WithEdge) {
				Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
				Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0.5f }, .m_direction = { -1, 0, 0 } };
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
			}
		}

		TEST_CASE(NoHit) {
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
			Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0 }, .m_direction = { 1, 0, 0 } };
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
		}
	}

	TEST_CASE(RaycastHit) {
		TEST_CASE(ShouldHit) {
			TEST_CASE(OnEdge) {
				Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
				Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0 }, .m_direction = { -1, 0, 0 } };
				Deep::RayHit3D hits[2];

				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a, hits));
				EXPECT_EQ(hits->m_normal, Deep::Vec3::k_right);
				EXPECT_EQ(hits->m_point, (Deep::Vec3{ 0.5f, 0.0f, 0.0f }));
				EXPECT_EQ(hits->m_distance, 0.0f);

				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a, hits));
				EXPECT_EQ(hits->m_normal, Deep::Vec3::k_right);
				EXPECT_EQ(hits->m_point, (Deep::Vec3{ 0.5f, 0.0f, 0.0f }));
				EXPECT_EQ(hits->m_distance, 0.0f);
			}

			TEST_CASE(WidthEdge) {
				Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
				Deep::Ray3D ray{ .m_origin = { 1.0f, 0, 0.5f }, .m_direction = { -1, 0, 0 } };
				Deep::RayHit3D hits[2];

				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a, hits));
				EXPECT_EQ(hits->m_normal, Deep::Vec3::k_right);
				EXPECT_EQ(hits->m_point, (Deep::Vec3{ 0.5f, 0.0f, 0.5f }));
				EXPECT_EQ(hits->m_distance, 0.5f);

				EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a, hits));
				EXPECT_EQ(hits->m_normal, Deep::Vec3::k_right);
				EXPECT_EQ(hits->m_point, (Deep::Vec3{ 0.5f, 0.0f, 0.5f }));
				EXPECT_EQ(hits->m_distance, 0.5f);
			}
		}

		TEST_CASE(NoHit) {
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
			Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0 }, .m_direction = { 1, 0, 0 } };
			Deep::RayHit3D hits[2];

			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a, hits));
			EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a, hits));
		}
	}

	TEST_CASE(RaycastHitAll) {
		TEST_CASE(ShouldHit) {
			TEST_CASE(OnEdge) {
				Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
				Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0 }, .m_direction = { -1, 0, 0 } };
				Deep::RayHit3D hits[2];

				EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsOutside>(ray, a, hits), 2);
				EXPECT_EQ(hits[0].m_normal, Deep::Vec3::k_right);
				EXPECT_EQ(hits[0].m_point, (Deep::Vec3{ 0.5f, 0.0f, 0.0f }));
				EXPECT_EQ(hits[0].m_distance, 0.0f);

				EXPECT_EQ(hits[1].m_normal, Deep::Vec3::k_left);
				EXPECT_EQ(hits[1].m_point, (Deep::Vec3{ -0.5f, 0.0f, 0.0f }));
				EXPECT_EQ(hits[1].m_distance, 1.0f);

				EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsInside>(ray, a, hits), 2);
				EXPECT_EQ(hits[0].m_normal, Deep::Vec3::k_right);
				EXPECT_EQ(hits[0].m_point, (Deep::Vec3{ 0.5f, 0.0f, 0.0f }));
				EXPECT_EQ(hits[0].m_distance, 0.0f);

				EXPECT_EQ(hits[1].m_normal, Deep::Vec3::k_left);
				EXPECT_EQ(hits[1].m_point, (Deep::Vec3{ -0.5f, 0.0f, 0.0f }));
				EXPECT_EQ(hits[1].m_distance, 1.0f);
			}

			TEST_CASE(WithEdge) {
				Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
				Deep::Ray3D ray{ .m_origin = { 1.0f, 0, 0.5f }, .m_direction = { -1, 0, 0 } };
				Deep::RayHit3D hits[2];

				EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsOutside>(ray, a, hits), 2);
				EXPECT_EQ(hits[0].m_normal, Deep::Vec3::k_right);
				EXPECT_EQ(hits[0].m_point, (Deep::Vec3{ 0.5f, 0.0f, 0.5f }));
				EXPECT_EQ(hits[0].m_distance, 0.5f);

				EXPECT_EQ(hits[1].m_normal, Deep::Vec3::k_left);
				EXPECT_EQ(hits[1].m_point, (Deep::Vec3{ -0.5f, 0.0f, 0.5f }));
				EXPECT_EQ(hits[1].m_distance, 1.5f);

				EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsInside>(ray, a, hits), 2);
				EXPECT_EQ(hits[0].m_normal, Deep::Vec3::k_right);
				EXPECT_EQ(hits[0].m_point, (Deep::Vec3{ 0.5f, 0.0f, 0.5f }));
				EXPECT_EQ(hits[0].m_distance, 0.5f);

				EXPECT_EQ(hits[1].m_normal, Deep::Vec3::k_left);
				EXPECT_EQ(hits[1].m_point, (Deep::Vec3{ -0.5f, 0.0f, 0.5f }));
				EXPECT_EQ(hits[1].m_distance, 1.5f);
			}
		}

		TEST_CASE(NoHit) {
			Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
			Deep::Ray3D ray{ .m_origin = { 0.5f, 0, 0 }, .m_direction = { 1, 0, 0 } };
			Deep::RayHit3D hits[2];

			EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsOutside>(ray, a, hits), 0);
			EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsInside>(ray, a, hits), 0);
		}
	}
}
