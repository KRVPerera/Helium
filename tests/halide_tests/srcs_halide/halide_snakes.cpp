#include <Halide.h>
#include <vector>
using namespace Halide;

int main(int argc, char **argv) {

	ImageParam input(UInt(8), 2);
	Func output("output");
	Var x("x"), y("y");

	uint8_t scale = 2;

	// The algorithm
	output(x, y) = input(x / scale, y / scale);
	std::vector<Argument> args = {input};
	output.compile_to_static_library("halide_snakes_gen", args);

	return 0;
}
