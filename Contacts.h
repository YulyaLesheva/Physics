#pragma once

struct Contacts
{
	Contacts() : Pn(0.0f), Pt(0.0f), Pnb(0.0f) {}
	FPoint position;
	FPoint normal;
	FPoint r1, r2;
	float separation;
	float Pn;	// accumulated normal impulse
	float Pt;	// accumulated tangent impulse
	float Pnb;	// accumulated normal impulse for position bias
	float massNormal, massTangent;
	float bias;
};