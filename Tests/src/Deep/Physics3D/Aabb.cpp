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