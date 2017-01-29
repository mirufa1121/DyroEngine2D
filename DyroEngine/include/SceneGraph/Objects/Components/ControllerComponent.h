#ifndef _CONTROLLERCOMPONENT_H
#define _CONTROLLERCOMPONENT_H

#include "SceneGraph\Objects\Components\Component.h"

class ControllerComponent : public Component
{
public:
	ControllerComponent(const std::tstring& name = _T("ControllerComponent"));
	virtual ~ControllerComponent();

	virtual bool initialize();
	virtual void update();
	virtual bool shutdown();

	void setSpeed(float speed);

	float getSpeed();

private:
	float speed;
};

#endif