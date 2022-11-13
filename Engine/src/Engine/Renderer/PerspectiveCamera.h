#pragma once

#include "Camera.h"

namespace Polyboid 
{

	class PerspectiveCamera : public Camera
	{
		public:
			PerspectiveCamera(float fov, float aspect, float near, float far);
			~PerspectiveCamera() = default;

		private:

	};


}