#include <string>

#include <fstream>

#include <algorithm>

#include <cstdint>

/* TODO: Find more cross-platform replacement for this since
        unistd.h isn't on all platforms and compilers */
#include <unistd.h> //access(const char* path, int amode)

#include "fdl/fileUtil/fileUtil.hpp"

namespace fdl {
    namespace fileUtil {
        bool isLittleEndian() {
            const uint16_t num = 0b0000'0000'0000'0001;
            const uint8_t* p = (uint8_t*)&num;
            return (1 == *p);
        }
        
        std::string stripPathFromFilename(std::string path) {
            std::replace(path.begin(), path.end(), '/', '\\'); //replace all forward-slashes with backslashes
            
            return path.substr(path.find_last_of('\\') + 1);
        }
        
        std::string stripExtensionFromFilename(const std::string& filename, const std::string& extension) {
            if (filename.size() - extension.size() == filename.rfind(extension)) {
                return filename.substr(0, filename.rfind(extension));
            }
            
            return filename;
        }
        
        std::string stripToBaseFilename(const std::string& path, const std::string& extension) {
            return stripExtensionFromFilename(stripPathFromFilename(path), extension);
        }
        
        bool fileExists(const std::string& fname) {
            return (access(fname.c_str(), F_OK) == 0);
        }
    }
}