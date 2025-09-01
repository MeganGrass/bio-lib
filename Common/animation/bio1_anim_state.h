/*
*
*	Megan Grass
*	August 31, 2025
*
*/

#include <cstddef>

enum class AnimStatePlayerBio1 : std::size_t
{
	Idle,
	Damage_Front,
	Walk_Backward_Startled,
	Walk_Backward,
	Death,
	Aim_Knife,
	Aim_Knife_Upward,
	Aim_Knife_Downward,
	Aim_Handgun,
	Aim_Handgun_Upward,
	Aim_Handgun_Downward,
	Aim_Shotgun,
	Aim_Shotgun_Upward,
	Aim_Shotgun_Downward,
	Aim_Revolver,
	Aim_Revolver_Upward,
	Aim_Revolver_Downward,
	Aim_DumDum,
	Aim_DumDum_Upward,
	Aim_DumDum_Downward,
	Aim_Flamethrower,
	Aim_Flamethrower_Upward,
	Aim_Flamethrower_Downward,
	Aim_Bazooka_Explosive,
	Aim_Bazooka_Explosive_Upward,
	Aim_Bazooka_Explosive_Downward,
	Aim_Bazooka_Acid,
	Aim_Bazooka_Acid_Upward,
	Aim_Bazooka_Acid_Downward,
	Aim_Bazooka_Flame,
	Aim_Bazooka_Flame_Upward,
	Aim_Bazooka_Flame_Downward,
	Aim_Rocket_Launcher,
	Aim_Rocket_Launcher_Upward,
	Aim_Rocket_Launcher_Downward,

	Idle_Pose_Begin = 0,
	Idle_Pose = 1,
	Walk_Forward = 2,
	Run = 3,
	Inspect_Kneel = 4,
	Aim_Begin = 5,
	Fire = 6,
	Aim = 7,
	Aim_Upward_Begin = 8,
	Fire_Upward = 9,
	Aim_Upward = 10,
	Aim_Downward_Begin = 11,
	Fire_Downward = 12,
	Aim_Downward = 13,
	Reload = 14,

	// non-vanilla
	Idle_Turn = 100,
	Quick_Turn = 101,
	Quick_Reload = 102
};