# Bibleajax program
# Bob Kasper, MVNU Computer Science
# updated January 2020 to use
# c++11 compiler option, current paths on cs.mvnu.edu

# This is a Makefile for the Bible web app demo program.
# Copy this directory to a location within your home directory. 
# Change the USER name value below to your own user name.
# Then use "make" to build the server program,
# and deploy it to the live web server directory.
# To test the program, go to http://cs.mvnu.edu/class/csc3004/USER/
# and open the bibleajax.html link.

# TO DO: Must replace "username" by your username
USER= samriffle

# Use GNU C++ compiler with C++11 standard
CC= g++
CFLAGS= -g -std=c++11

# Probably gonna mess up with the cgi uncommented from Proj-2 version and edited for Proj-3
# Added the non . files in myself
all:	LookupServer LookupClient LookupClient.cgi PutCGI PutHTML

LookupClient.cgi:	LookupClient.o Ref.o Verse.o Bible.o fifo.o
		$(CC) $(CFLAGS) -o LookupClient.cgi LookupClient.o Ref.o Verse.o Bible.o fifo.o -L/usr/local/lib -lcgicc
		# -l option is necessary to link with cgicc library

# P3pt2 additions
fifo.o: fifo.cpp fifo.h
	$(CC) $(CFLAGS) -c fifo.cpp

LookupClient.o: LookupClient.cpp Bible.o Verse.o Ref.o fifo.h logfile.h
	$(CC) $(CFLAGS) -c LookupClient.cpp 
	# Bible.o Verse.o Ref.o fifo.o logfile.h

LookupServer.o: LookupServer.cpp fifo.h 
	$(CC) $(CFLAGS) -c LookupServer.cpp 

# Build the executables

LookupClient: LookupClient.o fifo.o Bible.o Ref.o Verse.o
	$(CC) $(CFLAGS) -o LookupClient LookupClient.o fifo.o Bible.o Ref.o Verse.o -L/usr/local/lib -lcgicc

LookupServer: LookupServer.o fifo.o Bible.o Ref.o Verse.o
	$(CC) $(CFLAGS) -o LookupServer LookupServer.o fifo.o Bible.o Ref.o Verse.o

PutCGI:	LookupClient.cgi
		chmod 755 LookupClient.cgi
		cp LookupClient.cgi /var/www/html/class/csc3004/$(USER)/cgi-bin

		echo "Current contents of your cgi-bin directory: "
		ls -l /var/www/html/class/csc3004/$(USER)/cgi-bin/

PutHTML:
		cp bibleindex.html /var/www/html/class/csc3004/$(USER)

		echo "Current contents of your HTML directory: "
		ls -l /var/www/html/class/csc3004/$(USER)

clean:		
		rm *.o core LookupClient.cgi
