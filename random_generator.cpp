/* random_generator.cpp
 * For creating pseudo random numbers in desired distribution.
 * TODO edit this
 * Will cover: Normal, Exponential, Uniform and Random (Uniform from 0 to 1 interval)
 * (as in ims slides page 167)
 */

 #include "discrete_simulator.hpp"

RandomGenerator::RandomGenerator()
{
    // Base Seed
    this->randomNumber = 0x626f6f70;
    this->RandomNumberGenerator();
}

RandomGenerator::~RandomGenerator()
{
}

void random_loaded_test() 
{
    
    std::cout << "random_generator.cpp loaded" << std::endl;
}

void RandomGenerator::RandomNumberGenerator(void)
{
    this->randomNumber = this->randomNumber * 69069u * 1u;
    this->randomNumber = this->randomNumber / ((double) UINT32_MAX + 1.0);
}

double RandomGenerator::Random()
{
    this->RandomNumberGenerator();
    return this->randomNumber;
}

double RandomGenerator::Uniform(double L, double H)
{
    return (L+H)/2;
}

double RandomGenerator::Exponential(double E)
{
    return E;
}

double RandomGenerator::Normal(double M, double S)
{
    return M;
}