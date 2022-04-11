/*!
@copyright	All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
@project	Ricochet
@file       Collision.h
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Initial creation
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)
@role		Added Point vs Rect axis aligned static intersection check for
			mouse click in buttons
*//*_________________________________________________________________________*/

// ----- Include Guard -----
#ifndef COLLISION_H_
#define COLLISION_H_

// ----- Include Files -----
#include "AEEngine.h"

/**************************************************************************/
/*!
	AABB Struct Definition
*/
/**************************************************************************/
struct AABB
{
	//AEVec2	c; // center
	//float  r[2]; // holds half width and half height
	
	AEVec2	min;
	AEVec2	max;
};

/**************************************************************************/
/*!
	Collision Intersection Check between 2 Dynamic (Axis-Aligned) Rectangles 
*/
/**************************************************************************/
bool CollisionIntersection_RectRect(const AABB & aabb1, const AEVec2 & vel1, 
									const AABB & aabb2, const AEVec2 & vel2);

/**************************************************************************/
/*!
	Collision Intersection Check between a Point and a (Axis-Aligned) Rectangle
*/
/**************************************************************************/
bool CollisionIntersection_PointRect(float PointX, float PointY, const AABB& aabb);

#endif