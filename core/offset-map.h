#ifndef MARY_OFFSET_MAP_H
#define MARY_OFFSET_MAP_H

#include "core/types.h"

#include <vector>
#include <string>

#include <algorithm>

namespace mary {

template<typename ValueType>
struct OffsetMap : public std::vector<std::pair<unsigned, ValueType>>
{
	// this is just an abstraction behind a sorted vector
	// maybe I should just have used std::map

	using _super = std::vector<std::pair<unsigned, ValueType>>;

	using iterator = typename _super::iterator;
	using const_iterator = typename _super::const_iterator;

	using _super::_super; // TODO

	void set(unsigned offset, ValueType&& value)
	{
		auto pair = std::make_pair(offset, std::move(value));
		auto upit = std::upper_bound(_super::begin(), _super::end(),
			pair, [] (auto& a, auto& b) { return a.first < b.first; });

		_super::insert(upit, std::move(pair));
	}

	void set(unsigned offset, const ValueType& value)
	{
		auto pair = std::make_pair(offset, value);
		auto upit = std::upper_bound(_super::begin(), _super::end(),
			pair, [] (auto& a, auto& b) { return a.first < b.first; });

		_super::insert(upit, std::move(pair));
	}

	iterator get(unsigned offset)
	{
		auto index = get_index(offset);

		if (index == bad_index)
			return _super::end();

		return _super::begin() + index;
	}

	const_iterator get(unsigned offset) const
	{
		auto index = get_index(offset);

		if (index == bad_index)
			return _super::cend();

		return _super::cbegin() + index;
	}

	bool has(unsigned offset) const
	{
		return get_index(offset) != bad_index;
	}

	template<typename Func>
	void for_at(unsigned offset, Func func) const
	{
		auto index = get_index(offset);

		if (index != bad_index)
			func(_super::operator[](index).second);
	}

	std::size_t get_index(unsigned offset) const
	{
		if (!_super::empty())
		{
			// binary search yay

			std::ptrdiff_t left = 0;
			std::ptrdiff_t right = _super::size() - 1;

			while (left <= right)
			{
				std::size_t mid = (left + right) / 2;
				auto& pair = _super::operator[](mid);

				if (pair.first < offset)
				{
					left = mid+1;
					continue;
				}

				if (pair.first > offset)
				{
					right = mid-1;
					continue;
				}

				return mid;
			}
		}

		return bad_index;
	}

	static constexpr std::size_t bad_index = std::numeric_limits<std::size_t>::max();

protected:
	// this doesn't prevent *all* other modifying operations, but it will prevent common ones
	using _super::insert;
	using _super::emplace;
	using _super::push_back;
	using _super::emplace_back;
};

using NameMap = OffsetMap<std::string>;

} // namespace mary

#endif // MARY_OFFSET_MAP_H
