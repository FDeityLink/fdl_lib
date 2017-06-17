#ifndef ERRORUTIL_EXCEPTIONS_HPP
#define ERRORUTIL_EXCEPTIONS_HPP

#include <string>

#include <exception>

namespace fdl {
    namespace errorUtil {
        class fdlException : public std::exception {
            public:
                explicit fdlException(const std::string& what_arg);
                explicit fdlException(const char* what_arg);
                const char* what() const noexcept;
                
            protected:
                std::string errorText; //TODO: Make const?
        };
        
        class fileOpenError: public fdlException {
            public:
                explicit fileOpenError(const std::string& what_arg);
                explicit fileOpenError(const char* what_arg);
        };
        
        class fileReadError : public fdlException {
            public:
                explicit fileReadError(const std::string& what_arg);
                explicit fileReadError(const char* what_arg);
        };
        
        class fileWriteError : public fdlException {
            public:
                explicit fileWriteError(const std::string& what_arg);
                explicit fileWriteError(const char* what_arg);
        };
    }
}

#endif //ERRORUTIL_EXCEPTIONS_HPP