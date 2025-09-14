/*
*
*	Megan Grass
*	August 31, 2025
*
*/

#include <cstddef>

// PLD
enum class AnimStatePlayerBio2 : std::size_t
{
	Walk_Backward,
	Walk_Backward_Startled,
	Death,
	Damage_Front_Minor,
	Damage_Back,
	Damage_Front,
	Inspect_Kneel,
	Push_Begin,
	Push,
	Walk_Backward_Caution
};

// PLW 00, 02, 03, 04, 05, 06, 07, 08, 0C, 0D, 0E
enum class AnimStateW00Bio2 : std::size_t
{
	Walk_Forward,
	Run,
	Idle,
	Walk_Forward_Caution,
	Run_Caution,
	Idle_Caution,
	Walk_Forward_Danger,
	Run_Danger,
	Idle_Danger,
	Aim_Begin,
	Fire,
	Aim,
	Fire_Upward,
	Aim_Upward,
	Fire_Downward,
	Aim_Downward,
	Reload
};

// PLW 01
enum class AnimStateW01Bio2 : std::size_t
{
	Walk_Forward,
	Run,
	Idle,
	Walk_Forward_Caution,
	Run_Caution,
	Idle_Caution,
	Walk_Forward_Danger,
	Run_Danger,
	Idle_Danger,
	Aim_Begin,
	Fire,
	Aim,
	Fire_Upward,
	Aim_Upward,
	Fire_Downward,
	Aim_Downward,
	Fire_Blocked
};

// PLW 09, 0A, 0B
enum class AnimStateW09Bio2 : std::size_t
{
	Walk_Forward,
	Run,
	Idle,
	Walk_Forward_Caution,
	Run_Caution,
	Idle_Caution,
	Walk_Forward_Danger,
	Run_Danger,
	Idle_Danger,
	Aim_Begin,
	Fire,
	Aim,
	Fire_Upward,
	Aim_Upward,
	Fire_Downward,
	Aim_Downward
};

// PLW 0E
enum class AnimStateW0EBio2 : std::size_t
{
	Walk_Forward,
	Run,
	Idle,
	Walk_Forward_Caution,
	Run_Caution,
	Idle_Caution,
	Walk_Forward_Danger,
	Run_Danger,
	Idle_Danger,
	Aim_Begin,
	Fire,
	Fire_Charged,
	Aim,
	Aim_Downward,
	Aim_Downward_Copy_00,
	Aim_Downward_Copy_01,
	Reload
};

// PLW 0F, 10
enum class AnimStateW0FBio2 : std::size_t
{
	Walk_Forward,
	Run,
	Idle,
	Walk_Forward_Caution,
	Run_Caution,
	Idle_Caution,
	Walk_Forward_Danger,
	Run_Danger,
	Idle_Danger,
	Aim_Begin,
	Fire,
	Fire_Empty,
	Aim,
	Fire_Upward,
	Fire_Upward_Empty,
	Aim_Upward,
	Fire_Downward,
	Fire_Downward_Empty,
	Aim_Downward,
	Reload
};

// PLW 11
enum class AnimStateW11Bio2 : std::size_t
{
	Walk_Forward,
	Run,
	Idle,
	Walk_Forward_Caution,
	Run_Caution,
	Idle_Caution,
	Walk_Forward_Danger,
	Run_Danger,
	Idle_Danger,
	Aim_Begin,
	Fire,
	Aim
};

// PLW 12
enum class AnimStateW12Bio2 : std::size_t
{
	Walk_Forward,
	Run,
	Idle,
	Walk_Forward_Caution,
	Run_Caution,
	Idle_Caution,
	Walk_Forward_Danger,
	Run_Danger,
	Idle_Danger,
	Aim_Begin,
	Fire_Begin,
	Fire,
	Aim,
	Fire_End
};

// PLW 13
enum class AnimStateW13Bio2 : std::size_t
{
	Walk_Forward,
	Run,
	Idle,
	Walk_Forward_Caution,
	Run_Caution,
	Idle_Caution,
	Walk_Forward_Danger,
	Run_Danger,
	Idle_Danger,
	Aim_Begin,
	Fire,
	Aim,
	Fire_Upward,
	Aim_Upward,
	Fire_Downward,
	Aim_Downward,
	Reload,
	Idle_Pose_Begin,
	Idle_Pose
};

// EM 10
enum class AnimStateEM10Bio2 : std::size_t
{
	Walk = 0,
	Walk_Fast = 1,
	Walk_Loose = 2,
	Walk_Loose_Fast = 3,
	Walk_Reach0 = 4,
	Walk_Reach0_Fast = 5,
	Walk_Reach1 = 6,
	Walk_Reach1_Fast = 7,

	Idle = 0,
	Fall_Front = 1,
	Fall_Back = 2,
	Damage_Back_Major = 3,
	Damage_Front_Major = 4,
	Crawl = 5,
	Crawl_Damage = 6,
	Crawl_Death = 7,
	Stand_Up_Front = 8,
	Stand_Up_Back = 9,
	Leap = 10,
	Bite_Begin = 11,
	Bite = 12,
	Bite_End = 13,
	Bite_Leg_Begin = 14,
	Bite_Leg = 15,
	Bite_Leg_End = 16,
	Vomit = 17,
	Kneel_Eat0 = 18,
	Kneel_Eat1 = 19,
	Kneel_Eat_Fast = 20,
	Stand_Up_Kneel = 21,
	Twitch_Back = 22,
	Twitch_Front = 23,
	Kneel = 24,
	Reach_Begin = 25,
	Reach = 26,
	Reach_End = 27,
	Damage_Front = 28,
	Damage_Side = 29,
	Damage_Back = 30
};
