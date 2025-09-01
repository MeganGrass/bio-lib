/*
*
*	Megan Grass
*	November 21, 2024
*
*/


#pragma once

#include <std_cstyle.h>

#include <functional>

#ifdef MSTD_XINPUT
#include <std_xinput.h>
#endif

// Resident Evil 2 values for G.Key
enum class Resident_Evil_Key : std::int32_t
{
	NONE = (0 << 0),	// non-vanilla
	UP = (1 << 0),
	RIGHT = (1 << 1),
	DOWN = (1 << 2),
	LEFT = (1 << 3),
	AIM_UP = (1 << 4),
	AIM_DOWN = (1 << 5),
	AIM_FIRE = (1 << 6),
	INSPECT = (1 << 7),
	AIM = (1 << 8),
	RUN = (1 << 9),
	PREV = (1 << 10),
	NEXT = (1 << 11),
	CONFIRM = (1 << 12),
	CANCEL = (1 << 13),
	STATUS = (1 << 14),
	NOTHING = (1 << 15)
};

static Resident_Evil_Key operator | (Resident_Evil_Key _Mode0, Resident_Evil_Key _Mode1)
{
	return static_cast<Resident_Evil_Key>(std::to_underlying(_Mode0) | std::to_underlying(_Mode1));
}

static Resident_Evil_Key operator |= (Resident_Evil_Key& _Mode0, Resident_Evil_Key _Mode1)
{
	_Mode0 = static_cast<Resident_Evil_Key>(std::to_underlying(_Mode0) | std::to_underlying(_Mode1));
	return _Mode0;
}

static Resident_Evil_Key operator & (Resident_Evil_Key _Mode0, Resident_Evil_Key _Mode1)
{
	return static_cast<Resident_Evil_Key>(std::to_underlying(_Mode0) & std::to_underlying(_Mode1));
}

static Resident_Evil_Key operator &= (Resident_Evil_Key& _Mode0, Resident_Evil_Key _Mode1)
{
	_Mode0 = static_cast<Resident_Evil_Key>(std::to_underlying(_Mode0) & std::to_underlying(_Mode1));
	return _Mode0;
}

static Resident_Evil_Key operator ^ (Resident_Evil_Key _Mode0, Resident_Evil_Key _Mode1)
{
	return static_cast<Resident_Evil_Key>(std::to_underlying(_Mode0) ^ std::to_underlying(_Mode1));
}

static Resident_Evil_Key operator ^= (Resident_Evil_Key& _Mode0, Resident_Evil_Key _Mode1)
{
	_Mode0 = static_cast<Resident_Evil_Key>(std::to_underlying(_Mode0) ^ std::to_underlying(_Mode1));
	return _Mode0;
}

static Resident_Evil_Key operator ~ (Resident_Evil_Key _Mode)
{
	return static_cast<Resident_Evil_Key>(~std::to_underlying(_Mode));
}

static Resident_Evil_Key operator ! (Resident_Evil_Key _Mode)
{
	return static_cast<Resident_Evil_Key>(!std::to_underlying(_Mode));
}

/*
	Resident Evil Controller Input
*/
class Resident_Evil_Gamepad {
private:

	// Resident Evil Key Constants
	static constexpr Resident_Evil_Key UP = Resident_Evil_Key::UP;
	static constexpr Resident_Evil_Key RIGHT = Resident_Evil_Key::RIGHT;
	static constexpr Resident_Evil_Key DOWN = Resident_Evil_Key::DOWN;
	static constexpr Resident_Evil_Key LEFT = Resident_Evil_Key::LEFT;
	static constexpr Resident_Evil_Key AIM_UP = Resident_Evil_Key::AIM_UP;
	static constexpr Resident_Evil_Key AIM_DOWN = Resident_Evil_Key::AIM_DOWN;
	static constexpr Resident_Evil_Key AIM_FIRE = Resident_Evil_Key::AIM_FIRE;
	static constexpr Resident_Evil_Key INSPECT = Resident_Evil_Key::INSPECT;
	static constexpr Resident_Evil_Key AIM = Resident_Evil_Key::AIM;
	static constexpr Resident_Evil_Key RUN = Resident_Evil_Key::RUN;
	static constexpr Resident_Evil_Key PREV = Resident_Evil_Key::PREV;
	static constexpr Resident_Evil_Key NEXT = Resident_Evil_Key::NEXT;
	static constexpr Resident_Evil_Key CONFIRM = Resident_Evil_Key::CONFIRM;
	static constexpr Resident_Evil_Key CANCEL = Resident_Evil_Key::CANCEL;
	static constexpr Resident_Evil_Key STATUS = Resident_Evil_Key::STATUS;
	static constexpr Resident_Evil_Key NOTHING = Resident_Evil_Key::NOTHING;

	struct CONTROLLER_KEY
	{
		std::uint16_t Button;
		bool IsLeftTrigger;
		bool IsRightTrigger;
		Resident_Evil_Key Key;
	};

	struct CONTROLLER
	{
		CONTROLLER_KEY L2 = { 0xFFFF, false, false, NOTHING };
		CONTROLLER_KEY R2 = { 0xFFFF, false, false, NOTHING };
		CONTROLLER_KEY L1 = { 0xFFFF, false, false, NOTHING };
		CONTROLLER_KEY R1 = { 0xFFFF, false, false, AIM };
		CONTROLLER_KEY Triangle = { 0xFFFF, false, false, NOTHING };
		CONTROLLER_KEY Circle = { 0xFFFF, false, false, STATUS | CANCEL };
		CONTROLLER_KEY Cross = { 0xFFFF, false, false, AIM_FIRE | INSPECT | CONFIRM };
		CONTROLLER_KEY Square = { 0xFFFF, false, false, RUN };
		CONTROLLER_KEY Select = { 0xFFFF, false, false, NOTHING };
		CONTROLLER_KEY Start = { 0xFFFF, false, false, NOTHING };
		CONTROLLER_KEY Up = { 0xFFFF, false, false, UP };
		CONTROLLER_KEY Right = { 0xFFFF, false, false, RIGHT | NEXT };
		CONTROLLER_KEY Down = { 0xFFFF, false, false, DOWN };
		CONTROLLER_KEY Left = { 0xFFFF, false, false, LEFT | PREV };
	};

	std::unordered_map<std::string, std::uint16_t*> m_ButtonMap =
	{
		{ "Aim", &m_Map.R1.Button },
		{ "Cancel", &m_Map.Circle.Button },
		{ "Action", &m_Map.Cross.Button },
		{ "Run", &m_Map.Square.Button },
		{ "Options", &m_Map.Select.Button },
		{ "Pause", &m_Map.Start.Button },
		{ "Forward", &m_Map.Up.Button },
		{ "Backward", &m_Map.Down.Button },
		{ "Left", &m_Map.Left.Button },
		{ "Right", &m_Map.Right.Button }
	};

	std::unordered_map<std::string, bool*> m_TriggerMapL =
	{
		{ "Aim", &m_Map.R1.IsLeftTrigger },
		{ "Cancel", &m_Map.Circle.IsLeftTrigger },
		{ "Action", &m_Map.Cross.IsLeftTrigger },
		{ "Run", &m_Map.Square.IsLeftTrigger },
		{ "Options", &m_Map.Select.IsLeftTrigger },
		{ "Pause", &m_Map.Start.IsLeftTrigger },
		{ "Forward", &m_Map.Up.IsLeftTrigger },
		{ "Backward", &m_Map.Down.IsLeftTrigger },
		{ "Left", &m_Map.Left.IsLeftTrigger },
		{ "Right", &m_Map.Right.IsLeftTrigger }
	};

	std::unordered_map<std::string, bool*> m_TriggerMapR =
	{
		{ "Aim", &m_Map.R1.IsRightTrigger },
		{ "Cancel", &m_Map.Circle.IsRightTrigger },
		{ "Action", &m_Map.Cross.IsRightTrigger },
		{ "Run", &m_Map.Square.IsRightTrigger },
		{ "Options", &m_Map.Select.IsRightTrigger },
		{ "Pause", &m_Map.Start.IsRightTrigger },
		{ "Forward", &m_Map.Up.IsRightTrigger },
		{ "Backward", &m_Map.Down.IsRightTrigger },
		{ "Left", &m_Map.Left.IsRightTrigger },
		{ "Right", &m_Map.Right.IsRightTrigger }
	};

	void SetTrigger(const std::string& ButtonStr, bool b_LeftTrigger, bool b_RightTrigger);

#ifdef MSTD_XINPUT
	std::unique_ptr<Standard_XInput> m_XInput;
#endif

	CONTROLLER m_Map;

public:

	explicit Resident_Evil_Gamepad(void) :
		m_Map{}
	{
#ifdef MSTD_XINPUT
		m_XInput = std::make_unique<Standard_XInput>();
#endif
	}

	virtual ~Resident_Evil_Gamepad(void) = default;

#ifdef MSTD_XINPUT
	// XInput Interface
	Standard_XInput* XInput(void) const { return m_XInput.get(); }
#endif

	// Controller Map
	CONTROLLER& Map(void) { return m_Map; }

	/*
		Button Mapping
		 - return bit of button bitmask
	*/ 
	std::uint16_t GetButtonMapBit(const std::string& ButtonStr) const;

	// Trigger Mapping (Left)
	bool GetTriggerMapL(const std::string& ButtonStr) const;

	// Trigger Mapping (Right)
	bool GetTriggerMapR(const std::string& ButtonStr) const;

	// Set Button Mapping
	bool SetMapping(const std::string ButtonStr, std::function<void(bool&)> ProgressCallback);

	// Is Button Pressed?
	bool IsPressed(CONTROLLER_KEY& Key) const
	{
#ifdef MSTD_XINPUT
		std::uint16_t Button = XInput()->Gamepad().wButtons;
		bool b_TriggerL = XInput()->Gamepad().bLeftTrigger > 0;
		bool b_TriggerR = XInput()->Gamepad().bRightTrigger > 0;
#else
		std::uint16_t Button = 0;
		bool b_TriggerL = false;
		bool b_TriggerR = false;
#endif

		bool b_Pressed = Key.Button == 0xFFFF ? false : Button & Key.Button;
		Key.IsLeftTrigger ? b_Pressed |= b_TriggerL : 0;
		Key.IsRightTrigger ? b_Pressed |= b_TriggerR : 0;

		return b_Pressed;
	}

	// Poll Controller State
	bool PollState(void) const
	{
#ifdef MSTD_XINPUT
		return XInput() && XInput()->PollState();
#endif
	}

};