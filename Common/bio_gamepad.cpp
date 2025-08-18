/*
*
*	Megan Grass
*	August 15, 2025
*
*/


#pragma once

#include "bio_gamepad.h"


void Resident_Evil_Gamepad::SetTrigger(const std::string& ButtonStr, bool b_LeftTrigger, bool b_RightTrigger)
{
	auto ShoulderL = m_TriggerMapL.find(ButtonStr);
	if (ShoulderL != m_TriggerMapL.end()) { *(ShoulderL->second) = b_LeftTrigger; }

	auto ShoulderR = m_TriggerMapR.find(ButtonStr);
	if (ShoulderR != m_TriggerMapR.end()) { *(ShoulderR->second) = b_RightTrigger; }

	if (b_LeftTrigger || b_RightTrigger)
	{
		auto Button = m_ButtonMap.find(ButtonStr);
		if (Button != m_ButtonMap.end()) { *(Button->second) = 0xFFFF; }
	}
}

std::uint16_t Resident_Evil_Gamepad::GetButtonMapBit(const std::string& ButtonStr) const
{
	auto it = m_ButtonMap.find(ButtonStr);
	if (it != m_ButtonMap.end())
	{
		if (*(it->second) == 0xFFFF) { return 0xFFFF; }
		else
		{
			return static_cast<int>(std::log2(*(it->second)));
		}
	}
	return 0xFFFF;
}

bool Resident_Evil_Gamepad::GetTriggerMapL(const std::string& ButtonStr) const
{
	auto LeftShoulder = m_TriggerMapL.find(ButtonStr);
	if (LeftShoulder != m_TriggerMapL.end()) { return *(LeftShoulder->second); }
	return false;
}

bool Resident_Evil_Gamepad::GetTriggerMapR(const std::string& ButtonStr) const
{
	auto RightShoulder = m_TriggerMapR.find(ButtonStr);
	if (RightShoulder != m_TriggerMapR.end()) { return *(RightShoulder->second); }
	return false;
}

bool Resident_Evil_Gamepad::SetMapping(const std::string ButtonStr, std::function<void(bool&)> ProgressCallback)
{
#if MSTD_XINPUT
	if (!XInput()) { return false; }

	bool b_Execute = true;

	auto StartTime = std::chrono::steady_clock::now();

	while (b_Execute)
	{
		if (!XInput()->PollState(0))
		{
			if (std::chrono::steady_clock::now() - StartTime > std::chrono::seconds(2))
			{
				ProgressCallback(b_Execute = false);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10));

			continue;
		}

		const auto& Gamepad = XInput()->Gamepad();

		auto it = m_ButtonMap.find(ButtonStr);
		if (it != m_ButtonMap.end())
		{
			if (Gamepad.wButtons)
			{
				*(it->second) = Gamepad.wButtons;
				SetTrigger(ButtonStr, false, false);
			}
			else if (Gamepad.bLeftTrigger)
			{
				*(it->second) = 0xFFFF;
				SetTrigger(ButtonStr, true, false);
			}
			else if (Gamepad.bRightTrigger)
			{
				*(it->second) = 0xFFFF;
				SetTrigger(ButtonStr, false, true);
			}
		}

		ProgressCallback(b_Execute = false);
	}
#endif

	return false;
}