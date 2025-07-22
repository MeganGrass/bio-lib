/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO: 
*
*/


#include "bio2_cpt.h"


/*
	Decompress CPT from unsigned char array
*/
bool Resident_Evil_2_CPT::Decompress(std::vector<std::uint8_t> Input, std::vector<std::uint8_t>& Output)
{
	if (Output.size() != 0x30000)
	{
		Output.resize(0x30000);
	}

	if (Input.empty())
	{
		b_Complete = true;
		p_Input = 0;
		return false;
	}

	std::size_t pIn = 0;

	std::size_t pOut = 0;

	std::uint32_t Ctr = 0;

	std::uint8_t Zero_Ctr = 0;

	while ((pIn < Input.size()) && (pOut < Output.size()))
	{
		do
		{
			Ctr++;

			Output[pOut] = Input[pIn];
			Output[pOut + 1] = Input[pIn];
			Output[pOut + 2] = Input[pIn];

			pOut += 3;

			if (pOut > Output.size())
			{
				b_Complete = false;
				p_Input += pIn;
				return true;
			}

			if ((!Input[pIn]) && (!Input[pIn + 1]))
			{
				Output[pOut] = 0;

				pIn += 2;

				pOut += 3;

				Output[pOut] = 0;
				Output[pOut + 1] = 0;

				Zero_Ctr = Input[pIn];

				Ctr += (Zero_Ctr + 2);

				if (Zero_Ctr)
				{
					do
					{
						Output[pOut] = 0;
						Output[pOut + 1] = 0;
						Output[pOut + 2] = 0;

						pOut += 3;

						Zero_Ctr--;

					} while (Zero_Ctr & 0xFF);
				}
			}

			pIn++;

		} while ((Ctr & 0xFFFF) < 0x2000);
	}

	b_Complete = true;

	p_Input += pIn;

	return true;
}


/*
	Decompress CPT from file
*/
bool Resident_Evil_2_CPT::Decompress(std::filesystem::path Input)
{
	Standard_String Str;

	StdFile m_Input { Input, FileAccessMode::Read_Ex, true, false };

	if (!m_Input)
	{
		Str.Message("CPT Decompression: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::filesystem::path Dir = m_Input.GetDirectory();

	b_Complete = false;

	p_Input = 0;

	std::size_t Counter = 0;

	while (!b_Complete)
	{
		std::vector<std::uint8_t> CPT((m_Input.Size() - p_Input));

		m_Input.Read(p_Input, CPT.data(), (m_Input.Size() - p_Input));

		std::vector<std::uint8_t> Pixels;

		if (Decompress(CPT, Pixels))
		{
			std::filesystem::path Output = Str.FormatCStyle("%s/%s_%02d.bmp", Dir.string().c_str(), Input.stem().string().c_str(), Counter);

			Standard_Image Image { 24, 256, 256 };

			for (std::int32_t y = 0; y < 256; y++)
			{
				for (std::int32_t x = 0; x < 256; x++)
				{
					DWORD Color = Pixels.data()[(y * 256 * 3) + (x * 3) + 2] | (Pixels.data()[(y * 256 * 3) + (x * 3) + 1] << 8) | (Pixels.data()[(y * 256 * 3) + (x * 3)] << 16);
					Image.SetPixel(x, y, Color);
				}
			}

			Image.SaveBMP(Output);

			Image.Close();
		}
		else
		{
			break;
		}

		Counter++;
	}

	return true;
}