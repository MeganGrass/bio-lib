/*
*
*	Megan Grass
*	September 12, 2025
*
*/


#pragma once

#include <cstddef>

#include <animation/bio_state_machine.h>

#include <animation/bio1_anim_state.h>

#include <animation/bio2_anim_state.h>

#include <animation/bio2_anim_state_nov96.h>

#include <animation/bio3_anim_state.h>

using StateMachineType = StateMachine<StateType>;

enum class AnimStateCustom : std::size_t
{
	Dummy = 100,
	Idle_Turn = 101,
	Quick_Turn = 102,
	Quick_Reload = 103
};

inline StateType Idle;
inline StateType Idle_Pose_Begin;
inline StateType Idle_Pose;
inline StateType Idle_Pose_Extra;
inline StateType Idle_Caution;
inline StateType Idle_Danger;
inline StateType Idle_Turn;

inline StateType Quick_Turn;
inline StateType Quick_Reload;

inline StateType Walk_Backward;
inline StateType Walk_Backward_Startled;
inline StateType Walk_Backward_Caution;
inline StateType Walk_Backward_Danger;
inline StateType Walk_Forward;
inline StateType Walk_Forward_Caution;
inline StateType Walk_Forward_Danger;

inline StateType Run;
inline StateType Run_Caution;
inline StateType Run_Danger;

inline StateType Aim_Begin;
inline StateType Aim_Upward_Begin;
inline StateType Aim_Downward_Begin;
inline StateType Aim;
inline StateType Aim_Upward;
inline StateType Aim_Downward;

inline StateType Fire_Begin;
inline StateType Fire_Upward_Begin;
inline StateType Fire_Downward_Begin;
inline StateType Fire_End;
inline StateType Fire_Upward_End;
inline StateType Fire_Downward_End;
inline StateType Fire_Empty;
inline StateType Fire_Upward_Empty;
inline StateType Fire_Downward_Empty;
inline StateType Fire;
inline StateType Fire_Upward;
inline StateType Fire_Downward;
inline StateType Fire_Blocked;

inline StateType Equip;

inline StateType Reload;

inline StateType Inspect_Kneel;

inline StateType Push_Begin;
inline StateType Push;

inline StateType Damage_Front_Minor;
inline StateType Damage_Back;
inline StateType Damage_Front;

inline StateType Death;