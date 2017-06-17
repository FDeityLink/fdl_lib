#ifndef FILEUTIL_HPP
#define FILEUTIL_HPP

#include <string>

#include <fstream>

#include <exception>

#include <cstdint>

namespace fdl {
    /**
     * @addtogroup fileUtil
     * @{
     */
    
    /**
     * @brief File and filesystem utility functions.
     *
     * Namespace containing functions and classes for
     * performing small tasks that may be needed when
     * parsing files or working with the filesystem
     */
    namespace fileUtil {
        /**
         * Returns whether or not the system the program is executing
         * on uses the little-endian byte ordering system.
         *
         * @return True if the program is executing on a little-endian
         *         system, false otherwise.
         */
        bool isLittleEndian();
        
        /**
         * Swaps the bytes of a given short integer. This can be used
         * to correct values written and read on different systems
         * with different endianness.
         *
         * @param x The short integer whose bytes will be swapped
         *
         * @return A short integer with bytes opposite to those of x.
         */
        inline uint16_t byteswapUInt16(const uint16_t x) {
            return ((x >> 8) | (x << 8));
        }
        
        /**
         * @brief Strip the path from a filepath.
         *
         * Removes the preceding path from a string that
         * contains a path and filename.
         *
         * @param path The file path from which the path will
         *             be stripped.
         *
         * @return A string containing only the filename, without
         *         any preceding path or slashes.
         */
        std::string stripPathFromFilename(std::string path);
        
        /**
         * Removes the given extension from a filename.
         *
         * @param filename The filename from which the given
         *                 extension will be removed. It can
         *                 also be the full path to the file,
         *                 as the function merely eliminates the
         *                 trailing extension.
         * @param extension The extension (including '.') that
         *                   will be removed from the filename.
         *
         * @return A string equivalent to the given filename
                   except that it excludes the given extension.
         */
        std::string stripExtensionFromFilename(const std::string& filename, const std::string& extension);
        
        /**
         * Removes the path and a given extension from a
         * given filepath and provides only the base filename.
         *
         * @param  path The filepath to strip the path and
         *              extension from.
         * @param extension The extension that will be removed
         *                  from the filepath.
         *
         * @return The base filename excluding any preceding path
         *         or trailing extension.
         */
        std::string stripToBaseFilename(const std::string& path, const std::string& extension);
        
        /**
         * Tells whether or not the file with the
         * given name exists.
         *
         * @param fname Name of the file to check
         *
         * @return True if the file exists, false
         *         otherwise.
         */
        bool fileExists(const std::string& fname);
    }
    /**
     * @}
     */
}

#endif //FILEUTIL_HPP