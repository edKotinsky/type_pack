/**
 * @file type_pack.hpp
 * @brief Main and the only file of a library
 *
 * TypePack Library provides metaprogramming type list and a great set of
 * operations with it like indexing access, generating, copying, comparsion,
 * and even sorting.
 *
 * In fact, it is a hobby project, caused by my wish to deeply explore C++
 * metaprogramming and other topics, like doxygen usage and customisation,
 * documentation writing, testing, etc.
 *
 * @copyright
 * Copyright (c) 2023 Gleb Zlatanov
 *
 * @license MIT License
 *
 * @par
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * @par
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * @par
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TYPE_PACK_HPP
#define TYPE_PACK_HPP

#include <cstddef>
#include <type_traits>

/**
 * @addtogroup TypePack
 * @{
 */

namespace tp {

  /**
   * @brief Type wrapper
   */
  template <typename T>
  struct just_type {
      using type = T;
  };

  struct empty_type {};

  /**
   * @brief Type list
   *
   * Stores types in the template parameter pack
   */
  template <typename... Types>
  class type_pack {
    private:
      static constexpr std::size_t sz = (sizeof...(Types));
    public:
      /**
       * @brief Returns size of a type_pack
       *
       * @ingroup TypePackIndexing */
      static constexpr std::size_t size() noexcept { return sz; }

      /**
       * @brief Indicates is type_pack empty or not
       *
       * Returns `true`, if type_pack is empty, `false` otherwise
       * @ingroup TypePackIndexing
       */
      static constexpr bool empty() noexcept { return sz == 0; }
  };

  /**
   * @brief Type alias to empty type_pack
   */
  using empty_pack = type_pack<>;

  /**
   * @addtogroup TypePackRelationOps
   * @{
   */

  //* [isequalimpl]

  template <class T, class U>
  struct is_equal : std::false_type {};

  template <typename... Ts>
  struct is_equal<type_pack<Ts...>, type_pack<Ts...>> : std::true_type {};

  template <typename T>
  struct is_equal<just_type<T>, just_type<T>> : std::true_type {};

  template <typename T>
  struct is_equal<just_type<T>, type_pack<T>> : std::true_type {};

  template <class T, class U>
  struct is_not_equal : std::integral_constant<bool, !is_equal<T, U>::value> {};

  //* [isequalimpl]

  template <class T, class U>
  constexpr bool operator==(T, U) {
    return is_equal<T, U>::value;
  }

  template <class T, class U>
  constexpr bool operator!=(T, U) {
    return is_not_equal<T, U>::value;
  }

  namespace __details {

    template <bool Cond>
    using enable_if_t = typename std::enable_if<Cond, void>::type;

    template <class TP1, class TP2,
              template <typename A, typename B> class Less, typename AlwaysVoid>
    struct lexicographical_compare_impl {
        static constexpr bool value = false;
    };

    template <typename T, typename... Ts, typename U, typename... Us,
              template <typename A, typename B> class Less>
    struct lexicographical_compare_impl<
        type_pack<T, Ts...>, type_pack<U, Us...>, Less,
        enable_if_t<!Less<T, U>::value && !Less<U, T>::value>> {
        static constexpr bool value =
            lexicographical_compare_impl<type_pack<Ts...>, type_pack<Us...>,
                                         Less, void>::value;
    };

    // lenght of the first range is less than length of the second range
    template <typename U, typename... Us,
              template <typename A, typename B> class Less>
    struct lexicographical_compare_impl<empty_pack, type_pack<U, Us...>, Less,
                                        void> {
        static constexpr bool value = true;
    };

    // lenght of the first range is greater than length of the second range
    template <typename T, typename... Ts,
              template <typename A, typename B> class Less>
    struct lexicographical_compare_impl<type_pack<T, Ts...>, empty_pack, Less,
                                        void> {
        static constexpr bool value = false;
    };

    // the first mismatching element of the first range is less
    template <typename T, typename... Ts, typename U, typename... Us,
              template <typename A, typename B> class Less>
    struct lexicographical_compare_impl<type_pack<T, Ts...>,
                                        type_pack<U, Us...>, Less,
                                        enable_if_t<Less<T, U>::value>> {
        static constexpr bool value = true;
    };

    // the first mismatching element of the second range is less
    template <typename T, typename... Ts, typename U, typename... Us,
              template <typename A, typename B> class Less>
    struct lexicographical_compare_impl<type_pack<T, Ts...>,
                                        type_pack<U, Us...>, Less,
                                        enable_if_t<Less<U, T>::value>> {
        static constexpr bool value = false;
    };

  } // namespace __details

  template <class TP1, class TP2, template <typename A, typename B> class Less>
  struct lexicographical_compare
      : std::integral_constant<bool, __details::lexicographical_compare_impl<
                                         TP1, TP2, Less, void>::value> {};

  /**
   * @}
   *
   * @addtogroup TypePackElementAccess
   * @{
   */

  //* [headimpl]
  template <typename T>
  struct head {};

  template <typename T, typename... Ts>
  struct head<type_pack<T, Ts...>> {
      using type = T;
  };
  //* [headimpl]

  //* [headtimpl]
  template <class TP>
  using head_t = typename head<TP>::type;

  //* [headtimpl]

  //* [atimpl]
  namespace __details {

    template <typename>
    struct Error_Type_Pack_Out_Of_Range;

    template <std::size_t Idx, std::size_t Count, class TP, typename>
    struct at_helper {
        using type = typename Error_Type_Pack_Out_Of_Range<TP>::type;
    };

    template <std::size_t Idx, std::size_t Count, typename T, typename... Ts>
    struct at_helper<Idx, Count, type_pack<T, Ts...>,
                     typename std::enable_if<Idx == Count, void>::type> {
        using type = T;
    };

    template <std::size_t Idx, std::size_t Count, typename T, typename... Ts>
    struct at_helper<Idx, Count, type_pack<T, Ts...>,
                     typename std::enable_if<Idx != Count, void>::type> {
        using type =
            typename at_helper<Idx, Count + 1, type_pack<Ts...>, void>::type;
    };

  } // namespace __details

  template <std::size_t Idx, class TP>
  struct at : __details::at_helper<Idx, 0, TP, void> {};
  //* [atimpl]

  //* [attimpl]
  template <std::size_t Idx, class TP>
  using at_t = typename at<Idx, TP>::type;

  //* [attimpl]

  //* [maximpl]
  namespace __details {

    template <class TP, typename CurMax,
              template <typename A, typename B> class Less>
    struct max_impl {
        using type = CurMax;
    };

    template <typename T, typename... Ts, typename CurMax,
              template <typename A, typename B> class Less>
    struct max_impl<type_pack<T, Ts...>, CurMax, Less> {
      private:
        using max_t =
            typename std::conditional<Less<CurMax, T>::value, T, CurMax>::type;
      public:
        using type = typename max_impl<type_pack<Ts...>, max_t, Less>::type;
    };

    template <typename T, typename... Ts,
              template <typename A, typename B> class Less>
    struct max_impl<type_pack<T, Ts...>, empty_type, Less> {
        using type = typename max_impl<type_pack<Ts...>, T, Less>::type;
    };

  } // namespace __details

  template <class TP, template <typename A, typename B> class Less>
  struct max : __details::max_impl<TP, empty_type, Less> {};
  //* [maximpl]

  //* [maxtimpl]
  template <class TP, template <typename A, typename B> class Less>
  using max_t = typename max<TP, Less>::type;

  //* [maxtimpl]

  //* [minimpl]
  namespace __details {

    template <class TP, typename CurMin,
              template <typename A, typename B> class Less>
    struct min_impl {
        using type = CurMin;
    };

    template <typename T, typename... Ts, typename CurMin,
              template <typename A, typename B> class Less>
    struct min_impl<type_pack<T, Ts...>, CurMin, Less> {
      private:
        using min_t =
            typename std::conditional<Less<CurMin, T>::value, CurMin, T>::type;
      public:
        using type = typename min_impl<type_pack<Ts...>, min_t, Less>::type;
    };

    template <typename T, typename... Ts,
              template <typename A, typename B> class Less>
    struct min_impl<type_pack<T, Ts...>, empty_type, Less> {
        using type = typename min_impl<type_pack<Ts...>, T, Less>::type;
    };

  } // namespace __details

  template <class TP, template <typename A, typename B> class Less>
  struct min : __details::min_impl<TP, empty_type, Less> {};
  //* [minimpl]

  //* [mintimpl]
  template <class TP, template <typename A, typename B> class Less>
  using min_t = typename min<TP, Less>::type;
  //* [mintimpl]

  /**
   * @}
   *
   * @addtogroup TypePackIndexing
   * @{
   */

  /** @ingroup TypePackGenerating */
  template <class TP, std::size_t StartIdx, std::size_t EndIdx>
  struct copy;

  /** @ingroup TypePackGenerating */
  template <class TP, std::size_t Begin, std::size_t End>
  using copy_t = typename copy<TP, Begin, End>::type;

  //* [containsimpl]
  template <typename T, class TP>
  struct contains {};

  template <typename T>
  struct contains<T, type_pack<>> : std::false_type {};

  template <typename T, typename U, typename... Ts>
  struct contains<T, type_pack<U, Ts...>>
      : std::integral_constant<bool,
                               std::is_same<T, U>::value
                                   ? true
                                   : contains<T, type_pack<Ts...>>::value> {};

  //* [containsimpl]

  //* [findimpl]
  namespace __details {

    template <typename T, typename TP, size_t Idx>
    struct find_helper {};

    template <typename T, size_t Idx>
    struct find_helper<T, empty_pack, Idx> {
        static constexpr size_t value = Idx;
    };

    template <typename T, typename... Ts, size_t Idx>
    struct find_helper<T, type_pack<T, Ts...>, Idx> {
        static constexpr std::size_t value = Idx;
    };

    template <typename T, typename U, typename... Ts, size_t Idx>
    struct find_helper<T, type_pack<U, Ts...>, Idx> {
        static constexpr size_t value =
            find_helper<T, type_pack<Ts...>, Idx + 1>::value;
    };

  } // namespace __details

  template <typename T, typename TP, std::size_t From = 0>
  struct find
      : std::integral_constant<
            std::size_t, __details::find_helper<T, copy_t<TP, From, TP::size()>,
                                                From>::value> {};

  //* [findimpl]

  //* [findifimpl]
  namespace __details {

    template <template <typename...> class F, typename TP, size_t Idx,
              typename AlwaysVoid>
    struct find_if_helper {
        static constexpr size_t value = Idx;
    };

    template <template <typename...> class F, typename T, typename... Ts,
              size_t Idx>
    struct find_if_helper<F, type_pack<T, Ts...>, Idx,
                          enable_if_t<F<T>::value>> {
        static constexpr size_t value = Idx;
    };

    template <template <typename...> class F, typename T, typename... Ts,
              size_t Idx>
    struct find_if_helper<F, type_pack<T, Ts...>, Idx,
                          enable_if_t<F<T>::value == false>> {
        static constexpr size_t value =
            find_if_helper<F, type_pack<Ts...>, Idx + 1, void>::value;
    };

  } // namespace __details

  template <template <typename...> class F, class TP, std::size_t From = 0>
  struct find_if
      : std::integral_constant<std::size_t, __details::find_if_helper<
                                                F, copy_t<TP, From, TP::size()>,
                                                From, void>::value> {};

  //* [findifimpl]

  //* [allofimpl]
  template <template <typename...> class F, class TP>
  struct all_of {};

  /** @cond undocumented */

  template <template <typename...> class F>
  struct all_of<F, empty_pack> : std::true_type {};

  template <template <typename...> class F, typename T, typename... Ts>
  struct all_of<F, type_pack<T, Ts...>>
      : std::integral_constant<bool, F<T>::value &&
                                         all_of<F, type_pack<Ts...>>::value> {};

  //* [allofimpl]

  /** @endcond */

  //* [anyofimpl]
  template <template <typename...> class F, class TP>
  struct any_of {};

  /** @cond undocumented */

  template <template <typename...> class F>
  struct any_of<F, empty_pack> : std::false_type {};

  template <template <typename...> class F, typename T, typename... Ts>
  struct any_of<F, type_pack<T, Ts...>>
      : std::integral_constant<bool, F<T>::value ||
                                         any_of<F, type_pack<Ts...>>::value> {};

  //* [anyofimpl]

  /** @endcond */

  //* [noneofimpl]
  template <template <typename> class F, class TP>
  struct none_of : std::integral_constant<bool, !any_of<F, TP>::value> {};

  //* [noneofimpl]

  //* [countimpl]
  namespace __details {

    template <typename T, std::size_t Count, class TP>
    struct count_impl {
        static constexpr std::size_t value = Count;
    };

    template <typename T, std::size_t Count, typename... Ts>
    struct count_impl<T, Count, type_pack<T, Ts...>> {
        static constexpr std::size_t value =
            count_impl<T, Count + 1, type_pack<Ts...>>::value;
    };

    template <typename T, typename U, std::size_t Count, typename... Ts>
    struct count_impl<T, Count, type_pack<U, Ts...>> {
        static constexpr std::size_t value =
            count_impl<T, Count, type_pack<Ts...>>::value;
    };

  } // namespace __details

  template <typename T, class TP>
  struct count
      : std::integral_constant<std::size_t,
                               __details::count_impl<T, 0, TP>::value> {};

  //* [countimpl]

  //* [countifimpl]
  namespace __details {

    template <template <typename...> class F, std::size_t Count, class TP>
    struct count_if_impl {
        static constexpr std::size_t value = Count;
    };

    template <template <typename...> class F, std::size_t Count, typename T,
              typename... Ts>
    struct count_if_impl<F, Count, type_pack<T, Ts...>> {
      private:
        static constexpr std::size_t counter = F<T>::value ? Count + 1 : Count;
      public:
        static constexpr std::size_t value =
            count_if_impl<F, counter, type_pack<Ts...>>::value;
    };

  } // namespace __details

  template <template <typename...> class F, class TP>
  struct count_if
      : std::integral_constant<std::size_t,
                               __details::count_if_impl<F, 0, TP>::value> {};

  //* [countifimpl]

  //* [issortedimpl]
  namespace __details {

    template <class TP, template <typename A, typename B> class Less>
    struct is_sorted_impl {};

    template <typename T1, typename T2, typename... Ts,
              template <typename A, typename B> class Less>
    struct is_sorted_impl<type_pack<T1, T2, Ts...>, Less> {
        using inductive_t = is_sorted_impl<type_pack<Ts...>, Less>;
        using end_t = std::integral_constant<bool, false>;
        // if T1 less than T2, then continue recursion; otherwise false
        using next_t = typename std::conditional<!Less<T2, T1>::value,
                                                 inductive_t, end_t>::type;

        static constexpr bool value = next_t::value;
    };

    template <template <typename A, typename B> class Less>
    struct is_sorted_impl<empty_pack, Less> {
        static constexpr bool value = true;
    };

    template <typename T, template <typename A, typename B> class Less>
    struct is_sorted_impl<type_pack<T>, Less> {
        static constexpr bool value = true;
    };

  } // namespace __details

  template <class TP, template <typename A, typename B> class Less>
  struct is_sorted
      : std::integral_constant<bool,
                               __details::is_sorted_impl<TP, Less>::value> {};

  //* [issortedimpl]

  /**
   * @}
   *
   * @addtogroup TypePackGenerating
   * @{
   */

  //* [tailimpl]
  template <typename T>
  struct tail {};

  /** @cond undocumented */

  template <typename T, typename... Ts>
  struct tail<type_pack<T, Ts...>> {
      using type = type_pack<Ts...>;
  };
  //* [tailimpl]

  /** @endcond */

  //* [tailtimpl]
  template <class TP>
  using tail_t = typename tail<TP>::type;

  //* [tailtimpl]

  //* [concatenateimpl]
  template <class...>
  struct concatenate {};

  template <typename... Ts, typename... Us>
  struct concatenate<type_pack<Ts...>, type_pack<Us...>> {
      using type = type_pack<Ts..., Us...>;
  };

  template <typename T, typename... Ts>
  struct concatenate<type_pack<Ts...>, just_type<T>> {
      using type = type_pack<Ts..., T>;
  };

  template <typename T, typename... Ts>
  struct concatenate<just_type<T>, type_pack<Ts...>> {
      using type = type_pack<T, Ts...>;
  };

  template <typename A, typename B>
  struct concatenate<just_type<A>, just_type<B>> {
      using type = type_pack<A, B>;
  };

  template <class T, class... Ts>
  struct concatenate<T, Ts...>
      : concatenate<T, typename concatenate<Ts...>::type> {};
  //* [concatenateimpl]

  //* [concatenatetimpl]
  template <class... Ts>
  using concatenate_t = typename concatenate<Ts...>::type;

  //* [concatenatetimpl]

  template <class T, class U>
  constexpr auto operator+(T, U) -> concatenate_t<T, U> {
    return {};
  }

  //* [copyimpl]
  namespace __details {

    template <std::size_t Begin, std::size_t End, std::size_t Current>
    struct indexes {
        static constexpr std::size_t begin = Begin;
        static constexpr std::size_t end = End;
        static constexpr std::size_t current = Current;

        static_assert(begin <= end, "Begin index can not be greater that end");
    };

    template <class Indexes, class TP, typename AlwaysVoid>
    struct copy_impl {};

    template <class Indexes, typename T, typename... Ts>
    struct copy_impl<Indexes, type_pack<T, Ts...>,
                     enable_if_t<(Indexes::current < Indexes::begin)>> {
      private:
        using idxs_t =
            indexes<Indexes::begin, Indexes::end, Indexes::current + 1>;
      public:
        using type = typename copy_impl<idxs_t, type_pack<Ts...>, void>::type;
    };

    template <class Indexes, typename T, typename... Ts>
    struct copy_impl<Indexes, type_pack<T, Ts...>,
                     enable_if_t<(Indexes::current >= Indexes::begin &&
                                  Indexes::current < Indexes::end)>> {
      private:
        using idxs_t =
            indexes<Indexes::begin, Indexes::end, Indexes::current + 1>;

        static constexpr bool store_tail =
            Indexes::end - Indexes::current == type_pack<T, Ts...>::size();

        using recursive_copy =
            typename copy_impl<idxs_t, type_pack<Ts...>, void>::type;

        using just_tail = type_pack<Ts...>;

        // a little optimisation: if end - current == size of a type pack,
        // then just store remaining tail types, otherwise recursively apply
        // copy_impl
        using tail_t = typename std::conditional<store_tail, just_tail,
                                                 recursive_copy>::type;
      public:
        using type = concatenate_t<just_type<T>, tail_t>;
    };

    template <class Indexes, typename... Ts>
    struct copy_impl<Indexes, type_pack<Ts...>,
                     enable_if_t<(Indexes::current == Indexes::end)>> {
        using type = empty_pack;
    };

  } // namespace __details

  template <class TP, std::size_t StartIdx, std::size_t EndIdx>
  struct copy : __details::copy_impl<__details::indexes<StartIdx, EndIdx, 0>,
                                     TP, void> {};
  //* [copyimpl]

  //* [copytype]
  template <class TP, std::size_t Begin, std::size_t End>
  using copy_t = typename copy<TP, Begin, End>::type;

  //* [copytype]

  //* [copyifimpl]
  template <class TP, template <typename...> class Pred>
  struct copy_if {};

  template <template <typename...> class Pred>
  struct copy_if<empty_pack, Pred> {
      using type = empty_pack;
  };

  template <typename T, typename... Ts, template <typename...> class Pred>
  struct copy_if<type_pack<T, Ts...>, Pred> {
    private:
      using head_t = typename std::conditional<Pred<T>::value, type_pack<T>,
                                               empty_pack>::type;
    public:
      using type =
          concatenate_t<head_t, typename copy_if<type_pack<Ts...>, Pred>::type>;
  };
  //* [copyifimpl]

  //* [copyiftype]
  template <class TP, template <typename...> class Pred>
  using copy_if_t = typename copy_if<TP, Pred>::type;

  //* [copyiftype]

  //* [pushfrontimpl]
  template <typename T, class TP>
  struct push_front {};

  template <typename T, typename... Ts>
  struct push_front<T, type_pack<Ts...>> {
      using type = type_pack<T, Ts...>;
  };
  //* [pushfrontimpl]

  //* [pushfronttype]
  template <typename T, class TP>
  using push_front_t = typename push_front<T, TP>::type;

  //* [pushfronttype]

  //* [popfrontimpl]
  template <class TP>
  struct pop_front {
      using type = typename std::enable_if<std::is_same<TP, empty_pack>::value,
                                           empty_pack>::type;
  };

  template <typename T, typename... Ts>
  struct pop_front<type_pack<T, Ts...>> {
      using type = type_pack<Ts...>;
  };
  //* [popfrontimpl]

  //* [popfronttype]
  template <class TP>
  using pop_front_t = typename pop_front<TP>::type;

  //* [popfronttype]

  //* [pushbackimpl]
  template <typename T, class TP>
  struct push_back {};

  template <typename T, typename... Ts>
  struct push_back<T, type_pack<Ts...>> {
      using type = type_pack<Ts..., T>;
  };
  //* [pushbackimpl]

  //* [pushbacktype]
  template <typename T, class TP>
  using push_back_t = typename push_back<T, TP>::type;
  //* [pushbacktype]

  //* [popbackimpl]
  template <class TP>
  struct pop_back;

  template <typename T, typename U, typename... Ts>
  struct pop_back<type_pack<T, U, Ts...>> {
      using type = concatenate_t<type_pack<T, U>,
                                 typename pop_back<type_pack<Ts...>>::type>;
  };

  template <typename T>
  struct pop_back<type_pack<T>> {
      using type = empty_pack;
  };
  //* [popbackimpl]

  //* [popbacktype]
  template <class TP>
  using pop_back_t = typename pop_back<TP>::type;

  //* [popbacktype]

  //* [transformimpl]
  template <template <typename...> class F, class TP>
  struct transform {};

  template <template <typename...> class F, typename... Ts>
  struct transform<F, type_pack<Ts...>> {
      using type = type_pack<typename F<Ts>::type...>;
  };
  //* [transformimpl]

  //* [transformtype]
  template <template <typename...> class F, class TP>
  using transform_t = typename transform<F, TP>::type;

  //* [transformtype]

  //* [generateimpl]
  template <std::size_t N, typename T>
  struct generate {
      using type =
          concatenate_t<just_type<T>, typename generate<N - 1, T>::type>;
  };

  template <typename T>
  struct generate<0, T> {
      using type = empty_pack;
  };

  //* [generateimpl]

  //* [generatetype]
  template <std::size_t N, typename T>
  using generate_t = typename generate<N, T>::type;

  //* [generatetype]

  //* [removeimpl]
  template <typename T, class TP>
  struct remove {
      using type = TP;
  };

  template <typename T, typename... Ts>
  struct remove<T, type_pack<T, Ts...>> {
      using type = type_pack<Ts...>;
  };

  template <typename T, typename U, typename... Ts>
  struct remove<T, type_pack<U, Ts...>> {
      using type = concatenate_t<just_type<U>,
                                 typename remove<T, type_pack<Ts...>>::type>;
  };
  //* [removeimpl]

  //* [removetype]
  template <typename T, class TP>
  using remove_t = typename remove<T, TP>::type;

  //* [removetype]

  //* [removeifimpl]
  template <template <typename...> class F, class TP>
  struct remove_if {
      using type = TP;
  };

  template <template <typename...> class F, typename T, typename... Ts>
  struct remove_if<F, type_pack<T, Ts...>> {
    private:
      using head_type = typename std::conditional<F<T>::value, empty_pack,
                                                  type_pack<T>>::type;
    public:
      using type = concatenate_t<head_type, type_pack<Ts...>>;
  };
  //* [removeifimpl]

  //* [removeiftype]
  template <template <typename...> class F, class TP>
  using remove_if_t = typename remove_if<F, TP>::type;

  //* [removeiftype]

  //* [removeallimpl]
  template <typename T, class TP>
  struct remove_all {
      using type = TP;
  };

  template <typename T, typename... Ts>
  struct remove_all<T, type_pack<T, Ts...>> {
      using type = typename remove_all<T, type_pack<Ts...>>::type;
  };

  template <typename T, typename U, typename... Ts>
  struct remove_all<T, type_pack<U, Ts...>> {
      using type =
          concatenate_t<just_type<U>,
                        typename remove_all<T, type_pack<Ts...>>::type>;
  };
  //* [removeallimpl]

  //* [removealltype]
  template <typename T, class TP>
  using remove_all_t = typename remove_all<T, TP>::type;

  //* [removealltype]

  //* [removeallifimpl]
  namespace __details {

    template <template <typename...> class F, class TP, typename AlwaysVoid>
    struct remove_all_if_impl {
        using type = TP;
    };

    template <template <typename...> class F, typename T, typename... Ts>
    struct remove_all_if_impl<F, type_pack<T, Ts...>,
                              enable_if_t<F<T>::value>> {
        using type =
            typename remove_all_if_impl<F, type_pack<Ts...>, void>::type;
    };

    template <template <typename...> class F, typename T, typename... Ts>
    struct remove_all_if_impl<F, type_pack<T, Ts...>,
                              enable_if_t<F<T>::value == false>> {
        using type = concatenate_t<
            just_type<T>,
            typename remove_all_if_impl<F, type_pack<Ts...>, void>::type>;
    };

  } // namespace __details

  template <template <typename...> class F, class TP>
  struct remove_all_if : __details::remove_all_if_impl<F, TP, void> {};
  //* [removeallifimpl]

  //* [removealliftype]
  template <template <typename...> class F, class TP>
  using remove_all_if_t = typename remove_all_if<F, TP>::type;

  //* [removealliftype]

  //* [uniqueimpl]
  template <class TP>
  struct unique {
      using type = TP;
  };

  template <typename T, typename... Ts>
  struct unique<type_pack<T, Ts...>> {
    private:
      using tail = remove_all_t<T, type_pack<Ts...>>;
    public:
      using type = concatenate_t<just_type<T>, typename unique<tail>::type>;
  };
  //* [uniqueimpl]

  //* [uniquetype]
  template <class TP>
  using unique_t = typename unique<TP>::type;

  //* [uniquetype]

  //* [replaceimpl]
  template <typename Old, typename New, class TP>
  struct replace {
    private:
      template <typename T>
      struct rep_one_element {
          using type = typename std::conditional<std::is_same<T, Old>::value,
                                                 New, T>::type;
      };
    public:
      using type = transform_t<rep_one_element, TP>;
  };
  //* [replaceimpl]

  //* [replacetype]
  template <typename Old, typename New, class TP>
  using replace_t = typename replace<Old, New, TP>::type;

  //* [replacetype]

  //* [sortimpl]
  template <class TP, template <typename A, typename B> class Less>
  struct sort {};

  // base case 1
  template <typename T, template <typename A, typename B> class Less>
  struct sort<type_pack<T>, Less> {
      using type = type_pack<T>;
  };

  // base case 2
  template <template <typename A, typename B> class Less>
  struct sort<empty_pack, Less> {
      using type = empty_pack;
  };

  // inductive case
  template <typename T, typename... Ts,
            template <typename A, typename B> class Less>
  struct sort<type_pack<T, Ts...>, Less> {
    private:
      using tp_t = type_pack<T, Ts...>;
      using pivot = at_t<0, tp_t>;
      using pack = remove_t<pivot, tp_t>;

      // less than the pivot
      template <typename E>
      struct lt_pivot {
          static constexpr bool value = Less<E, pivot>::value;
      };

      // greater than or equal to the pivot
      template <typename E>
      struct gte_pivot {
          static constexpr bool value = !Less<E, pivot>::value;
      };

      // array of elements that are less
      using pack_lt = copy_if_t<tp_t, lt_pivot>;
      // array of elements that are greater/equal
      using pack_gt = copy_if_t<pack, gte_pivot>;

      using inductive_sort_lt = typename sort<pack_lt, Less>::type;
      using inductive_sort_gte = typename sort<pack_gt, Less>::type;
    public:
      using type = concatenate_t<inductive_sort_lt, just_type<pivot>,
                                 inductive_sort_gte>;
  };
  //* [sortimpl]

  //* [sorttype]
  template <class TP, template <typename A, typename B> class Less>
  using sort_t = typename sort<TP, Less>::type;

  //* [sorttype]

  /**
   * @}
   *
   * @addtogroup TypePackUtility
   * @{
   */

  //* [partcallerimpl]
  template <template <class...> class F, class... Ts>
  struct part_caller {
      template <class... Us>
      using type = typename F<Ts..., Us...>::type;
  };

  //* [partcallerimpl]

  //* cond undocumented
  //* [logicalimpl]
  //* [conjimpl]
  template <class...>
  struct conjunction : std::true_type {};

  template <class B>
  struct conjunction<B> : B {};

  template <class B1, class... B>
  struct conjunction<B1, B...>
      : std::conditional<bool(B1::value), conjunction<B...>, B1>::type {};
  //* [conjimpl]

  //* [disjimpl]
  template <class...>
  struct disjunction : std::false_type {};

  template <class B>
  struct disjunction<B> : B {};

  template <class B1, class... B>
  struct disjunction<B1, B...>
      : std::conditional<bool(B1::value), B1, disjunction<B...>>::type {};
  //* [disjimpl]

  //* [negimpl]
  template <class B>
  struct negation : std::integral_constant<bool, !B::value> {};
  //* [negimpl]

  //* [conjtype]
  template <class B1, class... Bn>
  using _and_ = conjunction<B1, Bn...>;
  //* [conjtype]

  //* [disjtype]
  template <class B1, class... Bn>
  using _or_ = disjunction<B1, Bn...>;
  //* [disjtype]

  //* [negtype]
  template <class B>
  using _not_ = negation<B>;
  //* [negtype]
  //* [logicalimpl]
  /** @endcond */

  /**
   * @}
   *
   * @addtogroup TypePackComparsion
   * @{
   */

  //* [sizeof_compare]
  template <typename A, typename B>
  struct sizeof_less : std::integral_constant<bool, (sizeof(A) < sizeof(B))> {};

  template <typename A, typename B>
  struct sizeof_more : std::integral_constant<bool, (sizeof(A) > sizeof(B))> {};

  //* [sizeof_compare]

  //* [baseis_compare]
  template <typename A, typename B>
  struct base_is_less
      : _and_<std::is_base_of<A, B>, _not_<std::is_same<A, B>>> {};

  template <typename A, typename B>
  struct derived_is_less
      : _and_<std::is_base_of<B, A>, _not_<std::is_same<A, B>>> {};

  //* [baseis_compare]

  /** @} */

} // namespace tp

/** @} */ // end of TypePack group

#endif
