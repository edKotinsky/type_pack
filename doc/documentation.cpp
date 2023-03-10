/**
 * @brief Main namespace of a library
 *
 * TypePack Library provides metaprogramming type list and a great set of
 * operations with it like indexing access, generating, copying, comparsion,
 * and even sorting.
 *
 * In fact, it is a hobby project, caused by my wish to deeply explore C++
 * metaprogramming and other topics, like doxygen usage and customisation,
 * documentation writing, testing, etc.
 *
 */
namespace tp {
  /**
   * @page license License
   *
   * @copyright
   * Copyright (c) 2023 Gleb Zlatanov
   *
   * @license
   * MIT License
   *
   * @par
   * Permission is hereby granted, free of charge, to any person obtaining a
   * copy of this software and associated documentation files (the "Software"),
   * to deal in the Software without restriction, including without limitation
   * the rights to use, copy, modify, merge, publish, distribute, sublicense,
   * and/or sell copies of the Software, and to permit persons to whom the
   * Software is furnished to do so, subject to the following conditions:
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
   * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   * DEALINGS IN THE SOFTWARE.
   */

  /**
   * @struct is_equal
   * @brief Checks if two type_packs or just_types equal
   *
   * @tparam T left operand
   * @tparam U right operand
   *
   * Operation compares just_type with just_type, type_pack with type_pack and
   * type_pack with just_type. The last comparsion is only allowed to type_pack,
   * which has only one type. Provides the member constant `value` that is equal
   * to `true` if the types are equal, otherwise `false`. `is_equal` inherits
   * `std::integral_constant` members.
   *
   * Implementation
   *
   * @snippet type_pack.hpp isequalimpl
   */

  /**
   * @struct is_not_equal
   * @brief Checks if type_packs or just_types not equal
   *
   * @tparam T left operand
   * @tparam U right operand
   *
   * Operation compares just_type with just_type, type_pack with type_pack and
   * type_pack with just_type. The last comparsion is only allowed to type_pack,
   * which has only one type. Provides the member constant `value` that is equal
   * to `true` if the types are not equal, otherwise `false`. `is_not_equal`
   * inherits `std::integral_constant` members.
   */

  /**
   * @struct lexicographical_compare
   * @brief Lexicographically compares two type packs
   *
   * @tparam TP1 first type_pack
   * @tparam TP2 second type_pack
   * @tparam Less @ref comp_meta_req "comparsion metafunction"
   *
   * Checks if the first type_pack is lexicographically less than the second.
   * Elements are compared using binary comparsion metafunction `Less`. This
   * metafunction must provide the member constant `value` with the type `bool`,
   * which is true, if the first element is less the second.
   *
   * Lexicographical comparison is an operation with the following properties:
   *
   * - Two ranges are compared element by element.
   * - The first mismatching element defines which range is lexicographically
   * less or greater than the other.
   * - If one range is a prefix of another, the shorter
   * range is lexicographically less than the other.
   * - If two ranges have equivalent
   * elements and are of the same length, then the ranges are lexicographically
   * equal.
   * - An empty range is lexicographically less than any non-empty range. Two
   * empty ranges are lexicographically equal.
   */

  /**
   * @struct head
   * @brief Returns the first type of a pack
   *
   * @tparam T type_pack
   *
   * Provides the member typedef `type`, which is the first type of the
   * type_pack as a result. If a pack is empty, member typedef will not be
   * provided, which may cause a compilation error.
   *
   * Helper type
   *
   * @snippet type_pack.hpp headtimpl
   *
   * Implementation
   *
   * @snippet type_pack.hpp headimpl
   */

  /**
   * @struct at
   * @brief Performs access to elements of a type_pack by index
   *
   * @tparam Idx index
   * @tparam TP type_pack
   *
   * Provides the member typedef `type`, which is the element of a type_pack
   * with the index `Idx`. If an index is greater than size of a type_pack,
   * compiler error will happen.
   *
   * Helper type
   *
   * @snippet type_pack.hpp attimpl
   *
   * Implementation
   *
   * @snippet type_pack.hpp atimpl
   */

  /**
   * @struct max
   * @brief Returns the max type of a type_pack
   *
   * @tparam TP type_pack
   * @tparam Less @ref comp_meta_req "comparsion metafunction"
   *
   * Provides the member typedef `type` with the "max" type from a type_pack.
   * If the type_pack is empty, the type will be `empty_type`.
   *
   * Helper type
   *
   * @snippet type_pack.hpp maxtimpl
   *
   * Implementation
   *
   * @snippet type_pack.hpp maximpl
   *
   * See sizeof_less, sizeof_more, base_is_less, derived_is_less.
   */

  /**
   * @struct min
   * @brief Returns the mintype of a type_pack
   *
   * @tparam TP type_pack
   * @tparam Less @ref comp_meta_req "comparsion metafunction"
   *
   * Provides the member typedef `type` with the "min" type from a type_pack.
   * If the type_pack is empty, the type will be `empty_type`.
   *
   * Helper type
   *
   * @snippet type_pack.hpp mintimpl
   *
   * Implementation
   *
   * @snippet type_pack.hpp minimpl
   *
   * See sizeof_less, sizeof_more, base_is_less, derived_is_less.
   */

  /**
   * @struct contains
   * @brief Checks if the certain type exists in the type_pack
   *
   * @tparam T type
   * @tparam TP type_pack
   *
   * Provides the member constant `value` with the type `bool`, which is `true`
   * if the type is found in a pack and `false` otherwise.
   *
   * Implementation
   *
   * @snippet type_pack.hpp containsimpl
   */

  /**
   * @struct find
   * @brief Finds the certain type in the type_pack
   *
   * @tparam T type
   * @tparam TP type_pack
   * @tparam From start index (optional)
   *
   * Provides the member constant `value` with the type `std::size_t`, which is
   * an index of a type in a type_pack. If the type is not found, index will
   * be equal to the size of a type_pack.
   *
   * If `From` index is given, searching will started from it. By default this
   * index is 0.
   *
   * Implementation
   *
   * @snippet type_pack.hpp findimpl
   *
   * See find_if.
   */

  /**
   * @struct find_if
   * @brief First the first type in a type_pack satisfying predicate condition
   *
   * @tparam F @ref unpred_req "unary_predicate"
   * @tparam TP type_pack
   * @tparam From start index (optional)
   *
   * Provides the member constant `value` with the type `std::size_t`, which is
   * an index of a type in a type_pack. If the type is not found, index will
   * be equal to the size of a type_pack.
   *
   * If `From` index is given, searching will started from it. By default this
   * index is 0.
   *
   * Implementation
   *
   * @snippet type_pack.hpp findifimpl
   *
   * See tp::find.
   */

  /**
   * @struct all_of
   * @brief Checks if unary predicate is `true` for all elements in a type_pack
   *
   * @tparam F @ref unpred_req "unary_predicate"
   * @tparam TP type_pack
   *
   * Provides the member constant `value` with the type `bool`, which is `true`
   * if unary predicate is `true` for all elements in a pack, `false` otherwise.
   * Inherits `std::integral_constant` members.
   *
   * Return value `true` if unary predicate returns true for all elements in the
   * range, `false` otherwise. Returns true if the range is empty.
   *
   * Implementation
   *
   * @snippet type_pack.hpp allofimpl
   *
   * See tp::any_of, tp::none_of.
   */

  /**
   * @struct any_of
   * @brief Checks if unary predicate is `true` for at least one element in a
   * type_pack
   *
   * @tparam F @ref unpred_req "unary_predicate"
   * @tparam TP type_pack
   *
   * Provides the member constant `value` with the type `bool`, which is `true`
   * if unary predicate is `true` for at least one element in a pack, `false`
   * otherwise. Inherits `std::integral_constant` members.
   *
   * Return value `true` if unary predicate returns `true` for at least one
   * element in the range, `false` otherwise. Returns `false` if the range is
   * empty.
   *
   * Implementation
   *
   * @snippet type_pack.hpp anyofimpl
   *
   * See tp::all_of, tp::none_of.
   */

  /**
   * @struct none_of
   * @brief Checks if unary predicate is `true` for no elements in a type_pack
   *
   * @tparam F @ref unpred_req "unary_predicate"
   * @tparam TP type_pack
   *
   * Provides the member constant `value` with the type `bool`, which is `true`
   * if unary predicate is `true` for no elements in a pack, `false`
   * otherwise. Inherits `std::integral_constant` members.
   *
   * Return value `true` if unary predicate returns `true` for no elements in
   * the range, `false` otherwise. Returns `true` if the range is empty.
   *
   * Implementation
   *
   * @snippet type_pack.hpp noneofimpl
   *
   * See tp::any_of, tp::all_of.
   */

  /**
   * @struct count
   * @brief Returns the number of elements with the specific type
   *
   * @tparam T type to look for
   * @tparam TP type_pack
   *
   * Provides the member constant `value` with the type `std::size_t`. Counts
   * the elements that are equal to type `T` (using `std::is_same`).  Inherits
   * `std::integral_constant` members.
   *
   * If a pack is empty, result will be 0.
   *
   * @snippet type_pack.hpp countimpl
   *
   * See tp::count_if.
   */

  /**
   * @struct count_if
   * @brief Returns the number of elements satisfying predicate condition
   *
   * @tparam F @ref unpred_req "unary predicate"
   * @tparam TP type_pack
   *
   * Provides the member constant `value` of type `std::size_t`. Counts elements
   * on which predicate returns `true`. Inherits `std::integral_constant`
   * members.
   *
   * If a pack is empty, result will be 0.
   *
   * Implementation
   *
   * @snippet type_pack.hpp countifimpl
   *
   * See tp::count.
   */

  /**
   * @struct tail
   *
   * @brief Returns the type_pack which is a subrange of an original pack except
   * the first element
   *
   * @tparam TP type_pack
   *
   * Provides the member typedef `type` which is a type_pack of all elements of
   * an original pack except first element (head). If a pack is empty,
   * member typedef will not be provided, which may cause a compilation error.
   *
   * Helper type
   *
   * @snippet type_pack.hpp tailtimpl
   *
   * Implementation
   *
   * @snippet type_pack.hpp tailimpl
   *
   * See tp::head.
   */

  /**
   * @struct concatenate
   * @brief Concatenates type_packs and just_types
   *
   * Provides the member typedef `type` which is a pack of elements of
   * concatenation operands. If type of operands is just_type, then the
   * resulting type is type_pack with two elements. Allows to concatenate a
   * type_pack with just_type. If just_type is the first operand, its type
   * will be added to front of a pack, otherwise if just_type is the second
   * operand and type_pack is the first, type will be concatenated to back.
   *
   * Operation acts recursively and can concatenate more, that two operands.
   *
   * Note that `concatenate` requires type wrapped either in just_type or
   * type_pack. Attempt to concatenate type itself will cause a compiler error.
   *
   * Helper type
   *
   * @snippet type_pack.hpp concatenatetimpl
   *
   * Implementation
   *
   * @snippet type_pack.hpp concatenateimpl
   *
   * See tp::push_back, tp::push_front.
   */

  /**
   * @struct copy
   * @brief Constructs new type_pack with the elements of other type_pack
   *
   * @tparam TP original type_pack
   * @tparam StartIdx begin
   * @tparam EndIdx end
   *
   * Provides the member typedef `type` that is a pack with elements of the
   * other type_pack on range [begin; end).
   *
   * In following cases compilation error will happen:
   * - If the begin index is greater than end index
   * - If the end index is greater than the size of a pack
   *
   * Helper type
   *
   * @snippet type_pack.hpp copytype
   *
   * Implementation
   *
   * @snippet type_pack.hpp copyimpl
   *
   * See tp::copy_if.
   */

  /**
   * @struct copy_if
   * @brief Constructs new type_pack with the elements, that satisfying a
   * condition, of other type_pack
   *
   * @tparam TP type_pack
   * @tparam F @ref unpred_req "unary predicate"
   *
   * Provides the member typedef `type` that is a pack with elements of other
   * type_pack. Algorithm copies elements, for which unary predicate returns
   * `true`.
   *
   * Helper type
   *
   * @snippet type_pack.hpp copyiftype
   *
   * Implementation
   *
   * @snippet type_pack.hpp copyifimpl
   *
   * See tp::copy.
   */

  /**
   * @struct push_front
   * @brief Generates new type_pack by adding a type to the front of other
   * type_pack
   *
   * @tparam T adding type
   * @tparam TP type_pack
   *
   * Provides the member typedef `type` that is a pack with all elements of
   * other type_pack, prepended with added type.
   *
   * Helper type
   *
   * @snippet type_pack.hpp pushfronttype
   *
   * Implementation
   *
   * @snippet type_pack.hpp pushfrontimpl
   */

  /**
   * @struct push_back
   * @brief Generates new type_pack by adding a type to the back of other
   * type_pack
   *
   * @tparam T adding type
   * @tparam TP type_pack
   *
   * Provides the member typedef `type` that is a pack with all elements of
   * other type_pack, appended with added type.
   *
   * Helper type
   *
   * @snippet type_pack.hpp pushbacktype
   *
   * Implementation
   *
   * @snippet type_pack.hpp pushbackimpl
   */

  /**
   * @struct pop_front
   * @brief Removes the first type from a type_pack
   *
   * @tparam TP type_pack
   *
   * Provides the member typedef `type` that is a pack with all elements of
   * other type_pack, except the first element. If the original pack is empty,
   * resulting type is empty_pack.
   *
   * Helper type
   *
   * @snippet type_pack.hpp popfronttype
   *
   * Implementation
   *
   * @snippet type_pack.hpp popfrontimpl
   */

  /**
   * @struct pop_back
   * @brief Removes the last type from a type_pack
   *
   * @tparam TP type_pack
   *
   * Provides the member typedef `type` that is a pack with all elements of
   * other type_pack, except the last element. If the original pack is empty,
   * resulting type is empty_pack.
   *
   * Helper type
   *
   * @snippet type_pack.hpp popbacktype
   *
   * Implementation
   *
   * @snippet type_pack.hpp popbackimpl
   */

  /**
   * @struct transform
   * @brief Generates new type_pack from other type_pack by applying to its
   * elements transforming metafunction
   *
   * @tparam F unary transforming metafunction, that is take one template type
   * and provide the member typedef `type`. The signature of transforming
   * metafunction must be equivalent to the following: `template <typename T>
   * struct trans;`.
   *
   * @tparam TP type_pack
   *
   * Provides the member typedef `type` that is a type_pack, filled with types,
   * generated by transforming metafunction.
   *
   * Helper type
   *
   * @snippet type_pack.hpp transformtype
   *
   * Implementation
   *
   * @snippet type_pack.hpp transformimpl
   */

  /**
   * @struct generate
   * @brief Creates a type_pack with count of elements of a specific type
   *
   * @tparam N number of elements
   * @tparam T type of an element
   *
   * Provides the member typedef `type` that is a type_pack with N elements of
   * a type T. If N is 0, the resulting type is empty_pack.
   *
   * Helper type
   *
   * @snippet type_pack.hpp generatetype
   *
   * Implementation
   *
   * @snippet type_pack.hpp generateimpl
   */

  /**
   * @struct remove
   * @brief Removes the first element of a specific type
   *
   * @tparam T type to remove
   *
   * Provides the member typedef `type` that is a type_pack with elements from
   * the original type_pack except the first occurency of a type T.
   *
   * If a pack is empty, the resulting type is empty_pack.
   *
   * Helper type
   *
   * @snippet type_pack.hpp removetype
   *
   * Implementation
   *
   * @snippet type_pack.hpp removeimpl
   *
   * See remove_if.
   */

  /**
   * @struct remove_if
   * @brief Removes the first element, that satisfies a specific criteria
   *
   * @tparam F @ref unpred_req "unary predicate"
   *
   * Provides the member typedef `type` that is a type_pack with elements from
   * the original type_pack except the first occurency of a type, for which
   * unary predicate returned `true`.
   *
   * If a pack is empty, the resulting type is empty_pack.
   *
   * Helper type
   *
   * @snippet type_pack.hpp removeiftype
   *
   * Implementation
   *
   * @snippet type_pack.hpp removeifimpl
   */

  /**
   * @struct remove_all
   * @brief Removes all elements of a specific type
   *
   * @tparam T removing type
   *
   * Provides the member typedef `type` that is a type_pack with all elements
   * from the original type_pack except all elements of a type T.
   *
   * If a pack is empty, the resulting type is empty_pack.
   *
   * Helper type
   *
   * @snippet type_pack.hpp removealltype
   *
   * Implementation
   *
   * @snippet type_pack.hpp removeallimpl
   */

  /**
   * @struct remove_all_if
   * @brief Removes all elements, that satisfies a specific criteria
   *
   * @tparam F @ref unpred_req "unary predicate"
   *
   * Provides the member typedef `type` that is a type_pack with all elements
   * from the original type_pack except all elements for which predicate returns
   * `true`.
   *
   * Helper type
   *
   * @snippet type_pack.hpp removealliftype
   *
   * Implementation
   *
   * @snippet type_pack.hpp removeallifimpl
   */

  /**
   * @struct unique
   * @brief Removes duplicate types
   *
   * @tparam TP type_pack
   *
   * Eliminates all except the first element from every consecutive group of
   * equivalent elements from a type_pack. Provides the member typedef `type`.
   *
   * Helper type
   *
   * @snippet type_pack.hpp uniquetype
   *
   * Implementation
   *
   * @snippet type_pack.hpp uniqueimpl
   */

  /**
   * @struct replace
   * @brief Replaces all elements of a specific type
   *
   * @tparam Old old type to replace
   * @tparam New the type to use as replacement
   *
   * Replaces all elements of type Old to type New.
   *
   * Helper type
   *
   * @snippet type_pack.hpp replacetype
   *
   * Implementation
   *
   * @snippet type_pack.hpp replaceimpl
   */

  /**
   * @struct sort
   * @brief Sorts type_pack
   *
   * @tparam TP type_pack
   * @tparam Less @ref comp_meta_req "comparsion metafunction"
   *
   * Sorts list of types using comparsion metafunction. Provides the member
   * typedef `type` which is a pack of types, sorted in ascending order.
   *
   * Helper type
   *
   * @snippet type_pack.hpp sorttype
   *
   * Implementation
   *
   * @snippet type_pack.hpp sortimpl
   *
   * See tp::copy_if, tp::at, tp::remove, tp::concatenate, tp::is_sorted, @ref
   * comp_meta_req "comparsion metafunction".
   */

  /**
   * @struct is_sorted
   * @brief Checks if type_pack is sorted
   *
   * @tparam TP type_pack
   * @tparam Less @ref comp_meta_req "comparsion metafunction"
   *
   * Checks if the list of types is sorted using comparsion metafunction.
   * Provides the member constant `value`, which is `true`, if list is sorted.
   * Otherwise `false`. Inherits `std::integral_constant`.
   *
   * Implementation
   *
   * @snippet type_pack.hpp issortedimpl
   *
   * See tp::sort, @ref comp_meta_req "comparsion metafunction".
   */

  /**
   * @struct part_caller
   * @brief Partially specializes template
   *
   * @tparam F template template class, must be inherited from
   * `std::integral_constant`
   * @tparam Ts template parameter pack
   *
   * Provides the template member typedef `type`, which is an alias to member
   * typedef `type` of an F class, which is partially specialized with `Ts` -
   * "partially calls" it. It allows to convert a @ref comparsion_meta
   * "comparsion metafunction" to a @ref unary_predicate_struct "unary
   * predicate" metafunction.
   *
   * Implementation
   *
   * @snippet type_pack.hpp partcallerimpl
   */

  /**
   * @struct sizeof_less
   * @brief Compares two types by their size using sizeof operator
   *
   * @tparam A first type
   * @tparam B second type
   *
   * @ref comparsion_meta "Comparsion metafunction". Provides the member
   * constant `value`, which is `true` is the first type is lesser than the
   * second. Otherwise, `false`. Inherits `std::integral_constant`.
   *
   * Implementation
   *
   * @snippet type_pack.hpp sizeof_compare
   *
   * See sizeof_more.
   */

  /**
   * @struct sizeof_more
   * @brief Compares two types by their size using sizeof operator
   *
   * @tparam A first type
   * @tparam B second type
   *
   * @ref comparsion_meta "Comparsion metafunction". Provides the member
   * constant `value`, which is `true` is the first type is greater than the
   * second. Otherwise, `false`. Inherits `std::integral_constant`.
   *
   * Implementation
   *
   * @snippet type_pack.hpp sizeof_compare
   *
   * See sizeof_less.
   */

  /**
   * @struct base_is_less
   * @brief Compares two classes by their inheritance relationship
   *
   * @tparam A first class
   * @tparam B second class
   *
   * @ref comparsion_meta "Comparsion metafunction". Provides the member
   * constant `value`, which is `true` if the first class is base of the second.
   * Value will be `false` in the following cases:
   *
   * - if the first class is not derived from the second
   * - if classes have not inheritance relationship
   * - if types A and B are the same class
   *
   * Suppose the following inheritance diagram:
   *
   * @dotfile inheritance_hierarchy.gv "Example class hierarchy"
   *
   * For this diagram the following code is correct:
   *
   * ```cpp
   * assert((tp::base_is_less<Base, Derived1>::value == true));
   * assert((tp::base_is_less<Base, Derived3>::value == true));
   *
   * assert((tp::base_is_less<Derived1, Derived3>::value == true));
   * assert((tp::base_is_less<Derived2, Derived3>::value == false));
   *
   * assert((tp::base_is_less<Derived1, Derived1>::value == false));
   * ```
   *
   * See derived_is_less.
   */

  /**
   * @struct derived_is_less
   * @brief Compares two classes by their inheritance relationship
   *
   * @tparam A first class
   * @tparam B second class
   *
   * @ref comparsion_meta "Comparsion metafunction". Provides the member
   * constant `value`, which is `true` if the first class is derived from the
   * second. Value will be `false` in the following cases:
   *
   * - if the first class is not derived from the second
   * - if classes have not inheritance relationship
   * - if types A and B are the same class
   *
   * Suppose the following inheritance diagram:
   *
   * @dotfile inheritance_hierarchy.gv "Example class hierarchy"
   *
   * For this diagram the following code is correct:
   *
   * ```cpp
   * assert((tp::derived_is_less<Derived1, Base>::value == true));
   * assert((tp::derived_is_less<Derived2, Base>::value == true));
   * assert((tp::derived_is_less<Derived3, Base>::value == true));
   *
   * assert((tp::derived_is_less<Derived3, Derived1>::value == true));
   * assert((tp::derived_is_less<Derived3, Derived2>::value == true));
   *
   * assert((tp::derived_is_less<Derived1, Derived1>::value == false));
   * ```
   *
   * See base_is_less.
   */

  /**
   * @struct negation
   * @brief Logical NOT metafunction
   *
   * @tparam B any type such that the expression bool(B::value) is a valid
   * constant expression
   *
   * Forms the [logical negation](https://en.wikipedia.org/wiki/Negation) of the
   * type trait `B`. This metafunction is an analog for
   * [`std::negation`](https://en.cppreference.com/w/cpp/types/negation), which
   * is provided by C++ 17 standard.
   *
   * Helper type
   *
   * @snippet type_pack.hpp negtype
   *
   * Implementation
   *
   * @snippet type_pack.hpp negimpl
   */

  /**
   * @struct conjunction
   * @brief Variadic logical AND metafunction
   *
   * @tparam B... every template argument Bi for which `Bi::value` is
   * instantiated must be usable as a base class and define member value that is
   * convertible to `bool`
   *
   * Forms the [logical
   * conjunction](https://en.wikipedia.org/wiki/Logical_conjunction) of the type
   * traits `B...`, effectively performing a logical AND on the sequence of
   * traits. `tp::conjunction` is an analog for
   * [`std::conjunction`](https://en.cppreference.com/w/cpp/types/conjunction).
   *
   * The specialization `std::conjunction<B1, ..., BN>` has a public and
   * unambiguous base that is
   *
   * - if `sizeof...(B) == 0`, `std::true_type`; otherwise
   * - the first type Bi in B1, ..., BN for which `bool(Bi::value) == false`,
   * or BN if there is no such type.
   *
   * Helper type
   *
   * @snippet type_pack.hpp conjtype
   *
   * Implementation
   *
   * @snippet type_pack.hpp conjimpl
   */

  /**
   * @struct disjunction
   * @brief Variadic logical OR metafunction
   *
   * @tparam B... every template argument Bi for which `Bi::value` is
   * instantiated must be usable as a base class and define member value that is
   * convertible to `bool`
   *
   * Forms the [logical
   * disjunction](https://en.wikipedia.org/wiki/Logical_disjunction) of the type
   * traits `B...`, effectively performing a logical OR on the sequence of
   * traits.
   *
   * The specialization `std::disjunction<B1, ..., BN>` has a public and
   * unambiguous base that is
   *
   * - if `sizeof...(B) == 0`, `std::false_type`; otherwise
   * - the first type Bi in B1, ..., BN for which `bool(Bi::value) == true`, or
   * BN if there is no such type.
   *
   * Helper type
   *
   * @snippet type_pack.hpp disjtype
   *
   * Implementation
   *
   * @snippet type_pack.hpp disjimpl
   */

  /**
   * @page unary_predicate_struct Unary predicate struct
   *
   * @anchor unpred_req Unary predicate class is a metafunction that must
   * take one template parameter and provide the public member constant `value`
   * of type `bool`. The value of a member depends if a template parameter type
   * satisfies or not the predicate's criteria.
   *
   * The C++ standard library offers a great set of unary predicates, which are
   * called "type categories" and "type properties". See
   * https://en.cppreference.com/w/cpp/header/type_traits
   *
   * Unary predicate definition may follow SFINAE principle. It is strongly
   * recommended to inherit unary_predicate classes from std::integral_constant.
   *
   * Example:
   *
   * ```cpp
   * template <typename T>
   * struct is_int : std::false_type {};
   *
   * template <>
   * struct is_int<int> : std::true_type {};
   * ```
   *
   * See copy_if, remove_if, count_if, tp::all_of, tp::any_of, tp::none_of.
   */

  /**
   * @page comparsion_meta Metafunctions that provide comparsion operations
   *
   * @anchor comp_meta_req Comparsion metafunction is a class, that takes
   * two template parameters and provides the public member constant `value`
   * of type `bool`. The value of a member depends if one type is "greater" or
   * "lesser" than other.
   *
   * Comparsion between types actually is not a comparsion in usual sense.
   * Types are quite abstract category to define a comparsion operation for
   * them.
   *
   * But in the computer representation all types has their size, which is a
   * material trait of a type. So types can be compared by their sizes.
   *
   * @snippet type_pack.hpp sizeof_compare
   *
   * Also types can be classes, which has the inheritance relationship. You can
   * say that is a derived class is "lesser" than the base class.
   *
   * @snippet type_pack.hpp baseis_compare
   *
   * It is strongly recommended to inherit comparsion classes from
   * `std::integral_constant` class. User can easily define its own comparsion
   * metafunction by using metaprogramming logical operations, that are
   * provided by the library.
   *
   * See sizeof_less, sizeof_more, base_is_less, derived_is_less, tp::negation,
   * tp::conjunction, tp::disjunction.
   *
   */

} // namespace tp
