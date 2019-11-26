#include "controller.h"
#include <iostream>
#include "./pal.h"
int main(int argc, char** argv)
{
    pal::encode("../input/text.txt", "../output/text.txt", Algorithm::lzw);
	Controller controller(argc, argv);
	std::cout << controller.getReturnValue() << std::endl;
	return controller.getReturnValue();
}