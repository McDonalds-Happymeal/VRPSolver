#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>

#include "threadFunctions.h"
#include "Solver/Problem.h"
#include "Solver/Solver.h"
#include "CBCI.h"
#include "Utils.h"

//GLOBAL VARS
std::string config = "config.cfg";
Problem problem(1, 1);
std::string ProblemSaveDir = "problems/";

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
    std::cout << "problem save called!" << std::endl;
    return 1;
}

int _ProblemLoad(int argc, std::string* argv) {
    std::cout << "problem load called!" << std::endl;
    return 1;
}

int _ProblemInfo(int argc, std::string* argv) {
    problem.printall();
    return 1;
}

int main(int argc, char** argv) {

    //creates main ui handeler object.
    CBCI::userinput ui("_>");
    ui.addCommand(_ProblemInfo, "ProblemInfo", "Will print out all availible data on currently loaded Vehicle Routing problem.");
    ui.addCommand(_ProblemGen, "ProblemGen", "Will generate a new Vehicle routing problem requires integer parameter for number of points and capacity.\nExample: ProblemGen 10 50\nrepresents a problem with 10 points and vehicle capacity of 50.");

    ui.run();
    
    std::shared_ptr<RenderData> renderData = std::make_shared<RenderData>();
    std::thread renderer(renderThread,renderData, std::ref(problem), 0.8f, 0, 10.0f);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    Solver solver(problem, renderData);
    solver.TSPNNSolver();
    renderer.join();

    return 0;

}