#pragma once
#include <gtest/gtest.h>
#include <cmath>
#include "PolyhedralMesh.hpp"
#include "Utils.hpp"

using namespace PolyhedralLibrary;
using namespace std;

TEST(TestUtils, TestNormalizzazione)
{
	Point p = {3, 0, 4};
	Point result = Normalizzazione(p);
	double norm = sqrt(result[0] * result[0] + result[1] * result[1] + result[2] * result[2]);
	
	EXPECT_NEAR(norm, 1.0, 1e-8);
}

TEST(TestUtils, TestBaricentro)
{
	Point A = {0, 0, 0};
	Point B = {1, 0, 0};
	Point C = {0, 1, 0};
	Point P = Baricentro(A, B, C, 0.5, 0.5, 0.0);
	
	EXPECT_NEAR(P[0], 0.5, 1e-8);
    EXPECT_NEAR(P[1], 0.5, 1e-8);
    EXPECT_NEAR(P[2], 0.0, 1e-8);
}

TEST (TestUtils, TestTriangolazioneClasseI3)
{
	PolyhedralMesh baseMesh;
	Tetraedro(baseMesh);
	
	int b = 3;
	// q = 3;
	
	int T = b * b;
	int V = 2 * T + 2; 
	int E = 6 * T;
	int F = 4 * T;
	
	PolyhedralMesh trianguledMesh;
	Triangolazione(baseMesh, b, trianguledMesh);
	
	EXPECT_EQ(trianguledMesh.NumCell0Ds, V); 
	EXPECT_EQ(trianguledMesh.NumCell1Ds, E); 
	EXPECT_EQ(trianguledMesh.NumCell2Ds, F); 
}

TEST (TestUtils, TestTriangolazioneClasseI4)
{
	PolyhedralMesh baseMesh;
	Ottaedro(baseMesh);
	
	int b = 3;
	// q = 4;
	
	int T = b * b;
	int V = 4 * T + 2; 
	int E = 12 * T;
	int F = 8 * T;
	
	PolyhedralMesh trianguledMesh;
	Triangolazione(baseMesh, b, trianguledMesh);
	
	EXPECT_EQ(trianguledMesh.NumCell0Ds, V); 
	EXPECT_EQ(trianguledMesh.NumCell1Ds, E); 
	EXPECT_EQ(trianguledMesh.NumCell2Ds, F); 
}

TEST (TestUtils, TestTriangolazioneClasseI5)
{
	PolyhedralMesh baseMesh;
	Icosaedro(baseMesh);
	
	int b = 3;
	// q = 5;
	
	int T = b * b;
	int V = 10 * T + 2; 
	int E = 30 * T;
	int F = 20 * T;
	
	PolyhedralMesh trianguledMesh;
	Triangolazione(baseMesh, b, trianguledMesh);
	
	EXPECT_EQ(trianguledMesh.NumCell0Ds, V); 
	EXPECT_EQ(trianguledMesh.NumCell1Ds, E); 
	EXPECT_EQ(trianguledMesh.NumCell2Ds, F); 
}

TEST (TestUtils, TestDuale)
{
	PolyhedralMesh baseMesh;
	Tetraedro(baseMesh);
	
	int V = baseMesh.NumCell0Ds;
	int E = baseMesh.NumCell1Ds;
	int F = baseMesh.NumCell2Ds;
	
	int V_duale = F; 
	int E_duale = E;
	int F_duale = V;
	
	PolyhedralMesh dualMesh;
	Duale(baseMesh, dualMesh);
	
	EXPECT_EQ(dualMesh.NumCell0Ds, V_duale); 
	EXPECT_EQ(dualMesh.NumCell1Ds, E_duale); 
	EXPECT_EQ(dualMesh.NumCell2Ds, F_duale); 
}

TEST(TestUtils, TestBFS)
{
    PolyhedralMesh mesh;

    mesh.NumCell0Ds = 4;

    mesh.NumCell1Ds = 5;
    mesh.Cell1DsExtrema = Eigen::MatrixXi(5, 2);
    mesh.Cell1DsExtrema << 
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        0, 2;

    std::vector<int> path = BFS(mesh, 0, 3);

    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), 0);
    EXPECT_EQ(path.back(), 3);

    EXPECT_TRUE(path.size() == 2 || path.size() == 3);
}
