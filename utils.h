#ifndef UTILS
#define UTILS

#include <vector>
#include <memory>

/*! \namespace  utils
 *  \brief  Tools for fixing managing ownership
 */
namespace utils {
    template<typename T>
    struct ignore {
        ignore(...) {}
    };

    template<typename T>
    using unique_vector = std::vector<std::unique_ptr<T>>;

    template<typename T, typename... Args>
    auto make_unique_vector(Args&& ...args) {
        unique_vector<T> v;
        v.reserve(sizeof...(args));
        ignore<int>((v.emplace_back(std::move(args)), 0)...);
        return v;
    }
}

#endif // UTILS
