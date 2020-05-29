#include "stdafx.h"
#include "TestWidget.h"
#include "Background.h"
#include "Helper.h"
#include "Quad.h"

TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)

{
	Init();
}

void TestWidget::Init()
{
	///_tex1 = Core::resourceManager.Get<Render::Texture>("btnStart_Text");
	_background = Background::Create(Helper::UseTexture("Background"));
	_blueQuad = Quad::Create(Helper::UseTexture("BlueQuad"), IPoint(Render::device.Width() * 0.5, 500));
	_pinkQuad = Quad::Create(Helper::UseTexture("PinkQuad"), IPoint(Render::device.Width() * 0.5, (Render::device.Height()*0.5)-50));

}

void TestWidget::Draw()
{
	_background->Draw();
	_blueQuad->Draw();
	_pinkQuad->Draw();
}

void TestWidget::Update(float dt)
{
	_blueQuad->Update(dt);
	_pinkQuad->Update(dt);

	auto rect1 = _blueQuad->GetRect();
	auto rect2 = _pinkQuad->GetRect();

	if (rect1.Intersects(rect2)) {
		Log::Warn("TOUCHTOUCHTUOUCH");
	}
	
	else {
		Log::Warn("........");
	}
	
	//Log::Info(std::to_string(_blueQuad->GetPos().x));
}

bool TestWidget::MouseDown(const IPoint &mouse_pos)
{
	
	return false;
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{
}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	
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
	_blueQuad->KeyPressed(keyCode);
}

void TestWidget::KeyReleased(int keyCode)
{
	_blueQuad->KeyReleased(keyCode);
}

void TestWidget::CharPressed(int unicodeChar)
{
	//
	// unicodeChar - Unicode код введённого символа
	//

	if (unicodeChar == L'а') {
		// Реакция на ввод символа 'а'
	}
}
