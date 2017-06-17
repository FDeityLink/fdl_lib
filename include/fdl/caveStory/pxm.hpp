#ifndef PXM_HPP
#define PXM_HPP

namespace fdl {
    namespace caveStory {
        class pxm {
            private:
                uint16_t width;
                uint16_t height;
                
                std::vector <uint8_t> tiles;
                
            public:
                pxm();
                pxm(const std::string& filename);
                
                void loadMap(const std::string& filename);
        };
    }
}

#endif //PXM_HPP