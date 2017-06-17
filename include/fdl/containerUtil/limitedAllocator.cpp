#include <memory>

#include "fdl/containerUtil/limitedAllocator.hpp"

namespace fdl {
    namespace containerUtil {
        template <typename T>
        limitedAllocator<T>::limitedAllocator(size_type maxSize) : maxSize(maxSize) {}
        
        template <typename T>
        typename limitedAllocator <T>::size_type limitedAllocator<T>::max_size() const {
            return maxSize;
        }
    }
}