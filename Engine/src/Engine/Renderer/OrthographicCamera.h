#pragma once
#include "Camera.h"

namespace Polyboid 
{

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera();

	private:

	};


}