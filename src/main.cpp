#include "pal.h"


int main(int argc, char** argv) {


    pal::encode("../test/shrek.bmp", "../test/shrek.pal", Algorithm::sequitur);
    //pal::encode("../test/beemovie.txt", "../test/beemovie.pal", Algorithm::sequitur);
	//pal::decode("./test/test.pal", "../test/test.txt");
    //pal::decode("tst/128mb.tar.pal", "test/new128mb.tar");
}