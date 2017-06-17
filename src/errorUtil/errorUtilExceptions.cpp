#include <string>

#include <exception>

#include "fdl/errorUtil/errorUtilExceptions.hpp"

namespace fdl {
    namespace errorUtil {
        fdlException::fdlException(const std::string& what_arg) : errorText(what_arg) {}
        fdlException::fdlException(const char* what_arg) : errorText(what_arg) {}
        const char* fdlException::what() const noexcept {
            return errorText.c_str();
        }
        
        fileOpenError::fileOpenError(const std::string& what_arg) : fdlException(what_arg) {}
        fileOpenError::fileOpenError(const char* what_arg) : fdlException(what_arg) {}
        
        fileReadError::fileReadError(const std::string& what_arg) : fdlException(what_arg) {}
        fileReadError::fileReadError(const char* what_arg) : fdlException(what_arg) {}
        
        fileWriteError::fileWriteError(const std::string& what_arg) : fdlException(what_arg) {}
        fileWriteError::fileWriteError(const char* what_arg) : fdlException(what_arg) {}
    }
}