#include "Tests.h"

#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Ray.h"
#include "Deep/Physics3D/Aabb.h"

TEST(Aabb3D, IsOverlapping) {
	{
		Deep::Vec3 point{ 0.25f, 0.25f, 0.25f };
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };

		EXPECT_TRUE(Deep::IsOverlapping(point, a));
	}

	{
		Deep::Vec3 point{ 1.0f, 1.0f, 1.0f };
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };

		EXPECT_FALSE(Deep::IsOverlapping(point, a));
	}

	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Aabb3D b{ .m_center = { 0.25f, 0.25f, 0.25f }, .m_extents = { 0.5f, 0.5f, 0.5f } };

		EXPECT_TRUE(Deep::IsOverlapping(a, b));
	}

	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Aabb3D b{ .m_center = { 1.0f, 1.0f, 1.0f }, .m_extents = { 0.5f, 0.5f, 0.5f } };

		EXPECT_FALSE(Deep::IsOverlapping(a, b));
	}
}

TEST(Aabb3D, Raycast) {
	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { 1, 0, 0 } };
		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
	}

	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { -1, 0, 0 } };
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
	}
}

TEST(Aabb3D, RaycastHit) {
	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { 1, 0, 0 } };
		Deep::RayHit3D hits[2];
		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a, hits));
		EXPECT_EQ(hits->m_normal, Deep::Vec3::k_left);
		EXPECT_EQ(hits->m_point.x, -0.5f);
		EXPECT_EQ(hits->m_point.y, 0.0f);
		EXPECT_EQ(hits->m_point.z, 0.0f);
		EXPECT_EQ(hits->m_distance, 0.5f);

		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a, hits));
		EXPECT_EQ(hits->m_normal, Deep::Vec3::k_left);
		EXPECT_EQ(hits->m_point.x, -0.5f);
		EXPECT_EQ(hits->m_point.y, 0.0f);
		EXPECT_EQ(hits->m_point.z, 0.0f);
		EXPECT_EQ(hits->m_distance, 0.5f);

		EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsOutside>(ray, a, hits), 2);
		EXPECT_EQ(hits[0].m_normal, Deep::Vec3::k_left);
		EXPECT_EQ(hits[0].m_point.x, -0.5f);
		EXPECT_EQ(hits[0].m_point.y, 0.0f);
		EXPECT_EQ(hits[0].m_point.z, 0.0f);
		EXPECT_EQ(hits[0].m_distance, 0.5f);
		EXPECT_EQ(hits[1].m_normal, Deep::Vec3::k_right);
		EXPECT_EQ(hits[1].m_point.x, 0.5f);
		EXPECT_EQ(hits[1].m_point.y, 0.0f);
		EXPECT_EQ(hits[1].m_point.z, 0.0f);
		EXPECT_EQ(hits[1].m_distance, 1.5f);

		EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsInside>(ray, a, hits), 2);
		EXPECT_EQ(hits[0].m_normal, Deep::Vec3::k_left);
		EXPECT_EQ(hits[0].m_point.x, -0.5f);
		EXPECT_EQ(hits[0].m_point.y, 0.0f);
		EXPECT_EQ(hits[0].m_point.z, 0.0f);
		EXPECT_EQ(hits[0].m_distance, 0.5f);
		EXPECT_EQ(hits[1].m_normal, Deep::Vec3::k_right);
		EXPECT_EQ(hits[1].m_point.x, 0.5f);
		EXPECT_EQ(hits[1].m_point.y, 0.0f);
		EXPECT_EQ(hits[1].m_point.z, 0.0f);
		EXPECT_EQ(hits[1].m_distance, 1.5f);
	}

	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { -1, 0, 0 } };
		Deep::RayHit3D hits[2];
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a, hits));
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a, hits));

		EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsOutside>(ray, a, hits), 0);
		EXPECT_EQ(Deep::RaycastAll<Deep::RaycastType::e_startsInside>(ray, a, hits), 0);
	}
}

TEST(Aabb3D, RaycastStartInside) {
	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { 0, 0, 0 }, .m_direction = { 1, 0, 0 } };
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
	}
}

TEST(Aabb3D, RaycastHitStartInside) {
	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { 0, 0, 0 }, .m_direction = { 1, 0, 0 } };
		Deep::RayHit3D hit;
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a, &hit));

		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a, &hit));
		EXPECT_EQ(hit.m_normal, Deep::Vec3::k_right);
		EXPECT_EQ(hit.m_point.x, 0.5f);
		EXPECT_EQ(hit.m_point.y, 0.0f);
		EXPECT_EQ(hit.m_point.z, 0.0f);
		EXPECT_EQ(hit.m_distance, 0.5f);
	}
}
