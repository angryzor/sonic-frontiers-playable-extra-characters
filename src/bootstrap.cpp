#include "bootstrap.h"
#include "ObjSwitchCharacterVolume.h"
#include "CharacterSelectionManager.h"

using namespace app::player;
using namespace app::level;


HOOK(Player::PlayerSetupInfo*, __fastcall, Player_PlayerSetupInfo_PlayerSetupInfo, 0x14A9CC3A0, Player::PlayerSetupInfo* self)
{
	auto* result = originalPlayer_PlayerSetupInfo_PlayerSetupInfo(self);

	if (auto* gameManager = hh::game::GameManager::GetInstance())
		if (auto* charMgr = gameManager->GetService<CharacterSelectionManager>())
			result->characterId = static_cast<CharacterIdU8>(charMgr->GetRespawnCharacter());

	return result;
}

HOOK(void, __fastcall, GameModeCyberStage_RebuildLevelState_OnEnter, 0x147463820, void* self, void* unk1, unsigned int prevState) {
	if (prevState == 14)
		if (auto* gameManager = hh::game::GameManager::GetInstance())
			if (auto* charMgr = gameManager->GetService<CharacterSelectionManager>())
				charMgr->HandleRetry();

	originalGameModeCyberStage_RebuildLevelState_OnEnter(self, unk1, prevState);
}

class KeyEventHandler : public hh::fw::KeyEventHandler, public hh::game::GameManagerListener {
	virtual bool OnKeyDown(hh::fw::KeyEventArgs& keyEventArgs) override {
		if (keyEventArgs.scanCode >= 0x1e && keyEventArgs.scanCode <= 0x21) {
			if (auto* gameManager = hh::game::GameManager::GetInstance()) {
				if (auto* charSelMgr = gameManager->GetService<CharacterSelectionManager>()) {
					auto charId = static_cast<CharacterId>(keyEventArgs.scanCode - 0x1e);

					charSelMgr->ChangeStartingCharacter(charId, keyEventArgs.modifier == 1);
				}
			}
		}
		return false;
	}
};

KeyEventHandler keyEventHandler;

void Bootstrap() {
	INSTALL_HOOK(GameModeCyberStage_RebuildLevelState_OnEnter);
	INSTALL_HOOK(Player_PlayerSetupInfo_PlayerSetupInfo);

	hh::fnd::RflClassNameRegistry::GetInstance()->Register(&ObjSwitchCharacterVolumeSpawner::rflClass);
	hh::fnd::RflTypeInfoRegistry::GetInstance()->Register(&ObjSwitchCharacterVolumeSpawner::rflTypeInfo);

	const hh::game::GameObjectClass* obj = ObjSwitchCharacterVolume::GetClass();
	hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->AddObject(&obj);

	auto* charMgr = hh::game::GameManager::GetInstance()->CreateService<CharacterSelectionManager>(hh::fnd::MemoryRouter::GetModuleAllocator());
	hh::game::GameManager::GetInstance()->RegisterService(charMgr);

	hh::game::GameApplication::GetInstance()->AddKeyEventHandler(&keyEventHandler, 0);
	hh::game::GameManager::GetInstance()->AddListener(&keyEventHandler);
}
