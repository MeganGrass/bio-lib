/*
*
*	Megan Grass
*	September 27, 2025
*
*/


#pragma once

#include <bio_common.h>

#include <scd/bio2_bytecode.h>

#include <type_traits>

#include <variant>

#include <functional>


enum class ScriptType : std::int32_t
{
	Init = (1 << 0),	// System function (called only once when setting a room)
	Loop = (1 << 1),	// System function (called every loop)
	Event = (1 << 2)	// Bio1 only
};


class Resident_Evil_Script :
	private Resident_Evil_Common {
private:

	// Script Type Constants
	static constexpr std::int32_t INIT = std::to_underlying(ScriptType::Init);
	static constexpr std::int32_t LOOP = std::to_underlying(ScriptType::Loop);
	static constexpr std::int32_t EVENT = std::to_underlying(ScriptType::Event);

	// Copy
	Resident_Evil_Script(const Resident_Evil_Script&) = delete;
	Resident_Evil_Script& operator = (const Resident_Evil_Script&) = delete;

	// Move
	Resident_Evil_Script(Resident_Evil_Script&&) = delete;
	Resident_Evil_Script& operator = (Resident_Evil_Script&&) = delete;

	// Script Type
	std::uint32_t GetScriptType(ScriptType Type) const { return std::to_underlying(Type); }

public:

	using Resident_Evil_Common::GameType;
	using Resident_Evil_Common::SetGame;

	// Has the initialization script been executed?
	std::atomic<bool> b_InitComplete;

	// System function (called only once when setting a room)
	std::vector<std::variant<Resident_Evil_2_Bytecode>> Init;

	// System function (called every loop)
	std::vector<std::variant<Resident_Evil_2_Bytecode>> Loop;

	// System function (called only once when setting a room)
	std::function<void()> Initialize;

	// System function (called every loop)
	std::function<void()> Routine;

	explicit Resident_Evil_Script(void)
	{
		b_InitComplete.store(false);
		Initialize = []() -> void {};
		Routine = []() -> void {};
	}

	~Resident_Evil_Script(void) = default;

	// Scenario Task Scheduler
	void Scheduler(void)
	{
		if (GameType() & BIO2)
		{
			Initialize = [this]() -> void
				{
					Resident_Evil_2_Bytecode().SceSchedulerSet();
				};

			Routine = [this]() -> void
				{
					Resident_Evil_2_Bytecode().SceScheduler();
				};
		}

		// TEMP
		else
		{
			Resident_Evil_2_Bytecode().NullScheduler();
		}
	}

	// Open
	const bool Open(ScriptType Type, std::vector<std::uint8_t> Buffer);

	// Close
	void Close(void);

};