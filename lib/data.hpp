#include <fstream>
#include <vector>
#include <string>
#include <sstream>

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

data::~data()
{
}
