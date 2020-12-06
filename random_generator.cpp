/* random_generator.cpp
 * For creating pseudo random numbers in desired distribution.
 * TODO edit this
 * Will cover: Normal, Exponential, Uniform and Random (Uniform from 0 to 1 interval)
 * (as in ims slides page 167)
 */

 #include "discrete_simulator.hpp"

uint32_t RandomGenerator::randomNumber;
double RandomGenerator::second_BM_variable;

void RandomGenerator::Init()
{
    // Base Seed
    randomNumber = time(nullptr);
    second_BM_variable = 0.0;
}

void RandomGenerator::SetSeed(uint32_t seed)
{
    randomNumber = seed;
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
}

double RandomGenerator::Random()
{
    RandomNumberGenerator();
    return (randomNumber * fraction);
}

double RandomGenerator::Uniform(double MIN, double MAX)
{
    
    RandomNumberGenerator();
    return MIN + (randomNumber / (((double) UINT32_MAX + 1.0) / (MAX - MIN)));
}

double RandomGenerator::Exponential(double MEAN)
{
    return - MEAN / log(Random());
}

double RandomGenerator::Normal(double mu, double sigma)
{
    if (second_BM_variable){
        auto temp = second_BM_variable;
        second_BM_variable = 0.0;
        return temp;
    }

        /*
     *  Zdroj: https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
     * Pouzite v sulade s licencnymi podmienkami wikipedia.org o znovu-pouzivani text pod licenciou
     * Creative Commons Attribution Share-Alike License 3.0 - https://creativecommons.org/licenses/by-sa/3.0/ 
     * 
     */
    constexpr double epsilon = std::numeric_limits<double>::epsilon();
    constexpr double two_pi = 2.0 * M_PI;

    double u1, u2;
    do
    {
        u1 = Random();
        u2 = Random();
    } while (u1 <= epsilon);

    auto mag = sigma * sqrt(-2.0 * log(u1));
    auto z0 = mag * cos(two_pi * u2) + mu;
    auto z1 = mag * sin(two_pi * u2) + mu;
    /*
     * Koniec kopirovaneho kodu
     */

    second_BM_variable = z1;
    
    return z0;
}