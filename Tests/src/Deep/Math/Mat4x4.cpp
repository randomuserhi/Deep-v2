#include "Tests.h"

#include "Deep/Math/Mat4x4.h"

TEST(Mat4, InverseAndTransposeRoundTrip) {
	for (int step = 1; step <= 16; ++step) {
		float offset = static_cast<float>(step) * 0.125f;
		Deep::Mat4x4 a{
			4,      offset, -1, 2,      //
			1,      5,      2,  -3,     //
			-2,     1,      6,  offset, //
			offset, 0,      1,  7       //
		};
		auto transpose = a.transposed();
		EXPECT_EQ(transpose.transposed(), a);
		for (size_t col = 0; col < 4; ++col) {
			for (size_t row = 0; row < 4; ++row)
				EXPECT_EQ(transpose.m_cols[col][row], a.m_cols[row][col]);
		}
		auto inverse = a.inversed();
		auto left = inverse * a;
		auto right = a * inverse;
		for (size_t i = 0; i < 16; ++i) {
			EXPECT_TRUE(Deep::Abs(left[i] - Deep::Mat4x4::k_identity[i]) < 0.00001f);
			EXPECT_TRUE(Deep::Abs(right[i] - Deep::Mat4x4::k_identity[i]) < 0.00001f);
		}
		EXPECT_TRUE(Deep::Abs(a.determinant() * inverse.determinant() - 1.0f) < 0.00001f);
		Deep::Vec4 vector{ 2, -3, offset, 1 };
		auto transformed = a * vector;
		for (size_t row = 0; row < 4; ++row) {
			float expected = 0;
			for (size_t col = 0; col < 4; ++col)
				expected += a.m_cols[col][row] * vector[col];
			EXPECT_TRUE(Deep::Abs(transformed[row] - expected) < 0.00001f);
		}
		auto projected = a * Deep::Vec3{ vector.x, vector.y, vector.z };
		for (size_t i = 0; i < 3; ++i)
			EXPECT_TRUE(Deep::Abs(projected[i] - transformed[i] / transformed.w) < 0.00001f);
	}
}

TEST(Mat4, Equality) {
	const Deep::Mat4x4 a{
		1, 0, 0, 0, //
		0, 1, 0, 0, //
		0, 0, 1, 0, //
		0, 0, 0, 1  //
	};
	const Deep::Mat4x4 b{
		1, 0, 0, 0, //
		0, 1, 0, 0, //
		0, 0, 1, 0, //
		0, 0, 0, 1  //
	};

	EXPECT_EQ(a, b);

	const Deep::Mat4x4 c{
		1, 0, 0, 0, //
		0, 1, 0, 0, //
		0, 0, 1, 0, //
		0, 0, 0, 1  //
	};
	const Deep::Mat4x4 d{
		0, 0, 0, 0, //
		0, 1, 0, 0, //
		0, 0, 1, 0, //
		0, 0, 0, 1  //
	};

	EXPECT_NE(c, d);
}

TEST(Mat4, Multiplication) {
	const Deep::Mat4x4 a{
		1, 1, 1, 0, //
		0, 0, 0, 4, //
		0, 3, 2, 0, //
		0, 0, 0, 1  //
	};
	const Deep::Mat4x4 b{
		2, 3, 0, 1, //
		0, 0, 0, 0, //
		1, 0, 2, 0, //
		0, 0, 4, 0  //
	};
	const Deep::Mat4x4 c{
		3, 3, 2,  1, //
		0, 0, 16, 0, //
		2, 0, 4,  0, //
		0, 0, 4,  0  //
	};

	EXPECT_EQ((a * b), c);
}

TEST(Mat4, Determinant) {
	const Deep::Mat4x4 a{
		1, 1, 0, 0, //
		0, 0, 2, 0, //
		0, 3, 0, 0, //
		0, 0, 0, 1  //
	};

	EXPECT_EQ(a.determinant(), -6);
}
