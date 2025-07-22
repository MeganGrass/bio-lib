/*
*
*	Resident Evil 2
*
*	CAPCOM Co., Ltd.
*
*/


#include <cstdint>


#pragma pack(push, 1)


struct ITEM_WORK
{
    std::uint8_t Id;
    std::uint8_t Num;
    std::uint8_t Size;
    std::uint8_t dummy;
};


struct DATA_POS
{
    std::uint32_t Offset;
    std::uint32_t Size;
};


struct DVIEW
{
	std::int32_t x;
	std::int32_t y;
	std::int32_t z;
};


struct BOX
{
	std::int16_t x;
	std::int16_t z;
	std::uint16_t w;
	std::uint16_t d;
};


#pragma pack(pop)