#include "Rendering/Objects/Visualization/ComponentVisualizations/EmitterComponentVisualization.h"
#include "Rendering/Objects/RenderItems/Particle.h"
#include "Rendering/Objects/Visualization/GameObjectVisualizations/GameObjectVisualization.h"

#include "SceneGraph\Objects\GameObjects\SceneObject.h"

#include "Core/Data/Objects/Descriptions/Particles/ParticleDescription.h"
#include "Core/Data/Objects/Descriptions/Particles/EmitterComponentDescription.h"
#include "Core/Defines/deletemacros.h"

#include "SceneGraph/Objects/Components/EmitterComponent.h"

EmitterComponentVisualization::EmitterComponentVisualization(Component* component, const std::tstring& name)
	:ConcreteComponentVisualization(component, name)
{

}
EmitterComponentVisualization::~EmitterComponentVisualization()
{

}

bool EmitterComponentVisualization::initialize()
{
	EmitterComponent* emitter_component = dynamic_cast<EmitterComponent*>(getObject());
	if (emitter_component != nullptr)
	{
		for (ParticleDescription* desc : emitter_component->getParticleDescriptions())
			this->particles.push_back(new Particle(desc));
	}

	return true;
}
bool EmitterComponentVisualization::shutdown()
{
	for (Particle* particle : this->particles)
		SafeDelete(particle);
	this->particles.clear();

	return true;
}

Rect2D EmitterComponentVisualization::getBoundingBox() const
{
	//Initialize bounds
	Rect2D bounds;
	bounds = this->particles[0]->getBounds();

	for (Particle* p : this->particles)
	{
		Rect2D temp_bounds = p->getBounds();

		//Expand bounding box to the entire visualization
		if (bounds.left > temp_bounds.left)
			bounds.left = temp_bounds.left;
		if (bounds.top > temp_bounds.top)
			bounds.top = temp_bounds.top;

		if (bounds.right < temp_bounds.right)
			bounds.right = temp_bounds.right;
		if (bounds.bottom < temp_bounds.bottom)
			bounds.bottom = temp_bounds.bottom;
	}

	return bounds;
}

void EmitterComponentVisualization::generateRenderItems(std::vector<RenderItem*>& items)
{
	GameObjectVisualization* parent = dynamic_cast<GameObjectVisualization*>(getParent());
	if (parent == nullptr)
		return;

	SceneObject* parent_object = dynamic_cast<SceneObject*>(parent->getObject());

	EmitterComponent* emitter_component = getConcreteComponent();
	//if (emitter_component->getEmitterDescription()->getDirty())
	//{
		for (Particle* particle : this->particles)
			SafeDelete(particle);
		this->particles.clear();

		for (ParticleDescription* desc : emitter_component->getParticleDescriptions())
		{
			Particle* particle = new Particle(desc);
			this->particles.push_back(particle);

			//Since an image will get renderer from his top corner we will need to center the image in order to achieve correct visuals.
			Vector2D center = Vector2D(particle->getWidth() / 2, particle->getHeight() / 2);

			particle->setLayer(parent_object->getLayer());
			particle->setTransform(particle->getParticleDescription()->getPosition() - center, particle->getParticleDescription()->getScale(), particle->getParticleDescription()->getRotation());

			items.push_back(particle);
		}
	//}
	//else
	//{
	//	for (Particle* particle : this->particles)
	//	{
	//
	//		//Since an image will get renderer from his top corner we will need to center the image in order to achieve correct visuals.
	//		Vector2D center = Vector2D(particle->getWidth() / 2, particle->getHeight() / 2);

	//		particle->setLayer(parent_object->getLayer());
	//		particle->setTransform(particle->getParticleDescription().getPosition(), particle->getParticleDescription().getScale(), particle->getParticleDescription().getRotation());

	//		items.push_back(particle);
	//	}
	//}
}