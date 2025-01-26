#include "ObjSwitchCharacterVolume.h"
#include "CharacterSelectionManager.h"

using namespace hh::fnd;
using namespace hh::game;
using namespace app::game;

const RflClassEnumMember characterIdMembers[]{
	{ 0, "Sonic", reinterpret_cast<const char*>(u8"ソニック") },
	{ 1, "Amy", reinterpret_cast<const char*>(u8"エミー") },
	{ 2, "Knuckles", reinterpret_cast<const char*>(u8"ナックルズ") },
	{ 3, "Tails", reinterpret_cast<const char*>(u8"テイルス") },
};

const RflClassEnumMember conditionMembers[]{
	{ 0, "COND_ON_TRIGGER", "COND_ON_TRIGGER" },
	{ 1, "COND_PULSE", "COND_PULSE" },
	{ 2, "COND_TIMER_ONCE", "COND_TIMER_ONCE" },
	{ 3, "COND_TIMER", "COND_TIMER" },
	{ 4, "COND_ON_STAY", "COND_ON_STAY" },
};

const RflClassEnum spawnerEnums[]{
	{ "CharacterId", characterIdMembers, sizeof(characterIdMembers) / sizeof(RflClassEnumMember) },
	{ "Condition", conditionMembers, sizeof(conditionMembers) / sizeof(RflClassEnumMember) },
};

const char* conditionCaption = "Condition";
const RflCustomAttribute conditionAttributes_[]{
	{ "Caption", &conditionCaption, nullptr },
};
const RflCustomAttributes conditionAttributes{ conditionAttributes_, sizeof(conditionAttributes_) / sizeof(RflCustomAttribute) };

const char* lifetimeCaption = "Lifetime";
const RflCustomAttribute lifetimeAttributes_[]{
	{ "Caption", &lifetimeCaption, nullptr },
};
const RflCustomAttributes lifetimeAttributes{ lifetimeAttributes_, sizeof(lifetimeAttributes_) / sizeof(RflCustomAttribute) };

const char* eventDrivenCaption = "Event driven";
const RflCustomAttribute eventDrivenAttributes_[]{
	{ "Caption", &eventDrivenCaption, nullptr },
};
const RflCustomAttributes eventDrivenAttributes{ eventDrivenAttributes_, sizeof(eventDrivenAttributes_) / sizeof(RflCustomAttribute) };

const char* characterIdCaption = "Character ID to change to";
const RflCustomAttribute characterIdAttributes_[]{
	{ "Caption", &characterIdCaption, nullptr },
};
const RflCustomAttributes characterIdAttributes{ characterIdAttributes_, sizeof(characterIdAttributes_) / sizeof(RflCustomAttribute) };

const char* volumeCaption = "Volume data";
const RflCustomAttribute volumeAttributes_[]{
	{ "Caption", &volumeCaption, nullptr },
};
const RflCustomAttributes volumeAttributes{ volumeAttributes_, sizeof(volumeAttributes_) / sizeof(RflCustomAttribute) };

const RflClassMember spawnerMembers[]{
	{ "condition", nullptr, &spawnerEnums[1], RflClassMember::Type::ENUM, RflClassMember::Type::SINT8, 0, 0, offsetof(ObjSwitchCharacterVolumeSpawner, condition), &conditionAttributes },
	{ "lifetime", nullptr, nullptr, RflClassMember::Type::FLOAT, RflClassMember::Type::VOID, 0, 0, offsetof(ObjSwitchCharacterVolumeSpawner, lifetime), &lifetimeAttributes },
	{ "eventDriven", nullptr, nullptr, RflClassMember::Type::BOOL, RflClassMember::Type::VOID, 0, 0, offsetof(ObjSwitchCharacterVolumeSpawner, eventDriven), &eventDrivenAttributes },
	{ "characterId", nullptr, &spawnerEnums[0], RflClassMember::Type::ENUM, RflClassMember::Type::UINT32, 0, 0, offsetof(ObjSwitchCharacterVolumeSpawner, characterId), &characterIdAttributes },
	{ "volume", rangerssdk::GetAddress(&heur::rfl::VolumeTriggerSpawner::rflClass), nullptr, RflClassMember::Type::STRUCT, RflClassMember::Type::VOID, 0, 0, offsetof(ObjSwitchCharacterVolumeSpawner, volume), &volumeAttributes },
};

const RflClass ObjSwitchCharacterVolumeSpawner::rflClass{
	"ObjSwitchCharacterVolumeSpawner",
	nullptr,
	sizeof(ObjSwitchCharacterVolumeSpawner),
	spawnerEnums,
	sizeof(spawnerEnums) / sizeof(RflClassEnum),
	spawnerMembers,
	sizeof(spawnerMembers) / sizeof(RflClassMember),
	nullptr,
};

void* constructObjSwitchCharacterVolumeSpawner(void* instance, csl::fnd::IAllocator* allocator) {
	auto* self = static_cast<ObjSwitchCharacterVolumeSpawner*>(instance);

	self->characterId = app::player::CharacterId::SONIC;
	self->condition = ObjSwitchCharacterVolumeSpawner::Condition::COND_ON_TRIGGER;
	self->lifetime = 0.0;
	self->eventDriven = 0;
	self->volume.ShapeType = heur::rfl::VolumeTriggerSpawner::Shape::SHAPE_BOX;
	self->volume.basePoint = heur::rfl::VolumeTriggerSpawner::BasePoint::BASE_CENTER;
	self->volume.CollisionFilter = heur::rfl::VolumeTriggerSpawner::ColliFilter::FILTER_DEFAULT;
	self->volume.CollisionWidth = 1.0;
	self->volume.CollisionHeight = 1.0;
	self->volume.CollisionDepth = 1.0;
	self->volume.CollisionOffset = csl::math::Vector3{ 0.0f, 0.0f, 0.0f };

	return self;
}

void finishObjSwitchCharacterVolumeSpawner(void* instance) {

}

void cleanObjSwitchCharacterVolumeSpawner(void* instance) {

}

const RflTypeInfo ObjSwitchCharacterVolumeSpawner::rflTypeInfo{
	"ObjSwitchCharacterVolumeSpawner",
	"ObjSwitchCharacterVolumeSpawner",
	constructObjSwitchCharacterVolumeSpawner,
	finishObjSwitchCharacterVolumeSpawner,
	cleanObjSwitchCharacterVolumeSpawner,
	sizeof(ObjSwitchCharacterVolumeSpawner),
};

const RflClassMember::Value gameObjectClassAttributes[]{
	{ "category", RflClassMember::Type::CSTRING, "Common/System" },
};

const GameObjectClass ObjSwitchCharacterVolume::gameObjectClass{
	"SwitchCharacterVolume",
	"SwitchCharacterVolume",
	sizeof(ObjSwitchCharacterVolume),
	&ObjSwitchCharacterVolume::Create,
	sizeof(gameObjectClassAttributes) / sizeof(RflClassMember::Value),
	gameObjectClassAttributes,
	&ObjSwitchCharacterVolumeSpawner::rflClass,
};

GameObject* ObjSwitchCharacterVolume::Create(csl::fnd::IAllocator* allocator) {
	return new (std::align_val_t(16), allocator) ObjSwitchCharacterVolume{ allocator };
}

ObjSwitchCharacterVolume::ObjSwitchCharacterVolume(csl::fnd::IAllocator* allocator) : GameObject{ allocator }, volume{ allocator }, condition{ ObjSwitchCharacterVolumeSpawner::Condition::COND_ON_TRIGGER } {
	SetLayer(6);
	SetUpdateFlag(hh::fnd::UpdatingPhase::PRE_ANIM, 0);
}

void* ObjSwitchCharacterVolume::GetRuntimeTypeInfo()
{
	return nullptr;
}

bool ObjSwitchCharacterVolume::ProcessMessage(hh::fnd::Message& message)
{
	switch (message.ID) {
	case MessageID::START_ACTIVE_OBJECT_IN_EDITOR:
		volume.StartActiveObjectInEditor();
		return true;
	case MessageID::UPDATE_ACTIVE_OBJECT_IN_EDITOR:
		volume.UpdateActiveObjectInEditor();
		return true;
	case MessageID::FINISH_ACTIVE_OBJECT_IN_EDITOR:
		volume.FinishActiveObjectInEditor();
		return true;
	case MessageID::PARAM_CHANGED_IN_EDITOR: {
		auto* worldData = GetWorldDataByClass<ObjSwitchCharacterVolumeSpawner>();
		volume.UpdateConfig(worldData->volume);
		volume.UpdateDebugVisual(worldData->volume);
		GetComponent<GOCEventCollision>()->UpdateConfig();
		return true;
	}
	case MessageID::NOTIFY_ACTION: {
		if (auto* gocCollider = GetComponent<hh::physics::GOCCollider>()) {
			switch (static_cast<app::game::MsgNotifyAction&>(message).action) {
			case 1: gocCollider->SetEnabled(true); break;
			case 2: gocCollider->SetEnabled(false); break;
			}
		}
		return true;
	}
	default:
		return GameObject::ProcessMessage(message);
	}
}

void ObjSwitchCharacterVolume::AddCallback(GameManager* gameManager)
{
	auto* worldData = GetWorldDataByClass<ObjSwitchCharacterVolumeSpawner>();
	auto state = status->GetObjectState(0);
	
	if (state == 2)
		return;

	volume.Setup(this, worldData->volume, 0, false);
	volume.SetupDebugVisual();

	GOCEventCollision::SetupInfo gocEventSetupInfo;
	gocEventSetupInfo.enabled = state == 1;
	gocEventSetupInfo.unk102 = 0;

	condition = worldData->condition;
	switch (worldData->condition) {
	case ObjSwitchCharacterVolumeSpawner::Condition::COND_PULSE:
		gocEventSetupInfo.condition = GOCEventCollision::Condition::PULSE;
		break;
	case ObjSwitchCharacterVolumeSpawner::Condition::COND_ON_STAY:
		gocEventSetupInfo.condition = GOCEventCollision::Condition::ON_STAY;
		break;
	case ObjSwitchCharacterVolumeSpawner::Condition::COND_TIMER:
	case ObjSwitchCharacterVolumeSpawner::Condition::COND_TIMER_ONCE:
		gocEventSetupInfo.lifetime = worldData->lifetime;
		[[fallthrough]];
	default:
		gocEventSetupInfo.condition = GOCEventCollision::Condition::ON_TRIGGER;
		break;
	}

	auto* gocEvent = CreateComponent<GOCEventCollision>();
	gocEvent->Setup(gocEventSetupInfo);
	AddComponent(gocEvent);
	gocEvent->AddListener(this);

	if (auto* gocCollider = GetComponent<hh::physics::GOCCollider>())
		gocCollider->SetEnabled(!worldData->eventDriven);

	GetComponent<GOCActivator>()->enabled = false;
}

void ObjSwitchCharacterVolume::EventCallback(unsigned int event)
{
	switch (event) {
	case 0: {
		switch (condition) {
		case ObjSwitchCharacterVolumeSpawner::Condition::COND_ON_TRIGGER:
			status->SetObjectState(0, 1);
			break;
		case ObjSwitchCharacterVolumeSpawner::Condition::COND_TIMER_ONCE:
			status->SetObjectState(0, 2);
			if (auto* gocCollider = GetComponent<hh::physics::GOCCollider>())
				gocCollider->SetEnabled(false);
			break;
		}

		auto characterId = GetWorldDataByClass<ObjSwitchCharacterVolumeSpawner>()->characterId;

		if (auto* levelInfo = gameManager->GetService<app::level::LevelInfo>()) {
			Handle<app::player::Player> hPlayer = levelInfo->GetPlayerInformation(0)->playerObject.value;

			if (auto* player = static_cast<GameObject*>(MessageManager::GetInstance()->GetMessengerByHandle((Handle<hh::fnd::Messenger>&)hPlayer)))
				if (auto* playerParam = player->GetComponent<app::player::GOCPlayerParameter>())
					if (playerParam->characterId != characterId) {
						MsgChangeCurrentCharacter msg{ characterId };

						SendMessageToGame(msg);
					}
		}
		break;
	}
	case 1:
		GetComponent<GOCActivator>()->enabled = true;
		break;
	}
}

const hh::game::GameObjectClass* ObjSwitchCharacterVolume::GetClass()
{
	return &gameObjectClass;
}
