//this is a test file do not use.

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "userinput.h"
using namespace CBCI;

int testfunc1(int argsc, std::string* argv) {
	std::cout << "Hello from test func 1!" << std::endl;
	return 1;
}

int testfunc2(int argsc, std::string* argv) {
	std::cout << "Hello from test func 2!" << std::endl;
	return 1;
}

int testfunc3(int argsc, std::string* argv) {
	std::cout << "Hello from test func 3!" << std::endl << "you passed the values: " << std::endl;
	for (int i = 0; i < argsc; i++) {
		std::cout << argv[i] << std::endl;
	}
	return 1;
}

void foo(int argc, std::string* argv) {
	for (int i = 0; i < argc;i++) {
		std::cout << argv[i] << ", \n";
	}
	return;
}

void bar(const int a = 3) {
	std::cout << a;
}

int main() {
	userinput mainLoop = userinput();
	mainLoop.addCommand(testfunc1, "func1");
	mainLoop.run();


}
