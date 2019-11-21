#include "pal.h"


int main(int argc, char** argv) {
	pal::encode("test/photo.bmp", "test/ward_encoded.pal", Algorithm::bisection);
	pal::decode("test/ward_encoded.pal", "test/ward_output");
	
	system("colordiff test/photo.bmp test/ward_output");
}