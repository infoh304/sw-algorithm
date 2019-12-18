/**
 *    Version 1.0
 * 
 **/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>    // std::max

#include "align_algorithm.cpp"
#include "get_residue.cpp"

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ios;
using std::vector;



typedef struct Score
{
	int offset;
	int _score;
	
	string _sequence;
} score;


int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "Le programme a besoin de deux arguments pour démarrer :"
			 << endl << "(1) Le fichier de la base de donnée, au format BLAST"
			 << endl << "(2) Le fichier de la protéine d'entrée, au format FASTA."
			 << endl;
		exit(1);
	}
	
	// Le fichier de la base de donnée doit exister au format BLAST
	// On vérifie que les fichiers .phr .pin et .psq existent

	ifstream f_phr(argv[1] + (string)".phr", ios::in | ios::binary);
	ifstream f_pin(argv[1] + (string)".pin", ios::in | ios::binary);
	ifstream f_psq(argv[1] + (string)".psq", ios::in | ios::binary);
	
	ifstream f_pro(argv[2]);
	
	bool files_exists = f_phr.is_open() &&
						f_pin.is_open() &&
						f_psq.is_open() &&
						f_pro.is_open();
	
	if (!files_exists)
	{
		cout << "Merci de renseigner des fichiers valides." << endl;
		exit(1);
	}


	// --- Lecture du fichier p.fasta ---
	
	// On isole la séquence contenue dans le fichier
	// à l'intérieur de la variable 'sequence' (string)
	
	string line;
	string sequence;

	while (std::getline(f_pro, line))
	{
		if (line.find(">") == string::npos)
			sequence += line;
	}
	
	// Affiche la séquence contenue dans p.fasta
	// cout << sequence << endl;
	
	// On calcule la taille du fichier f_psq
	int length_psq;
	
	f_psq.seekg (0, f_psq.end);
    length_psq = f_psq.tellg();
    f_psq.seekg (0, f_psq.beg);
	
	
	// On parcourt toutes les séquences du fichier f_psq
	// Pour chaque séquence, on associe un score de Smith
	char cara = 'a';
	string cmp_seq = "hello";
	
	// On retient le tout dans un tableau de structures
	vector<Score> tab_scores;
	
	for (int i = 0; i < length_psq; ++i)
	{
		f_psq.read((char*) &cara, sizeof(char));
		
		if (cara == 0) // Les séquences sont séparées par le caractère '\0'
		{
			// Quelle séquence est traitée :
			cout << "Séquence courante : " << cmp_seq << endl;
			
			int score_smith = alignment_method(cmp_seq, sequence);
			
			score s;
			s.offset = i+1;
			s._score = score_smith;
			
			tab_scores.push_back(s);
			cmp_seq = "";
		}
		else
		{
			// Ajoute un caractère en fin de chaîne
			cmp_seq.push_back(get_residue(cara));
		}
	}
	
	// --- Affichons tab_scores ---
	
	for (unsigned int i = 0; i < tab_scores.size(); ++i)
	{
		printf("Séquence analysée : %s\n", tab_scores[i]._sequence);
		printf("offset : %d, score : %d\n", tab_scores[i].offset, tab_scores[i]._score);
		printf("\n\n\n");
	}
	
	
	f_phr.close();
	f_pin.close();
	f_psq.close();
	f_pro.close();
	
	cout << "ok" << endl;
	
	return 0;
}