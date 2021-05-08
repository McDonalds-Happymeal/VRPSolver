#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>

#include "Visualisation/renderThreadFunc.h"
#include "Solver/Problem.h"
#include "Solver/Solver.h"
#include "CBCI.h"
#include "Utils.h"


//GLOBAL VARS
Problem problem(50, 1);
std::string ProblemSaveDir = "problems/";

/*----------------User interface classes and handelers!----------------------------------------*/
int _ProblemGen(int argc, std::string* argv) {
    if (argc < 3) {
        std::cout << "Command requires 2 additional parameters, type help for additional information!" << std::endl;
        return 1;
    }
    if (!is_number(argv[1])) {
        std::cout << "Parameter 1 is not convertable to type int, type help for additional information!" << std::endl;
        return 1;
    }
    if (!is_number(argv[2])) {
        std::cout << "Parameter 2 is not convertable to type int, type help for additional information!" << std::endl;
        return 1;
    }

    problem = Problem(stoi(argv[1]),stoi(argv[2]));
    std::cout << "New Problem Generated!" << std::endl;
    return 1;
}

int _ProblemSave(int argc, std::string* argv) {
    if (argc < 2) {
        std::cout << "Command requires 1 additional parameters, type help for additional information!" << std::endl;
        return 1;
    }
    std::string file = ProblemSaveDir + argv[1] + ".vrp";
    problem.saveProblem(file);
    return 1;
}

int _ProblemLoad(int argc, std::string* argv) {
    if (argc < 2) {
        std::cout << "Command requires 1 additional parameters, type help for additional information!" << std::endl;
        return 1;
    }
    std::string file = ProblemSaveDir + argv[1] + ".vrp";

    problem.loadProblem(file);

    return 1;
}

int _ProblemInfo(int argc, std::string* argv) {
    problem.printall();
    return 1;
}



int main(int argc, char** argv) {

    //creates main ui handler object with cursor type "_>"
    CBCI::userinput ui("_>");
    ui.addCommand(_ProblemInfo, "ProblemInfo", "Will print out all availible data on currently loaded Vehicle Routing problem.");
    ui.addCommand(_ProblemGen, "ProblemGen", "Will generate a new Vehicle routing problem, requires integer parameter for number of points and capacity.\nExample: ProblemGen 10 50\nrepresents a problem with 10 points and vehicle capacity of 50.");
    ui.addCommand(_ProblemSave, "ProblemSave", "Will save currently loaded problem, requires string parameter for file name.\nExample: ProblemGen problem1");
    ui.addCommand(_ProblemLoad, "ProblemLoad", "Will load selected .vrp file, requires string parameter for file name.\nExample: ProblemGen problem1");

    //run created userinterface
    ui.run();
    
    std::shared_ptr<RenderData> renderData = std::make_shared<RenderData>();
    std::thread renderer(renderThread,renderData, std::ref(problem), 0.8f, 0, 10.0f);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    Solver solver(problem, renderData);
    solver.TSPNNSolver();
    std::cout << "please close visualiser window to continue." << std::endl;
    renderer.join();

    return 0;

}