#include "genome.h"

Genome::Genome()
{
    m_factor = 0;
}

void
Genome::set_factor( double factor )
{
    m_factor = factor;
}

double
Genome::get_factor()
{
    return m_factor;
}
