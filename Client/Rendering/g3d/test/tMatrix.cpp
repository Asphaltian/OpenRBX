#include "../include/G3DAll.h"
#include "../contrib/Matrix/Matrix.cpp"


void testMatrix() {
    printf("Matrix ");
    // Zeros
    {
        Matrix M(3, 4);
        debugAssert(M.rows() == 3);
        debugAssert(M.cols() == 4);
        debugAssert(M.get(0, 0) == 0);
        debugAssert(M.get(1, 1) == 0);
    }

    // Identity
    {
        Matrix M = Matrix::identity(4);
        debugAssert(M.rows() == 4);
        debugAssert(M.cols() == 4);
        debugAssert(M.get(0, 0) == 1);
        debugAssert(M.get(0, 1) == 0);
    }

    // Add
    {
        Matrix A = Matrix::random(2, 3);
        Matrix B = Matrix::random(2, 3);
        Matrix C = A + B;
    
        for (int r = 0; r < 2; ++r) {
            for (int c = 0; c < 3; ++c) {
                debugAssert(fuzzyEq(C.get(r, c), A.get(r, c) + B.get(r, c)));
            }
        }
    }

    // Matrix multiply
    {
        Matrix A(2, 2);
        Matrix B(2, 2);

        A.set(0, 0, 1); A.set(0, 1, 3);
        A.set(1, 0, 4); A.set(1, 1, 2);

        B.set(0, 0, -6); B.set(0, 1, 9);
        B.set(1, 0, 1); B.set(1, 1, 7);

        Matrix C = A * B;

        debugAssert(fuzzyEq(C.get(0, 0), -3));
        debugAssert(fuzzyEq(C.get(0, 1), 30));
        debugAssert(fuzzyEq(C.get(1, 0), -22));
        debugAssert(fuzzyEq(C.get(1, 1), 50));
    }

    // Transpose
    {
        Matrix A(2, 2);

        A.set(0, 0, 1); A.set(0, 1, 3);
        A.set(1, 0, 4); A.set(1, 1, 2);

        Matrix C = A.transpose();

        debugAssert(fuzzyEq(C.get(0, 0), 1));
        debugAssert(fuzzyEq(C.get(0, 1), 4));
        debugAssert(fuzzyEq(C.get(1, 0), 3));
        debugAssert(fuzzyEq(C.get(1, 1), 2));

        A = Matrix::random(3, 4);
        A = A.transpose();

        debugAssert(A.rows() == 4);        
        debugAssert(A.cols() == 3);
    }

    // Copy-on-mutate
    {

        Matrix::debugNumCopyOps = Matrix::debugNumAllocOps = 0;

        Matrix A = Matrix::identity(2);

        debugAssert(Matrix::debugNumAllocOps == 1);
        debugAssert(Matrix::debugNumCopyOps == 0);

        Matrix B = A;
        debugAssert(Matrix::debugNumAllocOps == 1);
        debugAssert(Matrix::debugNumCopyOps == 0);

        B.set(0,0,4);
        debugAssert(B.get(0,0) == 4);
        debugAssert(A.get(0,0) == 1);
        debugAssert(Matrix::debugNumAllocOps == 2);
        debugAssert(Matrix::debugNumCopyOps == 1);
    }

    // Inverse
    {
        Matrix A(2, 2);

        A.set(0, 0, 1); A.set(0, 1, 3);
        A.set(1, 0, 4); A.set(1, 1, 2);

        Matrix C = A.inverse();

        debugAssert(fuzzyEq(C.get(0, 0), -0.2));
        debugAssert(fuzzyEq(C.get(0, 1), 0.3));
        debugAssert(fuzzyEq(C.get(1, 0), 0.4));
        debugAssert(fuzzyEq(C.get(1, 1), -0.1));
    }

    {
        Matrix A = Matrix::random(10, 10);
        Matrix B = A.inverse();

        B = B * A;

        for (int r = 0; r < B.rows(); ++r) {
            for (int c = 0; c < B.cols(); ++c) {

                float v = B.get(r, c);
                // The precision isn't great on our inverse, so be tolerant
                if (r == c) {
                    debugAssert(abs(v - 1) < 1e-4);
                } else {
                    debugAssert(abs(v) < 1e-4);
                }
                (void)v;
            }
        }
    }

    // Negate
    {
        Matrix A = Matrix::random(2, 2);
        Matrix B = -A;

        for (int r = 0; r < A.rows(); ++r) {
            for (int c = 0; c < A.cols(); ++c) {
                debugAssert(B.get(r, c) == -A.get(r, c));
            }
        }
    }

    // Transpose
    {
        Matrix A = Matrix::random(3,2);
        Matrix B = A.transpose();
        debugAssert(B.rows() == A.cols());
        debugAssert(B.cols() == A.rows());

        for (int r = 0; r < A.rows(); ++r) {
            for (int c = 0; c < A.cols(); ++c) {
                debugAssert(B.get(c, r) == A.get(r, c));
            }
        }
    }

    // SVD
    {
        Matrix A = Matrix(3, 3);
        A.set(0, 0,  1.0);  A.set(0, 1,  2.0);  A.set(0, 2,  1.0);
        A.set(1, 0, -3.0);  A.set(1, 1,  7.0);  A.set(1, 2, -6.0);
        A.set(2, 0,  4.0);  A.set(2, 1, -4.0);  A.set(2, 2, 10.0);
        A = Matrix::random(27, 15);

        Array<float> D;
        Matrix U, V;

        A.svd(U, D, V);

        // Verify that we can reconstruct
        Matrix B = U * Matrix::fromDiagonal(D) * V.transpose();

        Matrix test = abs(A - B) < 0.1f;

//        A.debugPrint("A");
//        U.debugPrint("U");
//        D.debugPrint("D");
//        V.debugPrint("V");
//        (U * D * V.transpose()).debugPrint("UDV'");

        debugAssert(test.allNonZero());

        float m = (A - B).norm() / A.norm();
        debugAssert(m < 0.01f);
        (void)m;
    }

    /*
    Matrix a(3, 5);
    a.set(0,0, 1);  a.set(0,1, 2); a.set(0,2,  3); a.set(0,3, 4);  a.set(0,4,  5);
    a.set(1,0, 3);  a.set(1,1, 5); a.set(1,2,  3); a.set(1,3, 1);  a.set(1,4,  2);
    a.set(2,0, 1);  a.set(2,1, 1); a.set(2,2,  1); a.set(2,3, 1);  a.set(2,4,  1);

    Matrix b = a;
    b.set(0,0, 1.8124); b.set(0,1,    0.5341); b.set(0,2,    2.8930); b.set(0,3,    5.2519); b.set(0,4,    4.8829);
    b.set(1,0, 2.5930); b.set(1,1,   2.6022); b.set(1,2,    4.2760); b.set(1,3,    5.9497); b.set(1,4,    6.3751);

    a.debugPrint("a");
    a.debugPrint("b");

    Matrix H = b * a.pseudoInverse();
    H.debugPrint("H");
    */


    printf("passed\n");
}
