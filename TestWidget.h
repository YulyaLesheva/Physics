#pragma once

template <typename T>
using Sprite = std::unique_ptr<T>;

class Background;
class Quad;
struct Manifold;
class Body;

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


	Body* _greyBody;
	Body* _yellowBody;

	std::vector<Body*>  CheckVector;


	///all brand new 
	std::vector<Body*> AllBodies;
	std::vector<Body*> Collider1;
	std::vector<Body*> Collider2;
	std::vector<Manifold*> Result;



};
