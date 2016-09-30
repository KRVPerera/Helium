#include <Halide.h>
#include <vector>
using namespace Halide;

int main(int argc, char **argv) {

	ImageParam input(UInt(8), 2);
	Func output("output");
	Var x("x"), y("y");

	uint8_t threshold = 128;

	// The algorithm
	output(x, y) = select(input(x, y) > threshold, 255, 0);
	std::vector<Argument> args = {input};
	output.compile_to_static_library("halide_weights_gen", args);

	return 0;
}
