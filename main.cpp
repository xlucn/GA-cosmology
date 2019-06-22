#include <unistd.h>
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

void export_population(FILE *fpop, const GAPopulation& pop)
{
    float h0, Om, Or;
    for(int i = 0; i < pop.size(); i++)
    {
        h0 = ((GABin2DecGenome&)(pop.individual(i))).phenotype(0);
        Om = ((GABin2DecGenome&)(pop.individual(i))).phenotype(1);
        Or = ((GABin2DecGenome&)(pop.individual(i))).phenotype(2);
        fprintf(fpop, "%f %f %f\n", h0, Om, Or);
        fflush(fpop);
    }
}

/* Initialize population in a confined region */
void special_init(const GAPopulation& pop)
{
    for(int i = 0; i < pop.size(); i++)
    {
        GABin2DecGenome& genome = (GABin2DecGenome&)(pop.individual(i));
        GABin2DecPhenotype pheno = genome.phenotypes();
        for(int j = 0; j < genome.nPhenotypes(); j++)
            genome.phenotype(
                j,
                GARandomFloat(pheno.min(j) * 0.8 + pheno.max(j) * 0.2,
                              pheno.min(j) * 0.9 + pheno.max(j) * 0.1)
            );
    }
}

int main(int argc, char *argv[])
{
    // test code
    // std::cout << score(0.7, 0.3, 0.001) << std::endl;
    // return 0;

    // Some options to toggle
    // 1: generation. 2: convergence
    int TermMethod = 1;
    // whether to output each generation's population data
    int iPlotPops = 1;
    // Initial method
    int SpecialInit = 0;
    // GA type
    // 1: Simple
    // 2: SteadyState
    // 3: Incremental
    // 4: Deme
    int WhichGA = 1;
    // Read commandline argumets
    char opt;
    while ((opt = getopt(argc, argv, "hIpPa:t:")) != -1) {
        switch (opt) {
        case 'I':
            SpecialInit = 1;
            break;
        case 'p':
            iPlotPops = 1;
            break;
        case 'P':
            iPlotPops = 0;
            break;
        case 'a':
            WhichGA = atoi(optarg);
            break;
        case 't':
            TermMethod = atoi(optarg);
            break;
        case 'h':
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-t 1 or 2] [-p] [GAparameters]\n"
                "-t\t1 for terminate upon specified generations.(default)\n"
                  "\t2 for terminate upon convergence.\n"
                "-a\tGA type\n"
                  "\t1: Simple (default)\n"
                  "\t2: SteadyState\n"
                  "\t3: Incremental\n"
                  "\t4: Deme\n"
                "-p\toutput population data for each generation"
                    "(default on).\n"
                "-P\ttrun -p off\n"
                "-I\tspecial initialization method(only when -p or no -P)\n"
                "GAparameters\tcommand-line parameters for GAlib. For details "
                  "see\n\thttp://lancet.mit.edu/galib-2.4/API.html#defparms\n",
                argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    std::cout << "This program is try to solve the cosmology\n";
    std::cout << "For more usage information, use " << argv[0] << " -h\n";


    // parameters for generation or convergence termination
    int ngen = 100;
    float pconv = 0.99;
    int nconv = 30;
    // Declare variables for the GA parameters and set them to some default value.
    int popsize = 30;
    float pmut = 0.01;
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
    switch(WhichGA)
    {
    case 2:
        {
        GASteadyStateGA ga(genome);
        ga.pReplacement(0.5);
        }
        break;
    case 3:
        {
        GAIncrementalGA ga(genome);
        ga.nOffspring(2);  /* 1(default) or 2 */
        }
        break;
    case 4:
        std::cerr << "Have bugs with Deme GA.\n";
        return 1;
        {
        GADemeGA ga(genome);
        ga.nReplacement(5);
        ga.nMigration(5);
        }
        break;
    case 1:
    default:
        break;
    }

    // usr sigma truncation scaling when the scores might be negative
    GASigmaTruncationScaling scale;
    ga.scaling(scale);
    // set parameters
    ga.populationSize(popsize);
    ga.pMutation(pmut);
    ga.pCrossover(pcross);
    ga.scoreFilename("score.dat");
    ga.scoreFrequency(1);
    ga.flushFrequency(5);
    // generation or convergence termination
    if(TermMethod == 1)
    {
        ga.nGenerations(ngen);
    }
    else
    {
        ga.pConvergence(pconv);
        ga.nConvergence(nconv);
        ga.terminator(GAGeneticAlgorithm::TerminateUponConvergence);
    }
    /* read GAlib arguments from commandline */
    ga.parameters(argc, argv);


    // Evaluation
    if(iPlotPops)
    {
        // Export result of each step
        FILE *fpop = fopen("pops.dat", "w");
        fprintf(fpop, "%d\n", popsize);
        fprintf(fpop, "%f %f %f %f %f %f\n",
                      map.min(0), map.max(0),
                      map.min(1), map.max(1),
                      map.min(2), map.max(2));

        if(SpecialInit)
            /* TODO: DemeGA */
            special_init(ga.population());
        else
            ga.initialize(GARandomInt());

        while(ga.done() == gaFalse)
        {
            ga.step();
            if(WhichGA == 4)
            {
                GADemeGA *demega = (GADemeGA*)&ga;
                for(int p = 0; p < demega->nPopulations(); p++)
                    export_population(fpop, demega->population(p));
            }
            else
            {
                export_population(fpop, ga.population());
            }
        }
        fclose(fpop);
        ga.flushScores();
    }
    else
    {
        // Evolve and output the results
        ga.evolve();
    }
    genome = ga.statistics().bestIndividual();
    std::cout << "\nthe list contains " << genome.size() << " nodes\n";
    std::cout << "the ga used the parameters:\n" << ga.parameters() << "\n";
    std::cout << "Result: h0 = " << genome.phenotype(0) << "; ";
    std::cout << "Om = " << genome.phenotype(1) << "; ";
    std::cout << "Or = " << genome.phenotype(2) << "\n";

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
