#ifndef STINGRAYKIT_COLLECTION_FLAT_MAP_H
#define STINGRAYKIT_COLLECTION_FLAT_MAP_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/collection/KeyExceptionCreator.h>

namespace stingray
{

	template < class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<Key, T>> >
	class flat_map
	{
		STINGRAYKIT_DEFAULTCOPYABLE(flat_map);
		STINGRAYKIT_DEFAULTMOVABLE(flat_map);

	public:
		using key_type = Key;
		using mapped_type = T;
		using value_type = std::pair<Key, T>;

	private:
		using Container = std::vector<value_type, Allocator>;

		template < typename Compare__, typename Enabler = void >
		struct IsTransparent : public FalseType
		{ };

		template < typename Compare__ >
		struct IsTransparent<Compare__, typename EnableIf<decltype(std::declval<typename Compare__::is_transparent*>(), TrueType())::Value, void>::ValueT> : public TrueType
		{ };

	public:
		using size_type = typename Container::size_type;
		using difference_type = typename Container::difference_type;
		using key_compare = Compare;
		using allocator_type = Allocator;
		using reference = typename Allocator::reference;
		using const_reference = typename Allocator::const_reference;
		using pointer = typename Allocator::pointer;
		using const_pointer = typename Allocator::const_pointer;
		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;
		using reverse_iterator = typename Container::reverse_iterator;
		using const_reverse_iterator = typename Container::const_reverse_iterator;

	private:
		struct CompareImpl
		{
			Compare		_cmp;

		public:
			CompareImpl(Compare comp) : _cmp(comp) { }

			bool operator () (const key_type& lhs, const key_type& rhs) const		{ return _cmp(lhs, rhs); }
			bool operator () (const value_type& lhs, const key_type& rhs) const		{ return _cmp(lhs.first, rhs); }
			bool operator () (const key_type& lhs, const value_type& rhs) const		{ return _cmp(lhs, rhs.first); }
			bool operator () (const value_type& lhs, const value_type& rhs) const	{ return _cmp(lhs.first, rhs.first); }

			template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
			bool operator () (const K& lhs, const key_type& rhs) const				{ return _cmp(lhs, rhs); }
			template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
			bool operator () (const key_type& lhs, const K& rhs) const				{ return _cmp(lhs, rhs); }
			template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
			bool operator () (const value_type& lhs, const K& rhs) const			{ return _cmp(lhs.first, rhs); }
			template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
			bool operator () (const K& lhs, const value_type& rhs) const			{ return _cmp(lhs, rhs.first); }
		};

	public:
		class value_compare : public function_info<bool (value_type, value_type)>
		{
			friend class flat_map;

		private:
			Compare		_cmp;

		protected:
			value_compare(Compare comp) : _cmp(comp) { }

		public:
			bool operator () (const value_type& lhs, const value_type& rhs) const
			{ return _cmp(lhs.first, rhs.first); }
		};

	private:
		Container		_container;
		CompareImpl		_cmp;

	public:
		flat_map()
			: _cmp(Compare())
		{ }

		explicit flat_map(const Compare& comp, const Allocator& alloc = Allocator())
			: _container(alloc), _cmp(comp)
		{ }

		explicit flat_map(const Allocator& alloc)
			: flat_map(Compare(), alloc)
		{ }

		template < class InputIterator >
		flat_map(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
			: _container(alloc), _cmp(comp)
		{ insert(first, last); }

		template < class InputIterator >
		flat_map(InputIterator first, InputIterator last, const Allocator& alloc)
			: flat_map(first, last, Compare(), alloc)
		{ }

		flat_map(const flat_map& other, const Allocator& alloc)
			: _container(other._container, alloc), _cmp(Compare())
		{ }

		flat_map(flat_map&& other, const Allocator& alloc)
			: _container(std::move(other._container), alloc), _cmp(Compare())
		{ }

		flat_map(std::initializer_list<value_type> list, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
			: _container(alloc), _cmp(comp)
		{ insert(list.begin(), list.end()); }

		flat_map(std::initializer_list<value_type> list, const Allocator& alloc)
			: flat_map(list, Compare(), alloc)
		{ }

		flat_map& operator = (std::initializer_list<value_type> list)
		{
			clear();
			insert(list.begin(), list.end());
			return *this;
		}

		allocator_type get_allocator() const	{ return _container.get_allocator(); }

		T& at(const Key& key)
		{
			const iterator result = find(key);
			STINGRAYKIT_CHECK(result != end(), CreateKeyNotFoundException(key));
			return result->second;
		}

		const T& at(const Key& key) const
		{
			const const_iterator result = find(key);
			STINGRAYKIT_CHECK(result != end(), CreateKeyNotFoundException(key));
			return result->second;
		}

		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		T& at(const K& key)
		{
			const iterator result = find(key);
			STINGRAYKIT_CHECK(result != end(), CreateKeyNotFoundException(key));
			return result->second;
		}

		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		const T& at(const K& key) const
		{
			const const_iterator result = find(key);
			STINGRAYKIT_CHECK(result != end(), CreateKeyNotFoundException(key));
			return result->second;
		}

		T& operator [] (const Key& key)
		{
			iterator result(lower_bound(key));
			if (result == end() || _cmp(key, result->first))
				result = _container.emplace(result, key, T());
			return result->second;
		}

		T& operator [] (Key&& key)
		{
			iterator result(lower_bound(key));
			if (result == end() || _cmp(key, result->first))
				result = _container.emplace(result, std::move(key), T());
			return result->second;
		}

		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		T& operator [] (K&& key)
		{
			iterator result(lower_bound(key));
			if (result == end() || _cmp(key, result->first))
				result = _container.emplace(result, std::move(key), T());
			return result->second;
		}

		iterator begin()						{ return _container.begin(); }
		const_iterator begin() const			{ return _container.begin(); }
		const_iterator cbegin() const			{ return _container.cbegin(); }

		iterator end()							{ return _container.end(); }
		const_iterator end() const				{ return _container.end(); }
		const_iterator cend() const				{ return _container.cend(); }

		reverse_iterator rbegin()				{ return _container.rbegin(); }
		const_reverse_iterator rbegin() const	{ return _container.rbegin(); }
		const_reverse_iterator crbegin() const	{ return _container.crbegin(); }

		reverse_iterator rend()					{ return _container.rend(); }
		const_reverse_iterator rend() const		{ return _container.rend(); }
		const_reverse_iterator crend() const	{ return _container.crend(); }

		bool empty() const						{ return _container.empty(); }
		size_type size() const					{ return _container.size(); }
		size_type max_size() const				{ return _container.max_size(); }

		void reserve(size_type size)			{ _container.reserve(size); }

		void clear()							{ _container.clear(); }

		std::pair<iterator, bool> insert(const value_type& value)
		{
			const iterator result(lower_bound(value.first));
			if (result == end() || _cmp(value.first, result->first))
				return std::make_pair(_container.insert(result, value), true);
			return std::make_pair(result, false);
		}

		std::pair<iterator, bool> insert(value_type&& value)
		{
			const iterator result(lower_bound(value.first));
			if (result == end() || _cmp(value.first, result->first))
				return std::make_pair(_container.insert(result, std::move(value)), true);
			return std::make_pair(result, false);
		}

		iterator insert(const_iterator hint, const value_type& value)
		{
			if ((hint == begin() || _cmp(*(hint - 1), value)) && (hint == end() || _cmp(value, *hint)))
				return _container.insert(hint, value);
			return insert(value).first;
		}

		iterator insert(const_iterator hint, value_type&& value)
		{
			if ((hint == begin() || _cmp(*(hint - 1), value)) && (hint == end() || _cmp(value, *hint)))
				return _container.insert(hint, std::move(value));
			return insert(std::move(value)).first;
		}

		template < class InputIterator >
		void insert(InputIterator first, InputIterator last)
		{
			if (IsInherited<typename std::iterator_traits<InputIterator>::iterator_category, std::random_access_iterator_tag>::Value)
				reserve(size() + std::distance(first, last));

			while (first != last)
				insert(*(first++));
		}

		void insert(std::initializer_list<value_type> list)
		{ insert(list.begin(), list.end()); }

		template < typename... Ts >
		std::pair<iterator, bool> emplace(Ts&&... args)
		{
			value_type value(std::forward<Ts>(args)...);
			const iterator result(lower_bound(value.first));
			if (result == end() || _cmp(value.first, result->first))
				return std::make_pair(_container.insert(result, std::move(value)), true);
			return std::make_pair(result, false);
		}

		template < typename... Ts >
		iterator emplace_hint(const_iterator hint, Ts&&... args)
		{
			value_type value(std::forward<Ts>(args)...);
			if ((hint == begin() || _cmp(*(hint - 1), value)) && (hint == end() || _cmp(value, *hint)))
				return _container.insert(hint, std::move(value));
			return insert(std::move(value)).first;
		}

		iterator erase(iterator pos)
		{ return _container.erase(pos); }

		iterator erase(const_iterator pos)
		{ return _container.erase(pos); }

		iterator erase(const_iterator first, const_iterator last)
		{ return _container.erase(first, last); }

		size_type erase(const Key& key)
		{
			const iterator result(find(key));
			if (result == end())
				return 0;
			erase(result);
			return 1;
		}

		void swap(flat_map& other)
		{
			_container.swap(other._container);
			std::swap(_cmp, other._cmp);
		}

		size_type count(const Key& key) const
		{ return find(key) == end() ? 0 : 1; }

		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		size_type count(const K& key) const
		{ return find(key) == end() ? 0 : 1; }

		iterator find(const Key& key)
		{
			const iterator result(lower_bound(key));
			if (result != end() && _cmp(key, result->first))
				return end();
			return result;
		}

		const_iterator find(const Key& key) const
		{
			const const_iterator result(lower_bound(key));
			if (result != end() && _cmp(key, result->first))
				return end();
			return result;
		}

		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		iterator find(const K& key)
		{
			const iterator result(lower_bound(key));
			if (result != end() && _cmp(key, result->first))
				return end();
			return result;
		}

		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		const_iterator find(const K& key) const
		{
			const const_iterator result(lower_bound(key));
			if (result != end() && _cmp(key, result->first))
				return end();
			return result;
		}

		std::pair<iterator, iterator> equal_range(const Key& key)						{ return std::equal_range(begin(), end(), key, _cmp); }
		std::pair<const_iterator, const_iterator> equal_range(const Key& key) const		{ return std::equal_range(begin(), end(), key, _cmp); }

		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		std::pair<iterator, iterator> equal_range(const K& key)							{ return std::equal_range(begin(), end(), key, _cmp); }
		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		std::pair<const_iterator, const_iterator> equal_range(const K& key) const		{ return std::equal_range(begin(), end(), key, _cmp); }

		iterator lower_bound(const Key& key)											{ return std::lower_bound(begin(), end(), key, _cmp); }
		const_iterator lower_bound(const Key& key) const								{ return std::lower_bound(begin(), end(), key, _cmp); }

		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		iterator lower_bound(const K& key)												{ return std::lower_bound(begin(), end(), key, _cmp); }
		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		const_iterator lower_bound(const K& key) const									{ return std::lower_bound(begin(), end(), key, _cmp); }

		iterator upper_bound(const Key& key)											{ return std::upper_bound(begin(), end(), key, _cmp); }
		const_iterator upper_bound(const Key& key) const								{ return std::upper_bound(begin(), end(), key, _cmp); }

		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		iterator upper_bound(const K& key)												{ return std::upper_bound(begin(), end(), key, _cmp); }
		template < typename K, typename Compare__ = Compare, typename EnableIf<IsTransparent<Compare__>::Value, int>::ValueT = 0 >
		const_iterator upper_bound(const K& key) const									{ return std::upper_bound(begin(), end(), key, _cmp); }

		key_compare key_comp() const													{ return _cmp._cmp; }
		value_compare value_comp() const												{ return value_compare(_cmp._cmp); }

		template < class K, class T_, class C, class A >
		friend bool operator == (const flat_map<K, T_, C, A>& lhs, const flat_map<K, T_, C, A>& rhs);
		template < class K, class T_, class C, class A >
		friend bool operator < (const flat_map<K, T_, C, A>& lhs, const flat_map<K, T_, C, A>& rhs);
	};


	template < class K, class T, class C, class A >
	bool operator == (const flat_map<K, T, C, A>& lhs, const flat_map<K, T, C, A>& rhs)
	{ return lhs._container == rhs._container; }
	STINGRAYKIT_GENERATE_NON_MEMBER_EQUALITY_OPERATORS_FROM_EQUAL(MK_PARAM(template < class K, class T, class C, class A >), MK_PARAM(flat_map<K, T, C, A>), MK_PARAM(flat_map<K, T, C, A>));


	template < class K, class T, class C, class A >
	bool operator < (const flat_map<K, T, C, A>& lhs, const flat_map<K, T, C, A>& rhs)
	{ return lhs._container < rhs._container; }
	STINGRAYKIT_GENERATE_NON_MEMBER_RELATIONAL_OPERATORS_FROM_LESS(MK_PARAM(template < class K, class T, class C, class A >), MK_PARAM(flat_map<K, T, C, A>), MK_PARAM(flat_map<K, T, C, A>));

}

#endif
