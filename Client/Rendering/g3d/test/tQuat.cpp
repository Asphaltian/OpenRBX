#include "../include/G3DAll.h"

static void testMatrixConversion() {

    {
        // This is a known corner case
        Matrix3 M = Matrix3::fromAxisAngle(Vector3::unitY(), toRadians(180));
        Quat q(M);
        Matrix3 M2 = q.toRotationMatrix();

		for (int r = 0; r < 3; ++r) {
			for (int c = 0; c < 3; ++c) {
				debugAssert(abs(M[r][c] - M2[r][c]) < 0.0005);
			}
		}
    }

    {
        // This is a known corner case (near the one above)
        Matrix3 M(-0.99999988f, 0, 0,
                   0, 1, 0,
                   0, 0, -0.99999988f);
        Quat q(M);
        Matrix3 M2 = q.toRotationMatrix();

		for (int r = 0; r < 3; ++r) {
			for (int c = 0; c < 3; ++c) {
				debugAssert(abs(M[r][c] - M2[r][c]) < 0.0005);
			}
		}
    }

    {

    }

    // Round trip M->q->M
	for (int i = 0; i < 100; ++i) {
		Matrix3 M = Matrix3::fromAxisAngle(Vector3::random(), random(0, G3D_TWO_PI));
        if (i == 0) {
            // Corner case, make sure we test it first.
            M = Matrix3::identity();
        }

		Quat q(M);
		Matrix3 M2 = q.toRotationMatrix();

		for (int r = 0; r < 3; ++r) {
			for (int c = 0; c < 3; ++c) {
				debugAssert(abs(M[r][c] - M2[r][c]) < 0.0005);
			}
		}
	}

	// Round trip q->M->q
	for (int i = 0; i < 100; ++i) {
		Quat q1 = Quat::fromAxisAngleRotation(Vector3::random(), random(0, G3D_TWO_PI));
		Matrix3 M = q1.toRotationMatrix();
		Quat q2(M);

		debugAssert(q1.fuzzyEq(q2) || q1.fuzzyEq(-q2));
	}
}


static void testSlerp() {
    // Test that we take the shortest path
    {
        Vector3 axis = Vector3::unitY();

        // Start
		float a0 = 0;

        // End
		float a1 = (float)(G3D_PI * 3/2);

		float a2 = (float)((G3D_PI * 3/2 + G3D_PI * 2) / 2);

		Quat q0 = Quat::fromAxisAngleRotation(axis, a0);
		Quat q1 = Quat::fromAxisAngleRotation(axis, a1);
		Quat q2 = Quat::fromAxisAngleRotation(axis, a2);

        // slerp result
		Quat rq2 = q0.slerp(q1, 0.5);

		float ra2;
		Vector3 raxis;
		rq2.toAxisAngleRotation(raxis, ra2);

		debugAssert(rq2.sameRotation(q2));
    }

    // Test general slerp
	for (int i = 0; i < 100; ++i) {
		Vector3 axis = Vector3::random();

        // We test 0->PI because that way we know the shortest path is
        // always between them (and not wrapping around the other way).

        // Start
		float a0 = random(0, G3D_PI);

        // End
		float a1 = random(0, G3D_PI);

		float a2 = (a0 + a1) / 2;
		Quat q0 = Quat::fromAxisAngleRotation(axis, a0);
		Quat q1 = Quat::fromAxisAngleRotation(axis, a1);
		Quat q2 = Quat::fromAxisAngleRotation(axis, a2);

		Quat rq2 = q0.slerp(q1, 0.5);

		float ra2;
		Vector3 raxis;
		rq2.toAxisAngleRotation(raxis, ra2);

		debugAssert(fuzzyEq(ra2, a2));
		debugAssert(raxis.fuzzyEq(axis));
		debugAssert(rq2.fuzzyEq(q2));
	}
}


void testQuat() {
	printf("Quat ");

	testSlerp();
	testMatrixConversion();

	printf("passed\n");
}
