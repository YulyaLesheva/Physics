#include "stdafx.h"
#include "TestWidget.h"
#include "Background.h"
#include "Helper.h"
#include "Quad.h"
#include "BodyCollision.h"
#include "Body.h"
#include "SapAlgorithm.h"


TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)

{
	Init();
}

void TestWidget::Init()
{
	///_tex1 = Core::resourceManager.Get<Render::Texture>("btnStart_Text");

	_background = Background::Create(Helper::UseTexture("Background"));
	_greyBody = Body::Create(Helper::UseTexture("GreyQuad"), FPoint(200, 200), 1.9f, 1.0);
	_yellowBody = Body::Create(Helper::UseTexture("YellowQuad"), FPoint(500,200), 0.0f, 1.5);
	_DarkBlueBody = Body::Create(Helper::UseTexture("DarkBlueQuad"), FPoint(122, 200), 1.1f, 0.7);
	_PinkBody = Body::Create(Helper::UseTexture("PinkQuad"), FPoint(800, 200), 1.5f, 1.0);
	
	AllBodies.push_back(_greyBody);
	AllBodies.push_back(_yellowBody);
	AllBodies.push_back(_PinkBody);
	//AllBodies.push_back(_DarkBlueBody);

	LinearProjectionPercent = 0.8f;
	PenetrationSlack = 0.01f;
	impulseIteration = 5;
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
	

	for (int i = 0; i < AllBodies.size(); ++i) {
		for (int j = i; j < AllBodies.size(); ++j) {
			if (AllBodies[i] == AllBodies[j]) continue;
		
			Manifold result;
			result.ResetManifold(&result);
			Body* a = (Body*)AllBodies[i];
			Body* b = (Body*)AllBodies[j];
			result = BodyColission::FindCollisionFeatures(a, b);
			if (result.colliding) {
				Collider1.push_back(a);
				Collider2.push_back(b);
				Results.push_back(result);

			}
		}
	}

	for (int i = 0; i < AllBodies.size(); ++i) {
		AllBodies[i]->ApplyForces();
	}

	for (int k = 0; k < impulseIteration; ++k) {
		for (int i = 0; i < Results.size(); ++i) {
			Body* a = Collider1[i];
			Body* b = Collider2[i];
			BodyColission::ApplyImpulse(a, b, &Results[i]);
			Log::Info("Impulse applied BODY A " + std::to_string(Collider1[i]->velocity.y));
			Log::Info("Impulse applied BODY B " + std::to_string(Collider2[i]->velocity.y));
					
		}
	}

	for (auto f :AllBodies) {
		if (!f->isAwake) {
			if (f->mass != 0) {
				Log::Info("SLEEEEEEEEEEEP");

			}
		}
	}



	for (auto &body : AllBodies) {
		body->Update(dt);
	}
	
	for (int i = 0; i < Results.size(); ++i) {
		
		//Log::Info("Penetration value is " + std::to_string(Results[i].depth));
		Body* a = Collider1[i];
		Body* b = Collider2[i]; 
		
		float totalMass = a->inverseMass + b->inverseMass;
		
		if (totalMass == 0.f) continue;

		/*float depth = math::max(Results[i].depth + PenetrationSlack, 0.f) * LinearProjectionPercent;
		float scalar = depth / totalMass;
		FPoint projection = scalar * Results[i].mNormal;
		a->_pos -= a->inverseMass * projection;
		b->_pos += b->inverseMass * projection;*/

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

void TestWidget::ProcessInteraction(float dt) {
	//remove entities to remove
	//find highest variance axis for aabb
	//sorts abbs by min on highest variance axis
	// go thrpigh the list test intersection in range

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
