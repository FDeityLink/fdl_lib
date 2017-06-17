#include <vector>

#include <cstdint>

#include "fdl/keroBlaster/pxPack.hpp"

#include "fdl/containerUtil/containerUtil.hpp"

namespace fdl {
    namespace keroBlaster {
        pxPack::tileLayer::tileLayer() : width(0), height(0), flag(0), tiles(0, 0) {}
        
        uint16_t pxPack::tileLayer::getWidth() const {
            return width;
        }
        
        uint16_t pxPack::tileLayer::getHeight() const {
            return height;
        }
        
        uint8_t pxPack::tileLayer::getFlag() const {
            return flag;
        }
        
        std::vector <uint8_t> pxPack::tileLayer::getTiles() const {
            return tiles;
        }
        
        void pxPack::tileLayer::reset() {
            width = 0;
            height = 0;
            flag = 0;
            tiles.clear();
        }
        
        void pxPack::tileLayer::setDimensions(const uint16_t width, const uint16_t height) {
            if ((this -> width == width) && (this -> height == height)) { //old and new sizes are same
                return;
            }
            
            const uint16_t OLD_WIDTH = this -> width; //save old dimensions
            const uint16_t OLD_HEIGHT = this -> height;
            this -> width = width; //write new dimensions
            this -> height = height;
            
            const std::vector <uint8_t> OLD_TILES(tiles); //save old tileset
            tiles.clear();
            tiles.resize(width * height); //resize new tileset and (automatically) set all values to 0
            
            const uint16_t LOOP_WIDTH = ((width < OLD_WIDTH) ? width : OLD_WIDTH); //loop for each index in the smaller dimension
            const uint16_t LOOP_HEIGHT = ((height < OLD_HEIGHT) ? height : OLD_HEIGHT);
            
            for (int i = 0; i < LOOP_HEIGHT; ++i) {
                for (int j = 0; j < LOOP_WIDTH; ++j) {
                    const size_t NEW_INDEX = fdl::containerUtil::indexFromCoords(j, i, width);
                    const size_t OLD_INDEX = fdl::containerUtil::indexFromCoords(j, i, OLD_WIDTH);
                    tiles.at(NEW_INDEX) = OLD_TILES.at(OLD_INDEX);
                }
            }
        }
        
        void pxPack::tileLayer::setFlag(const uint8_t flag) {
            this -> flag = flag;
        }
        
        void pxPack::tileLayer::setTile(const uint16_t x, const uint16_t y, const uint8_t tile) {
            tiles.at(fdl::containerUtil::indexFromCoords(x, y, width)) = tile;
        }
    }
}