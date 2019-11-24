#include "controller.h"
#include <iostream>

int main(int argc, char** argv) {
	Controller controller(argc, argv);
	std::cout << controller.getReturnValue();
	return controller.getReturnValue();
}