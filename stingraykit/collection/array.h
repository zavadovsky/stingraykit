#ifndef STINGRAYKIT_COLLECTION_ARRAY_H
#define STINGRAYKIT_COLLECTION_ARRAY_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/toolkit.h>

#include <algorithm>

namespace stingray
{
	/**
	 * @addtogroup toolkit_collections
	 * @{
	 */

	struct uninitialized_array_tag
	{ };


	template < typename T, size_t N >
	class array
	{
	public:
		using value_type = T;

		using iterator = T*;
		using const_iterator = const T*;

		using reference = T&;
		using const_reference = const T&;

		using size_type = size_t;
		using difference_type = ptrdiff_t;

	public:
		static const size_t Size = N;

	private:
		T					_data[N];

	public:
		array() : _data() { }
		explicit array(uninitialized_array_tag) { }
		array(const array& other)				{ assign(other); }
		array(const T (& list)[N])				{ std::copy(std::begin(list), std::end(list), begin()); }

		array& operator = (const array& other)	{ assign(other); return *this; }

		void assign(const array& other)			{ std::copy(other.begin(), other.end(), begin()); }

		const_iterator begin() const			{ return _data; }
		const_iterator cbegin() const			{ return _data; }
		iterator begin()						{ return _data; }

		const_iterator end() const				{ return _data + N; }
		const_iterator cend() const				{ return _data + N; }
		iterator end()							{ return _data + N; }

		reference front()						{ return _data[0]; }
		const_reference front() const			{ return _data[0]; }

		reference back()						{ return _data[N - 1]; }
		const_reference back() const			{ return _data[N - 1]; }

		value_type* data()						{ return _data; }
		const value_type* data() const			{ return _data; }

		reference at(size_type offset)							{ return ArrayGet(_data, offset); }
		const_reference at(size_type offset) const				{ return ArrayGet(_data, offset); }

		reference operator [] (size_type offset)				{ return _data[offset]; }
		const_reference operator [] (size_type offset) const	{ return _data[offset]; }

		void fill(const value_type& value)		{ std::fill(begin(), end(), value); }

		static constexpr size_type size()					{ return N; }
		static constexpr size_type max_size()				{ return N; }
		static constexpr bool empty()						{ return N == 0; }

		void swap(array& other)
		{
			for (size_type index = 0; index < N; ++index)
				std::swap(_data[index], other._data[index]);
		}

		bool operator == (const array& other) const { return std::equal(begin(), end(), other.begin()); }
		STINGRAYKIT_GENERATE_EQUALITY_OPERATORS_FROM_EQUAL(array);

		bool operator < (const array& other) const
		{ return std::lexicographical_compare(begin(), end(), other.begin(), other.end()); }
		STINGRAYKIT_GENERATE_RELATIONAL_OPERATORS_FROM_LESS(array);
	};

	/** @} */
}

#endif
