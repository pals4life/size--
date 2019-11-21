#include "pal.h"

int main(int argc, char** argv) {
	pal::encode("test/shreks.bmp", "test/ward_encoded.pal", Algorithm::bisection);
//	pal::decode("test/ward_encoded.pal", "test/ward_output");
}