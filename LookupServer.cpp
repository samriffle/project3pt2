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
		Verse verse;
		LookupResult lookupResult = OTHER;

		string out = "";
		stringstream ss;

		cout << "Now processing..." << endl;

		// Chose index to use for the request
		int bibleVersionNum = bibleVers;
		sendfifo.openwrite(); // Open write to client 
		if (bibleVersionNum == 2) { 
			// kjv
			verse = kjvBible.lookup(ref, lookupResult); 
			if (kjvBible.error(lookupResult) != "") { Verse noVerse; ss << noVerse.getRef().getBook() << "&" << noVerse.getRef().getChap() << "&" << noVerse.getRef().getVerse() << "&" << noVerse.getVerse() << "&" << lookupResult << "&"; out = ss.str(); sendfifo.send(out); }
			else {
				// send verse from server
				ss << verse.getRef().getBook() << "&" << verse.getRef().getChap() << "&" << verse.getRef().getVerse() << "&" << verse.getVerse() << "&" << lookupResult << "&";
				out = ss.str();
				log("First verse sent to client");
				ss.clear();
				if (nv > 1) {
					for (int i = 1; i < nv; i++) {
						Verse verseNext = kjvBible.nextVerse(lookupResult);
						// send following verses from server
						ss << verseNext.getRef().getBook() << "&" << verseNext.getRef().getChap() << "&" << verseNext.getRef().getVerse() << "&" << verseNext.getVerse() << "&" << lookupResult << "&";
						out = ss.str(); 
						log("Next Verse(s) sent to client");
						ss.clear();
						if (lookupResult != SUCCESS) {
							break;
						}
					}
				}
				sendfifo.send(out); // Send info as one long string to be split in Server
			}
		}
		else if (bibleVersionNum == 3) {
			// dby
			verse = dbyBible.lookup(ref, lookupResult); //NOW NOT WORKING HERE AFTER FIRST REQUEST
														//Pointer in index search may not have reset?
			if (dbyBible.error(lookupResult) != "") { Verse noVerse; ss << noVerse.getRef().getBook() << "&" << noVerse.getRef().getChap() << "&" << noVerse.getRef().getVerse() << "&" << noVerse.getVerse() << "&" << lookupResult << "&"; out = ss.str(); sendfifo.send(out); }
			else {
				// send verse from server
				ss << verse.getRef().getBook() << "&" << verse.getRef().getChap() << "&" << verse.getRef().getVerse() << "&" << verse.getVerse() << "&" << lookupResult << "&";
				out = ss.str();
				log("First verse sent to client");
				ss.clear();
				if (nv > 1) {
					for (int i = 1; i < nv; i++) {
						Verse verseNext = dbyBible.nextVerse(lookupResult);
						// send following verses from server
						ss << verseNext.getRef().getBook() << "&" << verseNext.getRef().getChap() << "&" << verseNext.getRef().getVerse() << "&" << verseNext.getVerse() << "&" << lookupResult << "&";
						out = ss.str();
						log("Next Verse(s) sent to client");
						ss.clear();
						if (lookupResult != SUCCESS) {
							break;
						}
					}
				}
				sendfifo.send(out); // Send info as one long string to be split in Server
			}
		}
		else if (bibleVersionNum == 4) {
			// ylt
			verse = yltBible.lookup(ref, lookupResult); //NOW NOT WORKING HERE AFTER FIRST REQUEST
														//Pointer in index search may not have reset?
			if (yltBible.error(lookupResult) != "") { Verse noVerse; ss << noVerse.getRef().getBook() << "&" << noVerse.getRef().getChap() << "&" << noVerse.getRef().getVerse() << "&" << noVerse.getVerse() << "&" << lookupResult << "&"; out = ss.str(); sendfifo.send(out); }
			else {
				// send verse from server
				ss << verse.getRef().getBook() << "&" << verse.getRef().getChap() << "&" << verse.getRef().getVerse() << "&" << verse.getVerse() << "&" << lookupResult << "&";
				out = ss.str();
				log("First verse sent to client");
				ss.clear();
				if (nv > 1) {
					for (int i = 1; i < nv; i++) {
						Verse verseNext = yltBible.nextVerse(lookupResult);
						// send following verses from server
						ss << verseNext.getRef().getBook() << "&" << verseNext.getRef().getChap() << "&" << verseNext.getRef().getVerse() << "&" << verseNext.getVerse() << "&" << lookupResult << "&";
						out = ss.str();
						log("Next Verse(s) sent to client");
						ss.clear();
						if (lookupResult != SUCCESS) {
							break;
						}
					}
				}
				sendfifo.send(out); // Send info as one long string to be split in Server
			}
		}
		else if (bibleVersionNum == 5) {
		// webster
		verse = websterBible.lookup(ref, lookupResult); //NOW NOT WORKING HERE AFTER FIRST REQUEST
													//Pointer in index search may not have reset?
		if (websterBible.error(lookupResult) != "") { Verse noVerse; ss << noVerse.getRef().getBook() << "&" << noVerse.getRef().getChap() << "&" << noVerse.getRef().getVerse() << "&" << noVerse.getVerse() << "&" << lookupResult << "&"; out = ss.str(); sendfifo.send(out); }
		else {
			// send verse from server
			ss << verse.getRef().getBook() << "&" << verse.getRef().getChap() << "&" << verse.getRef().getVerse() << "&" << verse.getVerse() << "&" << lookupResult << "&";
			out = ss.str();
			log("First verse sent to client");
			ss.clear();
			if (nv > 1) {
				for (int i = 1; i < nv; i++) {
					Verse verseNext = websterBible.nextVerse(lookupResult);
					// send following verses from server
					ss << verseNext.getRef().getBook() << "&" << verseNext.getRef().getChap() << "&" << verseNext.getRef().getVerse() << "&" << verseNext.getVerse() << "&" << lookupResult << "&";
					out = ss.str();
					log("Next Verse(s) sent to client");
					ss.clear();
					if (lookupResult != SUCCESS) {
						break;
					}
				}
			}
			sendfifo.send(out); // Send info as one long string to be split in Server
		}
		}
		else {
			// web
			verse = webBible.lookup(ref, lookupResult); //NOW NOT WORKING HERE AFTER FIRST REQUEST
														//Pointer in index search may not have reset?
			if (webBible.error(lookupResult) != "") { Verse noVerse; ss << noVerse.getRef().getBook() << "&" << noVerse.getRef().getChap() << "&" << noVerse.getRef().getVerse() << "&" << noVerse.getVerse() << "&" << lookupResult << "&"; out = ss.str(); sendfifo.send(out); }
			else {
				// send verse from server
				ss << verse.getRef().getBook() << "&" << verse.getRef().getChap() << "&" << verse.getRef().getVerse() << "&" << verse.getVerse() << "&" << lookupResult << "&";
				out = ss.str();
				log("First verse sent to client");
				ss.clear();
				if (nv > 1) {
					for (int i = 1; i < nv; i++) {
						Verse verseNext = webBible.nextVerse(lookupResult);
						// send following verses from server
						ss << verseNext.getRef().getBook() << "&" << verseNext.getRef().getChap() << "&" << verseNext.getRef().getVerse() << "&" << verseNext.getVerse() << "&" << lookupResult << "&";
						out = ss.str();
						log("Next Verse(s) sent to client");
						ss.clear();
						if (lookupResult != SUCCESS) {
							break;
						}
					}
				}
				sendfifo.send(out); // Send info as one long string to be split in Server
			}
		}
		sendfifo.fifoclose();  // Close send pipe
		log("close reply fifo");
		cout << "Process complete" << endl;
	}
	//recfifo.fifoclose();
	log("close request fifo");
}