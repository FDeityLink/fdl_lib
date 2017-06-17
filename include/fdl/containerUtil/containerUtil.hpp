#ifndef CONTAINERUTIL_HPP
#define CONTAINERUTIL_HPP

namespace fdl {
    /**
     * @addtogroup containerUtil
     * @{
     */
    
    /**
     * @brief Container utility functions and classes.
     *
     * Namespace containing methods and classes
     * for various tasks that may need to be done
     * when working with containers (arrays, vectors, etc.)
     */
    namespace containerUtil {
        
        /**
         * This performs the reverse of coordsFromIndex()
         * by returning the index of an element in a 2D
         * array stored as a 1D array given the element's
         * x and y coordinates and the array's 'width'.
         * For example, if the following 2D array were
         * stored as a 1D array (row by row such that
         * the elements contain incrementing numbers) <br>
         *
         * @verbatim
           0    1    2    3
           4    5    6    7
           8    9   10   11
           @endverbatim
         *
         * and you wanted to access the element at (1,1),
         * passing this function (1, 1, 4) would yield an
         * index of 5. Storing a 2D container as a 1D container
         * and using this function rather than using a full-fledged
         * 2D container is useful for speed and to reduce memory usage.
         *
         * @param x The x coordinate of the element to be accessed.
         * @param y The y coordinate of the element to be accessed.
         * @param width The 'width' of the 2D container stored as a
         *              1D container.
         *
         * @return The index of the element to be accessed.
         */
        inline size_t indexFromCoords(const size_t x, const size_t y, const size_t width) {
            return ((width * y) + x);
        }
        
        /**
         * Stores x and y coordinates.
         */
        struct coordinatePair {
            int x = 0, y = 0;
        };
        
        /**
         * This performs the reverse of indexFromCoords()
         * by returning the x and y coordinates of an
         * element in a 2D array stored as a 1D array
         * given the element's index and the array's 'width'.
         * See indexFromCoords() for more detail on how this
         * works.
         *
         * @param index The index to retrieve the x and y
         *              coordinates of.
         * @param width The 'width' of the 2D container stored
         *              as a 1D container.
         *
         * @return A coordinatePair containing the x and y
                   coordinates sought.
         */
        coordinatePair coordsFromIndex(const size_t index, const size_t width);
    }
    /**
     * @}
     */
}

#endif //CONTAINERUTIL_HPP