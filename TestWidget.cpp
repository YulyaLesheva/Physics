#include "stdafx.h"
#include "TestWidget.h"
#include "Background.h"
#include "Helper.h"
#include "Quad.h"
#include "BodyCollision.h"
#include "Body.h"
#include "SapAlgorithm.h"
#include "PhysicBody.h"
#include "Line.h"

TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)

{
	Init();
}

void TestWidget::Init()
{
	///_tex1 = Core::resourceManager.Get<Render::Texture>("btnStart_Text");

	_background = Background::Create(Helper::UseTexture("Background"));

	_greyBody = PhysicBody::Create(Helper::UseTexture("GreyQuad"), FPoint(200, 200), 1.9f, 0.95);
	_yellowBody = PhysicBody::Create(Helper::UseTexture("YellowQuad"), FPoint(500, 200), 3.1f, 0.95);
	_DarkBlueBody = PhysicBody::Create(Helper::UseTexture("DarkBlueQuad"), FPoint(122, 200), 1.1f, 0.2);
	_PinkBody = PhysicBody::Create(Helper::UseTexture("PinkQuad"), FPoint(800, 200), 2.5f, 0.95);
	_physicBody = PhysicBody::Create(Helper::UseTexture("Floor"), FPoint(800, 70), 0.f, 1.5f);
	_GreenLine = PhysicBody::Create(Helper::UseTexture("GreenLine"), FPoint(Render::device.Width() * .5f, 70), 0.f, 0.95f);

	AllBodies.push_back(_greyBody);
	//AllBodies.push_back(_yellowBody);
	//AllBodies.push_back(_PinkBody);
	AllBodies.push_back(_GreenLine);
	//AllBodies.push_back(_physicBody);
	//AllBodies.push_back(_DarkBlueBody);

	LinearProjectionPercent = 0.25f;
	PenetrationSlack = 0.01f;
	impulseIteration = 18;

	

}

void TestWidget::Draw()
{
	_background->Draw();
	for (auto &body : AllBodies) {
		body->Draw();
	}
}

void TestWidget::Update(float dt)
{
	Collider1.clear();
	Collider2.clear();
	Results.clear();

	//auto greyEdges = _greyBody->GetEdges();
	//auto greenEdges = _GreenLine->GetEdges();
	//
	//auto res = _greyBody->ClipEdges(greyEdges, _GreenLine);
	////_greyBody->ClipEdges(greyEdges, _GreenLine);
	//for (int i = 0; i < greyEdges.size(); ++i) {
	//	for (int j = 0; j < greenEdges.size(); ++j) {
	//		auto result = greyEdges[i].lineline(greenEdges[j]);
	//		if (result) {
	//			Log::Info("intersects " + std::to_string(i) + " and " + std::to_string(j));
	//		}
	//		else {
	//			break;	
	//		}
	//	}
	//}
	
	//auto p = _greyBody->ClipToEdges(_greyBody, _yellowBody);
	//auto p1 = _greyBody->ClipToEdges(_yellowBody, _greyBody);
	

	for (int i = 0; i < AllBodies.size(); ++i) {
		for (int j = i; j < AllBodies.size(); ++j) {
			if (AllBodies[i] == AllBodies[j]) continue;
			Manifold result;
			result.ResetManifold(&result);
			PhysicBody* a = (PhysicBody*)AllBodies[i];
			PhysicBody* b = (PhysicBody*)AllBodies[j];
			result = BodyColission::FindCollisionFeatures(a, b);
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
			auto jSize = Results[i].contacts.size();
			for (int j = 0; j < jSize; ++j) {
				PhysicBody* a = Collider1[i];
				PhysicBody* b = Collider2[i];
				BodyColission::ApplyImpulse(a, b, &Results[i], j);
			}
		}
	}

	for (auto &body : AllBodies) {
		body->Update(dt);
	}


	for (int i = 0; i < Results.size(); ++i) {
		//Log::Info("Penetration value is " + std::to_string(Results[i].depth));
		PhysicBody* a = Collider1[i];
		PhysicBody* b = Collider2[i];

		float totalMass = a->inverseMass + b->inverseMass;

		if (totalMass == 0.f) continue;

		const float percent = 0.2;
		const float slop = 0.01;
		FPoint corr = math::max(Results[i].depth - slop, 0.0f) / totalMass * percent * Results[i].mNormal;
		a->_pos -= a->inverseMass * corr;
		b->_pos += b->inverseMass * corr;
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