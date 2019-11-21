#include "pal.h"
#include <timer>

int main(int argc, char** argv) {
	{
		TimeFunction;
		std::vector<std::string> arguments(argv + 1, argv + argc);

		pal::encode("test/ward_input", "test/ward_encoded.pal", Algorithm::bisection);
	}
	pal::decode("test/ward_encoded.pal", "test/ward_output");
	system("diff test/ward_input test/ward_output");

//	pal::encode("test/128mb.tar", "test/128mb.tar.pal", Algorithm::bisection);
//    pal::decode("tst/128mb.tar.pal", "test/new128mb.tar");
}