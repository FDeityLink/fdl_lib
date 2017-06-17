#ifndef ERRORUTIL_HPP
#define ERRORUTIL_HPP

namespace fdl {
    namespace errorUtil {
        enum errorCode {
            FILE_OPENFAIL,
            NUM_ERROR_CODES
        };
        
        std::string errorStrings[NUM_ERROR_CODES] = {"ERROR: Failed to open file "};
        
        std::string getErrorString(const errorCode errCode) { //TODO: Make this constexpr (need C strings)
            return errorStrings[errCode];
        }
        
        void logToFile(std::string message, std::string fname); //TODO: Create function definition for this
    }
}

#endif //ERRORUTIL_HPP