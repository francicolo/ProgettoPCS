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
	optional<unsigned int> id1, id2;
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

	// Cammino
	if (id1 && id2) { // Se Ids esistono, fare cammino
		if (id1 == id2) { cout << "Gli Id dei vertici scelti sono uguali --> Il cammino più corto tra i due è 0" << endl; }
		else {
			cout << "Trovo cammino tra i vertici di Id " << *id1 << " e " << *id2 << "..." << endl;
			//TO CREATE: CamminoVertici()
		}
	}

	// Output
        cout << "Output..." << endl;
	// Creazione dei file .txt
        ScriviFileTxt(result);
	// Creazione dei file .inp
        Eigen::MatrixXd ptraspo = result.Cell0DsCoordinates.transpose();
	Eigen::MatrixXi ltraspo = result.Cell1DsExtrema.transpose();
        Gedim::UCDUtilities utilities;
        {
                utilities.ExportPoints("./Cell0Ds.inp", ptraspo);
        }
        {
                utilities.ExportSegments("./Cell1Ds.inp", ptraspo, ltraspo);
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

