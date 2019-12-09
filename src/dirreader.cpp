// This program outputs files in the specified directory to a .txt file
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
using namespace std;


void usage()
{
    cout << "directory  filename" << endl;
}

void read_directory(string name, vector<string> &vec)
{
    DIR* dir = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dir)) != NULL) {
        vec.push_back(dp->d_name);
    }
    closedir(dir);
}


int main(int argc, char* argv[]) {
    if (argc < 4)
        usage();

	vector<string> vec;
	read_directory(argv[1], vec);

	
	ofstream output_file(argv[2]);
    ostream_iterator<string> output_iterator(output_file, "\n");
    copy(vec.begin() + 2, vec.end(), output_iterator);
	cout << "There are " << vec.size() - 2 << " files in this dir." << endl;

	return 0;
}