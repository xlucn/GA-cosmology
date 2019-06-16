#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class data
{
private:
    std::vector<float> z;
    std::vector<float> mu;
    std::vector<float> sigma;
public:
    data(std::ifstream in);
    std::vector<float> getz() {return z;}
    std::vector<float> getmu() {return mu;}
    std::vector<float> getsigma() {return sigma;}
    void print();
    ~data();
};

data::data(std::ifstream in)
{
    std::string line;
    char comma;
    float zi, mui, sigmai;
    while (in)
    {
        in >> zi >> comma >> mui >> comma >> sigmai;
        z.push_back(zi);
        mu.push_back(mui);
        sigma.push_back(sigmai);
    }
}

void data::print()
{
    for (size_t i = 0; i < z.size(); i++)
    {
        std::cout << z[i] << " " << mu[i] << " " << sigma[i];
    }
    
}

data::~data()
{
}
