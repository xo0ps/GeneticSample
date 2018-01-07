#include "chromosome.h"

Chromosome::Chromosome()
{
    m_genomes.clear();
    m_genomes_count = 0;
    m_is_crossed = false;
    m_is_mutated = false;
    m_is_child = false;
}

/*
void
Chromosome::init()
{
    for( unsigned int i = 0 ; i < m_genomes_count ; i++ )
    {
        m_genomes.push_back( 0.0 );
    }
}
*/

bool
Chromosome::add_genomes( Genome genome )
{
    if( m_genomes.size() > m_genomes_count )
        return false;

    m_genomes.push_back( genome );
    return true;
}

void
Chromosome::set_genomes_count( int count )
{
    m_genomes_count = count;
}

int
Chromosome::get_genomes_count()
{
    return m_genomes_count;
}

GenomesCont
Chromosome::get_genomes()
{
    return m_genomes;
}

void
Chromosome::set_is_crossed( bool cross )
{
    m_is_crossed = cross;
}

void
Chromosome::set_is_mutated( bool mut )
{
    m_is_mutated = mut;
}

void
Chromosome::set_is_child( bool child )
{
    m_is_child = child;
}

bool
Chromosome::get_is_crossed()
{
    return m_is_crossed;
}

bool
Chromosome::get_is_mutated()
{
    return m_is_mutated;
}

bool
Chromosome::get_is_child()
{
    return m_is_child;
}

void
Chromosome::reset_actions()
{
    m_is_crossed = false;
    m_is_mutated = false;
    m_is_child = false;
}

/*
void
Chromosome::set_fitness( double fit )
{
    m_fitness = fit;
}
*/

double
Chromosome::get_fitness()
{
    return m_fitness;
}

void
Chromosome::calculate_fitness()
{
    double sum = 0;
    for( int i = 0 ; i < this->get_genomes_count() ; i++ )
    {
        sum += this->get_genomes()[i].get_factor() * this->get_genomes()[i].get_factor();
    }
    m_fitness = sum;
}
