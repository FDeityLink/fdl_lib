#ifndef PXPACK_HPP
#define PXPACK_HPP

#include <string>
#include <vector>
#include <array>

#include <fstream>

#include <cstdint>

#include "fdl/containerUtil/containerUtil.hpp"
#include "fdl/containerUtil/limitedAllocator.hpp"

namespace fdl {
    namespace keroBlaster {
        class pxPack {
            public:
                //CONSTANT VALUES
                
                static constexpr char FOLDER_NAME [] = "\\field\\"; /**< Name of the folder in which all PXPACK files are stored */
                
                static constexpr char FILE_EXTENSION [] = ".pxpack"; /**< File extension on all PXPACK files */
                
                static constexpr char HEADER [] = "PXPACK121127a**"; /**< 16-byte header that appears at the start of every PXPACK file */
                static constexpr int NUM_REFERENCED_MAPS = 3; /**< Number of map filenames that appear in the header of a PXPACK file */
                static constexpr int NUM_REFERENCED_TILESETS = 3; /**< Number of tileset filenames that appear in the header of a PXPACK file */
                
                static constexpr int DESCRIPTION_MAX_LEN = 31; /**< Maximum length of the description text that appears in the header of a PXPACK file */
                
                static constexpr int NUM_LAYERS = 3; /**< Number of layers in a PXPACK file */
                static constexpr char LAYER_HEADER [] = "pxMAP01"; /**< 8-byte header that appears at the start of every layer in every PXPACK file */
                
                static constexpr int MAX_NUM_ENTITIES = UINT16_MAX; /**< Maximum number of entities that a PXPACK file can store */
                
                /**
                 * Number of bytes following the coordinates of
                 * an entity in a PXPACK file. The purpose of
                 * these bytes is currently unknown.
                 */
                static constexpr int NUM_UNKNOWN_ENTITY_BYTES = 2;
                
                //INTERIOR CLASSES
                
                class tileLayer {
                    private:
                        uint16_t width, height;
                        uint8_t flag; //Potentially has no purpose but doesn't hurt to at least record it anyway
                        std::vector <uint8_t> tiles;
                        
                    public:
                        tileLayer();
                        
                        uint16_t getWidth() const;
                        uint16_t getHeight() const;
                        uint8_t getFlag() const;
                        std::vector <uint8_t> getTiles() const;
                        
                        void reset();
                        
                        void setDimensions(const uint16_t width, const uint16_t height);
                        void setFlag(const uint8_t flag);
                        void setTile(const uint16_t x, const uint16_t y, const uint8_t tile);
                };
                
                class entity {
                    public:
                        static constexpr int NAME_MAX_LEN = 15;
                    
                    private:
                        uint8_t flag, type, unknownByte;
                        uint16_t x, y;
                        std::array <uint8_t, NUM_UNKNOWN_ENTITY_BYTES> data;
                        std::string name;
                    
                    public:
                        entity();
                        
                        uint8_t getFlag() const;
                        uint8_t getType() const;
                        uint8_t getUnknownByte() const;
                        uint16_t getX() const;
                        uint16_t getY() const;
                        std::array <uint8_t, NUM_UNKNOWN_ENTITY_BYTES> getData() const;
                        std::string getName() const;
                        
                        void reset();
                        
                        void setFlag(const uint8_t flag);
                        void setType(const uint8_t type);
                        void setUnknownByte(const uint8_t unknownByte);
                        void setX(const uint16_t x);
                        void setY(const uint16_t y);
                        void setData(const size_t index, const uint8_t data);
                        void setName(const std::string& name);
                };
                
            private:
                //size of entity vector must be limited to MAX_NUM_ENTITIES (limit imposed in pxPack constructor)
                typedef fdl::containerUtil::limitedAllocator <entity> entityVectorAllocator;
                
            public:
                /**
                 * @brief A vector with limited size for storing pxPack::entity objects
                 *
                 * A vector with a custom allocator to limit the amount of
                 * entities that can be stored to MAX_NUM_ENTITIES. Other
                 * than that it can be used like a normal vector.
                 */
                typedef std::vector <entity, entityVectorAllocator> entityVector;
                
            private:    
                //PXPACK HEAD PROPERTIES
                
                std::string filename;
                
                /* 
                 * originalFilename stores the initial filename used to open a PXPACK file
                 * When pxPack::setFilename() is called, filename is changed while originalFilename isn't
                 * so that when we save the file's changes, we can rename the file with the original 
                 * name to have the new name
                 */
                std::string originalFilename;
                
                std::string description; //Short description of the map stored in Shift-JIS encoding; appears to a limit to how long it can be however
                
                std::string scriptName;
                std::array <std::string, NUM_REFERENCED_MAPS> mapNames; //TODO: Change this to three separate variables if each map corresponds to a specific purpose
                std::string spritesheetName;
                std::array <std::string, NUM_REFERENCED_TILESETS> tilesetNames;
                
            public:
                std::array <tileLayer, NUM_LAYERS> tileLayers; /**< An array holding all tile layers in the PXPACK file */
                
                /**
                 * @brief A vector holding all the entities in the PXPACK file
                 *
                 * A vector holding all the entities in the PXPACK file. Its size is
                 * limited by a custom allocator to hold only up to MAX_NUM_ENTITIES
                 * entities. As such, try/catches should be used to guard against
                 * storing too many entities. An std::length_error exception will
                 * be thrown if one attempts to add too many entities.
                 */
                entityVector entities;
                
            public:
                /**
                 * Creates a pxPack object with dummy values.
                 */
                pxPack();
                
                /**
                 * Creates a pxPack object and parses the 
                 * given file, storing its values. This is
                 * equivalent to calling the default
                 * constructor followed by calling loadMap().
                 *
                 * @param filename Name of the PXPACK file to open.
                 */
                pxPack(const std::string& filename);
                //TODO: Add constructor for initializing values
                
                /**
                 * @brief Parses a PXPACK file
                 *
                 * Opens a given PXPACK file, parses it, stores its
                 * information, and closes it.
                 * fdl::keroBlaster::basePath and fdl::keroBlaster::resourceFolder
                 * must be set prior to using this function,
                 * or an std::logic_error exception will be thrown.
                 * If the given file does not exist, a dummy one is created.
                 *
                 * Errors with writing dummy values will cause the dummy file
                 * to be deleted and an fdl::errorUtil::fileWriteError exception
                 * will be thrown. Errors with parsing the file will cause the
                 * object to completely reset its state to avoid data corruption.
                 * Additionally, parsing errors will cause an fdl::errorUtil::fileReadError
                 * exception to be thrown.
                 *
                 * @param filename Name of the PXPACK file to open.
                 */
                void loadMap(const std::string& filename);
                
                std::string getFilename() const;
                
                std::string getDescription () const;
                std::string getScriptName() const;
                std::array <std::string, NUM_REFERENCED_MAPS> getMapNames() const;
                std::string getSpritesheetName() const;
                std::array <std::string, NUM_REFERENCED_MAPS> getTilesetNames() const;
                
                /**
                  * Clears all values and properties held by the pxPack object.
                  * This includes closing the PXPACK file if one is open.
                  */
                void reset();
                
                void setFilename(std::string filename);
                
                void setDescription(const std::string& description);
                void setScriptName(std::string scriptName);
                void setMapName(const size_t index, std::string mapName);
                void setSpritesheetName(std::string spritesheetName);
                void setTilesetName(const size_t index, std::string tilesetName);
                
            private:
                /**
                 * @brief Opens a PXPACK file.
                 *
                 * Opens a PXPACK file for reading and writing. If the file
                 * does not exist, a dummy PXPACK file with blank values is
                 * created.
                 *
                 * @param filename Name of the PXPACK file that will be opened
                 *                 for read and write access.
                 */
                std::fstream open(std::string filename);
                
                /**
                 * @brief Reads the head.
                 *
                 * Reads the head from the given PXPACK file. If the
                 * header at the very start is incorrect, the method
                 * fails.
                 *
                 * @param file The PXPACK file to read the head from.
                 *
                 * @return True if there were no issues parsing the head,
                 *         false otherwise.
                 */
                void readHead(std::fstream& file);
                
                /**
                 * @brief Reads tile layers.
                 *
                 * Reads all the tile layers from the given PXPACK file.
                 * If the header at the start of any of them is incorrect,
                 * the method fails.
                 *
                 * @param file The PXPACK file to read the tile layers from.
                 *
                 * @return True if there were no issues parsing the tile
                 *         layers, false otherwise.
                 */
                void readTileLayers(std::fstream& file);
                
                /**
                 * @brief Reads entities.
                 *
                 * Reads all the entities from the given PXPACK file.
                 *
                 * @param file The PXPACK file to read the entities from.
                 *
                 * @return True if there were no issues parsing the
                 *         entities, false otherwise.
                 */
                void readEntities(std::fstream& file);
                
                /**
                 * @brief Reads the PXPACK header.
                 *
                 * Reads the 16-byte header at the top of the PXPACK
                 * file provided.
                 *
                 * @return The header string, which can be compared
                 *         to an exact string value to ensure the
                 *         validity of the PXPACK file.
                 */
                std::string readHeader(std::fstream& file);
                
                /**
                 * Reads a string from a PXPACK file.
                 *
                 * @return The string that was read.
                 */
                std::string readString(std::fstream& file);
        };
    }
}

#endif //PXPACK_HPP