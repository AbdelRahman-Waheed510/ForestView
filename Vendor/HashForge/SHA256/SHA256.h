#pragma once 
#include <iostream>
#include <array>
#include <span>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <future>
#include <memory>
// - The algorithm takes the input as array of unit8_t (byte)
// - it proccess the data in 512-bit blocks (64-bytes)
// - if you pass to it more than 64-bytes the it first hashes the input that is in the buffer then bake it to the m_state and after that it adds the rest of the input to the buffer
// ############### Funtions ###############
// Update(std::span<uint8_t> data) = Takes bytes from the input and fills the internal 64-byte buffer. Whenever the buffer becomes full, call Hash() to hash that 64-byte block and then call back() and continue with the remaining input.
// Hash(std::array<uint8_t, 64> data) = hashes the buffer
// Bake(std::array<uint8_t, 64> data) = bake the hashed buffer into the m_state
// Finalize() = hashes the buffer after calling filling it with the Update function

namespace HashForge
{
	class SHA256
	{
	private:

		// The state of the hash (The array that gets converted into std::string that gets returened at the end )
		static constexpr std::array<uint32_t, 8> INITIAL_STATE =
		{
			0x6a09e667,
			0xbb67ae85,
			0x3c6ef372,
			0xa54ff53a,
			0x510e527f,
			0x9b05688c,
			0x1f83d9ab,
			0x5be0cd19
		};

		std::array<uint32_t, 8> m_state = INITIAL_STATE;

		// The input buffer that Update() fills it and autohash it if it reaches 64-Bytes (512-bit)
		std::array<uint8_t, 64> m_buffer;
		size_t m_bufferUsedSize = 0;

		// Constants factors gets used in the rounds of calculations that happens on the data
		static constexpr uint32_t m_roundsFactors[64] =
		{
			0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
			0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
			0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
			0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
			0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
			0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
			0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
			0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
			0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
			0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
			0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
			0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
			0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
			0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
			0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
		};

		size_t m_totalBytes = 0;

	private:
		// The main hash function
		void Hash();

		uint32_t RotateRight(uint32_t value, const int& iterations);

		// For Building the rest of the 64 combinedData array
		uint32_t o1(const uint32_t& value);

		uint32_t o2(const uint32_t& value);

		// For the 64 Rounds calculations
		uint32_t S1(const uint32_t& value);

		uint32_t S2(const uint32_t& value);

		uint32_t Ch(const uint32_t& x, const uint32_t& y, const uint32_t& z);

		uint32_t Maj(const uint32_t& x, const uint32_t& y, const uint32_t& z);

	public:

		void Update(std::span<const uint8_t> blockOfData);

		void Update(std::string_view string);

		std::string Finalize();

		void Reset();

		std::string HashFile(const std::filesystem::path& filePath);

		std::string HashFile(const std::filesystem::path& filePath, std::atomic<bool>& cancelRequest);
	};
}