#pragma once

#include <vector>
#include <cstdint>
#include <raylib.h>

const int TILE_SIZE = 32;

enum class BiomeType : uint8_t {
    Plains = 0,
    Forest,
    Mountain
};

enum class ResourceType : uint8_t {
    None = 0,
    Wood,
    Ore
};

struct Tile {
    BiomeType biomeType;
    ResourceType resourceType;
    uint8_t resourceAmount;
    bool isVisible; // For sector logic
};

class World {
public:
    World(int width, int height);

    void Generate(unsigned int seed);

    Tile& GetTile(int x, int y);
    const Tile& GetTile(int x, int y) const;
    void SetTile(int x, int y, const Tile& tile);

    void DrawWorld(const Camera2D& camera, int screenWidth, int screenHeight);

    void RevealSector(int startX, int startY, int sectorWidth, int sectorHeight);

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    int m_width;
    int m_height;
    std::vector<Tile> m_tiles;

    int GetIndex(int x, int y) const;
    
    // Internal simple noise function
    float Noise2D(float x, float y, unsigned int seed) const;
};
