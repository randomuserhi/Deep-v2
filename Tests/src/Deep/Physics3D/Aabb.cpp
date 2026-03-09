#include "Tests.h"

#include "Deep/Physics3D/Aabb.h"

TEST(Aabb3D, IsOverlapping) {
    {
        Deep::Aabb3D a{ .m_min = { -1, -1, -1, }, .m_max = { 1, 1, 1 } };
        Deep::Aabb3D b{ .m_min = { -1, -1, -1, }, .m_max = { 1, 1, 1 } };

        EXPECT_TRUE(Deep::IsOverlapping(a, b));
    }

    {
        Deep::Aabb3D a{ .m_min = { -1, -1, -1, }, .m_max = { 1, 1, 1 } };
        Deep::Aabb3D b{ .m_min = { 1, 1, 1, }, .m_max = { 2, 2, 2 } };

        EXPECT_FALSE(Deep::IsOverlapping(a, b));
    }
}