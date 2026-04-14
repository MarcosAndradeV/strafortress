#include "simulation.hpp"
#include <cstdio>
#include <cmath>

// --- ExtractorManager ---

void ExtractorManager::SpawnExtractor(int gridX, int gridY, World& world) {
    // If outside bounds, abort
    if (gridX < 0 || gridX >= world.GetWidth() || gridY < 0 || gridY >= world.GetHeight()) return;

    const Tile& tile = world.GetTile(gridX, gridY);
    
    if (tile.resourceType != ResourceType::None) {
        Extractor ext;
        ext.gridX = gridX;
        ext.gridY = gridY;
        ext.targetResource = tile.resourceType;
        ext.currentBuffer = 0;
        ext.maxBuffer = 50; 
        ext.extractionRate = 2.0f; // 2 items per second
        
        ext.accumulatedWork = 0.0f;
        ext.prevCycleProgress = 0.0f;
        ext.currCycleProgress = 0.0f;
        
        m_extractors.push_back(ext);
        printf("DEBUG: Extractor Spawned at %d, %d mining resource %d!\n", gridX, gridY, (int)ext.targetResource);
    } else {
        printf("DEBUG: No resource at %d, %d. Extractor failed to spawn.\n", gridX, gridY);
    }
}

void ExtractorManager::FixedUpdate(float fixedDelta, World& world) {
    // Loop over contiguous array - Cache Friendly!
    for (size_t i = 0; i < m_extractors.size(); ++i) {
        Extractor& ext = m_extractors[i];
        
        ext.prevCycleProgress = ext.currCycleProgress;
        
        // Pause extraction if buffer is maxed
        if (ext.currentBuffer >= ext.maxBuffer) {
            continue; 
        }

        ext.accumulatedWork += ext.extractionRate * fixedDelta;
        
        // Loop the visual cycle representation smoothly
        ext.currCycleProgress = ext.accumulatedWork - std::floor(ext.accumulatedWork);

        if (ext.accumulatedWork >= 1.0f) {
            Tile t = world.GetTile(ext.gridX, ext.gridY);
            
            if (t.resourceAmount > 0) {
                t.resourceAmount -= 1;
                ext.currentBuffer += 1;
                world.SetTile(ext.gridX, ext.gridY, t);
                
                ext.accumulatedWork -= 1.0f;
            } else {
                ext.currCycleProgress = 0.0f; 
            }
        }
    }
}

void ExtractorManager::Draw(const Camera2D& camera, float alpha) {
    for (const auto& ext : m_extractors) {
        // Linearly interpolate between the physics ticks
        float cycle = ext.prevCycleProgress + (ext.currCycleProgress - ext.prevCycleProgress) * alpha;
        
        // "Pump" machine animation
        float sizeMod = std::sin(cycle * 3.14159f) * 4.0f; 
        
        Rectangle dest = {
            (float)(ext.gridX * TILE_SIZE) + 4.0f - sizeMod,
            (float)(ext.gridY * TILE_SIZE) + 4.0f - sizeMod,
            (TILE_SIZE - 8.0f) + sizeMod * 2.0f,
            (TILE_SIZE - 8.0f) + sizeMod * 2.0f
        };
        
        Color renderColor = ORANGE;
        if (ext.currentBuffer >= ext.maxBuffer) {
            renderColor = RED; 
        }
        
        DrawRectangleRec(dest, renderColor);
        DrawRectangleLinesEx(dest, 2.0f, MAROON);
    }
}

// --- SimulationManager ---

SimulationManager::SimulationManager() : m_accumulator(0.0f) {
}

void SimulationManager::Update(float dt, World& world) {
    m_accumulator += dt;
    
    // Cap accumulation to avoid spiral of death
    if (m_accumulator > 0.25f) {
        m_accumulator = 0.25f;
    }

    while (m_accumulator >= FIXED_DELTA_TIME) {
        m_extractorManager.FixedUpdate(FIXED_DELTA_TIME, world);
        m_accumulator -= FIXED_DELTA_TIME;
    }
}

float SimulationManager::GetInterpolationAlpha() const {
    return m_accumulator / FIXED_DELTA_TIME;
}

void SimulationManager::Draw(const Camera2D& camera) {
    float alpha = GetInterpolationAlpha();
    // Funnel down rendering requests to specific managers
    m_extractorManager.Draw(camera, alpha);
}
