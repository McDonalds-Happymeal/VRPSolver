#include <iostream>
#include <vector>
#include <memory>
#include "threadFunctions.h"
#include "Solver/Problem.h"

int main(int argc, char** argv) {


    Problem problem(50, 4, 500);
    std::shared_ptr<RenderData> renderData = std::make_shared<RenderData>();
    
    std::thread renderer(renderThread,renderData, std::ref(problem), 0.8f, 0, 10.0f);
    
    std::vector<unsigned int> line = { 0,1 };
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        renderData->LinesClear();
        renderData->AddLine(line, { 0.0f,0.0f,0.0f,1.0f }, 2.5f);
        //std::cout << "line added between vertex : " << line[0] << " and " << line[1] << std::endl;
        line[0]++;
        line[1]= line[1]+2;
        if (line[0] > 51) line[0] = 0;
        if (line[1] > 51) line[1] = 0;
    }



    renderer.join();

    return 0;

}