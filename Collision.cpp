/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       Collision.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Authored Functions
*//*_________________________________________________________________________*/

#include "Main.h"
#include <iostream>

/******************************************************************************/
/*!

*/
/******************************************************************************/
bool CollisionIntersection_PointRect(float PointX, float PointY, const AABB& aabb)
{
	//if (worldMouseX - ButtonInstance->boundingBox.min.x >= 0 )	//	mouse touch from left
	//if (worldMouseX - ButtonInstance->boundingBox.max.x >= 0 )	//	mouse touch from right
	//if (worldMouseY - ButtonInstance->boundingBox.max.y <= 0)		// mouse touch from top
	//if (worldMouseY - ButtonInstance->boundingBox.min.y >= 0)		// mouse touch from bottom
	if ((PointX - aabb.min.x >= 0 && PointX - aabb.max.x <= 0) &&
		(PointY - aabb.max.y <= 0 && PointY - aabb.min.y >= 0))
		return 1;
	else
		return 0;
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
	const AABB& aabb2, const AEVec2& vel2)
{
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