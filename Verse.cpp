// Verse class function definitions
// Computer Science, MVNU

#include "Verse.h"
#include <iostream>
using namespace std;

Verse::Verse() {  	// Default constructor
    verseText = "Uninitialized Verse!";
    verseRef = Ref();
} 

// REQUIRED: Parse constructor - pass verse string from file
Verse::Verse(const string s) {
	// use Ref constructor to create verseRef
	// store the remainder of the string in verseText

    string rtext = s; // make local copy of string to avoid modifying parameter //// Verse parser
    // parse the reference - notice, currently there is no error checking!
    // Get book number
    string strbook = GetNextToken(rtext, ":");
    int bookV = atoi(strbook.c_str());
    // Get the chapter number
    string strchap = GetNextToken(rtext, ":");
    int chapV = atoi(strchap.c_str());
    // Get the verse number
    string strverse = GetNextToken(rtext, " ");
    int verseV = atoi(strverse.c_str());                                        //// End of Verse Parser
    
    verseRef = Ref(bookV, chapV, verseV);
    verseText = rtext;
}  	

// REQUIRED: Accessors
string Verse::getVerse() {
    return verseText;
}

Ref Verse::getRef() {
    return verseRef;
}

// display reference and verse
void Verse::display(const int vNum) {
    verseRef.display(vNum);
    cout << " " << "<sub>" << getRef().getVerse() << "</sub>" << verseText;
 }

////////////////////////////////////////////////////////// New for project 2 formatting
// display only the verse
void Verse::displaySans() {
    cout << " " << "<sub>" << getRef().getVerse() << "</sub>" << verseText;
}

void Verse::displayCL() {
    string books[66] = { "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy", "Joshua", "Judges", "Ruth", "1 Samuel", "2 Samuel", "1 Kings", "2 Kings", "1 Chronicles", "2 Chronicles", "Ezra", "Nehimiah", "Esther", "Job", "Psalms", "Proverbs", "Ecclesiastes", "Song of Songs", "Isaiah", "Jeremiah", "Lamentations", "Ezekiel", "Daniel", "Hosea", "Joel", "Amos", "Obadiah", "Jonah", "Micha", "Nahum", "Habakkuk", "Zephaniah", "Haggai", "Zechariah", "Malachi", "Matthew", "Mark", "Luke", "John", "Acts", "Romans", "1 Corinthians", "2 Corinthians", "Galatians", "Ephesians", "Philippians", "Colossians", "1 Thessalonians", "2 Thessalonians", "1 Timothy", "2 Timothy", "Titus", "Philemon", "Hebrews", "James", "1 Peter", "2 Peter", "1 John", "2 John", "3 John", "Jude", "Revelation" };
    cout << books[getRef().getBook() - 1] << " " << getRef().getChap() << ":" << getRef().getVerse();
    cout << " " << verseText;
}
