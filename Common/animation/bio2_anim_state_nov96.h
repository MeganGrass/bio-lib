/*
*
*	Megan Grass
*	August 31, 2025
*
*/

#include <cstddef>

// PLD
enum class AnimStatePlayerBio2Nov96 : std::size_t
{
	Walk_Backward,
	Walk_Backward_Startled,
	Climb_Up,
	Climb_Down,
	Falling,
	Landing,
	Damage_Above,
	Death,
	Damage_Front,
	Damage_Back,
	Damage_Front_Minor,
	Inspect_Kneel,
	Damage_Laying,
	Knock_Backward_Begin,
	Knock_Backward_End,
	Knock_Forward_Begin,
	Knock_Forward_End,
	Push_Begin,
	Push,
	Stairs_Begin,
	Stairs_Ascend,
	Stairs_Descend,
	Idle_Caution,
	Idle_Danger,
};

// PLW 00, 02, 04, 06, 0E, 0F, 12
enum class AnimStateW00Bio2Nov96 : std::size_t
{
	Run,
	Idle_Pose_Begin,
	Idle_Pose,
	Idle_Pose_Extra,
	Aim_Begin,
	Fire,
	Aim,
	Fire_Upward,
	Aim_Upward,
	Fire_Downward,
	Aim_Downward
};

// PLW 01
enum class AnimStateW01Bio2Nov96 : std::size_t
{
	Run,
	Idle_Pose_Begin,
	Idle_Pose,
	Idle,
	Idle_Pose_Extra,
	Walk_Forward,
	Aim_Begin,
	Fire,
	Aim,
	Fire_Upward,
	Aim_Upward,
	Fire_Downward,
	Aim_Downward,
	Equip
};

// PLW 03, 05, 07, 08, 0D
enum class AnimStateW03Bio2Nov96 : std::size_t
{
	Run,
	Idle,
	Idle_Pose,
	Idle_Copy,
	Idle_Pose_Extra,
	Walk_Forward,
	Aim_Begin,
	Fire,
	Aim,
	Fire_Upward,
	Aim_Upward,
	Fire_Downward,
	Aim_Downward,
	Reload
};

// PLW 09, 0A, 0B
enum class AnimStateW09Bio2Nov96 : std::size_t
{
	Run,
	Idle,
	Idle_Pose,
	Idle_Copy,
	Idle_Pose_Extra,
	Walk_Forward,
	Aim_Begin,
	Fire,
	Aim,
	Fire_Upward,
	Aim_Upward,
	Fire_Downward,
	Aim_Downward
};

// PLW 0C, 13
enum class AnimStateW0CBio2Nov96 : std::size_t
{
	Run,
	Idle,
	Idle_Pose,
	Idle_Copy,
	Idle_Pose_Extra,
	Walk_Forward,
	Aim_Begin,
	Fire,
	Fire_Empty,
	Aim,
	Fire_Upward,
	Fire_Upward_Empty,
	Aim_Upward,
	Fire_Downward,
	Fire_Downward_Empty,
	Aim_Downward
};

// PLW 10, 11, 14
enum class AnimStateW10Bio2Nov96 : std::size_t
{
	Run,
	Idle_Pose_Begin,
	Idle_Pose,
	Idle_Pose_Extra,
	Aim_Begin,
	Fire,
	Fire_Empty,
	Aim,
	Fire_Upward,
	Fire_Upward_Empty,
	Aim_Upward,
	Fire_Downward,
	Fire_Downward_Empty,
	Aim_Downward
};