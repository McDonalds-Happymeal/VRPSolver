#pragma once
#include "Visualisation/Renderer.h"
#include "Visualisation/Window.h"
#include "Solver/Problem.h"



void renderThread(std::shared_ptr<RenderData>_renderData, Problem& problem, float windowScale, int moniter, float drawScale);
