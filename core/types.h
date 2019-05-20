#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <type_traits>
#include <array>

namespace mary {

using byte_type = std::uint8_t;

// TODO (C++20): use actual std::span instead of this bootleg one
template<typename Type>
struct Span
{
	constexpr Span() noexcept
		: mBegin(nullptr), mSize(0) {}

	constexpr Span(Type* first, std::size_t count)
		: mBegin(first), mSize(count) {}

	constexpr Span(Type* first, Type* last)
		: mBegin(first), mSize(last - first) {}

	template<std::size_t N>
	constexpr Span(Type (&patterns)[N]) noexcept
		: mBegin(patterns), mSize(N) {}

	template<std::size_t N>
	constexpr Span(std::array<Type, N>& arr) noexcept
		: mBegin(arr.data()), mSize(N) {}

	template<std::size_t N>
	constexpr Span(const std::array<Type, N>& arr) noexcept
		: mBegin(arr.data()), mSize(N) {}

	template<typename Container>
	constexpr Span(Container& container)
		: mBegin(container.data()), mSize(container.size()) {}

	template<typename Container>
	constexpr Span(const Container& container)
		: mBegin(container.data()), mSize(container.size()) {}

	constexpr Span(const Span& other) noexcept = default;
	constexpr Span& operator=(const Span& other) noexcept = default;

	Type* begin() const noexcept { return mBegin; }
	const Type* cbegin() const noexcept { return mBegin; }

	Type* end() const noexcept { return mBegin+mSize; }
	const Type* cend() const noexcept { return mBegin+mSize; }

	constexpr Type* data() const noexcept { return mBegin; }

	constexpr std::size_t size() const noexcept { return mSize; }
	constexpr std::size_t size_bytes() const noexcept { return mSize * sizeof(Type); }

	constexpr bool empty() const noexcept { return mSize == 0; }

	constexpr Type& front() const { return mBegin[0]; }
	constexpr Type& back() const { return mBegin[mSize-1]; }

	constexpr Span<Type> first(std::size_t count) const { return Span(mBegin, count); }
	constexpr Span<Type> last(std::size_t count) const { return Span(mBegin + mSize - count, count); }
	constexpr Span<Type> subspan(std::size_t offset) const { return Span(mBegin + offset, mBegin + mSize); }
	constexpr Span<Type> subspan(std::size_t offset, std::size_t count) const { return Span(mBegin + offset, count); }

	constexpr Type& operator [](std::size_t index) const { return mBegin[index]; }

private:
	Type* mBegin;
	std::size_t mSize;
};

} // namespace mary

#endif // TYPES_H
