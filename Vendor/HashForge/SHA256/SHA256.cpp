#include "SHA256.h"

namespace HashForge 
{
	void SHA256::Hash()
	{
		std::array<uint32_t, 64> CombinedData;

		// Combining each 4 buffer values (each 4 bytes into 1 32_bit value (4 bytes))
		for (size_t i = 0; i < 16; i++)
		{
			const int x = i * 4;
			CombinedData[i] = (m_buffer[x] << 24) | (m_buffer[x + 1] << 16) | (m_buffer[x + 2] << 8) | (m_buffer[x + 3]);
		}

		// Calculating the rest of the ComninedData array (16 -> 64) with a standard operation done on the last 16 combined values
		for (size_t i = 16; i < 64; i++)
		{
			CombinedData[i] = o2(CombinedData[i - 2]) + CombinedData[i - 7] + o1(CombinedData[i - 15]) + CombinedData[i - 16];
		}

		uint32_t a = m_state[0];
		uint32_t b = m_state[1];
		uint32_t c = m_state[2];
		uint32_t d = m_state[3];
		uint32_t e = m_state[4];
		uint32_t f = m_state[5];
		uint32_t g = m_state[6];
		uint32_t h = m_state[7];

		for (size_t i = 0; i < 64; i++)
		{
			uint32_t V1 = h + S2(e) + Ch(e, f, g) + m_roundsFactors[i] + CombinedData[i];
			uint32_t V2 = S1(a) + Maj(a, b, c);

			// Updating Variabls
			h = g;
			g = f;
			f = e;
			e = d + V1;
			d = c;
			c = b;
			b = a;
			a = V1 + V2;
		}

		// Updating state
		m_state[0] += a;
		m_state[1] += b;
		m_state[2] += c;
		m_state[3] += d;
		m_state[4] += e;
		m_state[5] += f;
		m_state[6] += g;
		m_state[7] += h;
	}

	uint32_t SHA256::RotateRight(uint32_t value, const int& iterations)
	{
		if (iterations == 0) return value;
		const int totalBits = 32;

		uint32_t firstValue = value >> iterations;
		uint32_t deletedBits = (static_cast<uint32_t>(1) << iterations) - 1;
		uint32_t rotatedBits = value & deletedBits;
		rotatedBits <<= (totalBits - iterations);
		return firstValue | rotatedBits;
	}

	uint32_t SHA256::o1(const uint32_t& value)
	{
		return RotateRight(value, 7) ^ RotateRight(value, 18) ^ (value >> 3);
	}

	uint32_t SHA256::o2(const uint32_t& value)
	{
		return RotateRight(value, 17) ^ RotateRight(value, 19) ^ (value >> 10);
	}

	// For the 64 Rounds calculations
	uint32_t SHA256::S1(const uint32_t& value)
	{
		return RotateRight(value, 2) ^ RotateRight(value, 13) ^ RotateRight(value, 22);
	}

	uint32_t SHA256::S2(const uint32_t& value)
	{
		return RotateRight(value, 6) ^ RotateRight(value, 11) ^ RotateRight(value, 25);
	}

	uint32_t SHA256::Ch(const uint32_t& x, const uint32_t& y, const uint32_t& z)
	{
		return (x & y) ^ (~x & z);
	}

	uint32_t SHA256::Maj(const uint32_t& x, const uint32_t& y, const uint32_t& z)
	{
		return (x & y) ^ (x & z) ^ (y & z);
	}

	void SHA256::Update(std::span<const uint8_t> blockOfData)
	{
		m_totalBytes += blockOfData.size();
		size_t i = 0;
		while (i < blockOfData.size())
		{
			if (m_bufferUsedSize >= 64)
			{
				Hash();
				m_bufferUsedSize = 0;
			}
			m_buffer[m_bufferUsedSize] = blockOfData[i];
			m_bufferUsedSize++;
			i++;
		}
	}

	void SHA256::Update(std::string_view string)
	{
		Update(std::span<const uint8_t>(
			reinterpret_cast<const uint8_t*>(string.data()),
			string.size()
		));
	}

	std::string SHA256::Finalize()
	{
		if (m_bufferUsedSize == 64)
		{
			Hash();
			m_bufferUsedSize = 0;
		}
		m_buffer[m_bufferUsedSize] = 0b10000000;
		m_bufferUsedSize++;

		if (m_bufferUsedSize > 56)
		{
			while (m_bufferUsedSize < 64)
			{
				m_buffer[m_bufferUsedSize] = 0b00000000;
				m_bufferUsedSize++;
			}
			Hash();
			m_bufferUsedSize = 0;
		}

		while (m_bufferUsedSize < 56)
		{
			m_buffer[m_bufferUsedSize] = 0b00000000;
			m_bufferUsedSize++;
		}

		uint64_t bitLength = m_totalBytes * 8;

		m_buffer[56] = static_cast<uint8_t>(bitLength >> 56);
		m_buffer[57] = static_cast<uint8_t>(bitLength >> 48);
		m_buffer[58] = static_cast<uint8_t>(bitLength >> 40);
		m_buffer[59] = static_cast<uint8_t>(bitLength >> 32);
		m_buffer[60] = static_cast<uint8_t>(bitLength >> 24);
		m_buffer[61] = static_cast<uint8_t>(bitLength >> 16);
		m_buffer[62] = static_cast<uint8_t>(bitLength >> 8);
		m_buffer[63] = static_cast<uint8_t>(bitLength);
		Hash();

		m_bufferUsedSize = 0;

		std::string result;

		for (uint32_t value : m_state)
		{
			result += std::format("{:08x}", value);
		}
		return result;
	}

	void SHA256::Reset()
	{
		m_state = INITIAL_STATE;
		m_totalBytes = 0;
		m_bufferUsedSize = 0;
	}

	std::string SHA256::HashFile(const std::filesystem::path& filePath)
	{
		if (m_totalBytes != 0) 
		{
			Reset();
		}
		constexpr int BufferSize = 1024 * 512;
		std::array<uint8_t, BufferSize> Buffer;

		std::ifstream readFile;
		readFile.rdbuf()->pubsetbuf(nullptr, 0);
		readFile.open(filePath, std::ios::in | std::ios::binary);

		if (!readFile.is_open()) {
			std::cerr << "Failed to open file.\n";
			return "Error";
		}
		while (readFile)
		{
			readFile.read(reinterpret_cast<char*>(Buffer.data()), Buffer.size());
			std::streamsize bytesRead = readFile.gcount();
			if (bytesRead > 0) 
			{
				Update(std::span<const uint8_t>(Buffer.data(), static_cast<size_t>(bytesRead)));
			}
		}
		std::string hash = Finalize();
		Reset();
		return hash;
	}
}