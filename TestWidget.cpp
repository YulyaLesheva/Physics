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

#define GRAVITY_CONST FPoint(0, -59.82f)

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
	bodyBox_b = BodyBox::Create("YellowQuad", FPoint(700, 500), 1.5); //0.1
	bodyBox_c = BodyBox::Create("Floor",FPoint(Render::device.Width() * .5f, 70), 0.f);
	bodyBox_d = BodyBox::Create("PinkQuad", FPoint(900, 500), 1.0); //0.1

	BodyBoxes = {
		bodyBox_a,
		bodyBox_b, 
		bodyBox_c,
		bodyBox_d
	};


	impulseIteration = 100;
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

void TestWidget::Update(float dt)
{
	float inv_dt = dt > 0.0f ? 1.0f / dt : 0.0f;

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

	
	Log::Info("Grey angle: " + std::to_string(bodyBox_a->rotation));
	Log::Info("Yellow angle: " + std::to_string(bodyBox_b->rotation));
	
	////add force
	for (int i = 0; i < BodyBoxes.size(); ++i) {
		BodyBox* b = BodyBoxes[i];
		b->AddForce(GRAVITY_CONST * 0.5);
	}
	
	//const float damping = 0.98;
	//FPoint acceleration = force * inverseMass;
	//velocity += acceleration * dt;
	//velocity *= damping;
	
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
}


bool TestWidget::MouseDown(const IPoint &mouse_pos)
{
	for (auto &body : AllBodies) {
		body->MouseDown(mouse_pos);
	}

	for (auto &body : BodyBoxes) {
		body->MouseDown(mouse_pos);
	}
	
	CreateQuad(mouse_pos);
	return false;
}

void TestWidget::CreateQuad(const IPoint &pos) {
	
	std::string str = RandomQuad();

	BodyBoxes.push_back(BodyBox::Create(str, FPoint(pos), 1.0, randomRotation()));
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{
	MOUSEPOS = mouse_pos;
}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	for (auto &body : AllBodies) {
		body->MouseUp(mouse_pos);
	}

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