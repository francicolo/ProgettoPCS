#include <iostream>
#include "PolyhedralMesh.hpp"
#include "Data.hpp"

using namespace std;
using namespace PolyhedralLibrary;

int main() {
	cout << "Hello world" << endl;

/*	PolyhedralMesh mesh;

	if(!test(mesh)){
		cerr << "Something went wrong..." << endl;
		return 1;
	}

	cout << "Value of mesh.NumCell0Ds: " << mesh.NumCell0Ds << endl;
*/
	PolyhedralMesh pm;
	Ottaedro(pm);
	cout << pm.NumCell0Ds << endl; 



	return 0;
}
