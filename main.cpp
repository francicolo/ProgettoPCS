#include <iostream>
#include <optional>
#include "PolyhedralMesh.hpp"
#include "Utils.hpp"
#include "Input.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace PolyhedralLibrary;

int main() {
	cout << "Hello world :-)" << endl;

	// Creazione variabili
	PolyhedralMesh mesh, base, triang, result;

	// Input
	vector<int> values;
	do { values = Input(); } while (values.empty()); // Sanificazione
	cout << "Input letto: ";
	for (int val : values) cout << val << " ";
	cout << endl;

	// Input salvato in variabili separate
	unsigned int p = values[0];
	unsigned int q = values[1];
	unsigned int b = values[2];
	unsigned int c = values[3];

// per cammino minimo
	double sum = 0.0; // lunghezza totale cammino minimo
	optional<unsigned int> id1, id2;
	vector<int> percorso = {};


	if (values.size() == 6) { // Ids salvati solo se aggiunti in input
		id1 = values[4];
		id2 = values[5];
	}

	// Controllo del simbolo di Schläfli
	int idSolid = Identify(p, q);
	switch (idSolid) {
		case 1: cout << "Solido selezionato: Tetraedro"; Tetraedro(mesh); break;
		case 2: cout << "Solido selezionato: Cubo"; Cubo(mesh); break;
		case 3: cout << "Solido selezionato: Ottaedro"; Ottaedro(mesh); break;
		case 4: cout << "Solido selezionato: Dodecaedro"; Dodecaedro(mesh); break;
		case 5: cout << "Solido selezionato: Icosaedro"; Icosaedro(mesh); break;
		default:
			cerr << "ERRORE (selezione invalida)\nNon esistono solidi platonici con simbolo di Schläfli {" << p << ", " << q << "}\nExiting..." << endl;
			// Nel caso di un simbolo di Schläfli non-esistente, dare errore

			return 1;
	}
	cout << endl;

	if (idSolid == 2 || idSolid == 4) { // Se viene selezionato Cubo/Dodecaedro, effettuare il duale per ottenere Ottaedro/Icosaedro
		cout << "Il solido selezionato non ha facce triangolari.\nCostruisco il duale..." << endl;
		Duale(mesh, base);
	}
	else { base = mesh; }

	// Controllo dei valori b e c per fare triangolazione
	if (b == c) {
		if (b == 0) {
			cerr << "ERRORE (suddivisioni invalide)\nImpossibile triangolare in 0 suddivisioni\nExiting..." << endl;
			// Nel caso di entrambi b e c uguali a 0, dare errore

			return 2;
		}
		cout << "Triangolazione (2)..." << endl; //  TO CREATE: Triangolazione2(mesh, b, result); // Triangolazione per solidi di classe II, solo se b è uguale a c
	}
	else if	((b == 0) != (c == 0)) {
		if (max(b, c) == 1) cout << "ATTENZIONE: si richiede di fare la triangolazione con una sola suddivisione, il risultato sarà uguale al solido platonico di base" << endl;
		cout << "Effettuo triangolazione in " << max(b, c) << " suddivision" << (max(b, c) == 1 ? "e" : "i") << "..." << endl; Triangolazione(base, max(b, c), triang); // OPZIONALE: Cambiare a Triangolazione(1) // Triangolazione per solidi di classe I, solo se b oppure c è uguale a 0
	}
	else {
		cerr << "ERRORE (suddivisioni invalide)\nI valori b c scelti eccedono l'ambito del progetto\nb oppure c devono essere uguali a 0 *OPPURE* b e c devono avere lo stesso valore\nExiting..." << endl;
		// Nel caso di b e c diversi, dare errore

		return 3;
	}

	if (idSolid == 2 || idSolid == 4) { // Se il solido di partenza era Cubo/Dodecaedro, è necessario rifare il duale per ottenere il solido di Goldberg
		cout << "Duale della triangolazione..." << endl;
		Duale(triang, result);
	}
	else { result = triang; } // Altrimenti, è già dato il solido Geodetico


	vector<double> cell1Ds_shortpath(result.NumCell1Ds, 0.0);
	vector<double> cell0Ds_shortpath(result.NumCell0Ds, 0.0);
//// Se ci vengono forniti id1 e id2 allora ne calcolo il cammino minimo
	if (id1.has_value() && id2.has_value()) {
		
		if (id1.value() >= result.NumCell0Ds || id2.value() >= result.NumCell0Ds) {
			cout << "I vertici forniti non sono validi.\nInserire vertici compresi tra 0 e " << (result.NumCell0Ds - 1) << endl;
		}
		else{
			cout << "Trovo cammino..." << endl;
			percorso = BFS(result, id1.value(), id2.value()); // Calcolo il cammino minimo tramite BFS

			for(size_t i = 0; i < percorso.size() - 1; i++){
				// verifico che i lati del cammino minimo siano presenti nei lati del poliedro

				for(int j = 0; j < result.NumCell1Ds; j++){ // scorro i lati
					if((result.Cell1DsExtrema(j,0) == percorso[i] && result.Cell1DsExtrema(j,1) == percorso[i+1] ) || // se c'è nella lista dei lati un lato tra i vertici del percorso
						(result.Cell1DsExtrema(j,1) == percorso[i] && result.Cell1DsExtrema(j,0) == percorso[i+1] )){ // ovvero il lato è nel percorso minimo
							// prendo le coordinate degli estremi dei lati nel percorso
							Eigen::Vector3d p1 = result.Cell0DsCoordinates.row(percorso[i]);
							Eigen::Vector3d p2 = result.Cell0DsCoordinates.row(percorso[i+1]); 
			
							sum += (p1-p2).norm(); // sommo la sua lunghezza alla lunghezza totale del percorso minimo
					}
				}
			}
	/// per esportazione
			// proprietà shortpath nei vertici
				
				// Aggiungo proprietà ShortPath = 1 ai lati che ne fanno parte
				if (!percorso.empty()){		
					for(const auto &idvertice : percorso){
						if (idvertice < result.NumCell0Ds){ // se il vertice è nel vettore dei vertici percorsi
							cell0Ds_shortpath.at(idvertice) = 1.0; // gli assegno 1 
						}
					}
				}

			// proprietà shortpath nei lati
				
				if (percorso.size() > 1) {
					for (size_t i = 0; i < percorso.size() - 1; i++) {
						int vertice1 = percorso[i];
						int vertice2 = percorso[i + 1];
				
				// Trova il lato che collega vertice1 e vertice2
						for (int j = 0; j < result.NumCell1Ds; j++) {
							if ((result.Cell1DsExtrema(j, 0) == vertice1 && result.Cell1DsExtrema(j, 1) == vertice2) || // se c'è un lato tra vertice1 e vertice2
								(result.Cell1DsExtrema(j, 1) == vertice1 && result.Cell1DsExtrema(j, 0) == vertice2)) { // o tra vertice2 a vertice1
				
							// Aggiungo proprietà ShortPath = 1 ai lati che ne fanno parte
								cell1Ds_shortpath.at(j) = 1.0;
							}
						}
					}
				}

			cout << "Il cammino minimo dal vertice con id1 = "<< id1.value() << " a quello con id2 = " << id2.value() << " è composto da " << percorso.size() - 1 << " lati" << endl;

			cout << "La lunghezza del cammino minimo è " << sum << endl;
		}
	}



        cout << "Output..." << endl;
        ScriviFileTxt(result);

        Eigen::MatrixXd ptraspo = result.Cell0DsCoordinates.transpose();
		Eigen::MatrixXi ltraspo = result.Cell1DsExtrema.transpose();
	
        Gedim::UCDUtilities utilities;

		// Esportazione dei vertici in Cell0Ds.inp
        {
			vector<Gedim::UCDProperty<double>> cell0Ds_properties(1);

			cell0Ds_properties[0].Label = "ShortPath";
			cell0Ds_properties[0].NumComponents = 1;
	    	cell0Ds_properties[0].Size = result.NumCell0Ds;
			cell0Ds_properties[0].Data = cell0Ds_shortpath.data();
			
			// Esportazione
			utilities.ExportPoints("./Cell0Ds.inp", ptraspo, cell0Ds_properties);
		}


		// Esportazione dei lati in Cell1Ds.inp
        {
			vector<Gedim::UCDProperty<double>> cell1Ds_properties(1);

			cell1Ds_properties[0].Label = "ShortPath";
			cell1Ds_properties[0].NumComponents = 1;
			cell1Ds_properties[0].Size = result.NumCell1Ds;		
			cell1Ds_properties[0].Data = cell1Ds_shortpath.data();
			
			// Esportazione
			utilities.ExportSegments("./Cell1Ds.inp", ptraspo, ltraspo, {}, cell1Ds_properties);

		}
	



//	---DEBUG---
/*	cout << "DEBUG -- triang.NumCell0Ds: " << triang.NumCell0Ds << endl;
	cout << "DEBUG -- result.NumCell0Ds: " << result.NumCell0Ds << endl;

	cout << "DEBUG -- triang.NumCell1Ds: " << triang.NumCell1Ds << endl;
        cout << "DEBUG -- result.NumCell1Ds: " << result.NumCell1Ds << endl;

        cout << "DEBUG -- triang.NumCell2Ds: " << triang.NumCell2Ds << endl;
        cout << "DEBUG -- result.NumCell2Ds: " << result.NumCell2Ds << endl;
*/

	cout << "Fine programma.\nExiting..." << endl;


	return 0;
}

