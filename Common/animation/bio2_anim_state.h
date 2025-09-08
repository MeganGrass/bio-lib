/*
*
*	Megan Grass
*	August 31, 2025
*
*/

#include <cstddef>

enum class AnimStatePlayerBio2 : std::size_t
{
	Walk_Backward = 0,
	Walk_Backward_Startled = 1,
	Death = 2,
	Damage_Front_Minor = 3,
	Damage_Back = 4,
	Damage_Front = 5,
	Inspect_Kneel = 6,
	Push_Begin = 7,
	Push = 8,
	Walk_Backward_Caution = 9,

	Walk_Forward = 0,
	Run = 1,
	Idle = 2,
	Walk_Forward_Caution = 3,
	Run_Caution = 4,
	Idle_Caution = 5,
	Walk_Forward_Danger = 6,
	Run_Danger = 7,
	Idle_Danger = 8,
	Aim_Begin = 9,
	Fire = 10,
	Aim = 11,
	Fire_Upward = 12,
	Aim_Upward = 13,
	Fire_Downward = 14,
	Aim_Downward = 15,
	Reload = 16,

	// non-vanilla
	Idle_Turn = 100,
	Quick_Turn = 101,
	Quick_Reload = 102
};

enum class AnimStateW00Bio2 : std::size_t
{
	Walk_Forward = 0,
	Run = 1,
	Idle = 2,
	Walk_Forward_Caution = 3,
	Run_Caution = 4,
	Idle_Caution = 5,
	Walk_Forward_Danger = 6,
	Run_Danger = 7,
	Idle_Danger = 8,
	Aim_Begin = 9,
	Fire = 10,
	Aim = 11,
	Fire_Upward = 12,
	Aim_Upward = 13,
	Fire_Downward = 14,
	Aim_Downward = 15,
	Reload = 16
};

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
