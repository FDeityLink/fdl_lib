#ifndef LIMITEDALLOCATOR_HPP
#define LIMITEDALLOCATOR_HPP

#include <memory>

namespace fdl {
    namespace containerUtil {
        
        /**
         * @brief An allocator with a specifiable size limit.
         *
         * Class that permits setting the limit to
         * how much memory the allocator can allocate.
         * This is useful if you need to use vectors
         * or strings for their ability to grow and shrink
         * but still must limit their size
         */
        template <typename T>
        class limitedAllocator : public std::allocator <T> {
            public:
                template <typename U> 
                struct rebind {
                    typedef limitedAllocator<U> other;
                };
                
                typedef typename std::allocator<T>::size_type size_type;
                
                limitedAllocator(size_type size);
                
                size_type max_size() const;
                
            private:
                size_type maxSize;
        };
    
    }
}

#include "limitedAllocator.cpp"

#endif //LIMITEDALLOCATOR_HPP