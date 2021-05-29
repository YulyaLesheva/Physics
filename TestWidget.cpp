#include "stdafx.h"
#include "TestWidget.h"
#include "Background.h"
#include "Helper.h"
#include "BodyCollision.h"
#include "Body.h"
#include "SapAlgorithm.h"
#include "PhysicBody.h"
#include "Line.h"
#include "Collide.h"
#include "BodyBox.h"
#include "Math.h"
#include "Random.h"
#include "Arbiter.h"
#include "Boundaries.h"

#define GRAVITY_CONST FPoint(0, -69.82f)

using std::vector;
using std::map;
using std::pair;
typedef map<ArbiterKey, Arbiter>::iterator ArbIter;
typedef pair<ArbiterKey, Arbiter> ArbPair;


TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
	
{
	Init();
}

void TestWidget::Init()
{
	///_tex1 = Core::resourceManager.Get<Render::Texture>("btnStart_Text");

	_background = Background::Create(Helper::UseTexture("Background"));
	bodyBox_a = BodyBox::Create("GreyQuad", FPoint(300, 500), 0.0);
	bodyBox_b = BodyBox::Create("YellowQuad", FPoint(700, 500), 70.f); //0.1
	bodyBox_c = BodyBox::Create("Floor",FPoint(Render::device.Width() * .5f, 70), 0.f);
	//bodyBox_d = BodyBox::Create("PinkQuad", FPoint(900, 500), 1.0); //0.1
	
	bodyBox_up = BodyBox::Create("Width", FPoint(Render::device.Width() * .5f, Render::device.Height()), 0.0);
	//bodyBox_down = BodyBox::Create("Width", FPoint(Render::device.Width() * .5f, 0), 0.0);
	bodyBox_left = BodyBox::Create("Height", FPoint(0, Render::device.Height() * .5f), 0.0);
	bodyBox_right = BodyBox::Create("Height", FPoint(Render::device.Width(), Render::device.Height() * .5f), 0.0);
	
	bodyBox_rect1 = BodyBox::Create("YellowRect", FPoint(200, 250), 3.0);
	bodyBox_rect2 = BodyBox::Create("PinkRect", FPoint(300, 250), 3.0);
	bodyBox_rect3 = BodyBox::Create("GreenRect", FPoint(400, 250), 3.0);



	bodyBox_a->SetDegrees(-30);

	BodyBoxes = {
		bodyBox_a,
		bodyBox_b, 
		bodyBox_c,
	//	bodyBox_d,
		bodyBox_up,
	//	bodyBox_down,
		bodyBox_left,
		bodyBox_right,
		bodyBox_rect1,
		bodyBox_rect2,
		bodyBox_rect3,
	};
		
	//TEST
	impulseIteration = 10;
	
	auto starty = 150;
	for (int i = 0; i < 9; ++i) {
		CreateQuad(IPoint(915,starty), 0);
		starty += 50;
	}
}

void TestWidget::Draw()
{
	_background->Draw();
	
	for (auto *b : BodyBoxes) {
		b->Draw();
	}
	
	//Render::BeginColor(Color(29, 26, 49, 255)); // Color(Red, Green, Blue, Alpha)
	//Render::BindFont("arial");
	//Render::PrintString(FPoint(800, 650), "velocity grey: " + std::to_string(_greyBody->velocity.x )+ " " + std::to_string(_greyBody->velocity.y), 1.5f, CenterAlign);
	//Render::PrintString(FPoint(800, 600), "velocity green: " + std::to_string(_GreenLine->velocity.x )+ " " + std::to_string(_GreenLine->velocity.y), 1.5f, CenterAlign);
	//Render::PrintString(FPoint(800, 550), "impulse grey: " + std::to_string(_greyBody->WHATIMPULSE.x)+ " " + std::to_string(_greyBody->WHATIMPULSE.y), 1.5f, CenterAlign);
	//Render::PrintString(FPoint(800, 500), "impulse green: " + std::to_string(_GreenLine->WHATIMPULSE.x)+ " " + std::to_string(_GreenLine->WHATIMPULSE.y), 1.5f, CenterAlign);
	//Render::EndColor();

}

void TestWidget::SweepAndPrune() {
	for (int i = 0; i < BodyBoxes.size(); ++i) {
		BodyBox *bi = BodyBoxes[i];

		for (int j = i + 1; j < BodyBoxes.size(); ++j) {

			BodyBox* bj = BodyBoxes[j];

			if (bi->inverseMass == 0.0f && bj->inverseMass == 0.0f)
				continue;

			Arbiter newArb(bi, bj);
			ArbiterKey key(bi, bj);

			if (newArb.numContacts > 0) {
				ArbIter iter = arbiters.find(key);
				if (iter == arbiters.end())
				{
					arbiters.insert(ArbPair(key, newArb));
				}
				else {
					iter->second.Update(newArb.contactsArray, newArb.numContacts);
				}
			}
			else {
				arbiters.erase(key);
			}
		}
	}
}
void TestWidget::Update(float dt)
{
	float inv_dt = dt > 0.0f ? 1.0f / dt : 0.0f;

	//sweep and pure algorithm
	SweepAndPrune();

	//integrate forces
	for (int i = 0; i < BodyBoxes.size(); ++i) {
		BodyBox* b = BodyBoxes[i];
		if (b->inverseMass == 0.0f) continue;
		
		b->velocity += dt * (GRAVITY_CONST + b->inverseMass * b->force);
		b->angularVelocity += dt * b->invI * b->torque;
	}

	for (ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb) {
		arb->second.PreStep(inv_dt);
	}

	for (int i = 0; i < impulseIteration; ++i) {
		for (ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb) {
		arb->second.ApplyImpulse2D();
		}
	}

	//integrate velocities
	for (int i = 0; i < BodyBoxes.size(); ++i) {
		BodyBox* body = BodyBoxes[i];

		body->position += dt * body->velocity;
		body->rotation += dt * body->angularVelocity;

		body->force = FPoint(0,0);
		body->torque = 0.f;
	}
	
	for (auto *b : BodyBoxes) {
		b->Update(dt);
	}

	for (int i = 0; i < BodyBoxes.size(); ++i) {
		if (OutOfScreen(BodyBoxes[i])) {
			BodyBoxes.erase(BodyBoxes.begin() + i);
			Log::Info("erased");
		}
	}

	mousePos = Core::mainInput.GetMousePos();

	PointInBodyBox(mousePos, bodyBox_a);
}

bool TestWidget::OutOfScreen(BodyBox* body) {

	auto w = Render::device.Width();
	auto h = Render::device.Width();
	auto offset = math::max(body->width.x, body->width.y);

	if (body->position.x > w + offset) return true;
	if (body->position.x < 0 - offset) return true;
	if (body->position.y > h + offset) return true;
	if (body->position.y < 0 - offset) return true;
	
	return false;
}

bool TestWidget::MouseDown(const IPoint &mouse_pos)
{
	for (auto &body : BodyBoxes) {
		body->MouseDown(mouse_pos);
	}

	for (auto &body : BodyBoxes) {
		if (!PointInBodyBox(mouse_pos, body)) {
			CreateQuad(mouse_pos, randomRotation());

		}
		return false;
	}
	return false;

}
void TestWidget::CreateQuad(const IPoint &pos, float angle) {
	
	std::string str = RandomQuad();

	BodyBoxes.push_back(BodyBox::Create(str, FPoint(pos), 10.0, angle));
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{
	mousePos = mouse_pos;
	Log::Info(std::to_string(mousePos.x) + " " + std::to_string(mousePos.y));
}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	for (auto &body : BodyBoxes) {
		body->MouseUp(mouse_pos);
	}
}

void TestWidget::AcceptMessage(const Message& message)
{
	//
	// Виджету могут посылаться сообщения с параметрами.
	//

	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
}

void TestWidget::KeyPressed(int keyCode)
{
	//_greyBody->KeyPressed(keyCode);
	if (keyCode == VK_A) {
		bodyBox_a->KeyPressed(keyCode);
	}

	if (keyCode == VK_D) {
		bodyBox_b->KeyPressed(keyCode);
	}

	if (keyCode == VK_SPACE) {
		bodyBox_b->KeyPressed(keyCode);
		//Log::Info(std::to_string(bodyBox_b->mass));
	}
}

void TestWidget::KeyReleased(int keyCode)
{

}

void TestWidget::CharPressed(int unicodeChar)
{
	//
	// unicodeChar - Unicode код введённого символа
	//
}