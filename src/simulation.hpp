#pragma once
#include <vector>
#include <raylib.h>
#include "world.hpp"

const float SIMULATION_RATE_HZ = 20.0f;
const float FIXED_DELTA_TIME = 1.0f / SIMULATION_RATE_HZ;

struct ItemPacket {
    ResourceType type;
    int amount;
};

// Pure POD Data-Oriented structure
struct Extractor {
    int gridX;
    int gridY;
    
    ResourceType targetResource;
    int currentBuffer;
    int maxBuffer;
    float extractionRate; // Units per second
    
    float accumulatedWork;
    float prevCycleProgress; // 0.0 to 1.0 mapped to animation
    float currCycleProgress; // 0.0 to 1.0 mapped to animation
};

class ExtractorManager {
public:
    ExtractorManager() = default;

    void SpawnExtractor(int gridX, int gridY, World& world);
    void FixedUpdate(float fixedDelta, World& world);
    void Draw(const Camera2D& camera, float alpha);

private:
    // Contiguous array matching DOD constraints perfectly
    std::vector<Extractor> m_extractors;
};

class SimulationManager {
public:
    SimulationManager();

    void Update(float dt, World& world);
    void Draw(const Camera2D& camera);

    float GetInterpolationAlpha() const;

    ExtractorManager& GetExtractorManager() { return m_extractorManager; }

private:
    float m_accumulator;

    ExtractorManager m_extractorManager;
};
