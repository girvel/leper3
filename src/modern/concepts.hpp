#pragma once

template<typename From, typename To>
concept ConvertibleTo = __is_convertible(From, To);

template<typename A, typename B>
concept Eq = requires(const A &a, const B &b) { {a == b} -> ConvertibleTo<bool>; };
