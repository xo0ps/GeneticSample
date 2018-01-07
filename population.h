#ifndef POPULATION_H_INCLUDED
#define POPULATION_H_INCLUDED

#include <vector>
#include <fstream>
#include "chromosome.h"

typedef std::vector< Chromosome > ChromosomeCont;

class Population
{
    private:
        ChromosomeCont m_chromosomes;
        int m_pop_size;
        double m_cross_rate;
        double m_mutation_rate;
        int m_max_generation_count;
        double m_min_random;
        double m_max_random;
        int m_generation_step;
        int m_genomes_count;
        int m_fitness_type;
        std::ofstream out;


    public:
        Population();
        void initialize();
        ChromosomeCont get_chromosomes();
        void set_pop_size( int pop_size );
        void set_cross_rate( double rate );
        void set_mutation_rate( double rate );
        void set_max_generation_count( int max );
        int get_max_generation_count();
        void set_min_random( double min );
        void set_max_random( double max );
        void add_chromosome( Chromosome chromosome );
        double get_random();
        bool isLess( Chromosome chromosome1 , Chromosome chromosome2 );

        void apply_cross_over();
        void apply_mutation();
        void select_best_chromosomes();
        void calculate_fitness_function( int fitness );
        double calculate_ackley_function( int genomes_count , double genome );
        int get_generation_step();
        bool population_is_stopped();
        void print_output();
        void set_genomes_count( int count );
        int roulette_selection();
};


#endif // POPULATION_H_INCLUDED
