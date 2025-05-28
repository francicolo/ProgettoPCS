#include "PolyhedralMesh.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Eigen;


namespace PolyhedralLibrary {


/*void Tetraedro(PolyhedralMesh& pm){

    pm.NumCell0Ds = 4; // numero vertici tetraedro
	
	unsigned int i= 0;
    for (i = 0; i < 4; i++){
        pm.Cell0DsId[i] = i; // vettore degli id dei vertici
    }
    pm.Cell0DsCoordinates << 0,577350269, 0,577350269, 0,577350269,
                            0,577350269, -0,577350269, -0,577350269,
                            -0,577350269, 0,577350269, -0,577350269,
                            -0,577350269, -0,577350269, 0,577350269;
                            
    pm.NumCell1Ds = 6;

    for (i = 0; i < 6; i++){
        pm.Cell1DsId[i] = i; // vettore degli id dei lati
    }

    pm.Cell1DsExtrema << 0, 1,
                        0, 2, 
                        0, 3, 
                        1, 2, 
                        1, 3, 
                        2, 3;
                        

    pm.NumCell2Ds = 4;

    for (i = 0; i < 4; i++){
        pm.Cell2DsId[i] = i; // vettore degli id delle facce
    }

    pm.Cell2DsVertices = 
    {
        {0, 1, 2},
        {0, 2, 3},
        {1, 2, 3},
        {0, 1, 3}
    };

    pm.Cell2DsEdges = 
    {
        {0, 1, 3},
        {1, 2, 5},
        {3, 4, 5},
        {0, 2, 4}

    };

    pm.NumCell3Ds = 1; 

    pm.Cell3DsId = 0; // vettore degli id dei poliedri

    pm.Cell3DsVertices = 
    {
        {0, 1, 2, 3}
    };
    //pm.Cell3DsVertices = pm.Cell0DsId

    pm.Cell3DsEdges = 
    {
        {0, 1, 2, 3, 4, 5}
    };
    //pm.Cell3DsEdges = pm.Cell1DsId

    pm.Cell3DsFaces = 
    {
        {0, 1, 2, 3}
    };
    //pm.Cell3DsFaces = pm.Cell2DsId
}

void Cubo(PolyhedralMesh& pm){
    pm.NumCell0Ds = 8; //numero vertici cubo

	unsigned int i= 0;
	
    for (i = 0; i < 8; i++){
        pm.Cell0DsId[i] = i; // vettore degli id dei vertici
    }
    pm.Cell0DsCoordinates << 0,577350269, 0,577350269, 0,577350269,
                            0,577350269, -0,577350269, -0,577350269,
                            -0,577350269, 0,577350269, -0,577350269,
                            -0,577350269, -0,577350269, 0,577350269,
                            0,577350269, 0,577350269, -0,577350269,
                            0,577350269, -0,577350269, 0,577350269,
                            -0,577350269, 0,577350269, 0,577350269,
                            -0,577350269, -0,577350269, -0,577350269;

                            
    pm.NumCell1Ds = 12; //numero lati cubo

    for (i = 6; i < 18; i++){
        pm.Cell1DsId[i-6] = i; // vettore degli id dei lati
    }

    pm.Cell1DsExtrema << 0, 4,
                        0, 5,
                        0, 6,
                        1, 4,
                        1, 5,
                        1, 7,
                        2, 4,
                        2, 6,
                        2, 7,
                        3, 5,
                        3, 6,
                        3, 7;
                        

    pm.NumCell2Ds = 6; // numero facce cubo

    for (i = 4; i < 10; i++){
        pm.Cell2DsId[i-4] = i; // vettore degli id delle facce
    }

    pm.Cell2DsVertices = 
    {
        {0, 4, 1, 5},
        {0, 4, 2, 6},
        {0, 6, 3, 5},
        {6, 2, 7, 3},
        {3, 5, 1, 7},
        {2, 7, 1, 4}
    };

    pm.Cell2DsEdges = 
    {
        {6, 9, 10, 7},
        {6, 12, 13, 8},
        {8, 16, 15, 7},
        {13, 14, 17, 16},
        {15, 10, 11, 17},
        {14, 11, 9, 12}

    };

    pm.NumCell3Ds = 1; 

    pm.Cell3DsId = 1;
    

    pm.Cell3DsVertices = 
    {
        {0, 1, 2, 3, 4, 5, 6, 7}
    };
    //pm.Cell3DsVertices = pm.Cell0DsId

    pm.Cell3DsEdges = 
    {
        {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}
    };
    //pm.Cell3DsEdges = pm.Cell1DsId

    pm.Cell3DsFaces = 
    {
        {4, 5, 6, 7, 8, 9}
    };
    //pm.Cell3DsFaces = pm.Cell2DsId
}

/*void Ottaedro(PolyhedralMesh &pm){
	pm.NumCell0Ds = 6;
}*/

void Ottaedro(PolyhedralMesh& pm){
    pm.NumCell0Ds = 6;
	
	unsigned int i= 0;
	pm.Cell0DsId.resize(pm.NumCell0Ds);
    for (i = 0; i < 6; i++){
        pm.Cell0DsId[i] = i; // vettore degli id dei vertici
    }
	pm.Cell0DsCoordinates.resize(pm.NumCell0Ds,3);
    pm.Cell0DsCoordinates << 1, 0, 0,
                            -1, 0, 0,
                            0, 1, 0,
                            0, -1, 0,
                            0, 0, 1,
                            0, 0, -1;


    pm.NumCell1Ds = 12; //numero lati ottaedro
	pm.Cell1DsId.resize(pm.NumCell1Ds);
    for (i = 0; i < 12; i++){
        pm.Cell1DsId[i] = i; // vettore degli id dei lati
    }
	
	pm.Cell1DsExtrema.resize(pm.NumCell1Ds, 2);
    pm.Cell1DsExtrema << 8, 10,
                        8, 11,
                        8, 12,
                        8, 13,
                        9, 10,
                        9, 11,
                        9, 12,
                        9, 13,
                        10, 12,
                        10, 13,
                        11, 12,
                        11, 13;

    pm.NumCell2Ds = 8; // numero facce ottaedro
	
	pm.Cell2DsId.resize(pm.NumCell2Ds);
    for (i = 0; i < 8; i++){
        pm.Cell2DsId[i] = i; // vettore degli id delle facce
    }
	pm.Cell2DsVertices.resize(pm.NumCell2Ds, 3);
	
    pm.Cell2DsVertices <<
        12, 10, 9,
        12, 11, 9,
        12, 11, 8,
        12, 8, 10,
        13, 8, 10,
        13, 10, 9,
        13, 9, 11,
        13, 11, 8;
		
	pm.Cell2DsEdges.resize(pm.NumCell2Ds, 3);

    pm.Cell2DsEdges <<
        26, 22, 24,
        28, 23, 24,
        28, 19, 20,
        20, 18, 26,
        21, 18, 27,
        27, 22, 25,
        25, 23, 29,
        29, 19, 21;

    pm.NumCell3Ds = 1;

    pm.Cell3DsId = 2;

    pm.Cell3DsVertices = pm.Cell0DsId;

    pm.Cell3DsEdges = pm.Cell1DsId;

    pm.Cell3DsFaces = pm.Cell2DsId;

}
/*

void Dodecaedro(PolyhedralMesh& pm){
    pm.NumCell0Ds = 20; // numero vertici dodecaedro
	
	unsigned int i = 0;
    for (i = 0; i < 8; i++){
        pm.Cell0DsId[i] = i; // vettore degli id dei vertici
    }
    for (i = 14; i < 26; i++){
        pm.Cell0DsId[i-6] = i; // vettore degli id dei vertici
    }

    pm.Cell0DsCoordinates << 0,577350269, 0,577350269, 0,577350269,
                            0,577350269, -0,577350269, -0,577350269,
                            -0,577350269, 0,577350269, -0,577350269,
                            -0,577350269, -0,577350269, 0,577350269,
                            0,577350269, 0,577350269, -0,577350269,
                            0,577350269, -0,577350269, 0,577350269,
                            -0,577350269, 0,577350269, 0,577350269,
                            -0,577350269, -0,577350269, -0,577350269,
                            0,934172359, 0,35682209, 0,
                            0,934172359, -0,35682209, 0,
                            -0,934172359, 0,35682209, 0,
                            -0,934172359, -0,35682209, 0,
                            0,35682209, 0, 0,934172359,
                            -0,35682209, 0, 0,934172359,
                            0,35682209, 0, -0,934172359,
                            -0,35682209, 0, -0,934172359,
                            0, 0,934172359, 0,35682209,
                            0, -0,934172359, 0,35682209,
                            0, 0,934172359, -0,35682209,
                            0, -0,934172359, -0,35682209;


    pm.NumCell1Ds = 30; //numero lati dodecaedro

    for (i = 30; i < 60; i++){
        pm.Cell1DsId[i-30] = i; // vettore degli id dei lati
    }

    pm.Cell1DsExtrema << 0, 22,
                        0, 18,
                        0, 14,
                        6, 22,
                        6, 19,
                        6, 16,
                        3, 23,
                        3, 17,
                        3, 19,
                        5, 23,
                        5, 15,
                        5, 18,
                        4, 14,
                        4, 24,
                        4, 20,
                        2, 24,
                        2, 16,
                        2, 21,
                        7, 17,
                        7, 21,
                        7, 25,
                        1, 15,
                        1, 18,
                        1, 25,
                        16, 17,
                        14, 15,
                        22, 24,
                        23, 25,
                        19, 18,
                        21, 20;

    pm.NumCell2Ds = 12; // numero facce dodecaedro

    for (i = 18; i < 30; i++){
        pm.Cell2DsId[i-18] = i; // vettore degli id delle facce
    }

    pm.Cell2DsVertices = 
    {
        {19, 6, 22, 0, 18},
        {3, 19, 18, 5, 23},
        {3, 17, 16, 6, 19},
        {5, 18, 0, 14, 15},
        {21, 7, 25, 1, 20},
        {2, 21, 20, 4, 24},
        {2, 16, 17, 7, 21},
        {4, 20, 1, 15, 14},
        {4, 14, 0, 22, 24},
        {2, 24, 22, 6, 16},
        {7, 17, 3, 23, 25},
        {1, 25, 23, 5, 15}
    };

    pm.Cell2DsEdges = 
    {
        {34, 33, 30, 31, 58},
        {38, 58, 41, 39, 36},
        {37, 54, 35, 34, 38},
        {41, 31, 32, 55, 40},
        {49, 50, 53, 52, 59},
        {47, 59, 44, 43, 45},
        {46, 54, 48, 49, 47},
        {44, 52, 51, 55, 42},
        {42, 32, 30, 56, 43},
        {45, 56, 33, 35, 46},
        {48, 37, 36, 57, 50},
        {53, 57, 39, 40, 51}
    };

    pm.NumCell3Ds = 1;

    pm.Cell3DsId = 3; // id ottaedro

    pm.Cell3DsVertices = 
    {
        {0, 1, 2, 3, 4, 5, 6, 7, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25}
    };
    //pm.Cell3DsVertices = pm.Cell0DsId

    pm.Cell3DsEdges = 
    {
        {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59}
    };
    //pm.Cell3DsEdges = pm.Cell1DsId

    pm.Cell3DsFaces = 
    {
        {18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29}
    };
    //pm.Cell3DsFaces = pm.Cell2DsId
}

void Icosaedro(PolyhedralMesh& pm){
	pm.NumCell0Ds = 12; // numero vertici icosaedro
	unsigned int i = 0;
    for (i = 26; i < 38; i++){
        pm.Cell0DsId[i-26] = i; // vettore degli id dei vertici
    }
  

    pm.Cell0DsCoordinates << 0,850650808, 0,525731112, 0,
                            0,850650808, -0,525731112, 0,
                            -0,850650808, -0,525731112, 0,
                            -0,850650808, 0,525731112, 0,
                            0,525731112, 0, 0,850650808,
                            -0,525731112, 0, 0,850650808,
                            -0,525731112, 0, -0,850650808,
                            0,525731112, 0, -0,850650808,
                            0, 0,850650808, 0,525731112,
                            0, 0,850650808, -0,525731112,
                            0, -0,850650808, -0,525731112,
                            0, -0,850650808, 0,525731112;


    pm.NumCell1Ds = 30; //numero lati icosaedro

    for (i = 30; i < 60; i++){
        pm.Cell1DsId[i-30] = i; // vettore degli id dei lati
    }

    pm.Cell1DsExtrema << 31, 34,
                        31, 29,
                        31, 28,
                        31, 37,
                        31, 30,
                        30, 34,
                        30, 26,
                        30, 27,
                        30, 37,
                        33, 26,
                        33, 27,
                        33, 36,
                        33, 35,
                        33, 32,
                        32, 35,
                        32, 29,
                        32, 28,
                        32, 36,
                        34, 26,
                        34, 35,
                        34, 29,
                        35, 26,
                        35, 29,
                        37, 28,
                        37, 36,
                        37, 27,
                        36, 28,
                        36, 27,
                        27, 26,
                        28, 29;

    pm.NumCell2Ds = 20; // numero facce icosaedro

    for (i = 30; i < 50; i++){
        pm.Cell2DsId[i-30] = i; // vettore degli id delle facce
    }

    pm.Cell2DsVertices = 
    {
        {37, 31, 30},
        {31, 34, 30},
        {31, 29, 34},
        {31, 28, 29},
        {28, 31, 37},
        {37, 30, 27},
        {27, 30, 26},
        {30, 34, 26},
        {32, 36, 33},
        {32, 35, 33},
        {32, 28, 36},
        {29, 28, 32},
        {29, 32, 35},
        {35, 33, 26},
        {26, 33, 27},
        {33, 36, 27},
        {36, 28, 37},
        {36, 37, 27},
        {35, 34, 29},
        {35, 26, 34}
    };

    pm.Cell2DsEdges = 
    {
        {68, 63, 64},
        {64, 65, 60},
        {61, 60, 80},
        {62, 89, 61},
        {62, 63, 83},
        {68, 67, 85},
        {67, 66, 88},
        {65, 78, 66},
        {77, 71, 73},
        {74, 72, 73},
        {76, 86, 77},
        {89, 76, 75},
        {75, 74, 82},
        {72, 69, 81},
        {69, 70, 88},
        {71, 87, 70},
        {86, 83, 84},
        {84, 85, 87},
        {79, 80, 82},
        {81, 78, 79}
    };

    pm.NumCell3Ds = 1;

    pm.Cell3DsId = 4; // id icosaedro

    pm.Cell3DsVertices = 
    {
        {26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37}
    };
    //pm.Cell3DsVertices = pm.Cell0DsId
    pm.Cell3DsEdges = 
    {
        {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89}
    };
    //pm.Cell3DsEdges = pm.Cell1DsId


    pm.Cell3DsFaces = 
    {
        {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49}
    };
    //pm.Cell3DsFaces = pm.Cell2DsId
    
}
*/
}


