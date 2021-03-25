#include <iostream>
#include "CBCI.h"

#include <string>
using namespace CBCI;

int helloWorld(int argc, std::string* argv) {
	std::cout << "Hello World!" << std::endl;
	return 1;
}

int main() {
	userinput mainLoop = userinput();
	mainLoop.addCommand(helloWorld, "hello");
	mainLoop.run();
}