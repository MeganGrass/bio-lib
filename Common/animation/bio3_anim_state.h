/*
*
*	Megan Grass
*	August 31, 2025
*
*/

#include <cstddef>

enum class AnimStatePlayerBio3 : std::size_t
{
	Walk_Backward = 0,
	Death = 1,
	Damage_Front_Minor = 2,
	Damage_Back = 3,
	Damage_Front = 4,
	Inspect_Kneel = 5,
	Push_Begin = 6,
	Push = 7,
	Knock_Forward_Begin = 8,
	Knock_Forward_End = 9,
	Knock_Backward_Major_Begin = 10,
	Knock_Backward_Major_End = 11,
	Knock_Backward = 12,
	Dodge_Backward_Begin = 13,
	Dodge_Backward = 14,
	Dodge_Rightward = 15,
	Dodge_Leftward = 16,
	Dodge_Push_Begin = 17,
	Dodge_Push = 18,
	Dodge_Rightward_Roll = 19,
	Dodge_Leftward_Roll = 20,
	Idle_Pose = 21,

	Walk_Forward = 0,
	Run = 1,
	Idle = 2,
	Walk_Forward_Caution = 3,
	Run_Caution = 4,
	Idle_Caution = 5,
	Walk_Forward_Danger = 6,
	Run_Danger = 7,
	Idle_Danger = 8,
	Walk_Backward_Startled = 9,
	Walk_Backward_Caution = 10,
	Idle_Pose_Alt_Begin = 11,
	Idle_Pose_Alt = 12,
	Dodge_Aim_Begin = 13,
	Dodge_Fire = 14,
	Dodge_Aim = 15,
	Dodge_Aim_End = 16,
	Dodge_End = 17,
	Aim_Begin = 18,
	Fire = 19,
	Aim = 20,
	Fire_Upward = 21,
	Aim_Upward = 22,
	Fire_Downward = 23,
	Aim_Downward = 24,
	Reload = 25
};