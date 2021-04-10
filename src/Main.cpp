#include <iostream>
#include <vector>
#include "Visualisation/Renderer.h"
#include "Visualisation/Window.h"
#include "Problem.h"
    
int main(int argc, char** argv) {

    Problem problem(50, 4, 500);
    

    Window window("hello World!", 0.8f, 0);

    Renderer render = Renderer(problem.getCoords());
    render.SetIndexColor(problem.distributionIndex(), { 0.0f,0.0f,1.0f,1.0f });


    //render.DrawLines(bois, {0.0f, 0.0f, 0.0f, 1.0f},true);
    //render.SetIndexColor(bois, { 0.0f,1.0f,0.0f,1.0f });
    render.SetScale(5);

    problem.printall();

    window.Run(&render);

    

}