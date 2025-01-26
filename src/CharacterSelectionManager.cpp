#include "CharacterSelectionManager.h"

using namespace app::player;
using namespace app::level;

auto GameModeStage_ChangePlayerCharacter = reinterpret_cast<void (*)(void* fsm, app::game::GameMode * gamemode, app::player::CharacterId charId)>(0x1401F0A60ui64);

MsgChangeStartingCharacter::MsgChangeStartingCharacter(app::player::CharacterId charId, bool force) : app::fnd::AppMessageCustom<MsgChangeStartingCharacter>{ MSG_CHANGE_STARTING_CHARACTER }, charId{ charId }, force{ force } {}
MsgChangeCurrentCharacter::MsgChangeCurrentCharacter(app::player::CharacterId charId) : app::fnd::AppMessageCustom<MsgChangeCurrentCharacter>{ MSG_CHANGE_CURRENT_CHARACTER }, charId{ charId } {}

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

void* CharacterSelectionManager::GetRuntimeTypeInfo()
{
	return nullptr;
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
	gameManager->AddListener(this);
}

void CharacterSelectionManager::OnRemovedFromGame()
{
	gameManager->RemoveListener(this);
}

void CharacterSelectionManager::MessageProcessedCallback(hh::game::GameManager* gameManager, const hh::fnd::Message& message)
{
	switch (message.ID) {
	case hh::fnd::MessageID::PASS_POINT_MARKER:
		if (auto* levelInfo = gameManager->GetService<LevelInfo>())
			if (auto* playerInfo = levelInfo->GetPlayerInformation(0))
				currentCharacter = static_cast<CharacterId>(playerInfo->characterId.value);
		break;
	case MSG_CHANGE_STARTING_CHARACTER: {
		auto& msg = static_cast<const MsgChangeStartingCharacter&>(message);
		ChangeStartingCharacter(msg.charId, msg.force);
		break;
	}
	case MSG_CHANGE_CURRENT_CHARACTER: {
		auto& msg = static_cast<const MsgChangeCurrentCharacter&>(message);
		ChangeCurrentCharacter(msg.charId);
		break;
	}
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
	if (auto* levelInfo = gameManager->GetService<LevelInfo>())
		return levelInfo->stageData->attributeFlags.test(GetCharacterAttributeFlag(charId));

	return charId == CharacterId::SONIC;
}

void CharacterSelectionManager::ForceCharacterOn(CharacterId charId) const {
	app::game::StageObjectModule::CharacterFilter filter = static_cast<app::game::StageObjectModule::CharacterFilter>(1 << static_cast<uint8_t>(charId));
	app::game::StageObjectModule::LoadCharacterObjInfo(hh::game::GameManager::GetInstance(), &filter, hh::fnd::MemoryRouter::GetModuleAllocator());

	if (auto* levelInfo = gameManager->GetService<LevelInfo>())
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
	CharacterIdU8 charIdU8 = static_cast<app::player::CharacterIdU8>(charId);

	if (auto* levelInfo = gameManager->GetService<app::level::LevelInfo>())
	if (auto* gameMode = static_cast<app::MyApplication*>(app::MyApplication::GetInstance())->GetExtension<app::game::ApplicationSequenceExtension>()->GetCurrentGameMode())
	if (auto* fxParamMgr = gameManager->GetService<app::gfx::FxParamManager>()) {
		app::player::Player::Kill(gameManager, 0);

		gameManager->ShutdownPendingObjects();

		auto& stageConfig = fxParamMgr->sceneParameters[fxParamMgr->currentSceneParameters]->sceneData->stageConfig;
		auto* playerInfo = levelInfo->GetPlayerInformation(0);

		app::player::Player::PlayerSetupInfo setupInfo{};
		setupInfo.playerId = 0;
		setupInfo.characterId = charIdU8;
		setupInfo.worldPos.m_Position = playerInfo->position.value;
		setupInfo.worldPos.m_Rotation = playerInfo->rotation.value;
		setupInfo.deadline = stageConfig.common.deadline;
		setupInfo.deadFallTime = stageConfig.common.deadFallTime;
		setupInfo.oceanSurface = stageConfig.common.oceanSurface;
		setupInfo.unk6 = 1;
		setupInfo.startType = app::player::Player::StartType::STAND;

		app::player::Player::Spawn(gameManager, setupInfo);

		app::ui::MsgUIChangePlayerCharacter msgUIChangePlayerCharacter{ charIdU8 };
		msgUIChangePlayerCharacter.Mask = 0x4000000;
		app::ut::SendMessageToUIObjects(*gameMode, gameManager, msgUIChangePlayerCharacter);

		app::player::MsgAddNotifyPreDeadListener msgAddNotifyPreDeadListener{};
		app::ut::SendMessageToPlayerObject(*gameMode, gameManager, 0, msgAddNotifyPreDeadListener);
	}
}
