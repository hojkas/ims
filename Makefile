testfile=test.cpp
testoutput=test
simulator_files= discrete_simulator.hpp simulator.cpp events.cpp statistics.cpp sho.cpp random_generator.cpp
simulation_files=
output=simulation

make:
	g++ $(simulation_files) $(simulator_files) -o $(output)

run:
	./$(output)

test:
	@g++ $(testfile) $(simulator_files) -o $(testoutput)
	@./$(testoutput)
	@rm $(testoutput)

lazy:
	g++ *pp -o $(testoutput)

learning:
	@g++ $(simulator_files) learning_sample.cpp -o learning_sample
	@./learning_sample
	@rm learning_sample

clean:
	rm $(output) $(testoutput)