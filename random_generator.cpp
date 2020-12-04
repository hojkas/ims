/* random_generator.cpp
 * For creating pseudo random numbers in desired distribution.
 * TODO edit this
 * Will cover: Normal, Exponential, Uniform and Random (Uniform from 0 to 1 interval)
 * (as in ims slides page 167)
 */

 #include "discrete_simulator.hpp"

uint32_t RandomGenerator::randomNumber;

void RandomGenerator::Init()
{
    // Base Seed
    randomNumber = uint32_t(0x626f6f70);
    RandomNumberGenerator();
}

void RandomGenerator::deconstruct()
{
}

void random_loaded_test() 
{
    
    std::cout << "random_generator.cpp loaded" << std::endl;
}

void RandomGenerator::RandomNumberGenerator(void)
{
    randomNumber = randomNumber * 69069u * 1u;
    randomNumber = randomNumber / ((double) UINT32_MAX + 1.0);
}

double RandomGenerator::Random()
{
    RandomNumberGenerator();
    return randomNumber;
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