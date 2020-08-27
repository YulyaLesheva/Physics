#include "stdafx.h"
#include "TestWidget.h"
#include "Background.h"
#include "Helper.h"
#include "Quad.h"
#include "BodyCollision.h"
#include "Body.h"


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
	/*_blueQuad = Quad::Create(Helper::UseTexture("BlueQuad"), FPoint(Render::device.Width() * 0.5, 500));
	_yellowQuad = Quad::Create(Helper::UseTexture("YellowQuad"), FPoint(_blueQuad->GetTexture()->Width() * 0.5+10, _blueQuad->GetTexture()->Height() * 0.5+10));
	_purpleQuad = Quad::Create(Helper::UseTexture("PurpleQuad"), FPoint(Render::device.Width() * 0.5 + 400, 500));
	_greyQuad = Helper::UseTexture("GreyQuad");*/

	_background = Background::Create(Helper::UseTexture("Background"));
	_greyBody = Body::Create(Helper::UseTexture("GreyQuad"), FPoint(0, 200), 0.5, 1.9, true);
	_yellowBody = Body::Create(Helper::UseTexture("YellowQuad"), FPoint(500,200), 0.0, 0.3);

	_yellowBody->mooveble = false;
	_greyBody->mooveble = true;
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
	
	if (BodyColission::CheckColissionAndGetNormal(_greyBody, _yellowBody)) {
		BodyColission::ResolveColission(_greyBody, _yellowBody);
	}
}



bool TestWidget::MouseDown(const IPoint &mouse_pos)
{
	_yellowBody->MouseDown(mouse_pos);
	return false;
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{
}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	_yellowBody->MouseUp(mouse_pos);
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
