#include "pal.h"

int main(int argc, char** argv) {
	std::vector<std::string> arguments(argv + 1, argv + argc);

	pal::encode("test/ward_input", "test/ward_encoded.pal", Algorithm::repair);
//	pal::decode("test/ward_encoded.pal", "test/ward_output.bmp");

//	pal::encode("test/128mb.tar", "test/128mb.tar.pal", Algorithm::bisection);
//    pal::decode("tst/128mb.tar.pal", "test/new128mb.tar");
}