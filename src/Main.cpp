#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>

#include "Visualisation/renderThreadFunc.h"
#include "Solver/Problem.h"
//#include "Solver/Solver.h"
#include "Solver/TspNN.h"
#include "Solver/VRP_Dantzig_Solver.h"
#include "CBCI.h"
#include "Utils.h"


//GLOBAL VARS
Problem problem(250, 50);
std::string ProblemSaveDir = "problems/";

/*----------------User interface classes and handelers!----------------------------------------*/
int _ProblemGen(int argc, std::string* argv) {
    if (argc < 3) {
        std::cout << "Command requires 2 additional parameters, type help for additional information!" << std::endl;
        return 1;
    }
    if (!is_number(argv[1])) {
        std::cout << "Parameter 1 is not convertible to type int, type help for additional information!" << std::endl;
        return 1;
    }
    if (!is_number(argv[2])) {
        std::cout << "Parameter 2 is not convertible to type int, type help for additional information!" << std::endl;
        return 1;
    }
    if (stoi(argv[1])<1) {
        std::cout << "Parameter 1 must be over a value of 1!" << std::endl;
        return 1;
    }
    if (stoi(argv[2]) < 1) {
        std::cout << "Parameter 2 must be over a value of 1!" << std::endl;
        return 1;
    }

    problem = Problem(stoi(argv[1]),stoi(argv[2]));
    std::cout << "New Problem Generated!" << std::endl;
    return 1;
}

int _ProblemSave(int argc, std::string* argv) {
    if (argc < 2) {
        std::cout << "Command requires 1 additional parameter, type help for additional information!" << std::endl;
        return 1;
    }
    std::string file = ProblemSaveDir + argv[1] + ".vrp";
    problem.saveProblem(file);
    return 1;
}

int _ProblemLoad(int argc, std::string* argv) {
    if (argc < 2) {
        std::cout << "Command requires 1 additional parameter, type help for additional information!" << std::endl;
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

int _tspNN(int argc, std::string* argv) {

    std::shared_ptr<RenderData> renderData = std::make_shared<RenderData>();
    std::thread renderer(renderThread, renderData, std::ref(problem), 0.8f, 0, 10.0f);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    auto startTime = std::chrono::high_resolution_clock::now();
    TspNN solver(problem, renderData);
    solver.run();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Algorithm completed operation in : " << std::to_string(execution_time.count()) << " milliseconds" << std::endl;
    std::cout << "please close the visualiser window to continue." << std::endl;
    renderer.join();
    
    return 1;
}

int _vrpDR(int argc, std::string* argv) {

    std::shared_ptr<RenderData> renderData = std::make_shared<RenderData>();
    std::thread renderer(renderThread, renderData, std::ref(problem), 0.8f, 0, 10.0f);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto startTime = std::chrono::high_resolution_clock::now();
    if (argc > 1) {
        if (!is_number(argv[1])) {
            std::cout << "Parameter 1 is not convertible to type int, type help for additional information!" << std::endl;
            return 1;
        }
        VRP_Dantzig_Solver solver(problem, renderData, stoi(argv[1]));
        solver.run();
    }
    else {
        VRP_Dantzig_Solver solver(problem, renderData);
        solver.run();
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Algorithm completed operation in : " << std::to_string(execution_time.count()) << " milliseconds" << std::endl;

    std::cout << "please close the visualiser window to continue." << std::endl;
    renderer.join();

    return 1;
}

//lauches VRP without renderer.
int _vrpDRNR(int argc, std::string* argv) {

    std::shared_ptr<RenderData> renderData = std::make_shared<RenderData>();
    //std::thread renderer(renderThread, renderData, std::ref(problem), 0.8f, 0, 10.0f);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto startTime = std::chrono::high_resolution_clock::now();
    VRP_Dantzig_Solver solver(problem, renderData);
    solver.run();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Algorithm completed operation in : " << std::to_string(execution_time.count()) << " milliseconds" << std::endl;
    //renderer.join();

    return 1;
}


int main(int argc, char** argv) {

    //creates main ui handler object with cursor type "_>"
    CBCI::userinput ui("_>");
    ui.addCommand(_ProblemInfo, "ProblemInfo", "Will print out all availible data on currently loaded Vehicle Routing problem.");
    ui.addCommand(_ProblemGen, "ProblemGen", "Will generate a new Vehicle routing problem, requires integer parameter for the number of points and capacity.\nExample: ProblemGen 10 50\n represents a problem with 10 points and a vehicle capacity of 50.");
    ui.addCommand(_ProblemSave, "ProblemSave", "Will save currently loaded problem, requires string parameter for file name.\nExample: ProblemGen problem1");
    ui.addCommand(_ProblemLoad, "ProblemLoad", "Will load selected .vrp file, requires string parameter for file name.\nExample: ProblemGen problem1");
    ui.addCommand(_tspNN, "TSPNN", "Traveling Sales Man Nearest Neighbour. Will execute a tspNN solver on loaded the problem using distribution as the start point.");
    ui.addCommand(_vrpDR, "VRPDantzig", "VRP Solver using the Dantzig and Ramser solver algorithm. Will execute solver on the loaded problem. May take an optional int parameter that corresponds to a milliseconds delay between renderer updates.\nExample : VRPDantzig 50");
    ui.addCommand(_vrpDRNR, "VRPDantzigNR", "VRP Solver using the Dantzig and Ramser solver algorithm. Will execute solver on the loaded problem. Will not launch renderer.");

    //run created userinterface
    ui.run();


    return 0;

}