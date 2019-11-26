#include "controller.h"
#include <iostream>
#include "pal/onlineHelper.h"

int main(int argc, char** argv)
{


    Controller controller(argc, argv);
	std::cout << controller.getReturnValue() << std::endl;
	return controller.getReturnValue();
}