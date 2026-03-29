#include "Tests.h"

#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Ray.h"
#include "Deep/Physics3D/Sphere.h"

TEST(Sphere, IsOverlapping) {
	{
		Deep::Vec3 point{ 0.25f, 0.25f, 0.25f };
		Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };

		EXPECT_TRUE(Deep::IsOverlapping(point, a));
	}

	{
		Deep::Vec3 point{ 1.25f, 0.25f, 0.25f };
		Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };

		EXPECT_FALSE(Deep::IsOverlapping(point, a));
	}

	{
		Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
		Deep::Sphere b{ .m_center = { 0.25f, 0.25f, 0.25f }, .m_radius = 0.5f };

		EXPECT_TRUE(Deep::IsOverlapping(a, b));
	}

	{
		Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
		Deep::Sphere b{ .m_center = { 1.0f, 1.0f, 1.0f }, .m_radius = 0.5f };

		EXPECT_FALSE(Deep::IsOverlapping(a, b));
	}
}

TEST(Sphere, Raycast) {
	{
		Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
		Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { 1, 0, 0 } };
		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
	}

	{
		Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
		Deep::Ray3D ray{ .m_origin = { -1, 0, 0 }, .m_direction = { -1, 0, 0 } };
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
	}
}

TEST(Sphere, RaycastStartInside) {
	{
		Deep::Sphere a{ .m_center = { 0, 0, 0 }, .m_radius = 0.5f };
		Deep::Ray3D ray{ .m_origin = { 0, 0, 0 }, .m_direction = { 1, 0, 0 } };
		EXPECT_FALSE(Deep::Raycast<Deep::RaycastType::e_startsOutside>(ray, a));
		EXPECT_TRUE(Deep::Raycast<Deep::RaycastType::e_startsInside>(ray, a));
	}
}
