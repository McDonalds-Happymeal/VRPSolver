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
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            points.push_back(y);
            points.push_back(x);
        }
    }

    points.push_back(100);
    points.push_back(250);

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