#include <iostream>
#include "genome.h"
#include "chromosome.h"
#include "population.h"

#define CROSS_RATE              0.5
#define MUTATION_RATE           0.1
#define MIN_RANDOM              -5.0
#define MAX_RANDOM              5.0
#define MAX_GENERATION_COUNT    40
#define POP_SIZE                35
#define GENOMES_COUNT           5

#define MINIMUM
#ifdef MINIMUM
#define FITNESS                 -1
#else
#define FITNESS                 1
#endif


int main()
{

    Population * population = new Population();

    if( ! population )
        return -1;

    population->set_cross_rate( CROSS_RATE );
    population->set_mutation_rate( MUTATION_RATE );
    population->set_min_random( MIN_RANDOM );
    population->set_max_random( MAX_RANDOM );
    population->set_max_generation_count( MAX_GENERATION_COUNT );
    population->set_pop_size( POP_SIZE );
    population->set_genomes_count( GENOMES_COUNT );

    population->initialize();
    population->print_output();

    int i = 0;
    while( ! population->population_is_stopped() )
    //while( i < MAX_GENERATION_COUNT )
    {
        population->apply_cross_over();
        population->apply_mutation();
        population->calculate_fitness_function( FITNESS );
        population->select_best_chromosomes();
        i++;
    }

    population->print_output();

    delete population;
    return 0;
}
