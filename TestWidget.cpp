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

TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)

{
	Init();
}

void TestWidget::Init()
{
	///_tex1 = Core::resourceManager.Get<Render::Texture>("btnStart_Text");

	_background = Background::Create(Helper::UseTexture("Background"));

	/*_greyBody = PhysicBody::Create(Helper::UseTexture("GreyQuad"), FPoint(200, 800), 1.0f, 1.5);
	_yellowBody = PhysicBody::Create(Helper::UseTexture("YellowQuad"), FPoint(500, 200), 1.1f, 1.0);
	_DarkBlueBody = PhysicBody::Create(Helper::UseTexture("DarkBlueQuad"), FPoint(122, 200), 1.1f, 1.2);
	_PinkBody = PhysicBody::Create(Helper::UseTexture("PinkQuad"), FPoint(800, 200), 1.5f, 0.95);
	_physicBody = PhysicBody::Create(Helper::UseTexture("Floor"), FPoint(800, 70), 0.f, 1.5f);
	_GreenLine = PhysicBody::Create(Helper::UseTexture("GreenLine"), FPoint(Render::device.Width() * .5f, 70), 0.f, 1.5f);*/

	bodyBox_a = BodyBox::Create("GreyQuad", FPoint(100,100), 2);
	bodyBox_b = BodyBox::Create("YellowQuad", FPoint(300,100), 2);
	

	//AllBodies.push_back(_greyBody);
	//AllBodies.push_back(_yellowBody);
	//AllBodies.push_back(_PinkBody);
	//AllBodies.push_back(_GreenLine);
	//AllBodies.push_back(_physicBody);
	//AllBodies.push_back(_DarkBlueBody);

	LinearProjectionPercent = 0.25f;
	PenetrationSlop = 0.1f;
	impulseIteration = 10;
}

void TestWidget::Draw()
{
	_background->Draw();
	
	for (auto &body : AllBodies) {
		body->Draw();
	}

	bodyBox_a->Draw();
	bodyBox_b->Draw();
	
	
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
	
	bodyBox_a->Update(dt);
	bodyBox_b->Update(dt);

	auto Av = bodyBox_a->GetVertices();
	auto Bv = bodyBox_b->GetVertices();

	ABBcollideABB(bodyBox_a, bodyBox_b);

	//Log::Info("grey " + std::to_string(bodyBox_a->rotationValue));
	//Log::Info("yellow " + std::to_string(bodyBox_b->rotationValue));


	Collider1.clear();
	Collider2.clear();
	Results.clear();

	for (int i = 0; i < AllBodies.size(); ++i) {
		for (int j = i; j < AllBodies.size(); ++j) {
			if (i == j) continue;
			Manifold result;
			result.ResetManifold(&result);
			PhysicBody* a = (PhysicBody*)AllBodies[i];
			PhysicBody* b = (PhysicBody*)AllBodies[j];
			result = BodyCollision::FindCollisionFeatures(a, b);
			if (result.colliding) {
				Collider1.push_back(AllBodies[i]);
				Collider2.push_back(AllBodies[j]);
				Results.push_back(result);
			}
		}
	}

	for (auto &b : AllBodies) {
		b->ApplyForces();
	}

	for (int k = 0; k < impulseIteration; ++k) {
		for (int i = 0; i < Results.size(); ++i) {
			for (int j = 0; j < Results[i].contacts.size(); ++j) {
				PhysicBody* a = Collider1[i];
				PhysicBody* b = Collider2[i];
				BodyCollision::ApplyImpulse(a, b, &Results[i], j);
			}
		}
	}

	for (auto &body : AllBodies) {
		body->Update(dt);
	}

	for (auto &body : AllBodies) {
		body->UpdatePosition(dt);
	}

	for (int i = 0; i < Results.size(); ++i) {
		//Log::Info("Penetration value is " + std::to_string(Results[i].depth));
		PhysicBody* a = Collider1[i];
		PhysicBody* b = Collider2[i];

		float totalMass = a->inverseMass + b->inverseMass;

		if (totalMass == 0.f) continue;

		FPoint corr = math::max(Results[i].depth - PenetrationSlop, 0.0f) / totalMass * LinearProjectionPercent * Results[i].normal;
		a->_pos -= a->inverseMass * corr;
		b->_pos += b->inverseMass * corr;

		/*float totalMass = a->inverseMass + b->inverseMass;
		if (totalMass == 0.0f) continue;

		float depth = fmaxf(Results[i].depth - PenetrationSlack, 0.0f);
		float scalar = (totalMass == 0.0f) ? 0.0f : depth / totalMass;
		FPoint correction = Results[i].mNormal * scalar * LinearProjectionPercent;

		a->_pos = a->_pos - correction * a->inverseMass;
		b->_pos = b->_pos + correction * b->inverseMass;
*/
	}

	for (auto &body : AllBodies) {
		body->KeepInBorders();
	}
}


bool TestWidget::MouseDown(const IPoint &mouse_pos)
{
	for (auto &body : AllBodies) {
		body->MouseDown(mouse_pos);
	}

	bodyBox_a->MouseDown(mouse_pos);
	bodyBox_b->MouseDown(mouse_pos);
	return false;
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{
}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	for (auto &body : AllBodies) {
		body->MouseUp(mouse_pos);
	}

	bodyBox_a->MouseUp(mouse_pos);
	bodyBox_b->MouseUp(mouse_pos);
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