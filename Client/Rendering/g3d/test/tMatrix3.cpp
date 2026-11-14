#include "../include/G3DAll.h"

static void mul(float (&A)[3][3], float (&B)[3][3], float (&C)[3][3]) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            float sum = 0;
            for (int i = 0; i < 3; ++i) {
                sum += A[r][i] * B[i][c];
            }
            C[r][c] = sum;
        }
    }
}


static void testEuler() {
    float x = 1;
    float y = 2;
    float z = -3;

    float x2, y2, z2;

    Matrix3 rX = Matrix3::fromAxisAngle(Vector3::UNIT_X, x);
    Matrix3 rY = Matrix3::fromAxisAngle(Vector3::UNIT_Y, y);
    Matrix3 rZ = Matrix3::fromAxisAngle(Vector3::UNIT_Z, z);
    Matrix3 rot = rZ * rX * rY;
    rot.toEulerAnglesZXY(x2, y2, z2);
    debugAssert(fuzzyEq(x, x2));
    debugAssert(fuzzyEq(y, y2));
    debugAssert(fuzzyEq(z, z2));
}


void testMatrix3() {
    printf("G3D::Matrix3  ");

    testEuler();

    {
        Matrix3 M = Matrix3::identity();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                M[i][j] = random(0, 1);
            }
        }

        Vector3 v = Vector3::random();

        Vector3 x1 = v * M;
        Vector3 x2 = M.transpose() * v;

        debugAssert(x1 == x2);

    }


    printf("passed\n");
}


void perfMatrix3() {
    printf("Matrix3:\n");
    uint64 raw, opt, overhead, naive;

    // 0.5 million operations
    int n = 0.5 * 1024 * 1024;

    // Use two copies to avoid nice cache behavior
    Matrix3 A = Matrix3::fromAxisAngle(Vector3(1, 2, 1), 1.2f);
    Matrix3 B = Matrix3::fromAxisAngle(Vector3(0, 1, -1), .2f);
    Matrix3 C = Matrix3::zero();

    Matrix3 D = Matrix3::fromAxisAngle(Vector3(1, 2, 1), 1.2f);
    Matrix3 E = Matrix3::fromAxisAngle(Vector3(0, 1, -1), .2f);
    Matrix3 F = Matrix3::zero();

    int i;
    System::beginCycleCount(overhead);
    for (i = n - 1; i >= 0; --i) {
    }
    System::endCycleCount(overhead);

    System::beginCycleCount(raw);
    for (i = n - 1; i >= 0; --i) {
        C = A.transpose();
        F = D.transpose();
        C = B.transpose();
    }
    System::endCycleCount(raw);

    System::beginCycleCount(opt);
    for (i = n - 1; i >= 0; --i) {
        Matrix3::transpose(A, C);
        Matrix3::transpose(D, F);
        Matrix3::transpose(B, C);
    }
    System::endCycleCount(opt);

    raw -= overhead;
    opt -= overhead;

    printf(" Transpose Performance                       outcome\n");
    printf("     transpose(A, C): %g cycles/mul       %s\n\n", 
        (double)opt / (3*n), (opt/(3*n) < 400) ? " ok " : "FAIL");
    printf("   C = A.transpose(): %g cycles/mul       %s\n", 
        (double)raw / (3*n), (raw/(3*n) < 150) ? " ok " : "FAIL");
    printf("\n");
    /////////////////////////////////


    printf(" Matrix-Matrix Multiplication\n");
    System::beginCycleCount(raw);
    for (i = n - 1; i >= 0; --i) {
        C = A * B;
        F = D * E;
        C = A * D;
    }
    System::endCycleCount(raw);

    System::beginCycleCount(opt);
    for (i = n - 1; i >= 0; --i) {
        Matrix3::mul(A, B, C);
        Matrix3::mul(D, E, F);
        Matrix3::mul(A, D, C);
    }
    System::endCycleCount(opt);

    
    {
        float A[3][3], B[3][3], C[3][3], D[3][3], E[3][3], F[3][3];

        System::beginCycleCount(naive);
        for (i = n - 1; i >= 0; --i) {
            mul(A, B, C);
            mul(D, E, F);
            mul(A, D, C);
        }
        System::endCycleCount(naive);
    }

    raw -= overhead;
    opt -= overhead;
    
    printf("  mul(A, B, C)          %g cycles/mul     %s\n", (double)opt / (3*n), (opt/(3*n) < 250) ? " ok " : "FAIL");
    printf("     C = A * B          %g cycles/mul     %s\n", (double)raw / (3*n), (raw/(3*n) < 500) ? " ok " : "FAIL");
    printf("  naive for-loops       %g cycles/mul\n", (double)naive / (3*n));

    printf("\n\n");
}
