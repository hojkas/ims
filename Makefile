testfile=test.cpp
testoutput=test
simulator_files= discreet_simulator.hpp simulator.cpp events.cpp statistics.cpp sho.cpp random_generator.cpp
simulation_files=
output=simulation

make:
	g++ $(simulation_files) $(simulator_files) -o $(output)

run:
	./$(output)

test:
	@g++ $(testfile) $(simulator_files) -o $(testoutput)
	@./$(testoutput)

clean:
	rm $(output) $(testoutput)