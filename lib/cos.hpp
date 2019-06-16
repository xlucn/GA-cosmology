#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

class Cosmos
{
private:
    float h0, Om, Or, Od;
    float dt;

public:
    Cosmos();
    Cosmos(float h0_, float Om_, float Or_, float dt_ = 0.00001) : h0(h0_), Om(Om_), Or(Or_), Od(1 - Om_ - Or_), dt(dt_) {}
    float Ez(const float &z);
    float dl(const float &z);
    std::vector<float> dl(const std::vector<float> &z);
    ~Cosmos();
};

float Cosmos::Ez(const float &z)
{
    return 1 / sqrt(Od + Om * pow(1 + z, 3) + Or * pow(1 + z, 4)); 
}

float Cosmos::dl(const float &z)
{
    int numberInt = z / dt;
    float f0 = 0;
    float t0;
    float u1, u2, u3, u4;
    for (size_t i = 0; i < numberInt; i++)
    {
        t0 = i * dt;
        u1 = Ez(t0);
        u2 = Ez(t0 + dt / 2.0);
        u3 = Ez(t0 + dt / 2.0);
        u4 = Ez(t0 + dt);
        f0 = f0 + dt / 6.0 * (u1 + 2 * u2 + 2 * u3 + u4);
    }
    return f0 * 3e5 * z;
}

std::vector<float> Cosmos::dl(const std::vector<float> &z)
{
    auto maxz = *std::max_element(std::begin(z), std::end(z));
    int numberInt = maxz / dt;
    std::vector<float> results(z.size());
    float u1, u2, u3, u4;
    float f0 = 0;
    float t0;
    for (size_t i = 0; i < numberInt; i++)
    {
        t0 = i * dt;
        u1 = Ez(t0);
        u2 = Ez(t0 + dt / 2.0);
        u3 = Ez(t0 + dt / 2.0);
        u4 = Ez(t0 + dt);
        f0 = f0 + dt / 6.0 * (u1 + 2 * u2 + 2 * u3 + u4);
        for (size_t j = 0; j < z.size(); j++)
        {
            if (abs(z[j] - t0 - dt) <= dt / 2) {
                results[j] = f0 * z[j] * 3e5;
            }
        }
    }
    return results;
}

Cosmos::~Cosmos()
{
}
