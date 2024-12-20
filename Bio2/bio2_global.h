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
	std::uint32_t X;
	std::uint32_t Y;
	std::uint32_t Z;
};


#pragma pack(pop)