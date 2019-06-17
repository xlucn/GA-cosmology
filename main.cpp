#include <iostream>
#include <fstream>
#include "lib/cos.hpp"
#include "lib/data.hpp"
#include <ga/ga.h>
#include <math.h>


std::ifstream datafile("data/C11.csv", std::ios::in);
Data Ia(datafile);

float objective(GAGenome &);

float score(float h0, float Om, float Or);

int main(int argc, char const *argv[])
{
    std::cout << "This program is try to solve the cosmology\n";

    // test code
    // std::cout << score(0.7, 0.3, 0.001) << std::endl;
    // return 0;

    // Declare variables for the GA parameters and set them to some default value.
    int popsize = 16;
    // int ngen = 100;
    float pconv = 0.99;
    int nconv = 3;
    float pmut = 0.1;
    float pcross = 0.6;

    // 2DecPhenotype
    GABin2DecPhenotype map;
    map.add(16, 0.5, 1); // reduced hubble paramter
    map.add(16, 0, 0.5); // Omega_m
    map.add(16, 0, 0.01); // Omega_r

    // Create the genome
    GABin2DecGenome genome(map, objective);

    // Create GA with the genome
    GASimpleGA ga(genome);

    // usr sigma truncation scaling when the scores might be negative
    GASigmaTruncationScaling scale;
    ga.scaling(scale);

    ga.populationSize(popsize);

    // ga.nGenerations(ngen);
    ga.pConvergence(pconv);
    ga.nConvergence(nconv);
    ga.terminator(GAGeneticAlgorithm::TerminateUponConvergence);

    ga.pMutation(pmut);
    ga.pCrossover(pcross);
    ga.scoreFilename("score.dat");
    ga.scoreFrequency(1);
    ga.flushFrequency(5);

    // whether to output each generation's population data
    int iPlot = 0;
    GAPopulation pop;
    float h0, Om, Or;
    if(iPlot)
    {
        // Export result of each step
        FILE *fpop = fopen("pops.dat", "w");
        ga.initialize();
        while(ga.done() == gaFalse)
        {
            ga.step();
            fprintf(fpop, "%d\n", popsize);
            pop = ga.population();
            for(int i = 0; i < pop.size(); i++)
            {
                h0 = ((GABin2DecGenome&)(pop.individual(i))).phenotype(0);
                Om = ((GABin2DecGenome&)(pop.individual(i))).phenotype(1);
                Or = ((GABin2DecGenome&)(pop.individual(i))).phenotype(2);
                fprintf(fpop, "%f %f %f\n", h0, Om, Or);
                fflush(fpop);
            }
        }
        fclose(fpop);
    }
    else
    {
        // Evolve and output the results
        ga.evolve();
    }
    genome = ga.statistics().bestIndividual();
    std::cout << "the list contains " << genome.size() << " nodes\n\n";
    std::cout << "Result: h0 = " << genome.phenotype(0) << "; ";
    std::cout << "Om = " << genome.phenotype(1) << "; ";
    std::cout << "Or = " << genome.phenotype(2) << "\n\n";
    std::cout << "the ga used the parameters:\n" << ga.parameters() << "\n";

    return 0;
}

// This function tries to maximize the value of the function
float objective(GAGenome & c)
{
    GABin2DecGenome & genome = (GABin2DecGenome &) c;
    float h0 = genome.phenotype(0) * 100; 
    float Om = genome.phenotype(1);
    float Or = genome.phenotype(2);
    float Od = 1 - Om - Or;

    Cosmos cos(h0, Om, Or);
    std::vector<float> muth, muobs, sigma;
    muth = cos.dl(Ia.getz());
    muobs = Ia.getmu();
    sigma = Ia.getsigma();
    for (size_t i = 0; i < muth.size(); i++)
    {
        muth[i] = muth[i] / h0;
        muth[i] = 5 * log10(muth[i]) + 25;
    }
    float A = 0, B = 0, C = 0;
    for (size_t i = 0; i < muth.size(); i++)
    {
        A += (muth[i] - muobs[i]) * (muth[i] - muobs[i]) / sigma[i] / sigma[i];
        B += (muobs[i] - muth[i]) / sigma[i] * sigma[i];
        C += 1 / sigma[i] * sigma[i];
    }
    return - (A - B * B / C);
}

float score(float h0, float Om, float Or)
{
    float Od = 1 - Om - Or;

    Cosmos cos(h0, Om, Or);
    std::vector<float> muth, muobs, sigma;
    muth = cos.dl(Ia.getz());
    muobs = Ia.getmu();
    sigma = Ia.getsigma();
    for (size_t i = 0; i < muth.size(); i++)
    {
        muth[i] = muth[i] / h0 / 100;
        muth[i] = 5 * log10(muth[i]) + 25;
    }
    float A = 0, B = 0, C = 0;
    for (size_t i = 0; i < muth.size(); i++)
    {
        A += (muth[i] - muobs[i]) * (muth[i] - muobs[i]) / sigma[i] / sigma[i];
        B += (muobs[i] - muth[i]) / sigma[i] * sigma[i];
        C += 1 / sigma[i] * sigma[i];
    }
    return - (A - B * B / C);
}