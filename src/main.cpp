#include "pal.h"

int main(int argc, char** argv) {
	std::vector<std::string> arguments(argv + 1, argv + argc);

	pal::encode("test/ward_input", "test/ward_encoded", Algorithm::bisection);
	pal::decode("test/ward_encoded", "test/ward_output");
	//pal::decode("./test.pal", "./test-decompressed.txt");
//    pal::decode("photo.pal", "new.bmp");
}