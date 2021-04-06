#include <iostream>
#include <vector>
#include "Renderer.h"
#include "Window.h"

int helloWorld(int argc, std::string* argv) {
	std::cout << "Hello World! is running" << std::endl;

	return 1;
}

static void neon(float* val, float* increment) {
    if (*val > 1.0f)
        *increment = -*increment;
    else if (*val < 0.0f)
        *increment = -*increment;

    *val += *increment;
}
    
int main(int argc, char** argv) {

    std::vector<double> points;
    for (int x = 0; x < 15; x++) {
        for (int y = 0; y < 15; y++) {
            points.push_back(x);
            points.push_back(y);
        }
    }


    Renderer render;
    Window window(&render, 2);
    window.CreateWindow("hello World!");
    render.Init(points);
    window.Run();

}