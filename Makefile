simulator_files= discrete_simulator.hpp simulator.cpp events.cpp statistics.cpp sho.cpp random_generator.cpp
simulation_files=simulation.cpp
output=simulation

make:
	g++ $(simulation_files) $(simulator_files) -o $(output)

run:
	./$(output)

clean:
	rm $(output)