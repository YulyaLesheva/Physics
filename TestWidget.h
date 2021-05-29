#pragma once
template <typename T>
using Sprite = std::unique_ptr<T>;
enum { MAX_BOUND = 4 };

#include <vector>
#include <map>

class Background;
class Quad;
struct Line;
struct Manifold;
class Body;
class BodyBox;
class PhysicBody;
struct Arbiter;
struct ArbiterKey;
struct Contact;
struct Math;
struct Boundaries;


class TestWidget : public GUI::Widget
{
public:
	TestWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void Draw() override;
	void Update(float dt) override;
	
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

	BodyBox *bodyBox_a;
	BodyBox *bodyBox_b;
	BodyBox *bodyBox_c;
	BodyBox *bodyBox_d;
	BodyBox *bodyBox_e;
	BodyBox *bodyBox_up;
	BodyBox *bodyBox_down;
	BodyBox *bodyBox_left;
	BodyBox *bodyBox_right;
	
	BodyBox *bodyBox_rect1;
	BodyBox *bodyBox_rect2;
	BodyBox *bodyBox_rect3;
	

	void SweepAndPrune();
	void CreateQuad(const IPoint &pos, float angle);
	bool OutOfScreen(BodyBox* body);
	///all brand new 
	std::vector<BodyBox*> BodyBoxes;
	
	std::vector<Arbiter> Arbiters;
	std::map<ArbiterKey, Arbiter> arbiters;
	
	int impulseIteration;

	FPoint mousePos;

};
