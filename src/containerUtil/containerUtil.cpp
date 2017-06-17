#include <memory>

#include "fdl/containerUtil/containerUtil.hpp"

namespace fdl {
    namespace containerUtil {
        coordinatePair coordsFromIndex(const size_t index, const size_t width) {
            coordinatePair cp;
            cp.x = index % width;
            cp.y = index / width;
            return cp;
        }
    }
}