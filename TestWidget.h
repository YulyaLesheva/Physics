#pragma once
template <typename T>
using Sprite = std::unique_ptr<T>;

class Background;
class Quad;
struct Line;
struct Manifold;
class Body;
class BodyBox;
class PhysicBody;
struct Arbiter;

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

	PhysicBody* _greyBody;
	PhysicBody* _yellowBody;
	PhysicBody* _DarkBlueBody;
	PhysicBody* _PinkBody;
	PhysicBody* _Floor;
	PhysicBody* _physicBody;
	PhysicBody* _GreenLine;
	std::vector<PhysicBody*>  CheckVector;
	Body* _justBody;

	BodyBox *bodyBox_a;
	BodyBox *bodyBox_b;
	///all brand new 
	std::vector<PhysicBody*> AllBodies;
	std::vector<PhysicBody*> Collider1;
	std::vector<PhysicBody*> Collider2;
	std::vector<Manifold> Results;

	std::vector<BodyBox*> BodyBoxes;

	//sleep
	std::vector<PhysicBody*> SleepBodies;
	std::vector<std::vector<PhysicBody*>> CollideVectorToCheckForSleep;
	std::vector<std::vector<PhysicBody*>> PairsOfSleepingBodies;
	

	float LinearProjectionPercent;
	float PenetrationSlop;
	int impulseIteration;

	Line* lineA;
	Line* lineB;

};
