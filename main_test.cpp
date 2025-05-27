#include <iostream>
#include "PolyhedralMesh.hpp"
#include "Data.hpp"

using namespace std;
using namespace PolygonalLibrary;

int main() {
	cout << "Hello world" << endl;

	if(!test()){
		cerr << "Something went wrong..." << endl;
		return 1;
	}

	cout << "Value of mesh.NumCell0Ds: " << mesh.NumCell0Ds << endl;

	return 0;
}
