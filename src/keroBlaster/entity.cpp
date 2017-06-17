#include <string>

#include <array>

#include <stdexcept>

#include <cstdint>

#include "fdl/keroBlaster/pxPack.hpp"

namespace fdl {
    namespace keroBlaster {
        constexpr int pxPack::entity::NAME_MAX_LEN;
        
        pxPack::entity::entity() : flag(0), type(0), unknownByte(0),
                                   x(0), y(0), data({0, 0}), name("\0") {}
        
        uint8_t pxPack::entity::getFlag() const {
            return flag;
        }
        
        uint8_t pxPack::entity::getType() const {
            return type;
        }
        
        uint8_t pxPack::entity::getUnknownByte() const {
            return unknownByte;
        }
        
        uint16_t pxPack::entity::getX() const {
            return x;
        }
        
        uint16_t pxPack::entity::getY() const {
            return y;
        }
        
        std::array <uint8_t, pxPack::NUM_UNKNOWN_ENTITY_BYTES> pxPack::entity::getData() const {
            return data;
        }
        
        std::string pxPack::entity::getName() const {
            return name;
        }
        
        void pxPack::entity::reset() {
            flag = 0;
            type = 0;
            unknownByte = 0;
            x = 0;
            y = 0;
            data.fill(0);
            name = "";
        }
        
        void pxPack::entity::setFlag(const uint8_t flag) {
            this -> flag = flag;
        }
        
        void pxPack::entity::setType(const uint8_t type) {
            this -> type = type;
        }
        
        void pxPack::entity::setUnknownByte(const uint8_t unknownByte) {
            this -> unknownByte = unknownByte;
        }
        
        void pxPack::entity::setX(const uint16_t x) {
            this -> x = x;
        }
        
        void pxPack::entity::setY(const uint16_t y) {
            this -> y = y;
        }
        
        void pxPack::entity::setData(const size_t index, const uint8_t data) {
            this -> data.at(index) = data;
        }
        
        void pxPack::entity::setName(const std::string& name) { //TODO: Figure out way to get entity number and filename into error message
            if (name.size() > NAME_MAX_LEN) {
                throw std::length_error("ERROR: Attempt to resize entity name to be longer than " +
                                        std::to_string(NAME_MAX_LEN) + " characters.");
            }
            else {
                this -> name = name;
            }
        }
    }
}