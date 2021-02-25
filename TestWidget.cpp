#include "stdafx.h"
#include "TestWidget.h"
#include "Background.h"
#include "Helper.h"
#include "Quad.h"
#include "BodyCollision.h"
#include "Body.h"
#include "SapAlgorithm.h"
#include "PhysicBody.h"


TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)

{
	Init();
}

void TestWidget::Init()
{
	///_tex1 = Core::resourceManager.Get<Render::Texture>("btnStart_Text");

	_background = Background::Create(Helper::UseTexture("Background"));

	_greyBody = PhysicBody::Create(Helper::UseTexture("GreyQuad"), FPoint(200, 200), 1.9f, 1.0);
	_yellowBody = PhysicBody::Create(Helper::UseTexture("YellowQuad"), FPoint(500, 200), 0.0f, 1.5);
	_DarkBlueBody = PhysicBody::Create(Helper::UseTexture("DarkBlueQuad"), FPoint(122, 200), 1.1f, 0.7);
	_PinkBody = PhysicBody::Create(Helper::UseTexture("PinkQuad"), FPoint(800, 200), 2.5f, 1.0);
	_physicBody = PhysicBody::Create(Helper::UseTexture("Floor"), FPoint(800, 70), 0.f, 1.5f);
	_GreenLine = PhysicBody::Create(Helper::UseTexture("GreenLine"), FPoint(Render::device.Width() * .5f, 70), 0.f, 1.5f);

	AllBodies.push_back(_greyBody);
	AllBodies.push_back(_yellowBody);
	AllBodies.push_back(_PinkBody);
	AllBodies.push_back(_GreenLine);
	//AllBodies.push_back(_physicBody);
	//AllBodies.push_back(_DarkBlueBody);

	LinearProjectionPercent = 0.8f;
	PenetrationSlack = 0.1f;
	impulseIteration = 4;

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
	SleepBodies.clear();

	for (int i = 0; i < AllBodies.size(); ++i) {
		for (int j = i; j < AllBodies.size(); ++j) {
			if (AllBodies[i] == AllBodies[j]) continue;
			Manifold result;
			result.ResetManifold(&result);
			PhysicBody* a = (PhysicBody*)AllBodies[i];
			PhysicBody* b = (PhysicBody*)AllBodies[j];
			result = BodyColission::FindCollisionFeatures(a, b);
			if (result.colliding) {
				Collider1.push_back(a);
				Collider2.push_back(b);
				Results.push_back(result);
			}
		}
	}

	//all current sleeping bodies 
	for (int i = 0; i < AllBodies.size(); ++i) {
		auto body = AllBodies[i];
		if (!body->IsAwake()) {
			SleepBodies.push_back(body);
		}
	}

	//мы берем спящие объекты и проверяем их коллизию, если коллизия есть, то они остаются спящими, но если нет, то мы их будим.
	//для этого нам нужен предположительно нужен вектор, в котором мы будем хранить пары спящих объектов. 
	//итерацию на проверку коллизий мы будем делать по этому вектору 

	//all current sleeping bodies  #2

	//for (int i = 0; i < AllBodies.size(); ++i) {
	//	auto body = AllBodies[i];
	//	if (!body->IsAwake()) {
	//		if (std::find(SleepBodies.begin(), SleepBodies.end(), body) != SleepBodies.end()) {
	//			/* v contains x */
	//			continue;
	//		}
	//		else {
	//			SleepBodies.push_back(body);
	//		}
	//	}
	//	else {
	//		//это сохранить, но  пока это работает неправильно, из-за функции управления мышкой
	//		auto l = std::find(SleepBodies.begin(), SleepBodies.end(), body);
	//		if (l != SleepBodies.end()) SleepBodies.erase(l);
	//	}
	//}

	if (!_greyBody->IsAwake()) Log::Info("grey sleep");
	else {
		Log::Info("grey not sleep");
	}

	if (!_yellowBody->IsAwake()) Log::Info("yellow sleep");
	else {
		Log::Info("yellow not sleep");
	}

	if (!_PinkBody->IsAwake()) Log::Info("pink sleep");
	else {
		Log::Info("pink not sleep");
	}

	// проверяем коллизии между спящими объектами и образуем пары спящих объектов
	for (int i = 0; i < SleepBodies.size(); ++i) {
		for (int j = i; j < SleepBodies.size(); ++j) {
			if (SleepBodies[i] == SleepBodies[j]) continue;
			std::vector<PhysicBody*> sleepPair;
			PhysicBody* a = (PhysicBody*)SleepBodies[i];
			PhysicBody* b = (PhysicBody*)SleepBodies[j];
			if (BodyColission::SAT(a, b)) {
				sleepPair.push_back(a);
				sleepPair.push_back(b);
				if (std::find(PairsOfSleepingBodies.begin(), PairsOfSleepingBodies.end(), sleepPair) != PairsOfSleepingBodies.end()) {
					continue;
				}
				else {
					PairsOfSleepingBodies.push_back(sleepPair);
				}
			}
		}
	}

	//проверка не перестали ли сопрекасаться пары, возможно придется переместить функцию в другое место(не вижу разницы до апдейт или после)
	for (int i = 0; i < PairsOfSleepingBodies.size(); ++i) {
		if (PairsOfSleepingBodies[i].size() > 2) break;
		PhysicBody* a = PairsOfSleepingBodies[i].front();
		PhysicBody* b = PairsOfSleepingBodies[i].back();
		if (!BodyColission::SAT(a, b)) {
			//Log::Info("no more penetrated");
			a->SetAwake(true);
			b->SetAwake(true);
			PairsOfSleepingBodies.erase(PairsOfSleepingBodies.cbegin() + i);
		}
		else {
			//Log::Info("still penetrating");
		}
	}

	for (int k = 0; k < impulseIteration; ++k) {
		for (int i = 0; i < Results.size(); ++i) {
			PhysicBody* a = Collider1[i];
			PhysicBody* b = Collider2[i];
			BodyColission::ApplyImpulse(a, b, &Results[i]);
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

	/*for (int i = 0; i < AllBodies.size(); ++i) {
		if (!AllBodies[i]->IsAwake()) {
			for (int j = i; j < AllBodies.size(); ++j) {
				if (AllBodies[i] == AllBodies[j]) continue;
				if (!BodyColission::SAT(AllBodies[i], AllBodies[j])) {
				//добавить в бул вектор значения, если все значения фолс, то значит ни с кем не пересекается и ее надо будить.
				}
			}
		}
	}*/


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