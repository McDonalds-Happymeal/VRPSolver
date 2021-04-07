#include <iostream>
#include <vector>
#include "Visualisation/Renderer.h"
#include "Visualisation/Window.h"

    
int main(int argc, char** argv) {

    std::vector<double> points;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            points.push_back(y);
            points.push_back(x);
        }
    }


    std::vector<unsigned int> bois = { 3,4,8,3 };

    Renderer render;
    Window window(&render, 0.5f);
    window.CreateWindow("hello World!");

    render.Init(points);


    render.DrawLines(bois, {0.0f, 0.0f, 0.0f, 1.0f},true);
    render.SetIndexColor(bois, { 0.0f,1.0f,0.0f,1.0f });
    render.SetScale(5);

    window.Run();

    

}