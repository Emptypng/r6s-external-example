#pragma once

#include "Vectors.h"
#include "stdafx.h"

namespace Engine
{
	class Entity
	{
		public:
			Vector3& GetPosition();
			Vector3& GetNeck();
			Vector3& GetHead();
			INT8 GetTeam();
			INT8 GetHealth();
			std::string GetName();
    };
}
