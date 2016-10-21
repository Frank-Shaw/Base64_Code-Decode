#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
static const char b64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char reverse_table[128] =
{
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
	64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
	64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
};

unsigned char *base64_encode(unsigned char *bindata, size_t inlen, unsigned char **out, size_t *outlen)
{
	size_t _outlen = *outlen;
	unsigned char *_out = NULL;
	size_t out_pos = 0;

	if (NULL == *out)
	{
		_outlen = (inlen / 3 + (inlen % 3 != 0)) * 4 + 1;
		_out = (unsigned char *)malloc(_outlen);
	}
	else
	{
		_outlen = *outlen;
		_out = *out;
	}

	memset(_out, '=', _outlen);
	_out[_outlen - 1] = 0;

	unsigned int bits_collected = 0;
	unsigned int accumulator = 0;
	for (int i = 0; i < inlen; i++)
	{
		accumulator = (accumulator << 8) | (bindata[i] & 0xffu);
		bits_collected += 8;
		while (bits_collected >= 6)
		{
			bits_collected -= 6;
			_out[out_pos++] = b64_table[(accumulator >> bits_collected) & 0x3fu];
		}
	}

	if (bits_collected >= 6)
	{
		if (NULL == *out)
		{
			free(_out);
		}
		return NULL;
	}

	if (bits_collected > 0)
	{
		// Any trailing bits that are missing.
		accumulator <<= 6 - bits_collected;
		_out[out_pos++] = b64_table[accumulator & 0x3fu];
	}

	*outlen = _outlen;
	*out = _out;
	return _out;
}

unsigned char *base64_decode(unsigned char *bindata, size_t inlen, unsigned char **out, size_t *outlen)
{
	size_t _outlen = *outlen;
	unsigned char *_out = NULL;
	int bits_collected = 0;
	unsigned int accumulator = 0;
	size_t out_pos = 0;

	if (NULL == *out)
	{
		_outlen = inlen;
		_out = (unsigned char *)malloc(_outlen);
	}
	else
	{
		_outlen = *outlen;
		_out = *out;
	}

	int c = 0;
	for (int i = 0; i < inlen; i++)
	{
		c = bindata[i];
		if (isspace(c) || c == '=')
		{
			// Skip whitespace and padding. Be liberal in what you accept.
			continue;
		}
		if ((c > 127) || (c < 0) || (reverse_table[c] > 63))
		{
			return NULL;
		}
		accumulator = (accumulator << 6) | reverse_table[c];
		bits_collected += 6;
		if (bits_collected >= 8)
		{
			bits_collected -= 8;
			_out[out_pos++] = (char)((accumulator >> bits_collected) & 0xffu);
		}
	}

	*outlen = _outlen;
	*out = _out;
	return _out;
}




int main()
{
	unsigned char *str = (unsigned char *)"你好ABC你不好";
	unsigned char *str_out = 0;
	unsigned char *out = (unsigned char *)"AAAAAB+5+MUAAAAAGvebDQASW0NROmVtb2ppLGlkPTk5OTZdABDDu9PQudjPtbbA19TSu8jLAAAAAAAAAAAAAFaZDxJYCdMvAATHv8nxAAAAAgAAAAAABNGnyfH/////AAAAAQ==";
	size_t len = 0;

	base64_encode(str, strlen((const char*)str), &str_out, &len);
	printf("%s\n", str_out);

	unsigned char *_out = 0;
	size_t _len = 0;

	base64_decode(out, strlen((const char*)out), &_out, &_len);

	unsigned char *num = _out;
	num[8] = '\0';



	for (int i = 0; i < 8; i++)
	{
		printf("%X ", num[i]);
	}



	

	system("pause\n");
	return 0;
	
}
