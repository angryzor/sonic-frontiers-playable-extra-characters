#pragma once

struct ObjSwitchCharacterVolumeSpawner {
	enum class Condition : char {
		COND_ON_TRIGGER = 0,
		COND_PULSE = 1,
		COND_TIMER_ONCE = 2,
		COND_TIMER = 3,
		COND_ON_STAY = 4,
	};

	Condition condition;
	float lifetime;
	bool eventDriven;
	app::player::CharacterId characterId;
	app::rfl::VolumeTriggerSpawner volume;

	static const hh::fnd::RflClass rflClass;
	static const hh::fnd::RflTypeInfo rflTypeInfo;
};

class ObjSwitchCharacterVolume
	: public hh::game::GameObject
	, public app::game::GOCEventListener
{
	GAMEOBJECT_CLASS_DECLARATION(ObjSwitchCharacterVolume)

	app::VolumeTrigger volume;
	ObjSwitchCharacterVolumeSpawner::Condition condition;
public:
	virtual void* GetRuntimeTypeInfo() override;
	virtual bool ProcessMessage(hh::fnd::Message& message) override;
	virtual void AddCallback(hh::game::GameManager* gameManager) override;
	virtual void EventCallback(unsigned int event) override;
};
