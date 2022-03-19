// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
	buildIndex(infile);
	bibleIter = index.begin();
}

// Constructor – pass bible filename
Bible::Bible(const string s) { 
	infile = s; 
	buildIndex(infile); 
	bibleIter = index.begin();
}

int Bible::buildIndex(string filename)
{
	bool debug = false;
	ifstream infile;     /* input file descriptor */
	int position;         /* location of line in the file */
	infile.open(filename.c_str(), ios::in);
	if (!infile) {
		cerr << "Error - can't open input file: " << filename << endl;
		return 1;  /* false, indicates error */
	}
	while (!infile.fail()) {
		/* Get the file position at beginning of line */
		position = infile.tellg();
		/* get the next line */
		string line;
		getline(infile, line);
		// make reference from the line, add to index
		Ref ref(line);
		index[ref] = position; // remember offset for current line
		if (ref.getBook() == 66 && ref.getChap() == 22 && ref.getVerse() == 21) { 
			break;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////// Project 3 pt1 diagnostics
	cout << "Refs made: " << index.size() << endl;
	auto iter = index.end();
	iter--;
	cout << "Last byte offset: " << iter->second << endl;
	iter = index.begin();
	if (debug) {
		cout << "Byte offset 1st ref: " << iter->second << endl;
		iter++;
		cout << "Byte offset 2nd ref: " << iter->second << endl;
		iter++;
		cout << "Byte offset 3rd ref: " << iter->second << endl;
		return 0;
	}
	return 0;
}

map<Ref, int>::iterator Bible::indexSearch(Ref ref)
{
	auto value = index.find(ref);
	auto iter = index.end();
	iter--;
	if (value->first < iter->first) {
		return value;
	}
	map<Ref, int>::iterator failSearch = index.begin();
	return failSearch;
}

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status) { 
	bibleIter = index.begin();
	bool errCheck = true;
	// Forego the pleasantries of error checking right now. Get that valid verse searched!
	instream.open(infile.c_str(), ios::in);
	string books[66] = { "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy", "Joshua", "Judges", "Ruth", "1 Samuel", "2 Samuel", "1 Kings", "2 Kings", "1 Chronicles", "2 Chronicles", "Ezra", "Nehimiah", "Esther", "Job", "Psalms", "Proverbs", "Ecclesiastes", "Song of Songs", "Isaiah", "Jeremiah", "Lamentations", "Ezekiel", "Daniel", "Hosea", "Joel", "Amos", "Obadiah", "Jonah", "Micha", "Nahum", "Habakkuk", "Zephaniah", "Haggai", "Zechariah", "Malachi", "Matthew", "Mark", "Luke", "John", "Acts", "Romans", "1 Corinthians", "2 Corinthians", "Galatians", "Ephesians", "Philippians", "Colossians", "1 Thessalonians", "2 Thessalonians", "1 Timothy", "2 Timothy", "Titus", "Philemon", "Hebrews", "James", "1 Peter", "2 Peter", "1 John", "2 John", "3 John", "Jude", "Revelation" };
	if (errCheck) {
		if (ref.getBook() < 1 || ref.getBook() > 66) {
			status = NO_BOOK;
			cout << error(status) << ref.getBook() << endl;
			Verse aVerse;
			return aVerse;
		}
		// Pull out variables out of the PARAMETER ref and create different 
		// refs to check validity of chapter and verse entries
		Ref compChap(ref.getBook(), ref.getChap(), 1); // May not work because chapter might not exist
		Ref compVerse(ref.getBook(), ref.getChap(), ref.getVerse());
		if (index.find(compChap) != index.end()) { // How to get this to work to check if it exists
			if (index.find(compVerse) != index.end()) {}
			else {
				status = NO_VERSE;
				cout << error(status) << ref.getVerse() << " in " << books[ref.getBook() - 1] << " " << ref.getChap() << endl;
				Verse aVerse;
				return aVerse;
			}
		}
		else {
			status = NO_CHAPTER;
			cout << error(status) << ref.getChap() << " in " << books[ref.getBook() - 1] << endl;
			Verse aVerse;
			return aVerse;
		}
	}
	// How would I error check verse? 
	// Perhaps use the og biblereader and get the whole book, 
	// check if chapter exists, then get the whole chapter and see if verse exists?
	bibleIter = indexSearch(ref); // ex)  bibleIter -> [Ref 1:1:2, int 62]
	int returnValue = bibleIter->second;
	instream.clear();
	instream.seekg(returnValue);
	string line = "";
	getline(instream, line);
	Verse pVerse(line);
	status = SUCCESS;
	return pVerse;
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status) {
	
	// Skipping error checking again for sake of making it work for correct case
	// Also how would I get the next ref out of the index without a way to access the iterator on the index that the lookup provides (Dont have ref to plug in)
	if (!instream) {
		instream.open(infile.c_str(), ios::in);
		string line = "";
		getline(instream, line);
		Verse pVerse(line);
		status = SUCCESS;
		return pVerse;
	}
	else {
		bibleIter++;
		auto iter = index.end();
		iter--;
		if (bibleIter->second == iter->second) {
			int returnValue = bibleIter->second;
			instream.seekg(returnValue, ios::beg); 
			string line = "";
			getline(instream, line);
			Verse pVerse(line);
			status = OTHER; 
			return pVerse;
		}
		else if (bibleIter->first < iter->first) { 
			int returnValue = bibleIter->second;
			instream.seekg(returnValue, ios::beg);
			string line = "";
			getline(instream, line);
			Verse pVerse(line);
			status = SUCCESS;
			return pVerse;
		}
		else {
			status = OTHER;
			Verse aVerse;
			return aVerse;
		}
	}
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status) {
	if (status == 1) {
		return "Error: no such book ";
	}
	else if (status == 2) {
		return "Error: no such chapter ";
	}
	else if (status == 3) {
		return "Error: no such verse ";
	}
	else {
		return "";
	}
}

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}
	
// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
Ref Bible::next(const Ref ref, LookupResult& status) {
	// have file advance to reference given, then return next reference after that
	auto value = indexSearch(ref); // Assume ref exists already
	value++;
	auto iter = index.end();
	iter--;
	if (value->first < iter->first) {
		status = SUCCESS;
		return value->first;
	}
	map<Ref, int>::iterator failSearch = index.begin();
	status = OTHER;
	return failSearch->first;
}

// OPTIONAL: Return the reference before the given ref
Ref Bible::prev(const Ref ref, LookupResult& status) {}

int Bible::indexSize()
{
	return index.size();
}
