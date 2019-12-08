#include <iostream>
#include "vlad_trainer.cpp"


int main(int argc, char* argv[]) {
	/*VLAD kittens1("../sift/kittens1.hesaff.sift");
	VLAD kittens2("../sift/kittens2.hesaff.sift");

	kittens1.drawVLAD();
	kittens2.drawVLAD();
	waitKey(0);*/
	
	
	VLAD_trainer jason("list.txt", 16);
	getchar();

	return 0;
}