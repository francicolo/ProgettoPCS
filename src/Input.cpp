#include "Input.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <cctype>

using namespace std;

namespace PolyhedralLibrary {
	// Rimozione spazi bianchi start/end
	string Trim(const string &s) {
		auto start = s.begin();
		while(start != s.end() && isspace(*start)) ++start; // Cicla da start a end

		auto end = s.end();
		do { --end; } while (distance(start, end) > 0 && isspace(*end)); // Cicla da end a start

		// Ottieni: Input originale senza spazi bianchi superflui
		return string(start, end + 1);
	}

	// Separazione dei valori nella tupla
	vector<string> Split(const string &input) {
		string cleaned;
		for (char c : input) {
			if (c != '(' && c != ')') { // Rimozione parentesi
				cleaned += c;
			}
		}
		regex re(R"([\s,]+)"); // Espressione regolare: necessaria per combinazioni di virgole e parentesi
		sregex_token_iterator it(cleaned.begin(), cleaned.end(), re, -1), end; // Rimozione di virgole e parentesi per ottenere il solo valore in input
		vector<string> tokens;
		for (; it != end; ++it) { // Valori salvati in vettore tokens
			string token = Trim(*it); // Rimozione spazi bianchi start/end
			if (!token.empty()) tokens.push_back(token);
		}

		// Ottieni: Vettore con valori in input separati
		return tokens;
	}

	// Sanificazione contro input non numerico
	bool isNumeric(const string &s) {

		// Ottieni: False se elemento vuoto oppure contiene caratteri non numerici
		return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
	}

	// Funzione di input principale
	vector<int> Input() {
		string line;
		vector<int> values;
		cout << "Inserire i valori della tupla, separati da virgola e/o spazi bianchi\n----- Es. (p,q,b,c) *OPPURE* p q b c id1 id2 ------\n>>  ";
		getline(cin, line);
		auto tokens = Split(line); // Separazione dei valori nella tupla
		for (const auto &token : tokens) {
			if (!isNumeric(token)) { // Sanificazione contro input non numerico
				cerr << "ERRORE (valore invalido)\nIl seguente non è un numero intero positivo: " << token << endl;
				// In caso di un valore che presenta caratteri illegali, dare errore

				return {};
			}
			values.push_back(stoi(token));
		}
		if (values.size() != 4 && values.size() != 6) {
			cerr << "ERRORE (tupla invalida)\nTupla prevista di dimensione 4 oppure 6. Ottenuto: " << values.size() << endl;
			// In caso di una tupla troppo piccola o troppo grande, dare errore

			return {};
		}


		return values;
	}

	// Identificazione del Solido Platonico
	int Identify(int p, int q) {
	        switch (p) {
	                case 3:
	                        switch (q) {
	                                case 3: return 1; 	//Tetraedro
                                	case 4: return 3; 	//Ottaedro
	                                case 5: return 5; 	//Icosaedro
                                	default: return 0; 	//ERRORE
                        }
                	case 4:
	                        if (q == 3) return 2; 		//Cubo
		                else return 0; 			//ERRORE
                        case 5:
				if (q == 3) return 4;		//Dodecaedro
	                        else return 0;			//ERRORE
	                default: return 0;			//ERRORE
	        }
	}
}

