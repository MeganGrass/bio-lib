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

	using StateAction = std::function<void()>;

	using StateKey = StateType;

	explicit StateMachine(std::shared_ptr<Resident_Evil_Model> Model)
	{
		m_Model = Model;
		m_KeyState.store(Resident_Evil_Key::NONE);
	}

	void Add(StateType iState, StateAction OnEnter, StateAction OnUpdate, StateAction OnExit)
	{
		m_State[iState] = { OnEnter, OnUpdate, OnExit };
	}

	void Set(StateType iState)
	{
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

			if (m_State.find(m_CurrentState) != m_State.end())
			{
				m_State[m_CurrentState].OnEnter();
			}
		}
	}

	void Init(StateType iState, std::size_t iFrame, bool b_PlayAllFrames, bool b_PlayInReverse, bool b_Loop)
	{
		if (m_PriorState != m_CurrentState)
		{
			Model()->Speed() = { 0, 0, 0 };
		}

		Model()->SetAnimIndex(iState.GetIndex());

		Model()->iClip.store(iState.GetClip());

		Model()->iFrame.store(iFrame);

		Model()->b_PlayAllFrames.store(b_PlayAllFrames);

		Model()->b_PlayInReverse.store(b_PlayInReverse);

		Model()->b_Loop.store(b_Loop);
	}

	void Update(Resident_Evil_Key iKeyState, bool b_UpdateTransform)
	{
		m_KeyState.store(iKeyState);

		if (m_State.find(m_CurrentState) != m_State.end())
		{
			m_State[m_CurrentState].OnUpdate();

			if (b_UpdateTransform)
			{
				auto& Animation = Model()->Animation(Model()->AnimIndex());

				auto iClip = min(Model()->iClip.load(), Animation->GetClipCount() - 1);

				auto iFrame = min(Model()->iFrame.load(), Animation->GetFrameCount(iClip) - 1);

				if (Animation->Clip.empty())
				{
					return;
				}

				auto& Frame = Animation->Clip[iClip][iFrame];

				if (!iFrame) { Model()->Speed() = { 0, 0, 0 }; }

				SVECTOR2 Delta{ (Frame.Speed.x - Model()->Speed().x), (Frame.Speed.y - Model()->Speed().y), (Frame.Speed.z - Model()->Speed().z) };

				Model()->Speed() = { Frame.Speed.x, Frame.Speed.y, Frame.Speed.z };

				Model()->AddSpeedXZ((SVECTOR*)&Delta);

				Model()->ClampPosition(Model()->Position());
			}
		}
	}

	void Reset(void)
	{
		Set(StateType{});
		m_State.clear();
		m_KeyState.store(Resident_Evil_Key::NONE);
	}

	const StateType& Prior(void) const { return m_PriorState; }

	const StateType& Current(void) const { return m_CurrentState; }

	const Resident_Evil_Key KeyState(void) const { return m_KeyState.load(); }

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
};