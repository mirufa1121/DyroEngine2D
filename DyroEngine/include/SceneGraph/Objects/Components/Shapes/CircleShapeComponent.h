#ifndef _CIRCLESHAPCOMPONENT_H
#define _CIRCLESHAPCOMPONENT_H

#include "SceneGraph\Objects\Components\Shapes\ShapeComponent.h"

#ifndef _VECTOR2D_H
#include "Math/Objects/Vector2D.h"
#endif

class CircleShapeDescription;

class CircleShapeComponent : public ShapeComponent
{
	/**
	\note Must be present in every subclass definition.
	*/
	OBJECT_STATICS("CircleShapeComponent")

public:
	CircleShapeComponent(CircleShapeDescription* description, const std::tstring& name = _T(""));
	virtual ~CircleShapeComponent();

	void setCenterPosition(const Vector2D& center);
	void setRadius(float radius);

	const Vector2D& getCenterPosition() const;
	float getRadius() const;

	CircleShapeDescription* getCircleDescription() const;
};

#endif