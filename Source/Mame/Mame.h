#pragma once

#include "Input/Mouse.h"

#include "./Scene/BaseScene.h"
#include "./Scene/SceneManager.h"



namespace Mame
{
	namespace Scene
	{
		inline BaseScene* GetCurrentScene()
		{
			return Scene::SceneManager::Instance().GetCurrentScene();
		}
	}
}