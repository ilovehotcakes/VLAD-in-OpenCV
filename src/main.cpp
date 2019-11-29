#include "vlad.cpp"
using namespace std;


int main(int argc, char* argv[]) {
	imread("pippy.jpg", input);
	VLAD vlad = (input, 16, 128);	


	return 0;
}