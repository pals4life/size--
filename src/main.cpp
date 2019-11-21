#include "pal.h"

int main(int argc, char** argv) {
	pal::encode("test/shrek.bmp", "test/ward_encoded.pal", Algorithm::bisection);
	pal::decode("test/ward_encoded.pal", "test/ward_output");
}