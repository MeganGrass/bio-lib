/*
*
*	Megan Grass
*	August 30, 2025
*
*/


#pragma once

#include <gte/lib_gte.h>

#include <vector>

#include <memory>

#include <utility>

#include <functional>

#include <unordered_map> 

enum class AnimationIndex : std::size_t;

enum class Resident_Evil_Key : std::int32_t;

typedef class Resident_Evil_Model BioModel;

class StateType {
public:

	StateType(void) : m_AnimIndex(AnimationIndex{}), m_iClip(0), m_Name("null") {}

	template <typename EnumType1, typename EnumType2, typename = std::enable_if_t<std::is_enum_v<EnumType1> && std::is_enum_v<EnumType2>>>
	explicit StateType(EnumType1 iAnimIndex, EnumType2 iClip, const std::string& sName = "") :
		m_AnimIndex(iAnimIndex),
		m_iClip(static_cast<std::underlying_type_t<EnumType2>>(iClip)),
		m_Name(sName)
	{
	}

	const AnimationIndex& GetIndex(void) const { return m_AnimIndex; }
	const std::size_t& GetClip(void) const { return m_iClip; }
	const std::string& GetName(void) const { return m_Name; }

	bool operator==(const StateType& State) const { return ((m_AnimIndex == State.m_AnimIndex) && (m_iClip == State.m_iClip) && (m_Name == State.m_Name)); }
	bool operator!=(const StateType& State) const { return !(*this == State); }

private:
	AnimationIndex m_AnimIndex;
	std::size_t m_iClip;
	std::string m_Name;
};

namespace std {
	template <>
	struct hash<StateType>
	{
		std::size_t operator()(const StateType& State) const noexcept
		{
			using AnimUT = std::underlying_type_t<AnimationIndex>;
			const AnimUT iAnimIndex = static_cast<AnimUT>(State.GetIndex());
			const std::size_t iClip = State.GetClip();
			const std::size_t h1 = std::hash<AnimUT>{}(iAnimIndex);
			const std::size_t h2 = std::hash<std::size_t>{}(iClip);
			const std::size_t h3 = std::hash<std::string>{}(State.GetName());
			std::size_t seed = h1;
			seed ^= h2 + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2);
			seed ^= h3 + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}

template <typename StateType>
class StateMachine {
public:

	bool b_IdleTurn;
	bool b_QuickTurn;
	bool b_Inspect;
	bool b_Reloading;
	bool b_AimBegin;
	bool b_Aiming;
	bool b_Firing;
	bool b_FireBegin;
	bool b_FireEnd;
	bool b_Damage;
	bool b_Alive;

	bool b_Sleep;

	using StateAction = std::function<void()>;

	using StateKey = StateType;

	explicit StateMachine(std::shared_ptr<Resident_Evil_Model> Model)
	{
		m_Model = Model;
		m_KeyState.store(Resident_Evil_Key::NONE);
		b_IdleTurn = false;
		b_QuickTurn = false;
		b_Inspect = false;
		b_Reloading = false;
		b_AimBegin = false;
		b_Aiming = false;
		b_Firing = false;
		b_FireBegin = false;
		b_FireEnd = false;
		b_Damage = false;
		b_Alive = false;

		b_Sleep = false;
	}

	void Add(StateType iState, StateAction OnEnter, StateAction OnUpdate, StateAction OnExit)
	{
		m_State[iState] = { OnEnter, OnUpdate, OnExit };
	}

	void Set(StateType iState)
	{
		//if (b_Sleep) { return; }

		if (m_CurrentState != iState)
		{
			if (m_CurrentState != StateType{})
			{
				if (m_State.find(m_CurrentState) != m_State.end())
				{
					m_State[m_CurrentState].OnExit();
				}
			}

			m_PriorState = m_CurrentState;

			m_CurrentState = iState;

			b_IdleTurn = (m_CurrentState.GetName() == "Idle_Turn");
			b_QuickTurn = (m_CurrentState.GetName() == "Quick_Turn");
			b_Inspect = (m_CurrentState.GetName() == "Inspect_Kneel");
			b_Reloading = (m_CurrentState.GetName() == "Reload" || m_CurrentState.GetName() == "Quick_Reload");
			b_AimBegin = (m_CurrentState.GetName() == "Aim_Begin" || m_CurrentState.GetName() == "Aim_Upward_Begin" || m_CurrentState.GetName() == "Aim_Downward_Begin");
			b_Aiming = (m_CurrentState.GetName() == "Aim" || m_CurrentState.GetName() == "Aim_Upward" || m_CurrentState.GetName() == "Aim_Downward");
			b_Firing = (m_CurrentState.GetName() == "Fire" || m_CurrentState.GetName() == "Fire_Upward" || m_CurrentState.GetName() == "Fire_Downward");
			b_FireBegin = (m_CurrentState.GetName() == "Fire_Begin" || m_CurrentState.GetName() == "Fire_Upward_Begin" || m_CurrentState.GetName() == "Fire_Downward_Begin");
			b_FireEnd = (m_CurrentState.GetName() == "Fire_End" || m_CurrentState.GetName() == "Fire_Upward_End" || m_CurrentState.GetName() == "Fire_Downward_End");
			b_Damage = (m_CurrentState.GetName() == "Damage_Front_Minor" || m_CurrentState.GetName() == "Damage_Back" || m_CurrentState.GetName() == "Damage_Front");
			b_Alive = (m_CurrentState.GetName() != "Death");

			if (m_State.find(m_CurrentState) != m_State.end())
			{
				m_State[m_CurrentState].OnEnter();
			}
		}
	}

	void Init(StateType iState, std::size_t iFrame, bool b_PlayAllFrames, bool b_PlayInReverse, bool b_Loop)
	{
		m_Model->ResetFrameCounter();

		m_Model->SetAnimIndex(iState.GetIndex());

		m_Model->SetClip(iState.GetClip());

		m_Model->SetFrame(iFrame);

		m_Model->b_PlayAllFrames.store(b_PlayAllFrames);

		m_Model->b_PlayInReverse.store(b_PlayInReverse);

		m_Model->b_Loop.store(b_Loop);
	}

	void Update(Resident_Evil_Key iKeyState)
	{
		using KeyUT = std::underlying_type_t<Resident_Evil_Key>;
		const KeyUT Current = static_cast<KeyUT>(iKeyState);
		const KeyUT Previous = static_cast<KeyUT>(m_KeyState.load());
		const Resident_Evil_Key Trigger = static_cast<Resident_Evil_Key>(static_cast<KeyUT>(Current & ~Previous));

		m_KeyState.store(iKeyState);
		m_KeyStateTrigger.store(Trigger);

		if (m_State.find(m_CurrentState) != m_State.end())
		{
			m_State[m_CurrentState].OnUpdate();

			if (m_KeyState == Resident_Evil_Key::NONE || m_Model->b_EditorMode || b_IdleTurn || b_AimBegin || b_Aiming || b_FireBegin || b_FireEnd || b_QuickTurn || b_Reloading)
			{
				return;
			}

			if (b_Firing && (!m_Model->b_WeaponKickback.load() || m_Model->b_WeaponKickbackComplete.load()))
			{
				return;
			}

			auto& Frame = m_Model->Frame();

			SVECTOR2 Delta{ (Frame.Speed.x - m_Model->Speed().x), (Frame.Speed.y - m_Model->Speed().y), (Frame.Speed.z - m_Model->Speed().z) };

			m_Model->Speed() = { Frame.Speed.x, Frame.Speed.y, Frame.Speed.z };

			m_Model->AddSpeedXZ((SVECTOR*)&Delta);

			m_Model->ClampPosition(m_Model->Position());
		}
	}

	void Reset(void)
	{
		Set(StateType{});
		m_State.clear();
		m_KeyState.store(Resident_Evil_Key::NONE);
		m_KeyStateTrigger.store(Resident_Evil_Key::NONE);
	}

	const StateType& Prior(void) const { return m_PriorState; }

	const StateType& Current(void) const { return m_CurrentState; }

	const Resident_Evil_Key KeyState(void) const { return m_KeyState.load(); }

	const Resident_Evil_Key KeyStateTrigger(void) { return m_KeyStateTrigger.exchange(Resident_Evil_Key::NONE); }

	const std::shared_ptr<Resident_Evil_Model>& Model(void) const { return m_Model; }

private:

	struct StateProc
	{
		StateAction OnEnter;
		StateAction OnUpdate;
		StateAction OnExit;
	};

	StateType m_PriorState{};

	StateType m_CurrentState{};

	std::shared_ptr<Resident_Evil_Model> m_Model;

	std::unordered_map<StateKey, StateProc> m_State;

	std::atomic<Resident_Evil_Key> m_KeyState{ Resident_Evil_Key::NONE };

	std::atomic<Resident_Evil_Key> m_KeyStateTrigger{ Resident_Evil_Key::NONE };
};