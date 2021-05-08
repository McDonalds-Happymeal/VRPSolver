#pragma once
#include "Renderer.h"
#include "Window.h"
#include "../Solver/Problem.h"



void renderThread(std::shared_ptr<RenderData>_renderData, Problem& problem, float windowScale, int moniter, float drawScale);
