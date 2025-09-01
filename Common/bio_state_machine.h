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

enum class AnimationIndex : std::size_t;

enum class Resident_Evil_Key : std::int32_t;

typedef class Resident_Evil_Model BioModel;

template <typename StateType>
class StateMachine {
public:

	using StateAction = std::function<void()>;
	using StateKey = std::pair<AnimationIndex, StateType>;

	explicit StateMachine(std::shared_ptr<Resident_Evil_Model> _Model)
	{
		m_Model = _Model;
		m_KeyState = Resident_Evil_Key::NOTHING;
	}

	void Add(AnimationIndex Index, StateType iState, StateAction OnEnter, StateAction OnUpdate, StateAction OnExit)
	{
		States[{ Index, iState }] = { OnEnter, OnUpdate, OnExit };
	}

	void Set(AnimationIndex Index, StateType iState)
	{
		if (m_CurrentState != iState || m_CurrentIndex != Index)
		{
			if (m_CurrentState != StateType{})
			{
				States[{m_CurrentIndex, m_CurrentState}].OnExit();
			}

			m_PriorIndex = m_CurrentIndex;
			m_PriorState = m_CurrentState;

			m_CurrentIndex = Index;
			m_CurrentState = iState;

			States[{m_CurrentIndex, m_CurrentState}].OnEnter();
		}
	}

	void Init(AnimationIndex Index, StateType iClip, size_t iFrame, bool b_PlayAllFrames, bool b_PlayInReverse, bool b_Loop)
	{
		if (m_PriorState != m_CurrentState)
		{
			Model()->Speed() = { 0, 0, 0 };
		}

		Model()->SetAnimIndex(Index);

		Model()->iClip.store(std::to_underlying(iClip));

		Model()->iFrame.store(iFrame);

		Model()->b_PlayAllFrames.store(b_PlayAllFrames);

		Model()->b_PlayInReverse.store(b_PlayInReverse);

		Model()->b_Loop.store(b_Loop);
	}

	void Update(Resident_Evil_Key KeyState, bool b_UpdateTransform)
	{
		m_KeyState = KeyState;

		if (States.find({ m_CurrentIndex, m_CurrentState }) != States.end())
		{
			States[{ m_CurrentIndex, m_CurrentState }].OnUpdate();

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

				if (!iFrame) { Model()->Speed() = {}; }

				SVECTOR2 Delta{ (Frame.Speed.x - Model()->Speed().x), (Frame.Speed.y - Model()->Speed().y), (Frame.Speed.z - Model()->Speed().z) };

				Model()->Speed() = { Frame.Speed.x, Frame.Speed.y, Frame.Speed.z };

				Model()->AddSpeedXZ((SVECTOR*)&Delta);

				Model()->ClampPosition(Model()->Position());
			}
		}
	}

	const StateType Prior(void) const { return m_PriorState; }

	const StateType Current(void) const { return m_CurrentState; }

	const Resident_Evil_Key KeyState(void) const { return m_KeyState; }

	const std::shared_ptr<Resident_Evil_Model>& Model(void) const { return m_Model; }

private:

	struct StateProc
	{
		StateAction OnEnter;
		StateAction OnUpdate;
		StateAction OnExit;
	};

	struct StateHash
	{
		std::size_t operator()(const StateKey& k) const noexcept
		{
			std::size_t h1 = std::hash<int>{}(static_cast<int>(k.first));
			std::size_t h2 = std::hash<StateType>{}(k.second);
			return h1 ^ (h2 + 0x9e3779b97f4a7c15ull + (h1 << 6) + (h1 >> 2));
		}
	};

	AnimationIndex m_PriorIndex{ AnimationIndex::Normal };
	AnimationIndex m_CurrentIndex{ AnimationIndex::Normal };
	StateType m_PriorState{};
	StateType m_CurrentState{};

	std::shared_ptr<Resident_Evil_Model> m_Model;

	std::unordered_map<StateKey, StateProc, StateHash> States;

	Resident_Evil_Key m_KeyState{ Resident_Evil_Key::NONE };
};