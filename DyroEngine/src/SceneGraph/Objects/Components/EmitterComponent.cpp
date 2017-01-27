#include "SceneGraph/Objects/Components/EmitterComponent.h"
#include "SceneGraph/Objects/Components/TransformComponent.h"

#include "SceneGraph/Objects/GameObjects/GameObject.h"

#include "SceneGraph/Objects/Scenes/Scene.h"

#include "Core/Data/Objects/Descriptions/Particles/EmitterComponentDescription.h"
#include "Core/Data/Objects/Descriptions/Particles/ParticleDescription.h"
#include "Core/Data/Objects/Timers/WorldTimer.h"
#include "Core/Data/Objects/Transition.h"

#include "Core/Data/Manager/SettingsManager.h"
#include "Core/Data/Manager/TransitionManager.h"

#include "Core/Defines/deletemacros.h"
#include "Core/Defines/debug.h"

EmitterComponent::EmitterComponent(EmitterComponentDescription* description,const std::tstring& name)
	:Component(name)
	, description(description)
	, current_emission_rate(description->getEmissionRate())
{
	OBJECT_INIT(_T("EmitterComponent"));
}
EmitterComponent::~EmitterComponent()
{}

bool EmitterComponent::initialize()
{
	if (this->description->canSpawnAtStart())
	{
		for (unsigned int i = 0; i < this->description->getEmissionBurst(); ++i)
			addParticle();

		this->current_emission_rate = description->getEmissionRate();
	}

	return true;
}
void EmitterComponent::update()
{
	this->current_emission_rate -= WorldTimer::getWorldDeltaTime();
	if (this->current_emission_rate < 0.0f)
	{
		if (this->particle_descriptions.size() < (this->description->getParticleAmount() - this->description->getEmissionRate()))
		{
			for (unsigned int i = 0; i < this->description->getEmissionBurst(); ++i)
				addParticle();

			this->current_emission_rate = description->getEmissionRate();
		}
	}

	for (unsigned int i = 0; i < (unsigned int)this->particle_descriptions.size(); ++i)
	{
		this->particle_descriptions[i].setLifeTime(this->particle_descriptions[i].getLifeTime() - WorldTimer::getWorldDeltaTime());
		if (this->particle_descriptions[i].getLifeTime() < 0.0f)
			this->particle_descriptions[i].destroy();

		this->particle_descriptions[i].setPosition(this->particle_descriptions[i].getPosition() + this->particle_descriptions[i].getVelocity() * WorldTimer::getWorldDeltaTime());
		this->particle_descriptions[i].setScale(this->particle_descriptions[i].getScale() + this->particle_descriptions[i].getScaleVelocity() * WorldTimer::getWorldDeltaTime());
		this->particle_descriptions[i].setRotation(this->particle_descriptions[i].getRotation() + this->particle_descriptions[i].getAngularVelocity() * WorldTimer::getWorldDeltaTime());

		if (this->particle_descriptions[i].canFade())
		{
			float normalized_lifetime = this->particle_descriptions[i].getLifeTime() / this->particle_descriptions[i].getInitialLifeTime();
			if (normalized_lifetime < this->particle_descriptions[i].getFadeStart())
			{
				Transition<float>* transition = getParent()->getScene()->getManager<TransitionManager>()->getTransition<float>(_T("particle_fade_transition") + TOSTRING(this->particle_descriptions[i].getID()));
				if (transition == nullptr || transition->isFinished())
					continue;

				if (!transition->isRunning())
					transition->start();
				else this->particle_descriptions[i].setFadeAmount(1.0f - transition->getValue());
			}
		}

		//LogManager::getInstance().log(new TodoLog(_T("Particle gravity"), LOG_INFO));
	}

	//Remove destroyed particles
	this->particle_descriptions.erase(std::remove_if(this->particle_descriptions.begin(), this->particle_descriptions.end(),
		[](const ParticleDescription& desc) -> bool
	{
		return desc.isDestroyed();
	}),this->particle_descriptions.end());

	if (!this->description->getDirty())
		return;

	//LogManager::getInstance().log(new TodoLog(_T("Apply particle emitter modifications"), LOG_INFO));

	this->description->setDirty(false);
}
bool EmitterComponent::shutdown()
{
	particle_descriptions.clear();

	SafeDelete(this->description);

	return true;
}

EmitterComponentDescription* EmitterComponent::getEmitterDescription() const 
{
	return this->description;
}
const std::vector<ParticleDescription>& EmitterComponent::getParticleDescriptions() const
{
	return this->particle_descriptions;
}

void EmitterComponent::addParticle()
{
	ParticleDescription desc;

	TransformComponent* transform = getParent()->getComponent<TransformComponent>();
	if (transform == nullptr)
		return;

	desc.setPosition(transform->getPosition() + this->description->getSpawnPositionOffset());
	desc.setScale(transform->getScale() + this->description->getSpawnScaleOffset());
	desc.setRotation(transform->getRotation() + this->description->getSpawnRotationOffset());

	desc.setGravityMultiplier(this->description->getGravityMultiplier());
	desc.setLifeTime(this->description->getLifeTime());

	desc.setVelocity(this->description->getVelocity());
	desc.setScaleVelocity(this->description->getScaleVelocity());
	desc.setAngularVelocity(this->description->getAngularVelocity());

	desc.enableFade(this->description->canFade());
	desc.setFadeStart(this->description->getFadeStart());
	desc.setFadeEnd(this->description->getFadeEnd());
	desc.setFadeAmount(1);

	if ((int)this->description->getImages().size() > 0)
		desc.setImage(this->description->getImages()[rand() % (int)this->description->getImages().size()]);

	//Setup particle fade
	if (desc.canFade())
	{
		float fade_distance = desc.getFadeEnd() - desc.getFadeStart();
		if (fade_distance <= 0.0f)
			return;

		float fade_speed = (1.0f / fade_distance) * WorldTimer::getWorldDeltaTime();

		Transition<float>* fading_transition = new Transition<float>(0.0f, 1.0f, _T("particle_fade_transition") + TOSTRING(desc.getID()));
		fading_transition->setSpeed(fade_speed);
		fading_transition->destroyOnFinish(false);

		getParent()->getScene()->getManager<TransitionManager>()->addTransition(fading_transition);
	}

	this->particle_descriptions.push_back(desc);
}