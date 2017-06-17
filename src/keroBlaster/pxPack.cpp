#include <ios>

#include <string>
#include <vector>
#include <array>

#include <fstream>

#include <stdexcept>
#include "fdl/errorUtil/errorUtilExceptions.hpp"

#include <cstdint>

#include <cstring> //strlen(const char* str)

#include <cstdio> //remove(const char* filename)

#include "fdl/keroBlaster/keroBlaster.hpp"
#include "fdl/keroBlaster/pxPack.hpp"

#include "fdl/fileUtil/fileUtil.hpp"

#include "fdl/containerUtil/containerUtil.hpp"
#include "fdl/containerUtil/limitedAllocator.hpp"

namespace fdl {
    namespace keroBlaster {
        //definition in hpp, declaration here
        
        constexpr char pxPack::FOLDER_NAME [];
        
        constexpr char pxPack::FILE_EXTENSION [];
        
        constexpr char pxPack::HEADER [];
        constexpr int pxPack::NUM_REFERENCED_MAPS;
        constexpr int pxPack::NUM_REFERENCED_TILESETS;
        
        constexpr int pxPack::DESCRIPTION_MAX_LEN;
        
        constexpr int pxPack::NUM_LAYERS;
        constexpr char pxPack::LAYER_HEADER [];
        
        constexpr int pxPack::MAX_NUM_ENTITIES;
        
        constexpr int pxPack::NUM_UNKNOWN_ENTITY_BYTES;
        
        
        pxPack::pxPack() : filename("\0"), originalFilename("\0"), description("\0"),
                           scriptName("\0"), mapNames({("\0"), ("\0"), ("\0")}),
                           spritesheetName("\0"), tilesetNames({("\0"), ("\0"), ("\0")}),
                           entities(entityVectorAllocator(MAX_NUM_ENTITIES)) {}
        
        pxPack::pxPack(const std::string& filename) : description("\0"), scriptName("\0"),
                                                      mapNames({("\0"), ("\0"), ("\0")}),
                                                      spritesheetName("\0"),
                                                      tilesetNames({("\0"), ("\0"), ("\0")}),
                                                      entities(entityVectorAllocator(MAX_NUM_ENTITIES)) {
            loadMap(filename);
        }
        
        void pxPack::loadMap(const std::string& filename) {
            std::fstream file = open(filename);
            
            try {
                readHead(file);
                readTileLayers(file);
                readEntities(file);
            }
            /* 
             * If any problems occur with parsing the file, completely reset
             * all properties to avoid data corruption and rethrow the error
             * so the caller knows parsing failed.
             */
            catch (const fdl::errorUtil::fileReadError&) {
                reset();
                throw;
            }
        }
        
        std::string pxPack::getFilename() const {
            return filename;
        }
        
        std::string pxPack::getDescription() const {
            return description;
        }
        
        std::string pxPack::getScriptName() const {
            return scriptName;
        }
        
        std::array <std::string, pxPack::NUM_REFERENCED_MAPS> pxPack::getMapNames() const {
            return mapNames;
        }
        
        std::string pxPack::getSpritesheetName() const {
            return spritesheetName;
        }
        
        std::array <std::string, pxPack::NUM_REFERENCED_TILESETS> pxPack::getTilesetNames() const {
            return tilesetNames;
        }
        
        void pxPack::reset() {
            filename = "";
            originalFilename = "";
            
            description = "";
            scriptName = "";
            mapNames = {"", "", ""};
            spritesheetName = "";
            tilesetNames = {"", "", ""};
            
            tileLayers.fill(tileLayer());
            entities.clear();
        }
        
        void pxPack::setFilename(std::string filename) {
            filename = fdl::fileUtil::stripToBaseFilename(filename, FILE_EXTENSION);
            
            if (filename.size() > fdl::keroBlaster::FILENAME_MAX_LEN) {
                throw std::length_error("ERROR: Attempt to resize filename for map " +
                                        originalFilename + " to be longer than " +
                                        std::to_string(fdl::keroBlaster::FILENAME_MAX_LEN)
                                        + " characters.");
            }
            else {
                this -> filename = filename;
            }
        }
        
        void pxPack::setDescription(const std::string& description) {
            if (description.size() > DESCRIPTION_MAX_LEN) {
                throw std::length_error("ERROR: Attempt to resize description text for map " +
                                        filename + " to be longer than " +
                                        std::to_string(DESCRIPTION_MAX_LEN)
                                        + " characters.");
            }
            else {
                this -> description = description;
            }
        }
        
        void pxPack::setScriptName(std::string scriptName) {
            scriptName = fdl::fileUtil::stripToBaseFilename(scriptName, ".pxeve"); //TODO: Make constexpr char [] for extension
            
            if (scriptName.size() > fdl::keroBlaster::FILENAME_MAX_LEN) {
                throw std::length_error("ERROR: Attempt to resize script filename for map " +
                                        filename + " to be longer than " +
                                        std::to_string(fdl::keroBlaster::FILENAME_MAX_LEN)
                                        + " characters.");
            }
            else {
                this -> scriptName = scriptName;
            }
        }
        
        void pxPack::setMapName(const size_t index, std::string mapName) {
            mapName = fdl::fileUtil::stripToBaseFilename(mapName, FILE_EXTENSION);
            
            if (mapName.size() > fdl::keroBlaster::FILENAME_MAX_LEN) {
                throw std::length_error("ERROR: Attempt to resize map filename " +
                                        std::to_string(index + 1) + " for map " +
                                        filename + " to be longer than " +
                                        std::to_string(fdl::keroBlaster::FILENAME_MAX_LEN)
                                        + " characters.");
            }
            else {
                mapNames.at(index) = mapName;
            }
        }
        
        void pxPack::setSpritesheetName(std::string spritesheetName) {
            spritesheetName = fdl::fileUtil::stripToBaseFilename(spritesheetName, ".png"); //TODO: Make constexpr char [] for extension
            
            if (spritesheetName.size() > fdl::keroBlaster::FILENAME_MAX_LEN) {
                throw std::length_error("ERROR: Attempt to resize spritesheet filename for map " +
                                        filename + " to be longer than " +
                                        std::to_string(fdl::keroBlaster::FILENAME_MAX_LEN)
                                        + " characters.");
            }
            else {
                this -> spritesheetName = spritesheetName;
            }
        }
        
        void pxPack::setTilesetName(const size_t index, std::string tilesetName) {
            tilesetName = fdl::fileUtil::stripToBaseFilename(tilesetName, ".png"); //TODO: Make constexpr char [] for extension
            
            if (tilesetName.size() > fdl::keroBlaster::FILENAME_MAX_LEN) {
                throw std::length_error("ERROR: Attempt to resize tileset filename " +
                                        std::to_string(index + 1) + " for map " +
                                        filename + " to be longer than " +
                                        std::to_string(fdl::keroBlaster::FILENAME_MAX_LEN)
                                        + " characters.");
            }
            else {
                tilesetNames.at(index) = tilesetName;
            }
        }
        
        std::fstream pxPack::open(std::string filename) {
            if ("" == fdl::keroBlaster::basePath || "" == fdl::keroBlaster::resourceFolder) { //these need to be set first
                throw std::logic_error("ERROR: Attempt to open PXPACK file without first setting \
fdl::keroBlaster::basePath and fdl::keroBlaster::resourceFolder.");
            }
            
            setFilename(filename); //sets this -> filename and ensures the filename's validity (not too long; otherwise std::length_error thrown)
            originalFilename = this -> filename; //store the original filename to allow renaming the PXPACK file later
            filename = this -> filename; //ensures filename is cut down to base filename without preceding path or trailing extension
            
            const std::string PATH = fdl::keroBlaster::basePath + '\\' +
                                     fdl::keroBlaster::resourceFolder +
                                     FOLDER_NAME + filename + FILE_EXTENSION;
            
            std::fstream file;
            
            if (!fdl::fileUtil::fileExists(PATH)) {
                //Create dummy PXPACK file if the given one doesn't exist
                
                //TODO: After implementing pxPack::save(), just call that here and erase everything else in this if statement
                
                file.open(PATH, std::fstream::out | std::fstream::binary);
                
                if (!file) {
                    throw fdl::errorUtil::fileOpenError("ERROR: Failed to open PXPACK file " + filename + " for dummy writing.");
                }
                
                file.write(HEADER, strlen(HEADER) + 1); //+ 1 to include null-terminator, which appears in the file
                
                file.put(0); //description
                
                file.put(0); //script
                
                for (int i = 0; i < NUM_REFERENCED_MAPS; ++i) { //mapnames
                    file.put(0);
                }
                
                file.put(0); //spritesheet
                
                for (int i = 0; i < 8; ++i) { //8 unknown bytes after spritesheet
                    file.put(0);
                }
                
                for (int i = 0; i < NUM_REFERENCED_TILESETS; ++i) { //tilesets
                    file.put(0);
                    
                    file.write("\0\0", 2); //2 unknown bytes after each tileset
                }
                
                for (int i = 0; i < NUM_LAYERS; ++i) {
                    file.write(LAYER_HEADER, strlen(LAYER_HEADER) + 1); //+ 1 to include null terminator, which is in the file
                    file.write("\0\0", 2); //width
                    file.write("\0\0", 2); //height
                }
                
                file.write("\0\0", 2); //number of entities
                
                if (!file.good()) {
                    remove(PATH.c_str());
                    throw fdl::errorUtil::fileWriteError("ERROR: Failed to write dummy values to PXPACK file " + filename + '.');
                }
                
                file.close();
            }
            
            file.open(PATH, std::fstream::in | std::fstream::binary);
            
            if (!file) {
                throw fdl::errorUtil::fileOpenError("ERROR: Failed to open PXPACK file " + filename + " for parsing.");
            }
            
            return file;
        }
        
        void pxPack::readHead(std::fstream& file) {
            if (std::string(HEADER) != readHeader(file)) {
                throw fdl::errorUtil::fileReadError("ERROR: Incorrect PXPACK header in file " + filename + '.');
            }
            
            description = readString(file);
            
            scriptName = readString(file);
            
            for (int i = 0; i < NUM_REFERENCED_MAPS; ++i) {
                mapNames.at(i) = readString(file);
            }
            
            spritesheetName = readString(file);
            
            file.ignore(8);
            
            for (int i = 0; i < NUM_REFERENCED_TILESETS; ++i) {
                tilesetNames.at(i) = readString(file);
                file.ignore(2);
            }
            
            if (!file.good()) { //parsing failed somewhere
                throw fdl::errorUtil::fileReadError("ERROR: Could not parse head of PXPACK file " + filename + '.');
            }
        }
        
        void pxPack::readTileLayers(std::fstream& file) {
            for (int i = 0; i < NUM_LAYERS; ++i) {
                char layerHeader[strlen(LAYER_HEADER) + 1] = {0}; //+ 1 to include null terminator, which is in the file
                file.read(layerHeader, strlen(LAYER_HEADER) + 1);
                if (std::string(LAYER_HEADER) != std::string(layerHeader)) { //always three instances of "pxMAP01", regardless of if all layers have content
                    throw fdl::errorUtil::fileReadError("ERROR: Incorrect PXPACK layer header for layer " +
                                                        std::to_string(i + 1) + " of file " + filename + '.');
                }
                
                uint16_t width, height;
                
                file.read((char*)&width, sizeof(width));
                file.read((char*)&height, sizeof(height));
                
                if (!fdl::fileUtil::isLittleEndian()) { //if we're on a big-endian system, swap bytes to little-endian
                    width = fdl::fileUtil::byteswapUInt16(width);
                    height = fdl::fileUtil::byteswapUInt16(height);
                }
                
                tileLayers.at(i).setDimensions(width, height);
                
                if (width * height > 0) {
                    tileLayers.at(i).setFlag(file.get());
                    
                    for (int j = 0; j < height; ++j) {
                        for (int k = 0; k < width; ++k) {                            
                            const uint8_t TILE = file.get();
                            tileLayers.at(i).setTile(k, j, TILE);
                        }
                    }
                }
                
                if (!file.good()) { //parsing failed somewhere
                    throw fdl::errorUtil::fileReadError("ERROR: Could not parse tile layer " + std::to_string(i + 1) + " of PXPACK file " + filename + '.');
                }
            }
        }
        
        void pxPack::readEntities(std::fstream& file) {
            uint16_t numEntities; //Pretty sure it's 2 bytes
            file.read((char*)&numEntities, sizeof(numEntities));
            entities.resize(numEntities);
            
            if (!file.good()) { //parsing failed somewhere
                throw fdl::errorUtil::fileReadError("ERROR: Could not parse entity number of PXPACK file " + filename + '.');
            }
            
            for (int i = 0; i < numEntities; ++i) {
                entities.at(i).setFlag(file.get());
                
                entities.at(i).setType(file.get());
                
                entities.at(i).setUnknownByte(file.get());
                
                uint16_t x, y;
                file.read((char*)&x, sizeof(x));
                file.read((char*)&y, sizeof(y));
                
                if (!fdl::fileUtil::isLittleEndian()) { //if we're on a big-endian system, swap bytes to little-endian
                    x = fdl::fileUtil::byteswapUInt16(x);
                    y = fdl::fileUtil::byteswapUInt16(y);
                }
                
                entities.at(i).setX(x);
                entities.at(i).setY(y);
                
                uint8_t data[NUM_UNKNOWN_ENTITY_BYTES];
                file.read((char*)&data, NUM_UNKNOWN_ENTITY_BYTES);
                for (int j = 0; j < NUM_UNKNOWN_ENTITY_BYTES; ++j) {
                    entities.at(i).setData(j, data[j]);
                }
                
                entities.at(i).setName(readString(file));
                
                if (!file.good()) { //parsing failed somewhere
                    throw fdl::errorUtil::fileReadError("ERROR: Could not parse entity " + std::to_string(i + 1) + " of PXPACK file " + filename + '.');
                }
            }
        }
        
        std::string pxPack::readHeader(std::fstream& file) {            
            char header [strlen(HEADER) + 1] = {0};  //+ 1 to include null-terminator, which appears in the file
            file.read(header, strlen(HEADER) + 1);
            
            return std::string(header);
        }
        
        std::string pxPack::readString(std::fstream& file) {
            const uint8_t LEN = file.get(); //grab string length
            char* str = new char [LEN + 1] {0};
            file.read(str, LEN); //grab string
            
            const std::string RTN_STR = str;
            delete[] str;
            
            return RTN_STR;
        }
    }
}