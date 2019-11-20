#include "pal.h"

int main(int argc, char** argv) {
	std::vector<std::string> arguments(argv + 1, argv + argc);

	pal::encode("test/photo.bmp", "test/photo.pal", Algorithm::bisection);
	pal::decode("test/photo.pal", "test/new.bmp");

	pal::encode("test/128mb.tar", "test/128mb.tar.pal", Algorithm::bisection);
//    pal::decode("tst/128mb.tar.pal", "test/new128mb.tar");
}