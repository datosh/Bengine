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
	void kill() { m_alive = false; }
	bool is_dead() { return !m_alive; }
	bool is_alive() { return m_alive; }

	// STATIC FUNCTIONS
	static const std::vector<std::string>& get_powerUpNames() { return m_powerUpNames; }

private:
	Bengine::ColorRGBA8 m_color;
	std::string m_name;
	std::string m_textureName;

	bool m_alive;

	static const std::vector<std::string> m_powerUpNames;
};

// Delte if the brick is marked dead or is out of screen
struct DeleteIfPowerUpDead
{
	void operator()(PowerUp* &ptr) const
	{
		auto brk = *ptr;
		bool brk_dead = brk.is_dead();
		bool brk_out_of_screen = brk.get_position().y < -50;
		if (brk_dead || brk_out_of_screen) {
			PowerUp* tmp = ptr;
			ptr = nullptr;
			delete tmp;
		}
	}
};
