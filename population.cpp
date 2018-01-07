#include "population.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>

#define NEPER   2.71828
#define PI      3.14159

Population::Population()
{
    std::srand( std::time( 0 ) );
    m_chromosomes.clear();
    m_pop_size = 0;
    m_cross_rate = 0.0;
    m_mutation_rate = 0.0;
    m_max_generation_count = 0;
    m_min_random = 0.0;
    m_max_random = 0.0;
    m_generation_step = 0;
    m_genomes_count = 0;
    out.open( "generations.log" );
}

void
Population::initialize()
{

    for( int i = 0 ; i < m_pop_size ; i++ )
    {
        Chromosome chromosome;
        chromosome.set_genomes_count( m_genomes_count );
        for( int j = 0 ; j < m_genomes_count ; j++ )
        {
            Genome genome;

            double rand = get_random();
            double factor = calculate_ackley_function( m_genomes_count , rand );
            genome.set_factor( factor );
            chromosome.add_genomes( genome );
        }
        chromosome.calculate_fitness();
        this->add_chromosome( chromosome );
    }

}

ChromosomeCont
Population::get_chromosomes()
{
    return m_chromosomes;
}

void
Population::set_pop_size( int pop_size )
{
    m_pop_size = pop_size;
}

void
Population::set_cross_rate( double rate )
{
    m_cross_rate = rate;
}

void
Population::set_mutation_rate( double rate )
{
    m_mutation_rate = rate;
}

void
Population::set_max_generation_count( int max )
{
    m_max_generation_count = max;
}

void
Population::set_min_random( double min )
{
    m_min_random = min;
}

void
Population::set_max_random( double max )
{
    m_max_random = max;
}

void
Population::apply_cross_over()
{
    //std::cout<<"cross_over"<<std::endl;
    int cross_num = this->get_chromosomes().size() * m_cross_rate;
    if( cross_num % 2 != 0 )
        cross_num++;

    int counter = 0;
    while( counter < ( cross_num / 2 ) )
    {
        int rand = roulette_selection();
        Chromosome parent1 = this->get_chromosomes()[rand];

        int rand2 = -1000;
        do
        {
            rand2 = roulette_selection();
        }
        while( rand == rand2 );

        Chromosome parent2 = this->get_chromosomes()[rand2];

        parent1.set_is_crossed(true);
        parent2.set_is_crossed(true);

        for( int i = 0 ; i < parent1.get_genomes_count() ; i++ )
        {
            double old_factor = parent1.get_genomes()[i].get_factor();
            double ackley_result = calculate_ackley_function( parent1.get_genomes().size() , old_factor );
            parent1.get_genomes()[i].set_factor( ackley_result );
        }

        for( int i = 0 ; i < parent2.get_genomes_count() ; i++ )
        {
            double old_factor = parent2.get_genomes()[i].get_factor();
            double ackley_result = calculate_ackley_function( parent2.get_genomes().size() , old_factor );
            parent2.get_genomes()[i].set_factor( ackley_result );
        }


        double landa1 = get_random() + m_max_random;
        double landa2 = 1.0 - landa1;
        Chromosome child1;
        child1.set_genomes_count( m_genomes_count );
        child1.set_is_child(true);
        for( int i = 0 ; i < child1.get_genomes_count() ; i++ )
        {
            Genome genome;
            double factor = landa1 * parent1.get_genomes()[i].get_factor() + landa2 * parent2.get_genomes()[i].get_factor();
            genome.set_factor(factor);
            child1.add_genomes(genome);
        }
        child1.calculate_fitness();
        this->add_chromosome( child1 );

        Chromosome child2;
        child2.set_genomes_count( m_genomes_count );
        child2.set_is_child(true);
        for( int i = 0 ; i < child2.get_genomes_count() ; i++ )
        {
            Genome genome;
            double factor = landa2 * parent1.get_genomes()[i].get_factor() + landa1 * parent2.get_genomes()[i].get_factor();
            genome.set_factor(factor);
            child2.add_genomes(genome);
        }
        child2.calculate_fitness();
        this->add_chromosome( child2 );

        counter++;
    }

}

void
Population::apply_mutation()
{
    //std::cout<<"mutation"<<std::endl;
    int mut_num = this->get_chromosomes().size() * m_mutation_rate;

    int counter = 0;
    while( counter < mut_num )
    {
        int rand = roulette_selection();
        Chromosome parent = this->get_chromosomes()[rand];
        parent.set_is_mutated(true);

        for( int i = 0 ; i < parent.get_genomes_count() ; i++ )
        {
            double old_factor = parent.get_genomes()[i].get_factor();
            double ackley_result = calculate_ackley_function( parent.get_genomes().size() , old_factor );
            parent.get_genomes()[i].set_factor( ackley_result );
        }

        double landa = ( get_random() + m_max_random ) * ( 1.0 - ( this->get_generation_step() / this->get_max_generation_count() ) );
        Chromosome child;
        child.set_genomes_count( m_genomes_count );
        child.set_is_child(true);
        for( int i = 0 ; i < child.get_genomes_count() ; i++ )
        {
            Genome genome;
            double factor = parent.get_genomes()[i].get_factor();
            factor = ( std::rand() % 2 == 0 ) ? ( factor + ( factor * landa ) ) : ( factor - ( factor * landa ) );
            genome.set_factor(factor);
            child.add_genomes(genome);
        }
        child.calculate_fitness();
        this->add_chromosome( child );

        counter++;
    }

}

void
Population::select_best_chromosomes()
{
    //std::cout<<"select"<<std::endl;
    ChromosomeCont new_chromosomes;
    int pop_size = m_pop_size;
    for( int i = 0 ; i < pop_size ; i++ )
    {
        Chromosome chromosome = this->get_chromosomes()[i];
        chromosome.reset_actions();
        new_chromosomes.push_back( chromosome );
    }

    this->m_chromosomes.clear();
    this->m_chromosomes = new_chromosomes;
}

void
Population::calculate_fitness_function( int fitness )
{
    //std::cout<<"fitness"<<std::endl;
    m_fitness_type = fitness;

    int population = this->get_chromosomes().size();
    if( m_fitness_type == 1 ) // maximum
    {
        for( int i = 0 ; i < population - 1 ; i++ )
        {
            for( int j = i + 1 ; j < population ; j++ )
            {
                bool isless = isLess( this->get_chromosomes()[i] , this->get_chromosomes()[j] );
                if( isless )
                {
                    Chromosome tmp = this->get_chromosomes()[i];
                    this->get_chromosomes()[i] = this->get_chromosomes()[j];
                    this->get_chromosomes()[j] = tmp;
                }
            }
        }
    }
    else // minumum
    {
        for( int i = 0 ; i < population - 1 ; i++ )
        {
            for( int j = i + 1 ; j < population ; j++ )
            {
                bool isless = isLess( this->get_chromosomes()[i] , this->get_chromosomes()[j] );
                if( ! isless )
                {
                    Chromosome tmp = this->get_chromosomes()[i];
                    this->get_chromosomes()[i] = this->get_chromosomes()[j];
                    this->get_chromosomes()[j] = tmp;
                }
            }
        }
    }
}

double
Population::calculate_ackley_function( int genomes_count , double genome )
{

    double s = 0;
    for( int i = 0 ; i < genomes_count ; i++ )
    {
        s += genome * genome;
    }
    s /= genomes_count;
    double val1 = -0.2 * std::sqrt( s );


    s = 0;
    for( int i = 0 ; i < genomes_count ; i++ )
    {
        s += std::cos( 2.0 * PI * genome );
    }
    s /= genomes_count;
    double val2 = s;

    double f1 = -20.0 * std::exp( val1 );
    double f2 = -1.0 * std::exp( val2 + 20.0 + NEPER );
    double f = f1 + f2;
    return f;
}

int
Population::get_generation_step()
{
    this->m_generation_step++;
    std::cout << "Generation Step : " << this->m_generation_step << std::endl;
    return this->m_generation_step;
}

bool
Population::population_is_stopped()
{
    if( this->get_generation_step() <= this->m_max_generation_count )
        return false;

    return true;
}

void
Population::print_output()
{
    int pop_size = this->get_chromosomes().size();
    int genomes_size = this->get_chromosomes()[0].get_genomes().size();

    for( int i = 0 ; i < pop_size ; i++ )
    {
        out << "Chromosome " << i+1;
        if( i < 9 )
            out << " ";
        out << " =>   ";
        for( int j = 0 ; j < genomes_size ; j++ )
        {
            out << this->get_chromosomes()[i].get_genomes()[j].get_factor() << "\t";
        }
        out << std::endl;
    }

    out << std::endl << " END " << std::endl << std::endl;

}

void
Population::set_genomes_count( int count )
{
    m_genomes_count = count;
}

void
Population::add_chromosome( Chromosome chromosome )
{
    m_chromosomes.push_back( chromosome );
}

double
Population::get_random()
{

    double rand = std::rand() % 100001;
    rand /= 100000.0;
    rand *= ( m_max_random - m_min_random );
    rand -= m_max_random;

    return rand;
}

bool
Population::isLess( Chromosome chromosome1 , Chromosome chromosome2 )
{

    if( chromosome1.get_fitness() < chromosome2.get_fitness() )
        return true;

    return false;
}

int
Population::get_max_generation_count()
{
    return m_max_generation_count;
}

int
Population::roulette_selection()
{
    double sum = 0;
    for( unsigned int i = 0 ; i < this->get_chromosomes().size() ; i++ )
    {
        sum += this->get_chromosomes()[i].get_fitness();
    }

    double rand = get_random() + m_max_random;
    rand *= sum;

    double fitness = 0;
    int index = 0;
    bool found = false;
    for( unsigned int i = 0 ; i < this->get_chromosomes().size() ; i++ )
    {
        fitness += this->get_chromosomes()[i].get_fitness();
        if( fitness >= rand )
        {
            index = i;
            Chromosome parent = this->get_chromosomes()[index];
            if( parent.get_is_crossed() || parent.get_is_mutated() || parent.get_is_child() )
            {
                continue;
            }
            else
            {
                found = true;
                break;
            }
        }
    }

    if( found )
    {
        return index;
    }
    else
    {
        while( true )
        {
            int rand = std::rand() % this->get_chromosomes().size();
            Chromosome parent = this->get_chromosomes()[rand];
            if( parent.get_is_crossed() || parent.get_is_mutated() || parent.get_is_child() )
            {
                continue;
            }
            return rand;
        }
    }
}
