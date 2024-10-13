/*
*
*	Haruhiko Okumura
*
*
*	TODO:
*
*/


#include <lzo.h>

#define EI	9				// typically 10..13

#define EJ	4				// typically 4..5

#define P	1				// If match length <= P then output one character

#define Nx	(1 << EI)		// buffer size

#define F	((1 << EJ) + 1)	// lookahead buffer size

unsigned long buffer[Nx * 2]{};


#define getbi(_in,_buf,_mask,_x)	{\
	if (_mask == 0)\
		{\
		_buf = *in++;\
		_mask = 0x80;\
		}\
	_x= _buf & _mask ? 1 : 0;\
	_mask >>= 1;\
}\


#define getbit(_in, n, buf, mask, x)	{\
	std::uint32_t _n=n;\
	x = 0;\
	for (; _n > 0; _n--)\
	{\
		if (mask == 0)\
		{\
			buf = *_in++;\
			mask = 0x80;\
		}\
		x <<= 1;\
		if (buf & mask) x |= 1;\
		mask >>= 1;\
	}\
}\


void LZO_Compression::putbit1(void)
{
	bit_buffer |= bit_mask;
	if ((bit_mask >>= 1) == 0)
	{
		if (m_Output->eof()) { error(); }
		m_Output->Get().put(bit_buffer);
		bit_buffer = 0;
		bit_mask = 128;
		codecount++;
	}
}
void LZO_Compression::putbit0(void) {
	if ((bit_mask >>= 1) == 0)
	{
		if (m_Output->eof()) { error(); }
		m_Output->Get().put(bit_buffer);
		bit_buffer = 0;
		bit_mask = 128;
		codecount++;
	}
}
void LZO_Compression::flush_bit_buffer(void)
{
	if (bit_mask != 128)
	{
		if (m_Output->eof()) { error(); }
		m_Output->Get().put(bit_buffer);
		codecount++;
	}
}
void LZO_Compression::output1(std::int32_t c)
{
	std::int32_t mask = 0;

	putbit1();

	mask = 256;

	while (mask >>= 1)
	{
		if (c & mask) { putbit1(); }
		else { putbit0(); }
	}
}
void LZO_Compression::output2(std::int32_t x, std::int32_t y)
{
	std::int32_t mask = 0;

	putbit0();

	mask = Nx;

	while (mask >>= 1)
	{
		if (x & mask) { putbit1(); }
		else { putbit0(); }
	}

	mask = (1 << EJ);

	while (mask >>= 1)
	{
		if (y & mask) { putbit1(); }
		else { putbit0(); }
	}
}
bool LZO_Compression::Compress(std::filesystem::path Input, std::filesystem::path Output)
{
	m_Input->Open(Input, FileAccessMode::Read_Ex, true, false);

	if (!m_Input->IsOpen())
	{
		Str->Message("LZO Compression: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	m_Output->Open(Output, FileAccessMode::Write_Ex, true, true);

	if (!m_Output->IsOpen())
	{
		Str->Message("LZO Compression: Error, could not create %s", Output.filename().string().c_str());
		return false;
	}

	bit_buffer = 0;

	bit_mask = 128;

	codecount = 0;

	textcount = 0;

	std::int32_t i, j, f1, x, y, r, s, bufferend, c;

	for (i = 0; i < Nx - F; i++) { buffer[i] = 0x20; }

	for (i = Nx - F; i < Nx * 2; i++)
	{
		c = m_Input->Get().get();
		if (c == EOF) break;
		buffer[i] = static_cast<std::uint8_t>(c);
		textcount++;
	}

	bufferend = i;

	r = Nx - F;

	s = 0;

	while (r < bufferend)
	{
		f1 = (F <= bufferend - r) ? F : bufferend - r;
		x = 0;  y = 1;  c = buffer[r];
		for (i = r - 1; i >= s; i--)
			if (buffer[i] == c) {
				for (j = 1; j < f1; j++)
					if (buffer[i + j] != buffer[r + j]) break;
				if (j > y) {
					x = i;  y = j;
				}
			}
		if (y <= P) { y = 1;  output1(c); }
		else output2(x & (Nx - 1), y - 2);
		r += y;  s += y;
		if (r >= Nx * 2 - F) {
			for (i = 0; i < Nx; i++) buffer[i] = buffer[i + Nx];
			bufferend -= Nx;  r -= Nx;  s -= Nx;
			while (bufferend < Nx * 2) {
				c = m_Input->Get().get();
				if (c == EOF) break;
				buffer[bufferend++] = static_cast<std::uint8_t>(c);
				textcount++;
			}
		}
	}

	flush_bit_buffer();

	// Complete

	m_Input->Get().clear();

	std::uintmax_t InputSize = m_Input->Size();

	m_Input->Close();

	std::vector<std::uint8_t> LZO(m_Output->Size());

	m_Output->Read(0, LZO.data(), m_Output->Size());

	m_Output->Write(0, &InputSize, 4);

	m_Output->Write(4, LZO.data(), LZO.size());

	m_Output->Close();

	return b_Fail;
}
void LZO_Compression::Decompress(std::uint8_t* in, std::uint8_t* out)
{
	std::uint32_t c,
		i,
		j,
		k,
		r,
		dec_size,
		dec_pos,
		buf = 0,
		mask;

	dec_size = *(std::uint32_t*)&in[0];

	std::vector<std::uint8_t> decbuffer(dec_size);

	for (r = 0; r < Nx - F; r++) decbuffer.data()[r] = 0x20;
	r = Nx - F;

	dec_pos = 0;
	mask = 0;

	in += 4;

	while (dec_pos < dec_size)
	{
		getbi(in, buf, mask, c);
		if (c)
		{
			getbit(in, 8, buf, mask, c);
			out[dec_pos++] = static_cast<std::uint8_t>(c);
			decbuffer.data()[r++] = static_cast<std::uint8_t>(c);
			r &= (Nx - 1);
		}
		else
		{
			getbit(in, EI, buf, mask, i);
			getbit(in, EJ, buf, mask, j);
			for (k = 0; k <= j + 1; k++)
			{
				c = decbuffer[(i + k) & (Nx - 1)];
				out[dec_pos++] = static_cast<std::uint8_t>(c);
				decbuffer.data()[r++] = static_cast<std::uint8_t>(c);
				r &= (Nx - 1);
			}
		}
	}
}