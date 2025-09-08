/*
*
*	Megan Grass
*	May 29, 2024
*
*
*	TODO:
*
*/


#include "bio2_msg.h"

extern std::uint8_t bio2_ds_font_space[0x108];


/*
	Open
*/
std::uintmax_t Resident_Evil_2_Message::Open(StdFile& File, std::uintmax_t _Ptr, std::vector<std::wint_t> _DelimiterList)
{

	bool _Break = false;
	std::uintmax_t p_Ptr = _Ptr;
	std::uintmax_t _DataSize = 0;
	std::uintmax_t _FileSize = File.Size();
	std::wint_t _Char = 0;

	do {

		File.ReadNext(p_Ptr++, &_Char, sizeof(std::wint_t));

		for (auto& _List : _DelimiterList)
		{
			if (_Char == _List)
			{
				_Break = true;
				break;
			}
		}

		if (_Break) { break; }

		_DataSize++;

	} while ((p_Ptr < _FileSize) && (!File.eof()));

	Data.resize(_DataSize * sizeof(std::uint8_t) + sizeof(std::wint_t));

	File.Read(_Ptr, Data.data(), Data.size());

	return _Ptr + Data.size();
}


/*
	Close
*/
void Resident_Evil_2_Message::Close(void)
{
	Data.clear();
	Data.shrink_to_fit();
}