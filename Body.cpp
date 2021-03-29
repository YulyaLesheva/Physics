#include "stdafx.h"
#include "Body.h"
#include "Line.h"



Body::Body(Render::Texture* tex) :
	_tex(tex)
{
}

Body::Body(Render::Texture* tex, FPoint& pos) :
	_tex(tex),
	_pos(pos),
	velocity(0,0),
	angle(0)
{
}

Body::~Body() {
	delete this;
}

Body* Body::Create(Render::Texture* tex) {
	return new Body(tex);
}

Body* Body::Create(Render::Texture * tex, FPoint & pos){
	return new Body(tex, pos);
}

void Body::Draw() {
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(_pos);
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), angle);
	Render::device.MatrixTranslate(_tex->_bitmap_width * -0.5f, _tex->_bitmap_height * -0.5f, 0);
	_tex->Draw();
	Render::device.PopMatrix();
}

void Body::Update(float dt) {

}

IRect& Body::GetRect() {
	_rect = IRect(IPoint(_pos.x - _tex->Width()*.5, _pos.y - _tex->Height()*.5), _tex->Width(), _tex->Height());
	return _rect;
}

FPoint& Body::GetPos() {
	return _pos;
}
Render::Texture* Body::GetTex() {
	return _tex;
}


bool Body::OnBorder() {
	auto rect = GetRect();

	if (rect.LeftBottom().x < 0 || rect.RightBottom().x > Render::device.Width()) {
		return true;
	}

	if (rect.LeftTop().y > Render::device.Height() || rect.LeftBottom().y < 0) {
		return true;
	}
	return false;
}

Intervals Body::GetInterval(Body* body, const FPoint& axis) {
	Intervals result;

	FPoint min = body->GetMin();
	FPoint max = body->GetMax();

	FPoint verst[] = {
	FPoint(min.x, min.y), FPoint(min.x, max.y),
	FPoint(max.x, max.y), FPoint(max.x, min.y)
	};

	result.min = result.max = axis.GetDotProduct(verst[0]);

	for (int i = 1; i < 4; ++i) {
		float projection = axis.GetDotProduct(verst[i]);
		if (projection < result.min) result.min = projection;
		if (projection > result.max) result.max = projection;
	}

	return result;
}

bool Body::OverlapOnAxis(Body*a, Body*b, const FPoint &axis) {
	
	Intervals iA = GetInterval(a, axis);
	Intervals iB = GetInterval(b, axis);

	return ((iB.min <= iA.max) && (iA.min <= iB.max));
}

bool Body::TestCollide(Body * body)
{
	auto bodyA = this;

	FPoint axisXY[] = {
	FPoint(1, 0), FPoint(0, 1)
	}; //добавить две дополнительные оси для вращающихся квадов позже

	for (int i = 0; i < 2; ++i) {
		if (!OverlapOnAxis(bodyA, body, axisXY[i])) {
			//Log::Info(".....");
			return false;

		}
	}
	//Log::Info("It's a colission!");
	return true;
}

void Body::KeepInBorders()
{
	auto rect = GetRect();

	if (rect.LeftBottom().x <= 0) {
		_pos.x = _tex->Width() * 0.5;
		ReverseCurrentVectorX();
	}

	if (rect.RightBottom().x >= Render::device.Width()) {
		_pos.x = Render::device.Width() - _tex->Width() * 0.5;
		ReverseCurrentVectorX();
	}

	if (rect.LeftTop().y >= Render::device.Height()) {
		_pos.y = Render::device.Height() - _tex->Height() * 0.5;
		ReverseCurrentVectorY();
	}

	if (rect.LeftBottom().y <= 0) {
		_pos.y = _tex->Height() * 0.5;
		ReverseCurrentVectorY();
	}
}

FPoint Body::GetMin() {
	
	FPoint resultMin;

	auto rect = GetRect();
	auto minX = rect.LeftBottom().x;
	auto minY = rect.LeftBottom().y;

	resultMin = FPoint(minX, minY);
	return resultMin;
}

FPoint Body::GetMax() {

	FPoint resultMax;
	
	auto rect = GetRect();
	auto maxX = rect.RightBottom().x;
	auto maxY = rect.RightTop().y;

	resultMax = FPoint(maxX, maxY);
	return resultMax;
}

void Body::ReverseCurrentVectorY() {
	velocity.y = velocity.y * -1;
}

void Body::ReverseCurrentVectorX() {
	velocity.x = velocity.x * -1;
}

std::vector<FPoint> Body::GetVertices() {
	std::vector<FPoint> verts;
	verts.resize(4);
	
	FPoint min = GetMin();
	FPoint max = GetMax();
	
	verts = {
	FPoint(min.x, max.y), FPoint(max.x, max.y), //0......1
												//.      .
	FPoint(min.x, min.y), FPoint(max.x, min.y)  //.	     .
	};											//2......3	

	return  verts;
}

std::vector<Line> Body::GetEdges() {

	std::vector<Line> result;
	result.reserve(4);

	std::vector<FPoint> vertices = GetVertices();
	// должно быть четыре ребра 
	 
	result = {
		Line(vertices[0], vertices[1]), //....0....
		Line(vertices[1], vertices[3]),// .       .
		Line(vertices[3], vertices[2]),// 3       1
		Line(vertices[2], vertices[0]),// ....2....
	};
	
	return result;
}

bool Body::PointOnPlane(const FPoint point, Line& line) {
	FPoint closest = ClosestPoint(point, line);
	float distanceSq = (closest - point).GetDotProduct(closest - point);
	return distanceSq == 0.f;
}

FPoint Body::ClosestPoint(const FPoint point, Line& line) {
	FPoint LVec = line.end - line.start; // line vector
	float t = (point - line.start).GetDotProduct(LVec) / LVec.GetDotProduct(LVec);
	t = fmaxf(t, 0.f);
	t = fminf(t, 1.f);
	return line.start + LVec * t;
}

std::vector<FPoint> Body::ClipToEdges(Body* bodyA, Body* bodyB) {
	
	std::vector<FPoint> result;
	FPoint intersection;
	//oo eto mne nado 
	auto edgesA = bodyA->GetEdges();
	auto edgesB = bodyB->GetEdges();

	result.reserve(edgesA.size());

	for (int i = 0; i < edgesA.size(); ++i) {
		for (int j = 0; j < edgesB.size(); ++j) {
			auto r = edgesA[i].lineline(edgesB[j], intersection);
			if (r) {
				//Log::Info("intersects " + std::to_string(i) + " and " + std::to_string(j));
				result.push_back(intersection);
			}
		}
	}
	return result;
}

std::vector<FPoint> Body::ClipEdges(const std::vector<Line>& edges, Body* body) {

	std::vector<FPoint> result;
	result.reserve(edges.size());
	FPoint intersection;
	
	std::vector<Line>& bodyEdges = GetEdges();

	for (int i = 0; i < bodyEdges.size(); ++i) {
		for (int j = 0; j < edges.size(); ++j) {
			if (bodyEdges[i].lineline(edges[j], intersection)) {
				auto lll = intersection;
				result.push_back(intersection);
			}
		}
	}

	return result;
}

void Body::KeyPressed(int keyCode)
{
	if (keyCode ==  VK_SPACE) {
		angle += 2;
	}
}
