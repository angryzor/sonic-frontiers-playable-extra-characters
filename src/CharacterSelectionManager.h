#pragma once

class CharacterSelectionManager : public hh::game::GameService, public hh::game::GameManagerListener {
	constexpr static app::player::CharacterId charOrder[]{ app::player::CharacterId::SONIC, app::player::CharacterId::AMY, app::player::CharacterId::KNUCKLES, app::player::CharacterId::TAILS };
	app::player::CharacterId startingCharacter{ app::player::CharacterId::SONIC };
	app::player::CharacterId currentCharacter{ app::player::CharacterId::SONIC };

	void PerformCharacterChange(app::player::CharacterId charId);
	bool IsCharacterAllowed(app::player::CharacterId charId) const;
	void ForceCharacterOn(app::player::CharacterId charId) const;
	static app::level::StageData::AttributeFlags GetCharacterAttributeFlag(app::player::CharacterId charId);

public:
	virtual void* GetRuntimeTypeInfo() override;
	virtual bool ProcessMessage(hh::fnd::Message& message) override;
	virtual void OnAddedToGame() override;
	virtual void OnRemovedFromGame() override;
	virtual void MessageProcessedCallback(hh::game::GameManager* gameManager, const hh::fnd::Message& message) override;

	void ChangeStartingCharacter(app::player::CharacterId charId, bool force);
	void ChangeCurrentCharacter(app::player::CharacterId charId);
	void HandleRetry();
	app::player::CharacterId GetRespawnCharacter() const;

	GAMESERVICE_CLASS_DECLARATION(CharacterSelectionManager)
};
