/*
*
*	Megan Grass
*	August 31, 2025
*
*/

#include <cstddef>

enum class AnimStatePlayerBio2Nov96 : std::size_t
{
	Walk_Backward = 0,
	Walk_Backward_Startled = 1,
	Climb_Up = 2,
	Climb_Down = 3,
	Falling = 4,
	Landing = 5,
	Damage_Above = 6,
	Death = 7,
	Damage_Front = 8,
	Damage_Back = 9,
	Damage_Front_Minor = 10,
	Inspect_Kneel = 11,
	Damage_Laying = 12,
	Knock_Backward_Begin = 13,
	Knock_Backward_End = 14,
	Knock_Forward_Begin = 15,
	Knock_Forward_End = 16,
	Push_Begin = 17,
	Push = 18,
	Step_Forward = 19,
	Stairs_Ascend = 20,
	Stairs_Descend = 21,
	Idle_Caution = 22,
	Idle_Danger = 23,

	Run = 0,
	Idle_Begin = 1,
	Idle = 2,
	Idle_Pose_Begin = 3,
	Idle_Pose = 4,
	Walk_Forward = 5,
	Aim_Begin = 6,
	Fire = 7,
	Aim = 8,
	Fire_Upward = 9,
	Aim_Upward = 10,
	Fire_Downward = 11,
	Aim_Downward = 12,
	Reload = 13,

	// non-vanilla
	Idle_Turn = 100,
	Quick_Turn = 101,
	Quick_Reload = 102
};