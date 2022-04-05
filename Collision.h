/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       BinaryMap.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Authored Functions
*//*_________________________________________________________________________*/

#ifndef COLLISION_H_
#define COLLISION_H_

#include "AEEngine.h"

/**************************************************************************/
/*!

	*/
/**************************************************************************/
struct AABB
{
	//AEVec2	c; // center
	//float  r[2]; // holds half width and half height
	
	AEVec2	min;
	AEVec2	max;
};

bool CollisionIntersection_RectRect(const AABB & aabb1, const AEVec2 & vel1, 
									const AABB & aabb2, const AEVec2 & vel2);

bool CollisionIntersection_PointRect(float PointX, float PointY, const AABB& aabb);


#endif // CSD1130_COLLISION_H_