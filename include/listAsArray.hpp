//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								lastAsArray.hpp
// SUBSYSTEM:						Fixed size list implemented using std::array as the storage.
// LANGUAGE:						C++20
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2026 Gavin Blakeman.
//                      This file is part of the Storage Class Library (SCL)
//
//                      SCL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      SCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//                      more details.
//
//                      You should have received a copy of the GNU General Public License along with SCL.  If not, see
//                      <http://www.gnu.org/licenses/>.
//
// OVERVIEW:						Implementation of a fixed size list using a std::array as the underlying storage.
//
// CLASSES INCLUDED:		listAsArray - Implements an STL drop in map function.
//
//
// HISTORY:             2026-05-08 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef SCL_LISTASARRAY_HPP
#define SCL_LISTASARRAY_HPP

#include <algorithm>
#include <array>
#include <cstdint>
#include <concepts>
#include <limits>
#include <tuple>
#include <type_traits>

namespace SCL
{
	template<class T,
					 typename I = std::uint64_t,
					 std::size_t N = 1024>
	requires requires (T a)
	{
		requires std::same_as<I, std::uint32_t> || std::same_as<I, std::uint64_t>;
		requires std::default_initializable<T>;
		{ a + 1} -> std::same_as<T>;
		requires (N <= std::numeric_limits<I>::max());
	}
	class listAsArray
	{
	public:
			using value_type = T;
			using size_type = I;
			using reference = value_type&;
			using const_reference = value_type const &;
			using pointer = value_type *;
			using const_pointer = value_type const *;

		listAsArray() = default;

		reference back()
		{
			return std::get<3>(storageArray[backIndx]);
		}

		const_reference back() const
		{
			return std::get<3>(storageArray[backIndx]);
		}

		void clear() noexcept
		{
			frontIndx = 0;
			backIndx = N;
			elementCount = 0;

			for(size_type indx = 0; indx != capacity; indx++)
			{
				std::get<2>(storageArray[indx]) = false;	// Mark as unused.
			}
		}

		bool empty() const noexcept
		{
			return elementCount == 0;
		}

		reference front()
		{
			return std::get<3>(storageArray[frontIndx]);
		}

		const_reference front() const
		{
			return std::get<3>(storageArray[frontIndx]);
		}

		bool full() const noexcept
		{
			return elementCount == capacity;
		}


		constexpr size_type max_size() const noexcept
		{
			return N;
		}

		size_type size() const noexcept
		{
			return elementCount;
		}

		void pop_back()
		{
			if (size() == 0)
			{
				// Do nothing.
			}
			else if (size() == 1)
			{
				std::get<2>(storageArray[frontIndx]) = false; // Mark element as unused.
				frontIndx = backIndx = 0;

				elementCount = 0;
			}
			else
			{
				std:get<2>(storageArray[backIndx]) = false;	// Mark as unused.
				std::get<0>(storageArray[std::get<1>(storageArray[backIndx])]) = std::get<0>(storageArray[backIndx]);	// Forward ptr of 1 back = backIndx forward
				std::get<1>(storageArray[std::get<0>(storageArray[backIndx])]) = std::get<1>(storageArray[backIndx]);

				elementCount--;
			}


		}

		void pop_front()
		{
			if (size() == 0)
			{
				// Do nothing.
			}
			else if (size() == 1)
			{
				std::get<2>(storageArray[frontIndx]) = false;
				frontIndx = backIndx = 0;
				elementCount = 0;
			}
			else
			{
				std::get<2>(storageArray(frontIndx)) = false; // Mark as unused.
				std::get<0>(storageArray[std::get<1>(storageArray[frontIndx])]) = std::get<0>(storageArray[frontIndx]);
				std::get<1>(storageArray[std::get<1>(storageArray[frontIndx])]) = std::get<1>(storageArray[frontIndx]);

				elementCount--;
			}
		}


		void push_back(value_type const &value)
		{
			std::get<3>(storageArray[push_back()]) = value;
		}

		void push_back(value_type &&value)
		{
			std::get<3>(storageArray[push_back()]) = std::move(value);
		}

		void push_front(value_type const &value)
		{
			std::get<3>(storageArray[push_front()]) = value;
		}

		void push_front(value_type&& value)
		{
			std::get<3>(storageArray[push_front()]) = std::move(value);
		}

	private:
		using storage_type = std::tuple<size_type, size_type, bool, value_type>;

		std::array<storage_type, N> storageArray;
		I frontIndx = 0;
		I backIndx = 0;
		I elementCount = 0;
		static constexpr I capacity = N;

		size_type push_back()
		{
			size_type newIndx;



			return newIndx;
		}

		size_type push_front()
		{
			size_type newIndx;
			if (elementCount == capacity)
			{
				// Evicting the back element.

				newIndx = backIndx;
				backIndx = std::get<1>(storageArray[backIndx]);
				std::get<1>(storageArray[frontIndx]) = newIndx;
				std::get<0>(storageArray[newIndx]) = frontIndx;
			}
			else if (!empty())
			{
				// No eviction
				// This is tricky, we have to traverse the entire frontIndx->backIndx and get the largest indx, while
				// keeping track of the next and last indx.

				size_type nextIndx = frontIndx;
				newIndx = 0;

				while ((newIndx != capacity) && std::get<2>(storageArray[newIndx]))
				{
					newIndx++;
				}

				std::get<2>(storageArray[newIndx]) = true;
				std::get<1>(storageArray[frontIndx]) = newIndx;				// Back pointer on the previous front element point to the new element.
				std::get<0>(storageArray[newIndx]) = frontIndx;				// Front pointer on the new element to the second element.
				std::get<1>(storageArray[newIndx]) = backIndx;				// Back pointer points to the last indx. (circular)
				std::get<0>(storageArray[backIndx]) = newIndx;				// Front pointer on the last element now points to the front. (curcular)

				elementCount++;
			}
			else
			{
				newIndx = 0;
				std::get<2>(storageArray[newIndx]) = true;						// Mark as used.

				elementCount++;
			}

			return newIndx;
		}

	};

}	// namespace SCL





#endif /* SCL_LISTASARRAY_HPP */
