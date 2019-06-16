#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class Data
{
private:
    std::vector<double> z;
    std::vector<double> mu;
    std::vector<double> sigma;
public:
    Data(std::ifstream &in);
    std::vector<double> getz() {return z;}
    std::vector<double> getmu() {return mu;}
    std::vector<double> getsigma() {return sigma;}
    void print();
    ~Data();
};

Data::Data(std::ifstream &in)
{
    std::string line;
    char comma;
    double zi, mui, sigmai;
    while (in)
    {
        in >> zi >> comma >> mui >> comma >> sigmai;
        z.push_back(zi);
        mu.push_back(mui);
        sigma.push_back(sigmai);
    }
}

void Data::print()
{
    for (size_t i = 0; i < z.size(); i++)
    {
        std::cout << z[i] << " " << mu[i] << " " << sigma[i];
    }
    
}

Data::~Data()
{
}
