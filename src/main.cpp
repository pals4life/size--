#include "pal.h"


int main(int argc, char** argv) {


	pal::encode("./test.txt", "../test/test.pal", Algorithm::sequitur);
	pal::decode("./test/test.pal", "../test/test.txt");
// pal::decode("tst/128mb.tar.pal", "test/new128mb.tar");
}