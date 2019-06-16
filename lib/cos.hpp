#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

class Cosmos
{
private:
    double h0, Om, Or, Od;
    double dt;

public:
    Cosmos();
    Cosmos(double h0_, double Om_, double Or_, double dt_ = 0.00001) : h0(h0_), Om(Om_), Or(Or_), Od(1 - Om_ - Or_), dt(dt_) {}
    double Ez(const double &z);
    double dl(const double &z);
    std::vector<double> dl(const std::vector<double> &z);
    ~Cosmos();
};

double Cosmos::Ez(const double &z)
{
    return 1 / sqrt(Od + Om * pow(1 + z, 3)); 
}

double Cosmos::dl(const double &z)
{
    int numberInt = z / dt;
    double f0 = 0;
    double t0;
    double u1, u2, u3, u4;
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

std::vector<double> Cosmos::dl(const std::vector<double> &z)
{
    auto maxz = *std::max_element(std::begin(z), std::end(z));
    int numberInt = maxz / dt;
    std::vector<double> results(z.size());
    double u1, u2, u3, u4;
    double f0 = 0;
    double t0;
    for (size_t i = 0; i < numberInt; i++)
    {
        t0 = i * dt;
        u1 = Ez(t0);
        // u2 = Ez(t0 + dt / 2.0);
        // u3 = Ez(t0 + dt / 2.0);
        u4 = Ez(t0 + dt);
        // f0 = f0 + dt / 6.0 * (u1 + 2 * u2 + 2 * u3 + u4);
        f0 = f0 + dt / 2.0 * (u1 + u4);
        if (i == 0) {std::cout << f0 << std::endl;}
        for (size_t j = 0; j < z.size(); j++)
        {
            if (abs(z[j] - t0 - dt) <= dt) {
                results[j] = f0 * z[j] * 3e5;
            }
        }
    }
    return results;
}

Cosmos::~Cosmos()
{
}
