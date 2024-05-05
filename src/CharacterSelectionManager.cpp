#include "CharacterSelectionManager.h"

using namespace app::player;
using namespace app::level;

auto GameModeStage_ChangePlayerCharacter = reinterpret_cast<void (*)(void* fsm, app::game::GameMode * gamemode, app::player::CharacterId charId)>(0x1401F0A60ui64);

CharacterSelectionManager::CharacterSelectionManager(csl::fnd::IAllocator* allocator) : GameService{ allocator } {

}

hh::game::GameService* CharacterSelectionManager::Create(csl::fnd::IAllocator* allocator) {
	return new (allocator) CharacterSelectionManager(allocator);
}

const hh::game::GameServiceClass CharacterSelectionManager::gameServiceClass = {
	"CharacterSelectionManager",
	CharacterSelectionManager::Create,
	nullptr,
};

const hh::game::GameServiceClass* CharacterSelectionManager::GetClass() {
	return &gameServiceClass;
}

bool CharacterSelectionManager::ProcessMessage(hh::fnd::Message& message) {
	switch (message.ID) {
	case hh::fnd::MessageID::NOTIFY_LEVEL_STATUS: {
		auto& msg = static_cast<app::game::MsgNotifyLevelStatus&>(message);
		switch (msg.status) {
		case 0:
			HandleRetry();
			break;
		}
		return true;
	}
	default:
		return false;
	}
}

void CharacterSelectionManager::OnAddedToGame()
{
	pGameManager->AddListener(this);
}

void CharacterSelectionManager::OnRemovedFromGame()
{
	pGameManager->RemoveListener(this);
}

void CharacterSelectionManager::MessageProcessedCallback(hh::game::GameManager* gameManager, const hh::fnd::Message& message)
{
	switch (message.ID) {
	case hh::fnd::MessageID::PASS_POINT_MARKER:
		if (auto* levelInfo = pGameManager->GetService<LevelInfo>())
			if (auto* playerInfo = levelInfo->GetPlayerInformation(0))
				currentCharacter = static_cast<CharacterId>(playerInfo->characterId.value);
		break;
	}
}

StageData::AttributeFlags CharacterSelectionManager::GetCharacterAttributeFlag(CharacterId charId)
{
	switch (charId) {
	case CharacterId::SONIC: return StageData::AttributeFlags::SONIC;
	case CharacterId::AMY: return StageData::AttributeFlags::AMY;
	case CharacterId::KNUCKLES: return StageData::AttributeFlags::KNUCKLES;
	case CharacterId::TAILS: return StageData::AttributeFlags::TAILS;
	default: return StageData::AttributeFlags::SONIC;
	}
}

bool CharacterSelectionManager::IsCharacterAllowed(CharacterId charId) const {
	if (auto* levelInfo = pGameManager->GetService<LevelInfo>())
		return levelInfo->stageData->attributeFlags.test(GetCharacterAttributeFlag(charId));

	return charId == CharacterId::SONIC;
}

void CharacterSelectionManager::ForceCharacterOn(CharacterId charId) const {
	app::game::StageObjectModule::CharacterFilter filter = static_cast<app::game::StageObjectModule::CharacterFilter>(1 << static_cast<uint8_t>(charId));
	app::game::StageObjectModule::LoadCharacterObjInfo(hh::game::GameManager::GetInstance(), &filter, hh::fnd::MemoryRouter::GetModuleAllocator());

	if (auto* levelInfo = pGameManager->GetService<LevelInfo>())
		levelInfo->stageData->attributeFlags.set(GetCharacterAttributeFlag(charId));
}

void CharacterSelectionManager::ChangeStartingCharacter(CharacterId charId, bool force)
{
	if (force)
		ForceCharacterOn(charId);

	if (IsCharacterAllowed(charId)) {
		PerformCharacterChange(charId);
		startingCharacter = charId;
		currentCharacter = charId;
	}
}

void CharacterSelectionManager::ChangeCurrentCharacter(app::player::CharacterId charId)
{
	if (IsCharacterAllowed(charId))
		PerformCharacterChange(charId);
}

void CharacterSelectionManager::HandleRetry()
{
	currentCharacter = startingCharacter;
}

app::player::CharacterId CharacterSelectionManager::GetRespawnCharacter() const
{
	if (IsCharacterAllowed(currentCharacter))
		return currentCharacter;

	for (auto charId : charOrder)
		if (IsCharacterAllowed(charId))
			return charId;

	return CharacterId::SONIC;
}

void CharacterSelectionManager::PerformCharacterChange(CharacterId charId) {
	if (auto* gameMode = static_cast<app::MyApplication*>(app::MyApplication::GetInstance())->GetExtension<app::game::ApplicationSequenceExtension>()->GetCurrentGameMode())
		GameModeStage_ChangePlayerCharacter(nullptr, gameMode, charId);
}
