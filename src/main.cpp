#include "pal.h"

int main(int argc, char** argv) {
	std::vector<std::string> arguments(argv + 1, argv + argc);

	pal::encode("test/photo.bmp", "test/photo.pal", Algorithm::bisection);
//	pal::decode("test/photo.pal", "test/new.bmp");
	//pal::decode("./test.pal", "./test-decompressed.txt");
//    pal::decode("photo.pal", "new.bmp");
}