#include <iostream>
#include "PolyhedralMesh.hpp"

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
	for (unsigned int i=0; i < pm.NumCell0Ds; i++)
		cout << pm.Cell3DsVertices[i] << endl;



	return 0;
}
