/*
*
*	Megan Grass
*	August 31, 2025
*
*/

#include <cstddef>

// EMD (Player)
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
	Aim_Rocket_Launcher_Downward
};

// EMW 00, 0B
enum class AnimStateW00Bio1 : std::size_t
{
	Idle_Pose_Begin,
	Idle_Pose,
	Walk_Forward,
	Run,
	Inspect_Kneel
};

// EMW 01
enum class AnimStateW01Bio1 : std::size_t
{
	Idle_Pose_Begin,
	Idle_Pose,
	Walk_Forward,
	Run,
	Inspect_Kneel,
	Aim_Begin,
	Fire,
	Fire_Upward,
	Fire_Downward,
	Aim,
	Fire_Begin,
	Aim_Upward,
	Fire_Upward_Begin,
	Aim_Downward,
	Fire_Downward_Begin
};

// EMW 02, 03, 04, 06, 0F
enum class AnimStateW02Bio1 : std::size_t
{
	Idle_Pose_Begin,
	Idle_Pose,
	Walk_Forward,
	Run,
	Inspect_Kneel,
	Aim_Begin,
	Fire,
	Aim,
	Aim_Upward_Begin,
	Fire_Upward,
	Aim_Upward,
	Aim_Downward_Begin,
	Fire_Downward,
	Aim_Downward,
	Reload
};

// EMW 05
enum class AnimStateW05Bio1 : std::size_t
{
	Idle_Pose_Begin,
	Idle_Pose,
	Walk_Forward,
	Run,
	Inspect_Kneel,
	Aim_Begin,
	Fire_Begin,
	Aim,
	Aim_Upward_Begin,
	Fire_Upward_Begin,
	Aim_Upward,
	Aim_Downward_Begin,
	Fire_Downward_Begin,
	Aim_Downward,
	Fire,
	Fire_Upward,
	Fire_Downward
};

// EMW 07
enum class AnimStateW07Bio1 : std::size_t
{
	Idle_Pose_Begin,
	Idle_Pose,
	Walk_Forward,
	Run,
	Inspect_Kneel,
	Aim_Begin,
	Fire,
	Aim,
	Aim_Upward_Begin,
	Fire_Upward,
	Aim_Upward,
	Aim_Downward_Begin,
	Fire_Downward,
	Aim_Downward
};