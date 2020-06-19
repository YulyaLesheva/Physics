#include "stdafx.h"
#include "TestWidget.h"
#include "Background.h"
#include "Helper.h"
#include "Quad.h"



TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name),
	_LinePos(IPoint(100, 600)),
	_BlueQuadPos(IPoint(100, 200)),
	_QuadMass(0),
	_LineMass(70.5),
	_inverseLineMass(1/_LineMass),
	_turbo(0)
	

{
	Init();
}

void TestWidget::Init()
{
	///_tex1 = Core::resourceManager.Get<Render::Texture>("btnStart_Text");
	_background = Background::Create(Helper::UseTexture("Background"));
	//_blueQuad = Quad::Create(Helper::UseTexture("BlueQuad"), FPoint(Render::device.Width() * 0.5, 500), Gravity::_TRUE);
	//_pinkQuad = Quad::Create(Helper::UseTexture("PinkQuad"), FPoint(Render::device.Width() * 0.5, (Render::device.Height()*0.5)-50));

	_BlueQuad = Helper::UseTexture("BlueQuad");
	_Line = Helper::UseTexture("PinkQuad");

	if (_QuadMass == 0) {
		_inverseQuadMass = 0;
	}
	else {
		_inverseQuadMass = 1 / _QuadMass;
	}
}

void TestWidget::Draw()
{
	_background->Draw();
	/*_blueQuad->Draw();
	_pinkQuad->Draw();*/

	Render::device.PushMatrix();
	Render::device.MatrixTranslate(_BlueQuadPos);
	_BlueQuad->Draw();
	Render::device.PopMatrix();	
	
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(_LinePos);
	_Line->Draw();
	Render::device.PopMatrix();

}

void TestWidget::Update(float dt)
{
	//_blueQuad->Update(dt);
	//_pinkQuad->Update(dt);

	////auto rect1 = _blueQuad->GetRect();
	////auto rect2 = _pinkQuad->GetRect();

	////if (rect1.Intersects(rect2)) {
	////	Log::Warn("TOUCHTOUCHTUOUCH");
	////}
	////
	////else {
	////	Log::Warn("........");
	////}
	//_blueQuad->Collide(_pinkQuad, _blueQuad);
	//
	_LinePos.y += _turbo;

	_LineRect = IRect(_LinePos, _Line->Width(), _Line->Height());
	_BlueQuadRect = IRect(_BlueQuadPos, _BlueQuad->Width(), _BlueQuad->Height());

	_LinePos.y -= 0.2;

	if (_BlueQuadRect.Intersects(_LineRect) ){
		Log::Info("TOUCH");
	}

	else {
		Log::Info("....");
	}
	
	

	if (_BlueQuadRect.Intersects(_LineRect)) {
		math::Vector3  rv = math::Vector3(_BlueQuadPos.x - _LinePos.x,
		_BlueQuadPos.y - _LinePos.y, 0).Normalized(); // relative velocity 

		auto  velAlongNormal = rv.DotProduct(math::Vector3(0, 1, 0));

		if (velAlongNormal > 0) return;

		float e = 0.5f;

		float  j = -(1 + e) * velAlongNormal;
		j /= _inverseQuadMass + _inverseLineMass;

		math::Vector3 impulse = j * math::Vector3(0, 1, 0);
		_BlueQuadPos -= IPoint(_inverseQuadMass * impulse.x, _inverseQuadMass * impulse.y);
		_LinePos += IPoint(_inverseLineMass * impulse.x, _inverseLineMass * impulse.y);

	}
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

	if (keyCode == VK_W) {
		_turbo = 10;
	}
}

void TestWidget::KeyReleased(int keyCode)
{
	_blueQuad->KeyReleased(keyCode);

	if (keyCode == VK_W) {
		_turbo = 0;
	}
}

void TestWidget::CharPressed(int unicodeChar)
{
	//
	// unicodeChar - Unicode код введённого символа
	//

	
}
