/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * Mount Vernon Nazarene University
 *
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 *     refers to the actual string entered in the form's "verse" field.
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Bible.h"
#include "Verse.h"
#include "Ref.h"
#define logging // enable log file
// #define LOG_FILENAME "/tmp/samriffle-LookupClient.log" 
#include "logfile.h"
#include "fifo.h"
#include <stdio.h>
#include <sstream>
using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
using namespace cgicc;


int main() {
	/* A CGI program must send a response header with content type
	 * back to the web client before any other output.
	 * For an AJAX request, our response is not a complete HTML document,
	 * so the response type is just plain text to insert into the web page.
	 */
	cout << "Content-Type: text/plain\n\n";

	Cgicc cgi;  // create object used to access CGI request data

	#ifdef logging
		logFile.open(logFilename.c_str(), ios::out);
	#endif

	log("Request recieved" << '\n');

	// GET THE INPUT DATA
	// browser sends us a string of field name/value pairs from HTML form
	// retrieve the value for each appropriate field name
	form_iterator st = cgi.getElement("search_type");
	form_iterator book = cgi.getElement("book");
	form_iterator chapter = cgi.getElement("chapter");
	form_iterator verse = cgi.getElement("verse");
	form_iterator nv = cgi.getElement("num_verse");
	form_iterator bibleVersion = cgi.getElement("bible");
	// cout << "<p>" << "Info retrieved from CGI object" << "<p>" << endl;

	// Convert and check input data
	bool validInput = false;
	if (book != cgi.getElements().end()) {
		int bookNum = book->getIntegerValue();
		if (bookNum > 66) {
			cout << "<p>The book number (" << bookNum << ") is too high.</p>" << endl;
		}
		else if (bookNum <= 0) {
			cout << "<p>The book must be a positive number.</p>" << endl;
		}
		else {
			validInput = true;
		}
	}

	/* TO DO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you! */
	validInput = false;
	if (chapter != cgi.getElements().end()) {
		int chapterNum = chapter->getIntegerValue();
		if (chapterNum > 150) {
			cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
		}
		else if (chapterNum <= 0) {
			cout << "<p>The chapter must be a positive number.</p>" << endl;
		}
		else {
			validInput = true;
		}
	}

	validInput = false;
	if (verse != cgi.getElements().end()) {
		int verseNum = verse->getIntegerValue();
		if (verseNum > 500) {
			cout << "<p>The verse number (" << verseNum << ") is too high.</p>" << endl;
		}
		else if (verseNum <= 0) {
			cout << "<p>The verse must be a positive number.</p>" << endl;
		}
		else {
			validInput = true;
		}
	}

	validInput = false;
	if (nv != cgi.getElements().end()) {
		int verses = nv->getIntegerValue();
		if (verses > 300) {
			cout << "<p>The number of requests (" << verses << ") is very high.</p>" << endl;
			validInput = true;
		}
		else if (verses <= 0) {
			cout << "<p>The amount of verses must be a positive number.</p>" << endl;
		}
		else {
			validInput = true;
		}
	}
	/* TO DO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
	 *        TO LOOK UP THE REQUESTED VERSES
	 */

	int verseNum = verse->getIntegerValue();
	int bookNum = book->getIntegerValue();
	int chapterNum = chapter->getIntegerValue();
	int verses = nv->getIntegerValue();
	int bibleVersionNum = bibleVersion->getIntegerValue();
	Ref ref(bookNum, chapterNum, verseNum);
	LookupResult result;
	
	/////////////////////// P3pt2 Workspace -- MOVE THIS ALL TO A NEW .cpp CALLED LookupClient.cpp !!!!! (It'll be a relative copy of this entire class sans the bottom
	string receive_pipe = "reply";
	string send_pipe = "request";
	Fifo recfifo(receive_pipe);
	Fifo sendfifo(send_pipe);
	stringstream ss;
	stringstream ss1;
	string out = "";

	// Call server to get results  
	ss << ref.getBook() << "&" << ref.getChap() << "&" << ref.getVerse() << "&" << verses << "&" << bibleVersionNum << "&"; // to_string on LookupResult probably janking out
	out = ss.str();
	sendfifo.openwrite();
	sendfifo.send(out); // Send info as one long string to be split in Server
	log("Request sent to server: " << ss.str() << '\n');
	ss.flush();
	ss.clear();
	cout << endl;
	sendfifo.fifoclose();
	recfifo.openread();
	string results = "";
	results = recfifo.recv(); // update to go outside the loop BUT it DO BE unoptimized. Consider server sending seperate message for each
	// recfifo.fifoclose();
	log("Results from server recieved: " << results << '\n');

	// Do first lookup with constVerse here
	string tokenValue[5];
	tokenValue[0] = GetNextToken(results, "&"); // Book
	tokenValue[1] = GetNextToken(results, "&"); // Chap
	tokenValue[2] = GetNextToken(results, "&"); // Verse
	tokenValue[3] = GetNextToken(results, "&"); // Verse Text
	tokenValue[4] = GetNextToken(results, "&"); // LookupResult
	ss1 << tokenValue[0] << ":" << tokenValue[1] << ":" + tokenValue[2] << " " << tokenValue[3];
	string lookResult = tokenValue[4]; 
	Verse constVerse(ss1.str());
	ss1.flush();
	ss1.clear();

	// Print first verse, then loop for more
	if (lookResult != "0") { 
		string books[66] = { "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy", "Joshua", "Judges", "Ruth", "1 Samuel", "2 Samuel", "1 Kings", "2 Kings", "1 Chronicles", "2 Chronicles", "Ezra", "Nehimiah", "Esther", "Job", "Psalms", "Proverbs", "Ecclesiastes", "Song of Songs", "Isaiah", "Jeremiah", "Lamentations", "Ezekiel", "Daniel", "Hosea", "Joel", "Amos", "Obadiah", "Jonah", "Micha", "Nahum", "Habakkuk", "Zephaniah", "Haggai", "Zechariah", "Malachi", "Matthew", "Mark", "Luke", "John", "Acts", "Romans", "1 Corinthians", "2 Corinthians", "Galatians", "Ephesians", "Philippians", "Colossians", "1 Thessalonians", "2 Thessalonians", "1 Timothy", "2 Timothy", "Titus", "Philemon", "Hebrews", "James", "1 Peter", "2 Peter", "1 John", "2 John", "3 John", "Jude", "Revelation" };
		int errNum = stoi(lookResult);

		if (errNum == 1) {
			cout << "<p>" << "Error: no such book " << ref.getBook() << "<p>" << endl;
		}
		else if (errNum == 2) {
			cout << "<p>" << "Error: no such chapter " << ref.getChap() << " in " << books[ref.getBook() - 1] << "<p>" << endl;
		}
		else if (errNum == 3) {
			cout << "<p>" << "Error: no such verse " << ref.getVerse() << " in " << books[ref.getBook() - 1] << " " << ref.getChap() << "<p>" << endl;
		}
		else {
			cout << "<p>" << "Verse search unsuccessful" << "<p>" << endl;
		}
	}
	else { // successful search
		if (verses <= 0) { cout << "<p>" << "No amount of verses to search" << "<p>" << endl; }
		else {
			constVerse.display(verses);
			if (verses > 0 && verses != NULL) {
				Verse preV = constVerse; // Helps check if next verse is rolled over into another chapter
				for (int i = 1; i < verses; i++) { // Loop to print any verses after first
					stringstream ss2; // Desparation because ss wont release its current contents 
					  // ex) 4&3&2&1&1&4:3:2 These are the names of the sons of Aaron: Nadab the firstborn, and Abihu, Eleazar, and Ithamar.
					  // 4&3&2&1&1& is the string I sent to the server. The rest is to create the verse object later in this client
					results = recfifo.recv(); // update to go outside the loop BUT it DO BE unoptimized. Consider server sending seperate message for each
					// recfifo.fifoclose();
					log("Results from server recieved: " << results << '\n');
					string rptValue[5];
					rptValue[0] = GetNextToken(results, "&"); // Book
					rptValue[1] = GetNextToken(results, "&"); // Chap
					rptValue[2] = GetNextToken(results, "&"); // Verse
					rptValue[3] = GetNextToken(results, "&"); // Verse Text
					rptValue[4] = GetNextToken(results, "&"); // LookupResult
					ss2 << rptValue[0] << ":" << rptValue[1] << ":" + rptValue[2] << " " << rptValue[3];
					string loopLookResult = "";
					loopLookResult = rptValue[4];
					Verse verseNext(ss2.str());
					ss2.flush();
					ss2.clear();
					cout << endl;
					if (loopLookResult != "0") { // Change to use result nextVerse returns (status code)
						break;
					}
					else { // Cons for splitting up books and chapters
						if (verseNext.getRef().getChap() == preV.getRef().getChap() + 1) {
							verseNext.display(verses - i);
						}
						else if (verseNext.getRef().getBook() == preV.getRef().getBook() + 1) {
							verseNext.display(verses - i);
						}
						else {
							verseNext.displaySans();
						}
					}
					preV = verseNext;
				}
			}
		}
		cout << endl;
	}
	recfifo.fifoclose();
}
