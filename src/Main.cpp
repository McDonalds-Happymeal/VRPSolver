#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>

#include "threadFunctions.h"
#include "Solver/Problem.h"
#include "Solver/Solver.h"


int main(int argc, char** argv) {


    Problem problem(3, 0, 50);
    std::shared_ptr<RenderData> renderData = std::make_shared<RenderData>();
    std::thread renderer(renderThread,renderData, std::ref(problem), 0.8f, 0, 10.0f);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    Solver solver(problem, renderData);
    solver.TSPSolver();
    renderer.join();

    return 0;

}