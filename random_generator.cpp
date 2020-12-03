/* random_generator.cpp
 * For creating pseudo random numbers in desired distribution.
 * TODO edit this
 * Will cover: Normal, Exponential, Uniform and Random (Uniform from 0 to 1 interval)
 * (as in ims slides page 167)
 */

 #include "discreet_simulator.hpp"

 void random_loaded_test() 
 {
    std::cout << "random_generator.cpp loaded" << std::endl;
 }

 double Random()
 {
    return 0.5;
 }

 double Uniform(double L, double H)
 {
    return (L+H)/2;
 }

 double Exponential(double E)
 {
    return E;
 }

 double Normal(double M, double S)
 {
    return M;
 }