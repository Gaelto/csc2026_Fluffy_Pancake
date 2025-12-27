// CSC Latin America 2026 - Track Reconstructor
// This file contains intentional bugs for Exercise E1!
#pragma once

#include "Particle.hpp"
#include <vector>
#include <memory>

namespace csc2026 {

struct Hit {
    double x, y, z;
    double energyDeposit;
};

struct Track {
    std::vector<Hit> hits;
    double pt;
    double eta;
    double phi;
    
    double chi2() const;
};

class TrackReconstructor {
public:
    TrackReconstructor(int maxHits = 100);
    ~TrackReconstructor();
    
    // Add a hit to the current event
    void addHit(const Hit& hit);
    
    // Reconstruct tracks from accumulated hits
    std::vector<Track> reconstruct();
    
    // Clear hits for next event
    void clear();
    
    // Get number of hits
    size_t numHits() const { return m_hits.size(); }
    
private:
    std::vector<Hit> m_hits;
    int m_maxHits;
    
    // Internal helper (may contain bugs!)
    void processHits();
};

} // namespace csc2026
