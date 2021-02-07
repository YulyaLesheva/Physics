#pragma once

template <typename T>
using Sprite = std::unique_ptr<T>;

class Background;
class Quad;
struct Manifold;
class Body;
class PhysicBody;

class TestWidget : public GUI::Widget
{
public:
	TestWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void Draw() override;
	void Update(float dt) override;
	void ProcessInteraction(float dt);
	
	void AcceptMessage(const Message& message) override;
	
	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseMove(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

	void KeyPressed(int keyCode) override;
	void CharPressed(int unicodeChar) override;
	void KeyReleased(int keyCode) override;


private:
	void Init();

private:
	Sprite<Background> _background;
	Sprite<Quad> _blueQuad;
	Sprite<Quad> _purpleQuad;
	Sprite<Quad> _yellowQuad;
	Render::Texture *_greyQuad;


	PhysicBody* _greyBody;
	PhysicBody* _yellowBody;
	PhysicBody* _DarkBlueBody;
	PhysicBody* _PinkBody;
	PhysicBody* _Floor;
	PhysicBody* _physicBody;
	std::vector<PhysicBody*>  CheckVector;
	Body* _justBody;

	///all brand new 
	std::vector<PhysicBody*> AllBodies;
	std::vector<PhysicBody*> Collider1;
	std::vector<PhysicBody*> Collider2;
	std::vector<Manifold> Results;

	float LinearProjectionPercent;
	float PenetrationSlack;
	int impulseIteration;
};
