/*! \file  utils.h
    \brief Contains various generic utils.
*/

#ifndef UTILS
#define UTILS

#include <vector>
#include <memory>

//! Tools for fixing managing ownership
namespace utils {
    //! Ignores any number of values.
    /*! Used to unpack template parameter tuple into statements with side-effects. */
    void ignore(...) {}

    //! Vector with ownership to its elements.
    template<typename T>
    using unique_vector = std::vector<std::unique_ptr<T>>;

    //! Constructor for vector owning its elements.
    /*! Fixes the fact that initializer lists use copying instead of moving. */
    template<typename T, typename... Args>
    auto make_unique_vector(Args&& ...args) {
        unique_vector<T> v;
        v.reserve(sizeof...(args));
        ignore<int>((v.emplace_back(std::move(args)), 0)...);
        return v;
    }
}

#endif // UTILS
