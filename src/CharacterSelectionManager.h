#pragma once

static constexpr hh::fnd::MessageID MSG_CHANGE_STARTING_CHARACTER = static_cast<hh::fnd::MessageID>(0x43534348u);
static constexpr hh::fnd::MessageID MSG_CHANGE_CURRENT_CHARACTER = static_cast<hh::fnd::MessageID>(0x43434348u);

class MsgChangeStartingCharacter : public app::fnd::AppMessageCustom<MsgChangeStartingCharacter> {
public:
	app::player::CharacterId charId;
	bool force;

	MsgChangeStartingCharacter(app::player::CharacterId charId, bool force);
};

class MsgChangeCurrentCharacter : public app::fnd::AppMessageCustom<MsgChangeCurrentCharacter> {
public:
	app::player::CharacterId charId;

	MsgChangeCurrentCharacter(app::player::CharacterId charId);
};

class CharacterSelectionManager : public hh::game::GameService, public hh::game::GameManagerListener {
	constexpr static app::player::CharacterId charOrder[]{ app::player::CharacterId::SONIC, app::player::CharacterId::AMY, app::player::CharacterId::KNUCKLES, app::player::CharacterId::TAILS };
	app::player::CharacterId startingCharacter{ app::player::CharacterId::SONIC };
	app::player::CharacterId currentCharacter{ app::player::CharacterId::SONIC };

	void PerformCharacterChange(app::player::CharacterId charId);
	bool IsCharacterAllowed(app::player::CharacterId charId) const;
	void ForceCharacterOn(app::player::CharacterId charId) const;
	static app::level::StageData::AttributeFlags GetCharacterAttributeFlag(app::player::CharacterId charId);

	void ChangeStartingCharacter(app::player::CharacterId charId, bool force);
	void ChangeCurrentCharacter(app::player::CharacterId charId);

public:
	virtual void* GetRuntimeTypeInfo() override;
	virtual bool ProcessMessage(hh::fnd::Message& message) override;
	virtual void OnAddedToGame() override;
	virtual void OnRemovedFromGame() override;
	virtual void MessageProcessedCallback(hh::game::GameManager* gameManager, const hh::fnd::Message& message) override;

	void HandleRetry();
	app::player::CharacterId GetRespawnCharacter() const;

	GAMESERVICE_CLASS_DECLARATION(CharacterSelectionManager)
};
