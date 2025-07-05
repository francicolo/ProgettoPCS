// File Utils.cpp
// Funzioni di utilità per la costruzione, normalizzazione e scrittura dei poliedri 
// Include triangolazione, proiezione su sfera, generazione dei lati e output su file .txt

#include <cmath>
#include <map>
#include <vector>
#include <array>
#include <queue>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include "PolyhedralMesh.hpp"
#include "Utils.hpp"


using namespace std;
using namespace Eigen;
using namespace PolyhedralLibrary;


namespace PolyhedralLibrary {

// Proietta un punto 3D sulla superficie della sfera unitaria
Point Normalizzazione(const Point& p) {
    double norm = sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]);
    return {p[0]/norm, p[1]/norm, p[2]/norm};
}


// Calcola un punto interno al triangolo ABC usando coordinate baricentriche (u, v, w)
Point Baricentro(const Point& A, const Point& B, const Point& C, double u, double v, double w) {
    return {
        u * B[0] + v * C[0] + w * A[0],
        u * B[1] + v * C[1] + w * A[1],
        u * B[2] + v * C[2] + w * A[2]
    };
}


Point Controllo(const Point& p, double er = 1e-8) {
    return {
        std::round(p[0] / er) * er,
        std::round(p[1] / er) * er,
        std::round(p[2] / er) * er
    };
}


// Triangola tutte le facce triangolari del poliedro base di classe I
// Genera nuovi vertici e triangoli, normalizza i vertici sulla sfera, evita duplicati con una mappa
// Popola il nuovo PolyhedralMesh 'result' con vertici e facce triangolate
void Triangolazione(const PolyhedralMesh& base, int b, PolyhedralMesh& result) {
   
    map<Point, int> newVertexMap;   // Mappa per evitare duplicati: Point -> id vertice
    vector<Point> newVertices;      // Lista dei nuovi vertici 3D
    vector<Triangle> newTriangles;  // Lista dei triangoli generati 
	

    for (int f = 0; f < base.NumCell2Ds; ++f) {  
        vector<Point> faceVerts;                           // Lista dei nuovi vertici di una faccia
        for (int vi = 0; vi < 3; ++vi) {   
            int id = base.Cell2DsVertices(f, vi);          // Salvo in 'id' l'id del vertice vi della faccia f
            Vector3d v = base.Cell0DsCoordinates.row(id);  // Recupero le coordinate 3D del vertice id
            faceVerts.push_back({v[0], v[1], v[2]});       // Salvo i vertici di ciascuna faccia nel vettore faceVerts
         
		}

		// Assegno i 3 vertici ai punti 3D A, B, C
        Point A = faceVerts[0]; 
        Point B = faceVerts[1];
        Point C = faceVerts[2];
		
		
        Eigen::MatrixXi idx_map = Eigen::MatrixXi::Constant(b + 1, b + 1, -1); // Matrice idx_map intera quadrata di dimensione (b+1)x(b+1) in cui tutti gli elementi sono inizializzati a -1


        for (int i = 0; i <= b; ++i) {           // Scorre tutti i punti sul lato AB inclusi i vertici
            for (int j = 0; j <= b - i; ++j) {   // Scorre tutti i punti sul lato AC inclusi i vertici, 'j <= b - i' serve per non uscire dal triangolo
				
				double u = (double) i / b;       // u misura quanto sei spostato da A verso B
                double v = (double) j / b;       // v misura quanto sei spostato da A verso C
                double w = 1.0 - u - v;          // u + v + w = 1
	
                Point P = Baricentro(A, B, C, u, v, w); 
                P = Normalizzazione(P); 
                

				int vertexpos;                          // Posizione (indice) del vertice corrente 
				std::array key = Controllo(P);                     // Crea una chiave per cercare P nella mappa newVertexMap
                auto it = newVertexMap.find(key);       // Cerca il punto key nella mappa, se esiste 'it' punta all'elemento corrispondente, se non esiste 'it == newVertexMap.end()'
                if (it == newVertexMap.end()) {         // Se il punto P non è stato ancora inserito
                    newVertices.push_back(P);           // Aggiungo il nuovo punto P alla lista newVertices
                    vertexpos = (int)newVertices.size() - 1;  // L'indice di questo punto è l'ultimo elemento della lista size() -1 perchè gli indici partono da 0
                    newVertexMap[key] = vertexpos;      // Memorizza nella mappa newVertexMap l'associazione tra il punto P (key) e l'indice assegnato
                } else {                                // Se il punto è già presente
				 vertexpos = it->second;                // Recupera l'indice già assegnato a P dalla mappa
                }
                idx_map(i, j) = vertexpos;              // Salva l'indice del vertice P nella posizione (i,j) della matrice idx_map 
            }
        }	

        // Triangolazione della griglia
        for (int i = 0; i < b; ++i) {                   // Itera sulle righe della griglia triangolare interna a una faccia
            for (int j = 0; j < b - i; ++j) {   		// Itera sulle colonne nella riga i, 'j < b - i' assicura che le righe si accorcino man mano che si scende nel triangolo
                int p1 = idx_map(i,j);          		// Recupera il primo vertice (angolo in alto a sinistra)
                int p2 = idx_map(i+1,j);        		// Recupera il secondo vertice (angolo in basso a sinistra)
                int p3 = idx_map(i,j+1);                // Recupera il terzo vertice (angolo in alto a destra)
                newTriangles.push_back({p1, p2, p3});   // Crea il primo triangolo con i vertici e lo aggiunge alla lista dei nuovi triangoli

                if (j < b - i - 1) {			   	      // Verifica se la cella corrente non è l'ultima nella riga i
                    int p4 = idx_map(i+1,j+1);            // Calcola il quarto vertice (in basso a destra) che serve per generare il secondo triangolo della cella
                    newTriangles.push_back({p2, p4, p3}); // Crea il secondo triangolo in questa cella e lo aggiunge alla lista dei nuovi triangoli
                }
            }
        }
    }

    // Salva dati in result
    result.NumCell0Ds = newVertices.size();                  // Salva il numero totale di vertici 
    
	result.Cell0DsId.resize(result.NumCell0Ds); 			 // Ridimensiona il vettore degli id dei vertici a contenere un id per ogni vertice
	
	result.Cell0DsCoordinates.resize(result.NumCell0Ds, 3);  // Ridimensiona la matrice delle coordinate dei vertici per contenere tutte le coordinate dei vertici
    
	for (int i = 0; i < newVertices.size(); ++i) {			 // Scorre tutti i nuovi vertici
        result.Cell0DsId[i] = i; 							 // Assegna a ogni vertice i un id univoco
        result.Cell0DsCoordinates(i, 0) = newVertices[i][0]; // Salva le coordinate x, y, z del vertice i nella matrice delle coordinate
        result.Cell0DsCoordinates(i, 1) = newVertices[i][1];
        result.Cell0DsCoordinates(i, 2) = newVertices[i][2];
    }

    result.NumCell2Ds = newTriangles.size();					// Salva il numero totale di triangoli generati nella triangolazione
	
    result.Cell2DsId.resize(result.NumCell2Ds);					// Ridimensiona il vettore degli id delle facce a contenere un id per ogni faccia
    
	result.Cell2DsVertices.resize(result.NumCell2Ds, 3);		// Ridimensiona la matrice dei vertici di ogni faccia per contenere tutti i vertici
    
	for (int i = 0; i < newTriangles.size(); ++i) {				// Scorre su ogni triangolo generato
        result.Cell2DsId[i] = i;								// Assegna a ogni triangolo i un id univoco
        for (int j = 0; j < 3; ++j) {							// Scorre sui 3 vertici del triangolo i
            result.Cell2DsVertices(i, j) = newTriangles[i][j];  // Inserisce l'indice del vertice j-esimo del triangolo i nella struttura result
        }
    }

    // Costruzione dei lati
    std::map<Edge, int> edgeMap;   // Mappa che tiene traccia di quali lati sono già stati aggiunti
    std::vector<Edge> edges;       // Lista con i nuovi lati trovati non duplicati 

    for (int f = 0; f < result.NumCell2Ds; ++f) {       // Scorre tutte le facce del poliedro
        int v[3] = { 
            result.Cell2DsVertices(f, 0),		       
            result.Cell2DsVertices(f, 1),		
            result.Cell2DsVertices(f, 2)
        };                                          // Estrae i 3 vertici della faccia f

        for (int i = 0; i < 3; ++i) {	            // Scorre i 3 lati della faccia 
            int a = v[i];
            int b = v[(i + 1) % 3];                 // Prende una coppia consecutiva di vertici a, b che formano un lato
            Edge e = std::minmax(a, b); 			// Ordina la coppia per evitare duplicati
            if (edgeMap.find(e) == edgeMap.end()) { // Se il lato non è già presente 
                edgeMap[e] = (int)edges.size();		// Salva il lato con un id 
                edges.push_back(e);					// Aggiunge il lato alla lista edges
            }
        }
    }

    result.NumCell1Ds = (int)edges.size();              // Imposta il numero totale di lati trovati
	
    result.Cell1DsId.resize(result.NumCell1Ds);				// Ridimensiona il vettore degli id dei lati
    
	result.Cell1DsExtrema.resize(result.NumCell1Ds, 2);     // Ridimensiona una matrice per contenere i vertici estremi di ogni lato
    
	for (int i = 0; i < result.NumCell1Ds; ++i) { 		// Scorre su tutti i lati rilevati
        result.Cell1DsId[i] = i;						// Assegna un id a ogni lato
        result.Cell1DsExtrema(i, 0) = edges[i].first;   // Salva i due estremi del lato i
        result.Cell1DsExtrema(i, 1) = edges[i].second;
    }
	
	// Salvo i lati associati a ciascuna faccia 
	
	result.Cell2DsEdges.resize(result.NumCell2Ds, 3);	// Ridimensiono matrice Cell2DsEdges in modo che abbia una riga per ogni faccia triangolare
	
    for (int f = 0; f < result.NumCell2Ds; ++f) {
        int v0 = result.Cell2DsVertices(f, 0);			// Estraggo i 3 vertici della faccia f
        int v1 = result.Cell2DsVertices(f, 1);
        int v2 = result.Cell2DsVertices(f, 2);
        std::array<Edge, 3> faceEdges = {				// Costruisco un array che contiene le tre coppie ordinate di vertici che definiscono i lati della faccia
            std::minmax(v0, v1),
            std::minmax(v1, v2),
            std::minmax(v2, v0)
        };
        for (int j = 0; j < 3; ++j) {
            result.Cell2DsEdges(f, j) = edgeMap[faceEdges[j]];	// Cerco in edgeMap l'id del lato corrispondente alla coppia e scrivo questo id nella matrice Cell2DsEdges, nella riga f, colonna j
        }
    }
	
    result.Cell3DsId = 0;							// Assegna un id al poliedro risultante
	
    result.Cell3DsVertices = result.Cell0DsId;		// Assegna tutti gli id dei vertici al poliedro
	
    result.Cell3DsEdges = result.Cell1DsId;			// Assegna tutti gli id dei lati al poliedro
	
	result.Cell3DsFaces = result.Cell2DsId;			// Assegna tutti gli id delle facce al poliedro

	
 
}





void ScriviFileTxt(const PolyhedralMesh& pm, const std::string& basepath) {
    
	const int L = 14;      // Larghezza fissa delle colonne per allineare intestazione e valori
	
	// Scrittura Cell0Ds.txt (vertici)
	std::ofstream out0(basepath + "Cell0Ds.txt");    // Apertura file Cell0Ds.txt
	
    out0 << std::left
         << std::setw(L) << "id"
         << std::setw(L) << "x"
         << std::setw(L) << "y"
         << std::setw(L) << "z" << "\n";  // Stampa l'intestazione allineata

    out0 << std::fixed << std::setprecision(6);   // Imposto 6 cifre decimali
    
	for (int i = 0; i < pm.NumCell0Ds; ++i) {
        out0 << std::left
             << std::setw(L) << pm.Cell0DsId[i]
             << std::setw(L) << pm.Cell0DsCoordinates(i, 0)
             << std::setw(L) << pm.Cell0DsCoordinates(i, 1)
             << std::setw(L) << pm.Cell0DsCoordinates(i, 2) << "\n";  // Stampa una riga per ogni vertice con id e coordinate x, y, z
    }

	// Scrittura Cell1Ds.txt (lati)
    std::ofstream out1(basepath + "Cell1Ds.txt");    // Apertura file Cell1Ds.txt
	
	out1 << std::left
         << std::setw(L) << "id"
         << std::setw(L) << "origin"
         << std::setw(L) << "end" << "\n";   // Stampa l'intestazione allineata
		
	for (int i = 0; i < pm.NumCell1Ds; ++i) {
        out1 << std::left
             << std::setw(L) << pm.Cell1DsId[i]
             << std::setw(L) << pm.Cell1DsExtrema(i, 0)
             << std::setw(L) << pm.Cell1DsExtrema(i, 1) << "\n";  // Stampa una riga per ogni lato con id lato, id vertice origine e id vertice fine
    }

	// Scrittura file Cell2Ds.txt (facce)
    std::ofstream out2(basepath + "Cell2Ds.txt");      // Apertura file Cell2Ds.txt

    out2 << std::left
         << std::setw(L) << "id"
         << std::setw(L) << "numvertici"
         << std::setw(L) << "numlati"
		 << std::setw(L) << "idvertici"
		 << std::setw(L) << "idlati" << "\n";		   // Intestazione
		 
	
		 
    for (int f = 0; f < pm.NumCell2Ds; ++f) {
		 
		std::string idvert = std::to_string(pm.Cell2DsVertices(f, 0)) + ", " +
							 std::to_string(pm.Cell2DsVertices(f, 1)) + ", " +
							 std::to_string(pm.Cell2DsVertices(f, 2));				// Stringa che contiene gli id dei tre vertici della faccia f

    
		std::string idlati = std::to_string(pm.Cell2DsEdges(f, 0)) + ", " +
							 std::to_string(pm.Cell2DsEdges(f, 1)) + ", " +
							 std::to_string(pm.Cell2DsEdges(f, 2));					// Stringa che contiene gli id dei tre lati della faccia f

		out2 << std::setw(L) << pm.Cell2DsId[f]
			 << std::setw(L) << pm.Cell2DsVertices.cols()
			 << std::setw(L) << pm.Cell2DsEdges.cols()
			 << std::setw(L) << idvert
			 << std::setw(L) << idlati
			 << "\n";     										
}

    // Scrittura file Cell3Ds.txt (poliedro)
	std::ofstream out3(basepath + "Cell3Ds.txt");
 
    out3 << std::left
         << std::setw(L) << "id"
         << std::setw(L) << "numvertici"
         << std::setw(L) << "numlati"
         << std::setw(L) << "numfacce" << "\n"
		 << std::setw(L) << pm.Cell3DsId
         << std::setw(L) << pm.Cell3DsVertices.size()
         << std::setw(L) << pm.Cell3DsEdges.size()
         << std::setw(L) << pm.Cell3DsFaces.size() << "\n\n";
		 
	out3 << "idvertici:\n";
        
    for (size_t i = 0; i < pm.Cell3DsVertices.size(); ++i) {
		out3 << pm.Cell3DsVertices[i];
		if (i + 1 < pm.Cell3DsVertices.size()) out3 << ",";
	}
	out3 << "\n\n";

	out3 << "idlati:\n";
	
	for (size_t i = 0; i < pm.Cell3DsEdges.size(); ++i) {
		out3 << pm.Cell3DsEdges[i];
		if (i + 1 < pm.Cell3DsEdges.size()) out3 << ",";
	}
	out3 << "\n\n";

	out3 << "idfacce:\n";
	
	for (size_t i = 0; i < pm.Cell3DsFaces.size(); ++i) {
		out3 << pm.Cell3DsFaces[i];
		if (i + 1 < pm.Cell3DsFaces.size()) out3 << ",";
	}
	out3 << "\n";
		   
}

// Calcolo duale
void Duale (const PolyhedralMesh& base, PolyhedralMesh& duale) {

	// Costruzione vertici duale
	duale.NumCell0Ds = base.NumCell2Ds;
	duale.Cell0DsId.resize(duale.NumCell0Ds);
	duale.Cell0DsCoordinates.resize(duale.NumCell0Ds, 3);

	 for (int f = 0; f < base.NumCell2Ds; ++f) {       // Scorre tutte le facce del poliedro base
		duale.Cell0DsId[f] = f;						   // Assegna un ID univoco f al vertice del duale corrispondente alla faccia f
		Eigen::RowVector3d sum = Eigen::RowVector3d::Zero();  // Inizializzo un vettore 3D somma a 0 per sommare le coordinate dei vertici della faccia f
		int count = 0;



		for (int j = 0; j < base.Cell2DsVertices.cols(); ++j) {  // Scorre tutti i vertici v della faccia f
			int v = base.Cell2DsVertices(f, j);
			if (v != -1) {
			sum += base.Cell0DsCoordinates.row(v);
			count ++;
			}
		}
		if (count > 0) {
			Eigen::RowVector3d centroid = (sum / base.Cell2DsVertices.cols()).normalized();  // Divido la somma per il numero di vertici e normalizzo
			duale.Cell0DsCoordinates.row(f) = centroid;  // Salvo baricentro normalizzato come coordinata del nuovo vertice del duale
		}
	 }

	// Costruzione lati del duale
	std::map<std::pair<int,int>, std::vector<int>> edgeFaceMap;   // Mappa: a ogni lato (coppia di vertici ordinata) si associano le face del poliedro base che lo condividono
	for (int f = 0; f < base.NumCell2Ds; ++f) {
		std::vector<int> verts;
		for (int i = 0; i < base.Cell2DsVertices.cols(); ++i) {  // Scorre tutti i lati (ab) di ogni faccia e prende i vertici a coppie consecutive
			int v = base.Cell2DsVertices(f, i);
			if (v != -1) verts.push_back(v);
		}

		int nv = verts.size();
		for (int j = 0; j < nv; ++j) {
			int a = verts[j];
			int b = verts[(j+1) % nv];  // Vedi iPad
			auto edge = std::minmax(a, b);    // minmax ordina la coppia per evitare duplicati
			edgeFaceMap[edge].push_back(f);   // Inserisce la faccia trovata f nel vettore delle facce che usano quel lato
		}
	}

	std::vector<std::pair<int, int>> dualEdges;   // Inizializzo un vettore di coppie di interi: ogni coppia (f1,f2) rappresenta un lato nel duale==> collego i baricentri delle facce f1 e f2 del poliedro originale
	for (auto& [edge, faces] : edgeFaceMap) {	  // Scorre la mappa edgeFaceMap
		if (faces.size() == 2) {				  // Se il vettore faces ha dimensione 2 (cioè un lato è associato a due id faccia) ==> OK, le due facce sono adiacenti
			dualEdges.emplace_back(faces[0], faces[1]);		// Collego il loro baricentro con un lato nel duale
		}
	}

	duale.NumCell1Ds = dualEdges.size();
	duale.Cell1DsId.resize(duale.NumCell1Ds);
	duale.Cell1DsExtrema.resize(duale.NumCell1Ds, 2);

	for (int i = 0; i < dualEdges.size(); ++i) {					// Scorre tutti i lati del duale memorizzati in dualEdges
		duale.Cell1DsId[i] = i;										// Assegna al lato i del duale l'ID i
		duale.Cell1DsExtrema(i, 0) = dualEdges[i].first;            // Inserisce il primo estremo del lato i nella colonna 0 della matrice Cell1DsExtrema... dualEdges[i].first è l'ID della prima faccia del base che condivide quel lato, e ora nel duale quella faccia è diventata un vertice
		duale.Cell1DsExtrema(i, 1) = dualEdges[i].second;
	}

	// Costruzione facce del duale

	duale.NumCell2Ds = base.NumCell0Ds;
	duale.Cell2DsId.resize(duale.NumCell2Ds);
	std::vector<std::vector<int>> facce_duale(base.NumCell0Ds);  // Crea un vettore di vettori di interi con una riga per ogni vertice del poliedro originale base.NumCell0Ds.. facce_duale[i] conterrà la lista degli ID delle facce del poliedro base che usano il vertice i

	for (int f = 0; f < base.NumCell2Ds; ++f) {					// Scorre facce del poliedro base
		for (int j = 0; j < base.Cell2DsVertices.cols(); ++j) {		// Scorre i vertici  di ogni faccia
			int v = base.Cell2DsVertices(f, j);
			if (v != -1) {// Salvo in v i vertici
			facce_duale[v].push_back(f);
			}
		}
	}

	duale.Cell2DsId.resize(facce_duale.size());    // Riporto dati nel duale
	duale.Cell2DsVertices = Eigen::MatrixXi::Constant(facce_duale.size(), base.NumCell2Ds, -1);
	duale.Cell2DsEdges = Eigen::MatrixXi::Constant(facce_duale.size(), base.NumCell2Ds, -1);

	for (int i = 0; i < facce_duale.size(); ++i) {
		duale.Cell2DsId[i] = i;     // ogni i rappresenta un nuovo id faccia nel duale
		for (int j = 0; j < facce_duale[i].size(); ++j) {
			duale.Cell2DsVertices(i, j) = facce_duale[i][j];
		}
	}

	/*int maxcols = 0;
	for (const auto& faccia : facce_duale)
		maxcols = std::max(maxcols, (int)faccia.size());*/  // trovo quanti vertici ha la faccia più "lunga", cioè il num max di lati nel duale

	duale.Cell3DsId = 0;
	duale.Cell3DsVertices = duale.Cell0DsId;
	duale.Cell3DsEdges = duale.Cell1DsId;
	duale.Cell3DsFaces = duale.Cell2DsId;


}

/// Algoritmo Breadth-First Search
vector<int> BFS(PolyhedralMesh& pm, const unsigned int id1, const unsigned int id2){
	if (id1 == id2){
		vector<int> percorso = {};
		percorso.push_back(id1);
		return percorso;
	}

	// Creo la lista di adiacenza
	vector<vector<int>> listaAdiacenza(pm.NumCell0Ds);
	for (int i = 0; i < pm.NumCell1Ds; i++){
		listaAdiacenza[pm.Cell1DsExtrema(i, 0)].push_back(pm.Cell1DsExtrema(i, 1));
		listaAdiacenza[pm.Cell1DsExtrema(i, 1)].push_back(pm.Cell1DsExtrema(i, 0));
	}

	// Algoritmo BFS
	unsigned int n = pm.NumCell0Ds;
	vector<bool> reached(n, false); 
	vector<int> pred(n, -1);
	queue<int> q;

	q.push(id1);
	reached[id1] = true;
	bool trovato = false; // flag falsa finché non trovo id2

	while(!q.empty() && !trovato){
		int u = q.front();
		q.pop();
		
		for (int w : listaAdiacenza[u] ){
			if (!reached[w]){ //se il nodo u non è stato raggiunto
				reached[w] = true;
				pred[w] = u;
				q.push(w); // aggiungo i vicino di u alla coda
				
				if (w == id2){
					trovato = true;
					break;
				}
			}
		}
	}

	if (trovato){
		vector<int> percorso;
		cout << "trovato!!"<< endl;
		int k = id2;
		while(k != -1) { // ripercorro il cammino al contrario...
			percorso.insert(percorso.begin(), k); // ... salvando gli id dei vertici percorsi all'inizio del vettore percorso...
			k = pred[k];
			/*if (k == id1)
				break;*/
		}
		return percorso; // .. il vettore percorso contiene in ordine i vertici visitati
	}
	else {
		cout << "Nessun cammino minimo trovato"<< endl; 
		return {};
	}
}

}
