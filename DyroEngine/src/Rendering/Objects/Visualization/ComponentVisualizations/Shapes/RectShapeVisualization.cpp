#include "Rendering/Objects/Visualization/ComponentVisualizations/Shapes/RectShapeVisualization.h"
#include "Rendering/Objects/Visualization/GameObjectVisualizations/GameObjectVisualization.h"
#include "Rendering/Objects/RenderItems/Shapes/RectShape.h"

#include "SceneGraph\Objects\GameObjects\SceneObject.h"
#include "SceneGraph\Objects\Components\Shapes\RectShapeComponent.h"
#include "SceneGraph\Objects\Scenes\Scene.h"

#include "Core\Data\Manager\GeometryManager.h"

RectShapeVisualization::RectShapeVisualization(Component* object, const std::tstring& name)
	:ShapeVisualization(object, name)
{
}
RectShapeVisualization::~RectShapeVisualization()
{
}

bool RectShapeVisualization::initialize()
{
	RectShapeComponent* component = dynamic_cast<RectShapeComponent*>(getObject());
	if (component == nullptr)
		return false;

	RectShapeDescription* description = component->getRectDescription();
	Geometry* geometry = getScene()->getManager<GeometryManager>()->getGeometry(description);

	setShape(new RectShape(description, geometry));

	assert(getShape() != nullptr);

	return true;
}
bool RectShapeVisualization::shutdown()
{
	return true;
}

void RectShapeVisualization::generateRenderItems(std::vector<RenderItem*>& items)
{
	GameObjectVisualization* parent = dynamic_cast<GameObjectVisualization*>(getParent());
	if (parent != nullptr)
	{
		SceneObject* parent_object = dynamic_cast<SceneObject*>(parent->getObject());

		//Since a rectangle will get renderer from his top corner we will need to center the rectangle in order to achieve correct visuals.
		Vector2D center = Vector2D(getShape()->getWidth() / 2, getShape()->getHeight() / 2);

		getShape()->setTransform(parent->getPosition() - center, parent->getScale(), parent->getRotation());
		getShape()->setLayer(parent_object->getLayer());
	}

	items.push_back(getShape());
}