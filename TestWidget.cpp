#include "stdafx.h"
#include "TestWidget.h"
#include "Background.h"
#include "Helper.h"
#include "Quad.h"
#include "BodyCollision.h"
#include "Body.h"
#include "SapAlgorithm.h"


	FPoint ABOVE = FPoint(500, 500);
	FPoint LEFT = FPoint(500, 500);
	FPoint RIGHT = FPoint(500, 500);
	FPoint UNDER = FPoint(500, 500);


TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)

{
	Init();
}

void TestWidget::Init()
{
	///_tex1 = Core::resourceManager.Get<Render::Texture>("btnStart_Text");

	_background = Background::Create(Helper::UseTexture("Background"));
	_greyBody = Body::Create(Helper::UseTexture("GreyQuad"), FPoint(200, 200), 0.5, 1.9, true);
	_yellowBody = Body::Create(Helper::UseTexture("YellowQuad"), FPoint(500,200), 0.0, 0.9);

	_yellowBody->mooveble = false;
	_greyBody->mooveble = true;

	mmm = Manifold::Create();

	mmm->bodyOne = _greyBody;
	mmm->bodyTwo = _yellowBody;
	

	CheckVector.push_back(_yellowBody);
	CheckVector.push_back(_greyBody);
	


}

void TestWidget::Draw()
{
	_background->Draw();
	_greyBody->Draw();
	_yellowBody->Draw();
}

void TestWidget::Update(float dt)
{
	_greyBody->Update(dt);
	_yellowBody->Update(dt);
	

	auto bbbb = SapAlgorithm::SAP(CheckVector);
	//if (BodyColission::CheckColissionAndGetNormal(*_greyBody, *_yellowBody)) {
	//	BodyColission::ResolveColission(*_greyBody, *_yellowBody);
	//	//Log::Info(std::to_string(_greyBodypenetrationDepth.x) + "  " + std::to_string(_greyBody->penetrationDepth.y));
	//}
	//else {
	//	Log::Info("..");
	//}

	if (BodyColission::CheckColission(mmm)) {
		BodyColission::ResolveCollide(mmm);
	}
	Log::Info(std::to_string(_greyBody->velocity.x)  + " " + std::to_string(_greyBody->velocity.y));
	
	_greyBody->KeepInBorders();
	_yellowBody->KeepInBorders();
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
