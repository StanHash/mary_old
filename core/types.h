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
	using element_type = Type;
	using value_type = std::remove_cv_t<Type>;

	using index_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	using pointer = Type*;
	using const_pointer = const Type*;

	using reference = Type&;
	using const_reference = const Type&;

	using iterator = pointer;
	using const_iterator = const_pointer;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	constexpr Span() noexcept
		: mBegin(nullptr), mSize(0) {}

	constexpr Span(Type* first, std::size_t count) noexcept
		: mBegin(first), mSize(count) {}

	constexpr Span(Type* first, Type* last) noexcept
		: mBegin(first), mSize(last - first) {}

	template<std::size_t N>
	constexpr Span(Type (&arr)[N]) noexcept
		: mBegin(arr), mSize(N) {}

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

	constexpr iterator begin() const noexcept { return mBegin; }
	constexpr const_iterator cbegin() const noexcept { return mBegin; }

	constexpr iterator end() const noexcept { return mBegin+mSize; }
	constexpr const_iterator cend() const noexcept { return mBegin+mSize; }

	constexpr reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(end()); }
	constexpr const_reverse_iterator crbegin() const noexcept { return std::make_reverse_iterator(cend()); }

	constexpr reverse_iterator rend() const noexcept { return std::make_reverse_iterator(begin()); }
	constexpr const_reverse_iterator crend() const noexcept { return std::make_reverse_iterator(cbegin()); }

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
