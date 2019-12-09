#include <windows.h>
#include <fstream>
#include <iterator>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;


void listdirandfiles(string dir, vector<string> &vec) {
	HANDLE hFind;
	WIN32_FIND_DATAA data;

	hFind = FindFirstFileA(dir.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			printf("%s\n", data.cFileName);
			vec.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}

int main(int argc, char** argv[]) {
	vector<string> vec;
	string dir = "C:\\Users\\kidultimate\\source\\repos\\CSS587\\VLAD\\holiday\\*";
	listdirandfiles(dir, vec);
	
	// Write to disk
	ofstream output_file("./list.txt");
	ostream_iterator<std::string> output_iterator(output_file, "\n");
	copy(vec.begin() += 2, vec.end(), output_iterator);

	getchar();
	return 0;
}