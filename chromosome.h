#ifndef CHROMOSOME_H_INCLUDED
#define CHROMOSOME_H_INCLUDED

#include "genome.h"
#include <vector>

typedef std::vector< Genome > GenomesCont;

class Chromosome
{

    private:
        GenomesCont m_genomes;
        int m_genomes_count;
        bool m_is_crossed;
        bool m_is_mutated;
        bool m_is_child;
        double m_fitness;

    public:
        Chromosome();
        //void init();
        GenomesCont get_genomes();
        bool add_genomes( Genome genome );
        void set_genomes_count( int count );
        int get_genomes_count();
        bool get_is_crossed();
        bool get_is_mutated();
        bool get_is_child();
        void set_is_mutated( bool mut );
        void set_is_crossed( bool cross );
        void set_is_child( bool child );
        void reset_actions();
        //void set_fitness( double fit );
        double get_fitness();
        void calculate_fitness();
};



#endif // CHROMOSOME_H_INCLUDED
