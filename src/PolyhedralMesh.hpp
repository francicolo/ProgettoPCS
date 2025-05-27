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
	std::vector<std::vector<unsigned int>> Cell2DsVertices = {};
	std::vector<std::vector<unsigned int>> Cell2DsEdges = {};
	
	unsigned int NumCell3Ds = 0;
	std::vector <unsigned int> Cell3DsId = {};
	std::vector<std::vector<unsigned int>> Cell3DsVertices = {};
	std::vector<std::vector<unsigned int>> Cell3DsEdges = {};
	std::vector<std::vector<unsigned int>> Cell3DsFaces = {};
};
}
