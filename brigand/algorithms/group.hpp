/*==================================================================================================
  Copyright (c) 2015 Edouard Alligand and Joel Falcou

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
=================================================================================================**/
#pragma once

#include <brigand/algorithms/reverse.hpp>
#include <brigand/algorithms/sort.hpp>
#include <brigand/algorithms/split_at.hpp>
#include <brigand/algorithms/transform.hpp>
#include <brigand/algorithms/unique.hpp>
#include <brigand/functions/arithmetic/minus.hpp>
#include <brigand/functions/lambda/apply.hpp>
#include <brigand/functions/lambda/bind.hpp>
#include <brigand/sequences/front.hpp>
#include <brigand/sequences/size.hpp>
#include <brigand/types/integer.hpp>
#include <brigand/types/type.hpp>

namespace brigand
{
    namespace detail
	{
        template<class L, class Key>
		struct group
		{
			using keys = brigand::transform<L, Key>;
			using zip = brigand::transform<keys, L, bind<list, _1, _2>>;
			using key_sorted = brigand::sort<
							zip,
							bind<
								less,
								bind<front, _1>,
								bind<front, _2>
							>
						>;
			using keys_sorted = brigand::transform<key_sorted, bind<front, _1>>;
			using values_sorted = brigand::transform<key_sorted, bind<back, _1>>;
			using keys_unique = sort<unique<keys>>;

			using end_idx = brigand::transform<
				keys_unique,
				bind<
					minus,
					pin<size<keys_sorted>>,
					bind<
						index_of,
						pin<reverse<keys_sorted>>,
						_1
					>
				>
			>;
			using start_idx = push_front<pop_back<end_idx>, brigand::size_t<0>>;

			using type = brigand::transform<start_idx, end_idx,
				bind<
					back,
					bind<
						split_at,
						bind<
							front,
							bind<split_at, pin<values_sorted>, _2>
						>,
						_1
					>
				>
			>;
		};

        template<class Key>
		struct group<brigand::list<>, Key>
        {
            using type = brigand::list<>;
        };
	}

	template<class L, class Key = _1>
	using group = type_from<detail::group<L, Key>>;
}
