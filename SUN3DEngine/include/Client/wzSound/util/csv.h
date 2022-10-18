#ifndef __CSV_H__
#define __CSV_H__


/* Copyright (C) 1999 Lucent Technologies */
/* Excerpted from 'The Practice of Programming' */
/* by Brian W. Kernighan and Rob Pike */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class Csv {	// read and parse comma-separated values
	// sample input: "LU",86.25,"11/4/1998","2:19PM",+4.0625
	
public:
//	Csv(istream& fin = cin, string sep = ",") : 
//	Csv(fstream& fin = fstream("packet.txt", ios::in), string sep = ",") :
     // fin(fin), fieldsep(sep) {}
	Csv(fstream& _fin) : fin(_fin)
	{
	//	fin = fstream("C:\PRJ\WinsockTesterGDI\bin\packet.txt", ios::in);
		fieldsep = ",";
	}
	~Csv();
      
      int getline(string&);
      string getfield(int n);
      int getnfield() const { return nfield; }
      
private:
//	istream& fin;			// input file pointer
	fstream& fin;
	string line;			// input line
	vector<string> field;	// field strings
	int nfield;				// number of fields
	string fieldsep;		// separator characters
	
	int split();
	int endofline(char);
	int advplain(const string& line, string& fld, int);
	int advquoted(const string& line, string& fld, int);
};


#endif //__CSV_H__