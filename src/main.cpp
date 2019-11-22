#include "pal.h"

int main(int argc, char** argv) {
	const std::string file = "test/photo.bmp";
	pal::encode(file, "test/a.pal", Algorithm::optimisedBisection);
	pal::encode(file, "test/b.pal", Algorithm::bisection);
//	pal::decode("test/ward_encoded.pal", "test/ward_output");
}