#include "world.hpp"
#include <cmath>
#include <algorithm>

World::World(int width, int height) : m_width(width), m_height(height) {
    m_tiles.resize(m_width * m_height);
}

int World::GetIndex(int x, int y) const {
    if (x < 0) x = 0;
    if (x >= m_width) x = m_width - 1;
    if (y < 0) y = 0;
    if (y >= m_height) y = m_height - 1;
    return y * m_width + x;
}

Tile& World::GetTile(int x, int y) {
    return m_tiles[GetIndex(x, y)];
}

const Tile& World::GetTile(int x, int y) const {
    return m_tiles[GetIndex(x, y)];
}

void World::SetTile(int x, int y, const Tile& tile) {
    m_tiles[GetIndex(x, y)] = tile;
}

void World::RevealSector(int startX, int startY, int sectorWidth, int sectorHeight) {
    for (int y = startY; y < startY + sectorHeight; ++y) {
        for (int x = startX; x < startX + sectorWidth; ++x) {
            if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
                m_tiles[GetIndex(x, y)].isVisible = true;
            }
        }
    }
}

static float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
static float lerp(float t, float a, float b) { return a + t * (b - a); }

static float grad(int hash, float x, float y) {
    int h = hash & 3;
    float u = h < 2 ? x : y;
    float v = h < 2 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

float World::Noise2D(float x, float y, unsigned int seed) const {
    auto hash = [seed](int i, int j) -> int {
        unsigned int h = seed + i * 374761393 + j * 668265263;
        h = (h ^ (h >> 13)) * 1274126177;
        return h ^ (h >> 16);
    };

    int X = (int)std::floor(x);
    int Y = (int)std::floor(y);
    x -= std::floor(x);
    y -= std::floor(y);

    float u = fade(x);
    float v = fade(y);

    int aa = hash(X, Y);
    int ab = hash(X, Y + 1);
    int ba = hash(X + 1, Y);
    int bb = hash(X + 1, Y + 1);

    float res = lerp(v, lerp(u, grad(aa, x, y), grad(ba, x - 1, y)),
                        lerp(u, grad(ab, x, y - 1), grad(bb, x - 1, y - 1)));

    return (res + 1.0f) / 2.0f; // Roughly normalizes -1..1 to 0..1
}

void World::Generate(unsigned int seed) {
    const float noiseScale = 0.05f;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            float n = Noise2D(x * noiseScale, y * noiseScale, seed);

            Tile t;
            t.isVisible = false; // By default completely locked

            if (n < 0.35f) {
                t.biomeType = BiomeType::Plains;
                t.resourceType = ResourceType::None;
                t.resourceAmount = 0;
            } else if (n < 0.65f) {
                t.biomeType = BiomeType::Forest;
                t.resourceType = ResourceType::Wood;
                t.resourceAmount = 10;
            } else {
                t.biomeType = BiomeType::Mountain;
                t.resourceType = ResourceType::Ore;
                t.resourceAmount = 5;
            }

            SetTile(x, y, t);
        }
    }
}

void World::DrawWorld(const Camera2D& camera, int screenWidth, int screenHeight) {
    Vector2 topLeftWorld = GetScreenToWorld2D({0, 0}, camera);
    Vector2 bottomRightWorld = GetScreenToWorld2D({(float)screenWidth, (float)screenHeight}, camera);

    int startX = std::max(0, (int)std::floor(topLeftWorld.x / TILE_SIZE));
    int startY = std::max(0, (int)std::floor(topLeftWorld.y / TILE_SIZE));
    int endX = std::min(m_width - 1, (int)std::ceil(bottomRightWorld.x / TILE_SIZE));
    int endY = std::min(m_height - 1, (int)std::ceil(bottomRightWorld.y / TILE_SIZE));

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            const Tile& t = GetTile(x, y);

            if (!t.isVisible) continue;

            Color c = BLACK;
            switch (t.biomeType) {
                case BiomeType::Plains: c = GREEN; break;
                case BiomeType::Forest: c = DARKGREEN; break;
                case BiomeType::Mountain: c = GRAY; break;
            }

            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, c);
            DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, Fade(LIME, 0.2f));
        }
    }
}
