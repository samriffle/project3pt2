#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char** argv) {
	// Create Bible object to process the raw text file
	Bible webBible("/home/class/csc3004/Bibles/web-complete");

	Verse verse;
	int b, c, v, n;
	LookupResult result;

	if (argc == 1) {
		cout << "Error: everything is missing" << endl;
		return 34;
	}
	else if (argc == 2) {
		cout << "Error: chapter and verse number are missing" << endl;
		return 34;
	}
	else if (argc == 3) {
		cout << "Error: verse number is missing" << endl;
		return 34;
	}
	else if (argc > 5) {
		cerr << "Error: too many arguments" << endl;
		return 34;
	}
	b = stoi(argv[1]);
	c = stoi(argv[2]);
	v = stoi(argv[3]);
	n = 1;
	if (argc > 4) {
		n = stoi(argv[4]);
	}

	Ref ref(b, c, v);
	verse = webBible.lookup(ref, result);

	if (webBible.error(result) != "") {}
	else {
		verse.displayCL();
		if (argc > 4) {
			if (n > 0) {
				for (int i = 1; i < n; i++) {
					Verse verseNext = webBible.nextVerse(result);
					cout << endl;
					verseNext.displayCL();
					if (result != SUCCESS) {
						break;
					}
				}
			}
		}
		cout << endl;
	}
}