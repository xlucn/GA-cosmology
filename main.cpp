#include <iostream>
#include <fstream>
#include "lib/cos.hpp"
#include "lib/data.hpp"
#include <ga/ga.h>

std::ifstream datafile("data/C11.csv", std::ios::in);
data Ia(datafile);

float objective(GAGenome &);

int main(int argc, char const *argv[])
{
    std::cout << "This program is try to solve the cosmology";

    // Declare variables for the GA parameters and set them to some default value.
    int popsize = 30;
    int ngen = 100;
    float pmut = 0.01;
    int pcross = 0.6;

    // 2DecPhenotype
    GABin2DecPhenotype map;
    map.add(16, 0, 1); // reduced hubble paramter
    map.add(16, 0, 1); // Omega_m
    map.add(16, 0, 1); // Omega_r

    // test code
    Ia.print();

    return 0;
}

// This function tries to maximize the value of the function
float objective(GAGenome & c)
{
    GABin2DecGenome & genome = (GABin2DecGenome &) c;
    float h0 = genome.phenotype(0); 
    float Om = genome.phenotype(1);
    float Or = genome.phenotype(2);
    float Od = 1 - Om - Or;

    


}

