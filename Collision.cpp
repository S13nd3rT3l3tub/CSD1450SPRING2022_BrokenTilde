/******************************************************************************/
/*!
\file		Collision.cpp
\author 	DigiPen
\par    	email: digipen\@digipen.edu
\date   	January 01, 20xx
\brief		ToDo: give a brief explanation here

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "main.h"
#include <iostream>

/**************************************************************************/
/*!

	*/
	/**************************************************************************/
bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
	const AABB& aabb2, const AEVec2& vel2)
{
	//UNREFERENCED_PARAMETER(aabb1);
	//UNREFERENCED_PARAMETER(vel1);
	//UNREFERENCED_PARAMETER(aabb2);
	//UNREFERENCED_PARAMETER(vel2);

	/*
	Implement the collision intersection over here.

	The steps are:
	Step 1: Check for static collision detection between rectangles (before moving).
				If the check returns no overlap you continue with the following next steps (dynamics).
				Otherwise you return collision true

	Step 2: Initialize and calculate the new velocity of Vb
			tFirst = 0
			tLast = dt

	Step 3: Working with one dimension (x-axis).
			if(Vb < 0)
				case 1
				case 4
			if(Vb > 0)
				case 2
				case 3

			case 5

	Step 4: Repeat step 3 on the y-axis

	Step 5: Otherwise the rectangles intersect
	*/

	/*if (aabb1.min.x > aabb2.max.x)
		return false;
	if (aabb1.min.y > aabb2.max.y)
		return false;
	if (aabb2.min.x > aabb1.max.x)
		return false;
	if (aabb2.min.y > aabb1.max.y)
		return false;*/
	if (aabb1.max.x < aabb2.min.x)
		return false;
	if (aabb1.min.x > aabb2.max.x)
		return false;
	if (aabb1.max.y < aabb2.min.y)
		return false;
	if (aabb1.min.y > aabb2.max.y)
		return false;


	float tFirst{ 0 }, tLast{ g_dt }, tTmp{ 0 };
	AEVec2 Vb{};
	Vb.x = vel2.x - vel1.x;
	Vb.y = vel2.y - vel1.y;

	// ------ X axis -----
	if (Vb.x < 0) {
		if (aabb1.min.x > aabb2.max.x) // Case 1
			return false;
		if (aabb1.max.x < aabb2.min.x) {// Case 4
			tTmp = (aabb1.max.x - aabb2.min.x) / Vb.x;
			tFirst = tTmp > tFirst ? tTmp : tFirst;
		}
		if (aabb1.min.x < aabb2.max.x) {// Case 4
			tTmp = (aabb1.min.x - aabb2.max.x) / Vb.x;
			tLast = tTmp < tLast ? tTmp : tLast;
		}
	}
	if (Vb.x > 0) {
		if (aabb1.min.x > aabb2.max.x) {// Case 2
			tTmp = (aabb1.min.x - aabb2.max.x) / Vb.x;
			tFirst = tTmp > tFirst ? tTmp : tFirst;
		}
		if (aabb1.max.x > aabb2.min.x) {// Case 2
			tTmp = (aabb1.max.x - aabb2.min.x) / Vb.x;
			tLast = tTmp < tLast ? tTmp : tLast;
		}
		if (aabb1.max.x < aabb2.min.x) // Case 3
			return false;
	}
	if (tFirst > tLast) // Case 5
		return false;

	// ------ Y axis -----
	if (Vb.y < 0) {
		if (aabb1.min.y > aabb2.max.y) // Case 1
			return false;
		if (aabb1.max.y < aabb2.min.y) {// Case 4
			tTmp = (aabb1.max.y - aabb2.min.y) / Vb.y;
			tFirst = tTmp > tFirst ? tTmp : tFirst;
		}
		if (aabb1.min.y < aabb2.max.y) {// Case 4
			tTmp = (aabb1.min.y - aabb2.max.y) / Vb.y;
			tLast = tTmp < tLast ? tTmp : tLast;
		}
	}
	if (Vb.y > 0) {
		if (aabb1.min.y > aabb2.max.y) {// Case 2
			tTmp = (aabb1.min.y - aabb2.max.y) / Vb.y;
			tFirst = tTmp > tFirst ? tTmp : tFirst;
		}
		if (aabb1.max.y > aabb2.min.y) {// Case 2
			tTmp = (aabb1.max.y - aabb2.min.y) / Vb.y;
			tLast = tTmp < tLast ? tTmp : tLast;
		}
		if (aabb1.max.y < aabb2.min.y) // Case 3
			return false;
	}
	if (tFirst > tLast) // Case 5
		return false;

	return true;
}