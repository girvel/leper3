#pragma once

#include "integer.hpp"

template<typename From, typename To>
concept ConvertibleTo = __is_convertible(From, To);

template<typename A, typename B>
concept Eq = requires(const A &a, const B &b) { {a == b} -> ConvertibleTo<bool>; };

// INTEGRAL IMPLEMENTATION //

template<typename T> struct remove_cv { using type = T; };
template<typename T> struct remove_cv<const T> { using type = T; };
template<typename T> struct remove_cv<volatile T> { using type = T; };
template<typename T> struct remove_cv<const volatile T> { using type = T; };
template<typename T> using remove_cv_t = typename remove_cv<T>::type;

template<typename T> struct is_integral { static constexpr bool value = false; };
template<> struct is_integral<u8> { static constexpr bool value = true; };
template<> struct is_integral<u16> { static constexpr bool value = true; };
template<> struct is_integral<u32> { static constexpr bool value = true; };
template<> struct is_integral<u64> { static constexpr bool value = true; };
template<> struct is_integral<i8> { static constexpr bool value = true; };
template<> struct is_integral<i16> { static constexpr bool value = true; };
template<> struct is_integral<i32> { static constexpr bool value = true; };
template<> struct is_integral<i64> { static constexpr bool value = true; };

template<typename T>
concept Integral = is_integral<remove_cv_t<T>>::value;
