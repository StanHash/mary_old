
#include "riff.h"

#include <array>
#include <stdexcept>
#include <fstream>

namespace mary {

Riff unpack_riff(const std::vector<unsigned char>& data)
{
	std::size_t readOffset = 0;

	auto read_name = [&data, &readOffset] () -> std::string
	{
		readOffset += 4;

		if (readOffset > data.size())
			throw std::runtime_error("unexpectedly reached end of data"); // TODO: better error

		return std::string(reinterpret_cast<const char*>(data.data()) + readOffset - 4, 4);
	};

	auto read_size = [&data, &readOffset] () -> std::size_t
	{
		readOffset += 4;

		if (readOffset > data.size())
			throw std::runtime_error("unexpectedly reached end of data"); // TODO: better error

		return (data[readOffset-4]) + (data[readOffset-3] << 8) + (data[readOffset-2] << 16) + (data[readOffset-1] << 24);
	};

	auto read_data = [&data, &readOffset] (std::size_t size) -> std::vector<unsigned char>
	{
		readOffset += size;

		if (readOffset > data.size())
			throw std::runtime_error("unexpectedly reached end of data"); // TODO: better error

		return std::vector<unsigned char>(data.cbegin() + readOffset - size, data.cbegin() + readOffset);
	};

	Riff result;

	if (read_name() != "RIFF")
		throw std::runtime_error("this doesn't follow RIFF!"); // TODO: better error

	auto riffSize = read_size();
	auto riffName = read_name();

	result.name = std::move(riffName);

	while (readOffset < riffSize)
	{
		auto chunkName = read_name();
		auto chunkSize = read_size();
		auto chunkData = read_data(chunkSize);

		result.chunks.push_back(RiffChunk { std::move(chunkName), std::move(chunkData) });
	}

	return result;
}

Riff unpack_riff(std::istream& in)
{
	std::array<unsigned char, 8> header;

	// Read header, to get data size

	if (!in.read(reinterpret_cast<char*>(header.data()), header.size()))
		throw std::runtime_error("unexpectedly reached end of file"); // TODO: better error

	if (header[0] != 'R' || header[1] != 'I' || header[2] != 'F' || header[3] != 'F')
		throw std::runtime_error("this is not a RIFF!"); // TODO: better error

	std::size_t size = (header[4]) + (header[5] << 8) + (header[6] << 16) + (header[7] << 24);

	// Construct data

	std::vector<unsigned char> data(size + 8);

	// Read rest of data
	if (!in.read(reinterpret_cast<char*>(data.data()) + 8, size))
		throw std::runtime_error("unexpectedly reached end of file"); // TODO: better error

	// Put header at the start of data
	std::copy(
		header.begin(), header.end(),
		data.begin());

	return unpack_riff(data);
}

Riff unpack_riff(const std::string& fileName, std::uint64_t offset)
{
	std::ifstream file(fileName, std::ios::in | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("couldn't open file"); // TODO: better error

	file.seekg(offset);

	return unpack_riff(file);
}

std::vector<unsigned char> pack_riff(const Riff& riff)
{
	std::vector<unsigned char> result(std::accumulate(riff.chunks.begin(), riff.chunks.end(), 12,
		[] (std::size_t value, const RiffChunk& chunk)
		{
			return value + 8 + chunk.data.size();
		}
	));

	std::size_t writeOffset = 0;

	auto write_name = [&result, &writeOffset] (const std::string& name)
	{
		writeOffset += 4;

		if (writeOffset > result.size())
			throw std::logic_error("Something went very wrong!"); // TODO: better error

		std::copy(name.cbegin(), name.cbegin() + 4, result.begin() + writeOffset - 4);
	};

	auto write_size = [&result, &writeOffset] (std::size_t size)
	{
		writeOffset += 4;

		if (writeOffset > result.size())
			throw std::logic_error("Something went very wrong!"); // TODO: better error

		result[writeOffset-4] = 0xFF & (size);
		result[writeOffset-3] = 0xFF & (size >> 8);
		result[writeOffset-2] = 0xFF & (size >> 16);
		result[writeOffset-1] = 0xFF & (size >> 24);
	};

	auto write_data = [&result, &writeOffset] (const std::vector<unsigned char>& data)
	{
		writeOffset += data.size();

		if (writeOffset > result.size())
			throw std::logic_error("Something went very wrong!"); // TODO: better error

		std::copy_n(data.cbegin(), data.size(), result.begin() + writeOffset - data.size());
	};

	write_name("RIFF");
	write_size(result.size() - 8);

	write_name(riff.name);

	for (auto& chunk : riff.chunks)
	{
		write_name(chunk.name);
		write_size(chunk.data.size());
		write_data(chunk.data);
	}

	return result;
}

} // namespace mary
