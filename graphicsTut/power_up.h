#pragma once

#include "entity.h"

class PowerUp : public Entity
{
public:
	PowerUp();
	~PowerUp();

	virtual void init(MainGame * mainGame) override;
	virtual void update(MainGame * mainGame, float deltaTime) override;
	virtual void draw(Bengine::SpriteBatch & spriteBatch) override;

	void init(glm::vec2 position, glm::vec2 size, Bengine::ColorRGBA8 color, const std::string& name);

	// GETTER SETTER
	const std::string& get_name() const { return m_name; }
	
	// STATIC FUNCTIONS
	static const std::vector<std::string>& get_powerUpNames() { return m_powerUpNames; }

private:
	Bengine::ColorRGBA8 m_color;
	std::string m_name;
	std::string m_textureName;

	static const std::vector<std::string> m_powerUpNames;
};