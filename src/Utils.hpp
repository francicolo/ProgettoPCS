// File Utils.hpp
// Definizione di alias
// Dichiarazione di funzioni per la triangolazione e la scrittura su file .txt


#pragma once
#include "PolyhedralMesh.hpp"
#include <map>
#include <iomanip>
#include <sstream>

namespace PolyhedralLibrary {

	using Point = std::array<double, 3>;

	using Edge = std::pair<int, int>;

	using Triangle = std::array<int, 3>;

	Point Normalizzazione(const Point& p);
	
	Point Baricentro(const Point& A, const Point& B, const Point& C, double u, double v, double w);

	void Triangolazione(const PolyhedralMesh& base, int b, PolyhedralMesh& result);

	void ScriviFileTxt(const PolyhedralMesh& pm, const std::string& basepath = "");

	Point Controllo(const Point& p, double er);

	void Duale(const PolyhedralMesh& base, PolyhedralMesh& duale);

	std::vector<int> BFS(PolyhedralMesh& pm, const unsigned int id1, const unsigned int id2);

}



