#pragma once

template<typename T>
constexpr const T &min(const T &a, const T &b) {
    return a < b ? a : b;
}

template<typename T, typename... Args>
constexpr const T &min(const T &head, const Args &...tail) {
    return min(head, min(tail...));
}

template<typename T>
constexpr const T &max(const T &a, const T &b) {
    return a > b ? a : b;
}

template<typename T, typename... Args>
constexpr const T &max(const T &head, const Args &...tail) {
    return max(head, max(tail...));
}
