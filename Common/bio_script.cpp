/*
*
*	Megan Grass
*	September 27, 2025
*
*/


#include "bio_script.h"

const bool Resident_Evil_Script::Open(ScriptType Type, std::vector<std::uint8_t> Buffer)
{
	if (Buffer.empty()) { return false; }

	if (GameType() & BIO2)
	{
		if (GetScriptType(Type) & INIT)
		{
			Init.push_back(Resident_Evil_2_Bytecode());
			std::visit([&Buffer](auto& Bytecode)
				{
					if constexpr (std::is_same_v<decltype(Bytecode), Resident_Evil_2_Bytecode&>)
					{
						Bytecode.Open(Buffer);
					}
				}, Init.back());
		}

		if (GetScriptType(Type) & LOOP)
		{
			Loop.push_back(Resident_Evil_2_Bytecode());
			std::visit([&Buffer](auto& Bytecode)
				{
					if constexpr (std::is_same_v<decltype(Bytecode), Resident_Evil_2_Bytecode&>)
					{
						Bytecode.Open(Buffer);
					}
				}, Loop.back());
		}
	}

	return true;
}

void Resident_Evil_Script::Close(void)
{
	Initialize = []() -> void {};
	Routine = []() -> void {};

	for (auto& Bytecode : Init)
	{
		std::visit([](auto& BC) { BC.Close(); }, Bytecode);
	}

	Init.clear();
	Init.shrink_to_fit();

	for (auto& Bytecode : Loop)
	{
		std::visit([](auto& BC) { BC.Close(); }, Bytecode);
	}

	Loop.clear();
	Loop.shrink_to_fit();
}