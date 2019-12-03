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
    // Todo
}

void read_directory(string name, vector<string> &v)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        v.push_back(dp->d_name);
    }
    closedir(dirp);
}


int main(int argc, char* argv[]) {
	vector<string> v;
	read_directory(argv[1], v);

	// Todo: push_front size
	ofstream output_file("./list.txt");
    ostream_iterator<string> output_iterator(output_file, "\n");
    copy(v.begin() + 2, v.end(), output_iterator);
	cout << "There are " << v.size() - 2 << " files in this dir." << endl;

	
	return 0;
}