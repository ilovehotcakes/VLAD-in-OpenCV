#include "vlad.cpp"
using namespace std;


int main(int argc, char* argv[]) {
	VLAD kittens1("../sift/kittens1.hesaff.sift");
	VLAD kittens2("../sift/kittens2.hesaff.sift");

	kittens1.drawVLAD();
	kittens2.drawVLAD();


	return 0;
}