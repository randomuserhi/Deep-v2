#include "Deep/Math/Vec3.h"
#include "Tests.h"

#include "Deep/Physics3D/Aabb.h"

TEST(Aabb3D, IsOverlapping) {
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
		EXPECT_TRUE(Deep::Raycast(ray, a));
	}

	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { -1, 0, 0 } };
		EXPECT_FALSE(Deep::Raycast(ray, a));
	}
}

TEST(Aabb3D, RaycastHit) {
	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { 1, 0, 0 } };
		Deep::RayHit3D hit;
		EXPECT_TRUE(Deep::Raycast(ray, a, &hit));
		EXPECT_EQ(hit.m_normal, Deep::Vec3::k_left);
		EXPECT_EQ(hit.m_point.x, -0.5f);
		EXPECT_EQ(hit.m_point.y, 0.0f);
		EXPECT_EQ(hit.m_point.z, 0.0f);
		EXPECT_EQ(hit.m_distance, 0.5f);
	}

	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { -1, 0, 0 } };
		Deep::RayHit3D hit;
		EXPECT_FALSE(Deep::Raycast(ray, a, &hit));
	}
}

TEST(Aabb3D, RaycastStartInside) {
	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { 0, 0, 0 }, .m_direction = { 1, 0, 0 } };
		EXPECT_TRUE(Deep::Raycast(ray, a));
	}
}

TEST(Aabb3D, RaycastHitStartInside) {
	{
		Deep::Aabb3D a{ .m_center = { 0, 0, 0 }, .m_extents = { 0.5f, 0.5f, 0.5f } };
		Deep::Ray3D ray{ .m_origin = { 0, 0, 0 }, .m_direction = { 1, 0, 0 } };
		Deep::RayHit3D hit;
		EXPECT_TRUE(Deep::Raycast(ray, a, &hit));
		EXPECT_EQ(hit.m_normal, Deep::Vec3::k_left);
		EXPECT_EQ(hit.m_point.x, -0.5f);
		EXPECT_EQ(hit.m_point.y, 0.0f);
		EXPECT_EQ(hit.m_point.z, 0.0f);
		EXPECT_EQ(hit.m_distance, -0.5f); // NOTE(randomuserhi): Moves hit to left wall behind origin since thats the wall
		                                  // that would be hit by a right facing ray
	}
}