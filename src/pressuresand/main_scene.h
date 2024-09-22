#pragma once
#include "common.h"
#include "world.h"

class MainScene {
public:
	static RZUF3_SceneDefinition* getSceneDef();
	static void destroy();

private:
	static RZUF3_SceneDefinition* m_sceneDef;
	static World* m_world;
};