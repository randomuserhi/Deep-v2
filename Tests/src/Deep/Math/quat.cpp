#include "Tests.h"

#include "Deep/Math/Constants.h"
#include "Deep/Math/Quat.h"

TEST(Quat, CompositionAndMatrix) {
	for (int step = -8; step <= 8; ++step) {
		Deep::Quat a{ Deep::Vec3{ 1, 2, 3 }.m_Normalized(), static_cast<float>(step) * 0.25f };
		Deep::Quat b{ Deep::Vec3{ -2, 1, 3 }.m_Normalized(), static_cast<float>(step) * -0.125f };
		Deep::Vec3 vector{ 2, -3, 4 };
		auto composed = (a * b) * vector;
		auto sequential = a * (b * vector);
		auto matrix = a.ToMat4();
		auto matrixResult = matrix * vector;
		auto rotated = a * vector;
		auto restored = a.m_Inversed() * rotated;
		for (size_t i = 0; i < 3; ++i) {
			EXPECT_TRUE(Deep::Abs(composed[i] - sequential[i]) < 0.00001f);
			EXPECT_TRUE(Deep::Abs(matrixResult[i] - rotated[i]) < 0.00001f);
			EXPECT_TRUE(Deep::Abs(restored[i] - vector[i]) < 0.00001f);
		}
		EXPECT_EQ(matrix.m30, 0.0f);
		EXPECT_EQ(matrix.m31, 0.0f);
		EXPECT_EQ(matrix.m32, 0.0f);
		EXPECT_EQ(matrix.m33, 1.0f);
	}
}

TEST(Quat, Equality) {
	const Deep::Quat a{ 0.0f, 0.0f, 0.0f, 1.0f };
	const Deep::Quat b{ 0.0f, 0.0f, 0.0f, 1.0f };

	EXPECT_TRUE(a == b);
}

TEST(Quat, Conjugate) {
	const Deep::Quat a{ 0.0f, 1.0f, 0.0f, 0.0f };
	const Deep::Quat b = a.m_Conjugated();

	EXPECT_EQ(b.x, -0.0f);
	EXPECT_EQ(b.y, -1.0f);
	EXPECT_EQ(b.z, -0.0f);
	EXPECT_EQ(b.w, 0.0f);
}

TEST(Quat, FromAxisAngle) {
	const Deep::Quat a{ Deep::Vec3{ 0.0f, 1.0f, 0.0f }, Deep::k_pi };

	EXPECT_TRUE(Deep::IsClose(a.x, 0.0f));
	EXPECT_TRUE(Deep::IsClose(a.y, 1.0f));
	EXPECT_TRUE(Deep::IsClose(a.z, 0.0f));
	EXPECT_TRUE(Deep::IsClose(a.w, 0.0f));
}

TEST(Quat, RotateVector) {
	const Deep::Quat q{ Deep::Vec3{ 0.0f, 1.0f, 0.0f }, 0.5 * Deep::k_pi };
	const Deep::Vec3 v{ 1.0f, 0.0f, 0.0f };
	const Deep::Vec3 result = q * v;

	EXPECT_TRUE(Deep::IsClose(result.x, 0.0f));
	EXPECT_TRUE(Deep::IsClose(result.y, 0.0f));
	EXPECT_TRUE(Deep::IsClose(result.z, -1.0f));
}

TEST(Quat, InverseRotateVector) {
	Deep::Quat q{ Deep::Vec3{ 0.0f, 1.0f, 0.0f }, 0.5f * Deep::k_pi };
	const Deep::Vec3 v{ 1.0f, 0.0f, 0.0f };
	const Deep::Vec3 result = Deep::Quat::s_InverseRotate(q, v);

	EXPECT_TRUE(Deep::IsClose(result.x, 0.0f));
	EXPECT_TRUE(Deep::IsClose(result.y, 0.0f));
	EXPECT_TRUE(Deep::IsClose(result.z, 1.0f));
}
