#include "tachyon/math/matrix/matrix.h"

#include <limits>

#include "gtest/gtest.h"

#include "tachyon/math/finite_fields/test/gf7.h"

namespace tachyon::math {

namespace {

using Matrix33GF7 = Matrix<GF7, 3, 3>;

class MatrixTest : public testing::Test {
 public:
  static void SetUpTestSuite() { GF7Config::Init(); }
};

}  // namespace

TEST_F(MatrixTest, Construct) {
  Matrix33GF7 matrix;
  EXPECT_TRUE(matrix.IsZero());

#define MAT_CONSTRUCT_TEST(Rows, Cols, name, ...) \
  Matrix<GF7, Rows, Cols> name(__VA_ARGS__);      \
  for (int i = 0; i < Rows * Cols; ++i) {         \
    EXPECT_EQ(name[i], GF7(i % 7));               \
  }

  // clang-format off
  MAT_CONSTRUCT_TEST(2, 1, matrix21, GF7(0), GF7(1))
  MAT_CONSTRUCT_TEST(3, 1, matrix31, GF7(0), GF7(1), GF7(2))
  MAT_CONSTRUCT_TEST(2, 2, matrix22, GF7(0), GF7(1), GF7(2), GF7(3))
  MAT_CONSTRUCT_TEST(5, 1, matrix51, GF7(0), GF7(1), GF7(2), GF7(3), GF7(4))
  MAT_CONSTRUCT_TEST(2, 3, matrix23, GF7(0), GF7(1), GF7(2), GF7(3), GF7(4), GF7(5))
  MAT_CONSTRUCT_TEST(7, 1, matrix71, GF7(0), GF7(1), GF7(2), GF7(3), GF7(4), GF7(5), GF7(6))
  MAT_CONSTRUCT_TEST(4, 2, matrix42, GF7(0), GF7(1), GF7(2), GF7(3), GF7(4), GF7(5), GF7(6), GF7(0))
  MAT_CONSTRUCT_TEST(3, 3, matrix33, GF7(0), GF7(1), GF7(2), GF7(3), GF7(4), GF7(5), GF7(6), GF7(0), GF7(1))
  MAT_CONSTRUCT_TEST(5, 2, matrix52, GF7(0), GF7(1), GF7(2), GF7(3), GF7(4), GF7(5), GF7(6), GF7(0), GF7(1), GF7(2))
  MAT_CONSTRUCT_TEST(4, 3, matrix43, GF7(0), GF7(1), GF7(2), GF7(3), GF7(4), GF7(5), GF7(6), GF7(0), GF7(1), GF7(2), GF7(3), GF7(4))
  MAT_CONSTRUCT_TEST(4, 4, matrix44, GF7(0), GF7(1), GF7(2), GF7(3), GF7(4), GF7(5), GF7(6), GF7(0), GF7(1), GF7(2), GF7(3), GF7(4), GF7(5), GF7(6), GF7(0), GF7(1))
  // clang-format on

#undef MAT_CONSTRUCT_TEST
}

TEST_F(MatrixTest, Zero) {
  EXPECT_TRUE(Matrix33GF7::Zero().IsZero());
  EXPECT_FALSE(Matrix33GF7::Identity().IsZero());
}

TEST_F(MatrixTest, Identity) {
  EXPECT_TRUE(Matrix33GF7::Identity().IsIdentity());
  EXPECT_FALSE(Matrix33GF7::Zero().IsIdentity());
}

TEST_F(MatrixTest, Properties) {
  // clang-format off
  Matrix<GF7, 3, 2> matrix(
    GF7(1), GF7(2),
    GF7(3), GF7(4),
    GF7(5), GF7(6)
  );
  // clang-format on
  EXPECT_EQ(matrix.rows(), 3);
  EXPECT_EQ(matrix.cols(), 2);
  EXPECT_EQ(matrix.stride(), 2);
}

TEST_F(MatrixTest, Trace) {
  // clang-format off
  Matrix33GF7 matrix(
    GF7(0), GF7(1), GF7(2),
    GF7(3), GF7(4), GF7(5),
    GF7(6), GF7(0), GF7(1)
  );
  // clang-format on
  EXPECT_EQ(matrix.Trace(), GF7(5));
}

TEST_F(MatrixTest, Transpose) {
  // clang-format off
  Matrix33GF7 matrix(
    GF7(0), GF7(1), GF7(2),
    GF7(3), GF7(4), GF7(5),
    GF7(6), GF7(0), GF7(1)
  );
  Matrix33GF7 expected(
    GF7(0), GF7(3), GF7(6),
    GF7(1), GF7(4), GF7(0),
    GF7(2), GF7(5), GF7(1)
  );
  // clang-format on
  EXPECT_EQ(matrix.Transpose(), expected);
}

TEST_F(MatrixTest, Block) {
#define EXPECT_MATRIX_VIEW_EQ(actual, expected, expected_data) \
  do {                                                         \
    EXPECT_EQ(actual, expected);                               \
    for (size_t row = 0; row < expected.rows(); ++row) {       \
      for (size_t col = 0; col < expected.cols(); ++col) {     \
        EXPECT_EQ(expected.at(row, col),                       \
                  expected_data[row * expected.cols() + col]); \
      }                                                        \
    }                                                          \
  } while (false)

  // clang-format off
  Matrix33GF7 matrix(
    GF7(0), GF7(1), GF7(2),
    GF7(3), GF7(4), GF7(5),
    GF7(6), GF7(0), GF7(1)
  );
  {
    GF7 expected_data[9] = {
      GF7(0), GF7(1), GF7(2),
      GF7(3), GF7(4), GF7(5),
      GF7(6), GF7(0), GF7(1),
    };
    MatrixView<GF7> expected(expected_data, 3, 3, 3);
    EXPECT_MATRIX_VIEW_EQ(matrix.Block(0, 0), expected, expected_data);
  }
  {
    GF7 expected_data[6] = {
      GF7(0), GF7(1), GF7(2),
      GF7(3), GF7(4), GF7(5),
    };
    MatrixView<GF7> expected(expected_data, 2, 3, 3);
    EXPECT_MATRIX_VIEW_EQ(matrix.Block(0, 0, 2, 3), expected, expected_data);
  }
  {
    GF7 expected_data[6] = {
      GF7(0), GF7(1),
      GF7(3), GF7(4),
      GF7(6), GF7(0),
    };
    MatrixView<GF7> expected(expected_data, 3, 2, 2);
    EXPECT_MATRIX_VIEW_EQ(matrix.Block(0, 0, 3, 2), expected, expected_data);
  }
  {
    GF7 expected_data[4] = {
      GF7(0), GF7(1),
      GF7(3), GF7(4),
    };
    MatrixView<GF7> expected(expected_data, 2, 2, 2);
    EXPECT_MATRIX_VIEW_EQ(matrix.Block(0, 0, 2, 2), expected, expected_data);
  }
  {
    GF7 expected_data[4] = {
      GF7(4), GF7(5),
      GF7(0), GF7(1),
    };
    MatrixView<GF7> expected(expected_data, 2, 2, 2);
    EXPECT_MATRIX_VIEW_EQ(matrix.Block(1, 1), expected, expected_data);
  }
  // clang-format on

#undef EXPECT_MATRIX_VIEW_EQ
}

TEST_F(MatrixTest, Determinant) {
  // clang-format off
  {
    Matrix<GF7, 2, 2> matrix(
      GF7(1), GF7(2),
      GF7(3), GF7(4)
    );
    EXPECT_EQ(matrix.Determinant(), GF7(5));
  }
  {
    Matrix33GF7 matrix(
      GF7(1), GF7(6), GF7(6),
      GF7(2), GF7(4), GF7(0),
      GF7(0), GF7(6), GF7(5)
    );
    EXPECT_EQ(matrix.Determinant(), GF7(4));
  }
  {
    Matrix<GF7, 4, 4> matrix(
      GF7(2), GF7(4), GF7(1), GF7(4),
      GF7(0), GF7(2), GF7(2), GF7(5),
      GF7(3), GF7(3), GF7(2), GF7(2),
      GF7(0), GF7(5), GF7(1), GF7(0)
    );
    EXPECT_EQ(matrix.Determinant(), GF7(0));
  }
  {
    GF7 data[25] = {
      GF7(1), GF7(3), GF7(0), GF7(2), GF7(2),
      GF7(2), GF7(5), GF7(1), GF7(1) ,GF7(0),
      GF7(3), GF7(5), GF7(5), GF7(6) ,GF7(3),
      GF7(2), GF7(2), GF7(1), GF7(5), GF7(6),
      GF7(4), GF7(0), GF7(6), GF7(1), GF7(1),
    };
    Matrix<GF7, 5, 5> matrix(data);
    EXPECT_EQ(matrix.Determinant(), GF7(3));
  }
  // clang-format on
}

TEST_F(MatrixTest, Inverse) {
  // clang-format off
   {
      Matrix<GF7, 2, 2> matrix(
        GF7(1), GF7(2),
        GF7(3), GF7(4)
      );
      Matrix<GF7, 2, 2> expected(
        GF7(5), GF7(1),
        GF7(5), GF7(3)
      );
     EXPECT_EQ(matrix.Inverse(), expected);
     EXPECT_TRUE((matrix * matrix.Inverse()).IsIdentity());
   }
   {
      Matrix33GF7 matrix(
        GF7(1), GF7(6), GF7(6),
        GF7(2), GF7(4), GF7(0),
        GF7(0), GF7(6), GF7(5)
      );
      Matrix33GF7 expected(
        GF7(5), GF7(5), GF7(1),
        GF7(1), GF7(3), GF7(3),
        GF7(3), GF7(2), GF7(5)
      );
      EXPECT_EQ(matrix.Inverse(), expected);
      EXPECT_TRUE((matrix * matrix.Inverse()).IsIdentity());
   }
   {
      Matrix<GF7, 4, 4> matrix(
        GF7(2), GF7(4), GF7(1), GF7(4),
        GF7(3), GF7(2), GF7(2), GF7(5),
        GF7(3), GF7(3), GF7(2), GF7(2),
        GF7(0), GF7(5), GF7(1), GF7(0)
      );
      Matrix<GF7, 4, 4> expected(
        GF7(2), GF7(5), GF7(1), GF7(0),
        GF7(5), GF7(1), GF7(5), GF7(4),
        GF7(3), GF7(2), GF7(3), GF7(2),
        GF7(4), GF7(3), GF7(6), GF7(6)
      );
      EXPECT_EQ(matrix.Inverse(), expected);
      EXPECT_TRUE((matrix * matrix.Inverse()).IsIdentity());
   }
   {
      GF7 data[25] = {
        GF7(1), GF7(3), GF7(0), GF7(2), GF7(2),
        GF7(2), GF7(5), GF7(1), GF7(1) ,GF7(0),
        GF7(3), GF7(5), GF7(5), GF7(6) ,GF7(3),
        GF7(2), GF7(2), GF7(1), GF7(5), GF7(6),
        GF7(4), GF7(0), GF7(6), GF7(1), GF7(1),
      };
      Matrix<GF7, 5, 5> matrix(data);
      GF7 inv_data[25] = {
        GF7(6), GF7(3), GF7(6), GF7(0), GF7(5),
        GF7(2), GF7(6), GF7(4), GF7(4) ,GF7(2),
        GF7(1), GF7(5), GF7(1), GF7(1) ,GF7(3),
        GF7(5), GF7(2), GF7(2), GF7(0), GF7(5),
        GF7(0), GF7(5), GF7(3), GF7(1), GF7(0),
      };
      Matrix<GF7, 5, 5> expected(inv_data);
      EXPECT_EQ(matrix.Inverse(), expected);
      EXPECT_TRUE((matrix * matrix.Inverse()).IsIdentity());
   }
  // clang-format on
}

TEST_F(MatrixTest, Addition) {
  // clang-format off
  Matrix33GF7 matrix(
    GF7(1), GF7(2), GF7(3),
    GF7(4), GF7(5), GF7(6),
    GF7(0), GF7(1), GF7(2)
  );
  Matrix33GF7 expected(
    GF7(2), GF7(4), GF7(6),
    GF7(1), GF7(3), GF7(5),
    GF7(0), GF7(2), GF7(4)
  );
  // clang-format on
  EXPECT_EQ(matrix + matrix, expected);
}

TEST_F(MatrixTest, MultiplicationWithScalar) {
  // clang-format off
  Matrix33GF7 matrix(
    GF7(1), GF7(2), GF7(3),
    GF7(4), GF7(5), GF7(6),
    GF7(0), GF7(1), GF7(2)
  );
  Matrix33GF7 expected(
    GF7(2), GF7(4), GF7(6),
    GF7(1), GF7(3), GF7(5),
    GF7(0), GF7(2), GF7(4)
  );
  // clang-format on
  EXPECT_EQ(matrix * GF7(2), expected);
  EXPECT_EQ(GF7(2) * matrix, expected);

  EXPECT_EQ(expected / GF7(2), matrix);
}

TEST_F(MatrixTest, MultiplicationWithMatrix) {
  // clang-format off
  Matrix<GF7, 3, 2> matrix32(
    GF7(1), GF7(2),
    GF7(3), GF7(4),
    GF7(5), GF7(6)
  );
  Matrix<GF7, 2, 4> matrix24(
    GF7(1), GF7(2), GF7(3), GF7(4),
    GF7(5), GF7(6), GF7(0), GF7(1)
  );
  Matrix<GF7, 3, 4> expected(
    GF7(4), GF7(0), GF7(3), GF7(6),
    GF7(2), GF7(2), GF7(2), GF7(2),
    GF7(0), GF7(4), GF7(1), GF7(5)
  );
  // clang-format on
  EXPECT_EQ(matrix32 * matrix24, expected);

  // clang-format off
  Matrix33GF7 matrix33(
    GF7(1), GF7(2), GF7(3),
    GF7(4), GF7(5), GF7(6),
    GF7(0), GF7(1), GF7(2)
  );
  Matrix33GF7 expected2(
    GF7(2), GF7(1), GF7(0),
    GF7(3), GF7(4), GF7(5),
    GF7(4), GF7(0), GF7(3)
  );
  // clang-format on
  EXPECT_EQ(matrix33 * matrix33, expected2);
}

TEST_F(MatrixTest, ToString) {
  // clang-format off
  Matrix<GF7, 3, 4> matrix(
    GF7(1), GF7(2), GF7(3), GF7(4),
    GF7(5), GF7(6), GF7(0), GF7(1),
    GF7(2), GF7(3), GF7(4), GF7(5)
  );
  std::string expected =
    "[[1, 2, 3, 4],\n"
    " [5, 6, 0, 1],\n"
    " [2, 3, 4, 5]]";
  // clang-format on
  EXPECT_EQ(matrix.ToString(), expected);
}

}  // namespace tachyon::math