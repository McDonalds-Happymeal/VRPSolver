#include "CBCI.h"

namespace CBCI {
	//OPTION CLASS DEFINITIONS HERE------------------------------------------------------------

	option::option(int(*func)(int, std::string*), std::string _name, std::string _description) : description(_description), name(_name) {
		this->func = func;
	}

	option::~option() {
		std::cout << "option : " << this->name << " deallocated";// to ensure memory freed.
	}


	//USERINPUT CLASS DEFINITIONS HERE ----------------------------------------------

	userinput::userinput(const char* _cursor) : cursor(_cursor) {
		addCommand(exit, "exit", exitdesc);
		addCommand(_help, "help", "List all currently availible commands.");
	}


	userinput::~userinput()
	{
		for (auto& opt : funcMap) {//clears option from heap.
			delete opt.second;
		}
	}

	//creates option object on heap and adds to func map.
	void userinput::addCommand(int(*func)(int, std::string*), std::string name, std::string description)
	{
		for (auto const& pair : funcMap) {
			if (pair.first == name) {
				std::cout << "Adding new command failed, command name : " << name << " already in use!";
				return;
			}
		}
		funcMap[name] = new option(func, name, description);
	}

	//check if alias in use, then clears memeory and removes from map if present.
	void userinput::removeCommand(const char* alias) {
		if (funcMap.count(alias)) {
			delete funcMap[alias];
			funcMap.erase(alias);
		}
	}

	void userinput::run() {
		int status = 1;//status of 0 will exit loop.

		do {

			//takes input from cin and outputs tokened strings (argv) and count (argc) of tokens.
			std::string input;
			std::cout << cursor;
			std::getline(std::cin, input);
			std::stringstream ss(input);
			int argc = 0;
			std::string tmp;
			std::vector<std::string> argv;
			while (std::getline(ss, tmp, ' ')) {
				argc++;
				argv.push_back(tmp);
			}
			if (argc) {
				if (argv[0] == "help") {//special case handeler for help command.
					help();
				}
				if (funcMap.count(argv[0]) == 1) {
					status = funcMap[argv[0]]->func(argc, &argv[0]);
				}
				else {
					std::cout << "Command \"" << argv[0] << "\" is not recognised, use the \"help\" command to see availible commands.\n";
				}
			}
		} while (status);

	}

	//DEFAULT OPTIONS FUNCTIONS BELOW--------------------------

	void userinput::help() {
		for (auto const& pair : funcMap) {
			std::cout << pair.second->name << " - " << pair.second->description << "\n\n";
		}
	}


	int userinput::_help(int argc, std::string* argv) {//does nothing but may be overwritten to extend help functionality.
		return 1;
	}

	int userinput::exit(int argc, std::string* argv) {
		std::cout << "exiting..." << std::endl;
		return 0;
	}
}