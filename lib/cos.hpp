#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

class cosmos
{
private:
    float h0, Om, Or, Od;
    float dt;

public:
    cosmos();
    cosmos(float h0_, float Om_, float Or_, float dt_ = 0.001) : h0(h0_), Om(Om_), Or(Or_), Od(1 - Om_ - Or_), dt(dt_) {}
    float Ez(const float &z);
    float dl(const float &z);
    std::vector<float> dl(const std::vector<float> &z);
    ~cosmos();
};

float cosmos::Ez(const float &z)
{
    return 1 / sqrt(Od + Om * pow(1 + z, 3)); 
}

float cosmos::dl(const float &z)
{
    int numberInt = z / dt;
    float f0 = 0, f1;
    float t0;
    float u1, u2, u3, u4;
    for (size_t i = 0; i < numberInt; i++)
    {
        t0 = i * dt;
        u1 = Ez(t0);
        u2 = Ez(t0 + dt / 2.0);
        u3 = Ez(t0 + dt / 2.0);
        u4 = Ez(t0 + dt);
        f1 = f0 + dt / 6.0 * (u1 + 2 * u2 + 2 * u3 + u4);
        f0 = f1;
    }
    return f1;
}

std::vector<float> cosmos::dl(const std::vector<float> &z)
{
    auto maxz = *std::max_element(std::begin(z), std::end(z));
    int numberInt = maxz / dt;
    std::vector<float> results(z.size());
    float u1, u2, u3, u4;
    float f0 = 0, f1;
    float t0;
    for (size_t i = 0; i < numberInt; i++)
    {
        t0 = i * dt;
        u1 = Ez(t0);
        u2 = Ez(t0 + dt / 2.0);
        u3 = Ez(t0 + dt / 2.0);
        u4 = Ez(t0 + dt);
        f1 = f0 + dt / 6.0 * (u1 + 2 * u2 + 2 * u3 + u4);
        f0 = f1;
        for (auto &&i : v)
        {
            
        }
        
    }
    

}

cosmos::~cosmos()
{
}
