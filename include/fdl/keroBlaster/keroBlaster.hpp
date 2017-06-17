#ifndef KEROBLASTER_HPP
#define KEROBLASTER_HPP

#include <string>

namespace fdl {
    /**
      * @addtogroup keroBlaster
      * @{
      */
    
    /**
     * @brief Functions and classes for working with the
     *        Kero Blaster engine.
     *
     * Namespace containing everything pertaining to
     * parsing and modifying games that use the
     * Kero Blaster engine.
     */
    namespace keroBlaster {
        /**
         * Name of the base path containing the Kero Blaster executable and <B>rsc_x</B> folder
         * This and resourceFolder must be set before opening a PXPACK file.
         */
        extern std::string basePath;
        
        /**
         * Name of the resource folder used by the Kero Blaster engine (either <B>rsc_p</B> or <B>rsc_k</B>)
         * This and basePath must be set before opening a PXPACK file.
         */
        extern std::string resourceFolder;
            
        constexpr int FILENAME_MAX_LEN = 15; /**< Maximum length of any filename (excluding extension) in the <B>rsc_x</B> folder */
    }
    /**
      * @}
      */
}

#endif //KEROBLASTER_HPP