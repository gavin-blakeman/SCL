//**********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								LRU
// SUBSYSTEM:						Least Recently Used Cache
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
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
// OVERVIEW:            A caching class using a LRU strategy.
//
// CLASSES INCLUDED:    LRU
//
// HISTORY:             2020-02-16 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef INCLUDE_LRU_HPP
#define INCLUDE_LRU_HPP

// Standard C++ Libraries
#include <array>
#include <atomic>
#include <memory>
#include <new>
#include <optional>
#include <tuple>
#include <utility>

#include <sys/mman.h>


namespace SCL
{

	/* A LRU cache, where the oldest cache member is evicted to make room for newer cached values.
	 * Each entry in the cache consists of a Key (normally a U32), a tag (U8, using the LSB 7 bits), and a blob up to a maximum size of 65535.
	 * The blob size is stored separately to the blob. The blob is treated as uint8_t.
	 *
	 */

	template<class Key,																// Key Type
					 std::size_t blob_size = 1024,						// Type to be stored
					 std::size_t N = 65536,										// Number of elements to be stored
					 typename I = std::size_t,								// Size of the indexes
					 std::size_t hashShards = 65536,					// The number of hashing shards.
					 std::size_t shardSize = 1024,
					 class Hash = std::hash<Key>,							// Hashing function for the key
					 class KeyEqual = std::equal_to<Key>			// Equality function for the jey
					 >
	class LRU_cache
	{
	public:
		using size_type 				= I;
		using value_type 				= std::tuple<std::uint8_t, std::uint16_t, std::uint8_t *>;		// The tag value, sizeof BLOB and pointer to the blob data.
		using reference         = value_type&;
		using const_reference 	= value_type const &;
		using pointer           = value_type*;
		using const_pointer 		= value_type const *;

//		template <bool isConst>
//		class LRU_iterator
//		{
//			using iterator_category = std::bidirectional_iterator_tag;
//			using value_type        = std::pair<const Key, T>;
//			using difference_type   	= std::ptrdiff_t;
//		};

		LRU_cache()
		{
//			std::size_t memorySize = 2 << 32;

//			m_raw_memory = mmap(nullptr, total_bytes, PROT_READ | PROT_WRITE,
//          MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_HUGE_1GB, -1, 0);
		}

		/*! @brief Clears all elements from the LRU cache.
		 */
		void clear()
		{
			frontIndx = backIndx = 0;
			nodeArrayInit();
		}

		bool empty() const noexcept { return size() == 0; }



		constexpr size_type max_size() const noexcept { return capacity; }

		/*!	@brief Pushes a new (Key, T) into the cache.
		 * 	@param[in] k: The key to insert.
		 * 	@param[in] v: The value to insert.
		 */
		void push(Key k, value_type v)
		{
		}


		pointer get(Key);
		bool update(Key, value_type v) const;

		/*! @brief Check if a Key/Value pair exists in the cache. This does not modify the cache, or update the LRU order.
		 * 	@param[in] k: The key to find in the Cache.
		 * 	@returns An optional containing a const pointer to the data.
		 */
		std::optional<const_pointer> peek(Key) const
		{

		}

		bool contains(Key) const
		{
		}

		size_type size() const noexcept { return elementCount; }


	private:
		using atomic_index = std::atomic<size_type>;
		struct guard_struct
		{
			std::uint16_t blobLength;
			std::atomic_flag recordLock;
			union
			{
				struct
				{
					unsigned dataTag 	: 7;
					unsigned inUse		: 1;
				};
				uint8_t u8;
			};
		};
		using node_type = std::tuple<atomic_index, atomic_index, atomic_index, guard_struct>;
		using shard_type = std::array<size_t, shardSize>;
		using blob_type = std::array<std::uint8_t, blob_size>;

		std::unique_ptr<std::array<node_type, N>> nodeArray;
		std::unique_ptr<std::array<blob_type, N>> blobStorage;		// Storage for all the blobs
		size_type elementCount = 0;
		static constexpr size_type capacity = N;
		size_type frontIndx = 0;
		size_type backIndx = 0;
		void *m_raw_memory;

		void nodeArrayInit()
		{
			for (size_type indx = 0; indx != capacity; indx++)
			{
				std::get<3>(*nodeArray[indx]).inUse = false;
			}
		}

		LRU_cache(LRU_cache const &) = delete;
		LRU_cache(LRU_cache &&) = delete;
		LRU_cache &operator=(LRU_cache const &) = delete;
		LRU_cache &operator=(LRU_cache &&) = delete;

	};


}	// Namespace

#endif /* INCLUDE_LRU_HPP */
