#include "Data.hpp"
//#include "PolyhedralMesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary {

	bool test(PolygonalMesh &mesh) {
		mesh.NumCell0Ds = 1;
		if(mesh.NumCell0Ds == 1) {
			return true;
		}
		else {
			return false;
		}
	}

}
