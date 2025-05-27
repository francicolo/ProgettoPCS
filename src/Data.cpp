#include "Data.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolyhedralLibrary {

	bool test(PolyhedralMesh &mesh) {
		mesh.NumCell0Ds = 2;
		if(mesh.NumCell0Ds == 1) {
			return true;
		}
		else {
			return false;
		}
	}

}
