#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h> 
#include <sstream>
#include <map>

/*----------------------------------------------------------------
C++ Basic Command Interface - CBCI
Created by Peter Maltby 13-03-2021 
ver1.0

creates a class that takes function pointers of type:
"int (*)(int, string*)"
and creates a command loop supporting basic argument handeling
and a help default help function.

userInput public functions:
userinput(const char* );
cretaes object optional cursor input.

void addCommand(int(*)(int, std::string*), std::string, std::string);
adds command to current userinput with name and description.

void removeCommand(const char*);
removes command from user input using name.

void run();
runs command loop.

TODO / future features
regex for userinput
support additional fucntion pointers types.
build in parameter handeler localy???

-----------------------------------------------------------------*/
namespace CBCI {

	class option {
	private:
	public:
		option(int(*)(int, std::string*), std::string, std::string = "No Description availible for this command.");
		//option(int(*)(int, std::string*), std::string);
		~option();

		const std::string name;
		const std::string description;
		int(*func)(int, std::string*);
	};


	class userinput
	{
	private:

		const char* cursor;

		std::map<std::string, option*> funcMap;
		//std::vector<option*> commands;

		static int _help(int, std::string*);//command will never call.
		void help();
		static int exit(int, std::string*);
		const std::string exitdesc = "Exits current program loop.";;

	public:
		userinput(const char* = "_>");
		~userinput();

		void addCommand(int(*)(int, std::string*), std::string, std::string = "No description availible for this command.");

		//removes all options of type by wither option or alias.
		void removeCommand(const char*);

		void run();


	};

}