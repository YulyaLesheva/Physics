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
	_greyBody = Body::Create(Helper::UseTexture("GreyQuad"), FPoint(200, 200), 0.5, 0.8);
	_yellowBody = Body::Create(Helper::UseTexture("YellowQuad"), FPoint(500,200), 1.0, 0.3);

	AllBodies.push_back(_greyBody);
	AllBodies.push_back(_yellowBody);

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
	


	//auto bbbb = SapAlgorithm::SAP(AllBodies);
	//if (BodyColission::CheckColissionAndGetNormal(*_greyBody, *_yellowBody)) {
	//	BodyColission::ResolveColission(*_greyBody, *_yellowBody);
	//	//Log::Info(std::to_string(_greyBodypenetrationDepth.x) + "  " + std::to_string(_greyBody->penetrationDepth.y));
	//}
	//else {
	//	Log::Info("..");
	//}

	
	//if (BodyColission::CheckColission(mmm)) {	
	//	BodyColission::ApplyImpulse(_greyBody, _yellowBody, mmm, 3);
	//	//Log::Info(std::to_string(mmm->normal.x) + "  " + std::to_string(mmm->normal.y));
	//	Log::Info("It's a colission!");
	//}
	//else {
	//	Log::Info(".....");
	//}

	//BodyColission::CheckColissionAndGetNormal(*_yellowBody, *_greyBody);
	
	//BodyColission::SAT(_greyBody, _yellowBody);

	
	/*	auto m = BodyColission::FindCollisionFeatures(_yellowBody, _greyBody);
		Log::Info("NORMAL IS  " + std::to_string(m.mNormal.x) + "  " + std::to_string(m.mNormal.y));
		Log::Info("PENETRATION DEPTH IS  " + std::to_string(m.depth));
		
		if (m.colliding) {
			BodyColission::ApplyImpulse(_yellowBody, _greyBody, &m, 2);
			Log::Info("COLLIDING");

		}*/
		
	/*	FPoint axisXY[] = {
	FPoint(1, 0), FPoint(0, 1)
		};
		
		BodyColission::SAT(_yellowBody, _greyBody);

		bool* blyat = false;

		auto depth = BodyColission::PenetrationDepth(_yellowBody, _greyBody, *axisXY, blyat);
		Log::Info("PENETRATION DEPTH IS  " + std::to_string(depth));
*/



	Collider1.clear();
	Collider2.clear();
	Result.clear();
	
	Manifold* result(0);
	for (int i = 0; i < AllBodies.size(); ++i) {
		for (int j = i; j < AllBodies.size(); ++j) {
			if (AllBodies[i] == AllBodies[j]) continue;
			result->ResetManifold(result);
			Body* a = AllBodies[i];
			Body* b = (Body*)AllBodies[j];
			result = &BodyColission::FindCollisionFeatures(a, b);
			if (result->colliding) {
				Collider1.push_back(a);
				Collider2.push_back(b);
				Result.push_back(result);
			}
		}
	}

	for (int i = 0; i < Result.size(); ++i) {
		Body* a = Collider1[i];
		Body* b = Collider2[i];
		BodyColission::ApplyImpulse(a,b, &result[i], 1);
	}


	

	for (auto &body : AllBodies) {
		body->Update(dt);
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
	_yellowBody->MouseDown(mouse_pos);
	_greyBody->MouseDown(mouse_pos);
	return false;
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{
}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	_yellowBody->MouseUp(mouse_pos);
	_greyBody->MouseUp(mouse_pos);
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
