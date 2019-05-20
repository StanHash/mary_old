#ifndef MARY_RIFF_H
#define MARY_RIFF_H

#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

namespace mary {

struct RiffChunk
{
	inline bool is_valid() const
	{
		return name.size() == 4;
	}

	std::string name;
	std::vector<unsigned char> data;
};

struct Riff
{
	inline bool is_valid() const
	{
		return name.size() == 4 && std::all_of(chunks.begin(), chunks.end(),
			[] (const RiffChunk& chunk) { return chunk.is_valid(); });
	}

	std::string name;
	std::vector<RiffChunk> chunks;
};

Riff unpack_riff(const std::vector<unsigned char>& data);
Riff unpack_riff(std::istream& in);
Riff unpack_riff(const std::string& fileName, std::size_t offset);

std::vector<unsigned char> pack_riff(const Riff& riff);

} // namespace mary

#endif // MARY_RIFF_H
