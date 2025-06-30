//strutture dati

#pragma once
#include <vector>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PolyhedralLibrary {

struct PolyhedralMesh
{
	unsigned int NumCell0Ds = 0;
	std::vector <unsigned int> Cell0DsId = {};
	Eigen::MatrixXd Cell0DsCoordinates = {};
	
	unsigned int NumCell1Ds = 0;
	std::vector <unsigned int> Cell1DsId = {};
	Eigen::MatrixXi Cell1DsExtrema = {};

	unsigned int NumCell2Ds = 0;
	std::vector <unsigned int> Cell2DsId = {};
	Eigen::MatrixXi Cell2DsVertices = {};
	Eigen::MatrixXi Cell2DsEdges = {};

	unsigned int Cell3DsId = 0;
	std::vector<unsigned int> Cell3DsVertices = {};
	std::vector<unsigned int> Cell3DsEdges = {};
	std::vector<unsigned int> Cell3DsFaces = {};
};


// funzioni

void Tetraedro(PolyhedralMesh& pm);
void Cubo(PolyhedralMesh& pm);
void Ottaedro(PolyhedralMesh& pm);
void Dodecaedro(PolyhedralMesh& pm);
void Icosaedro(PolyhedralMesh& pm);

}