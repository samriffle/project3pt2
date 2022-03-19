#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "logfile.h"
#include "fifo.h"
// #define logging // enable log file
// #define LOG_FILENAME "/tmp/samriffle-LookupServer.log" 
#include "logfile.h"
#include <stdio.h>
#include <string.h>
#include <sstream>
using namespace std;
// Using namespace bible
// #define logging // enable log file

int main() {
	string receive_pipe = "request";
	string send_pipe = "reply";
	Fifo recfifo(receive_pipe);
	Fifo sendfifo(send_pipe);
	#ifdef logging
		logFile.open(logFilename.c_str(), ios::out);
	#endif

	// build indices
	cout << "" << endl;
	Bible kjvBible("/home/class/csc3004/Bibles/kjv-complete");
	Bible dbyBible("/home/class/csc3004/Bibles/dby-complete");
	Bible yltBible("/home/class/csc3004/Bibles/ylt-complete");
	Bible websterBible("/home/class/csc3004/Bibles/webster-complete");
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
	cout << "Indices built" << endl;
	

	recfifo.openread();
	log("Open request fifo");
	while (1) { 
		// Process request
		string results = "";
		string tokenValue[5];
		cout << "" << endl;
		cout << "Waiting for request..." << endl;
		results = recfifo.recv();
		log(results);
		cout << "Info recieved by server: " << results;
		cout << endl;
		tokenValue[0] = GetNextToken(results, "&"); // Book
		tokenValue[1] = GetNextToken(results, "&"); // Chap
		tokenValue[2] = GetNextToken(results, "&"); // Verse
		tokenValue[3] = GetNextToken(results, "&"); // Number of verses
		tokenValue[4] = GetNextToken(results, "&"); // Bible Version

		// Parse string into usable variables for request
		int b = stoi(tokenValue[0]);
		int c = stoi(tokenValue[1]);
		int v = stoi(tokenValue[2]);
		int nv = stoi(tokenValue[3]);
		int bibleVers = stoi(tokenValue[4]);
		cout << "Bible version: " << bibleVers << endl;
		
		Ref ref(b, c, v);
		Verse lVerse;
		LookupResult result = OTHER;

		cout << "Now processing..." << endl;

		// Chose index to use for the request
		int bibleVersionNum = bibleVers; 
		cout << bibleVersionNum << endl;
		Bible* bible;
		if (bibleVersionNum == 1) {
			bible = &webBible;
		}
		else if (bibleVersionNum == 2) {
			bible = &kjvBible;
		}
		else if (bibleVersionNum == 3) {
			bible = &dbyBible;
		}
		else if (bibleVersionNum == 4) {
			bible = &yltBible;
		}
		else if (bibleVersionNum == 5) {
			bible = &websterBible;
		}
		else {
			return 0;
		}
		sendfifo.openwrite(); // Open write to client 

		// Return Verse(s)
		lVerse = bible->lookup(ref, result);
		if (result != SUCCESS) { Verse noVerse; stringstream ss; ss << noVerse.getRef().getBook() << "&" << noVerse.getRef().getChap() << "&" << noVerse.getRef().getVerse() << "&" << noVerse.getVerse() << "&" << result << "&"; string out = ss.str(); sendfifo.send(out); }
		else { // successful search
			int numVerses = nv;
			if (numVerses <= 0) { Verse noVerse; stringstream ss; ss << noVerse.getRef().getBook() << "&" << noVerse.getRef().getChap() << "&" << noVerse.getRef().getVerse() << "&" << noVerse.getVerse() << "&" << result << "&"; string out = ss.str(); sendfifo.send(out); }
			else {
				if (result != SUCCESS) { Verse noVerse; stringstream ss; ss << noVerse.getRef().getBook() << "&" << noVerse.getRef().getChap() << "&" << noVerse.getRef().getVerse() << "&" << noVerse.getVerse() << "&" << result << "&"; string out = ss.str(); sendfifo.send(out); }
				else {
					string out = "";
					stringstream ss;
					ss << lVerse.getRef().getBook() << "&" << lVerse.getRef().getChap() << "&" << lVerse.getRef().getVerse() << "&" << lVerse.getVerse() << "&" << result << "&";
					out = ss.str();
					sendfifo.send(out);
					ss.flush();
					ss.clear();
					if (numVerses > 0 && numVerses != NULL) {
						for (int i = 1; i < numVerses; i++) { 
							string out2 = "";
							stringstream ss2;
							Verse verseNext = bible->nextVerse(result);
							ss2 << verseNext.getRef().getBook() << "&" << verseNext.getRef().getChap() << "&" << verseNext.getRef().getVerse() << "&" << verseNext.getVerse() << "&" << result << "&";
							out2 = ss2.str();
							sendfifo.send(out2);
							ss2.flush();
							ss2.clear();
							if (result != SUCCESS) {
								break;
							}
						}
					}
				}
			}
		}
		sendfifo.fifoclose();  // Close send pipe
		log("close reply fifo");
		cout << "Process complete" << endl;
	}
	//recfifo.fifoclose();
	//log("close request fifo");
}