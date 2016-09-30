#include "Halide.h"
#include <vector>
using namespace Halide;

int main(int argc, char **argv) {

    ImageParam input(UInt(8), 2);

    Func output("output");
    Var x("x"), y("y");

	Func clamped("clamped");
	Expr clamped_x = clamp(x, 0, input.width() - 1);
	Expr clamped_y = clamp(y, 0, input.height() - 1);
	clamped(x, y) = cast<double>(input(clamped_x, clamped_y));

    /*
	 * Reference http://popscan.blogspot.com/2012/04/fisheye-lens-equation-simple-fisheye.html
	*/

	Expr nx = ((cast<double>(2.0f) * x)/input.width())-1;	// normalize x coordinate to -1 ... 1
	Expr ny = ((cast<double>(2.0f) * y)/input.height())-1;	// normalize y coordinate to -1 ... 1

	Expr r = sqrt(pow(nx,2) + pow(ny,2));	// calculate radius

	Expr nr = (r + (1 - (sqrt(1-r*r)) )) / cast<double>(2.0f);	// new distance is between 0 ... 1

	Expr theta = atan2(ny,nx);	// calculate the angle for polar coordinates

	// calculate new x and y position with new distance in same angl
	Expr nxn = nr * cos(theta);
	Expr nyn = nr * sin(theta);

	// map from -1 ... 1 to image coordinates
	Expr newX = ((nxn + 1) * input.width())/2;
	Expr newY = ((nyn + 1) * input.height())/2;

	output(x, y) = cast<uint8_t>(clamped(
		cast<uint8_t>(newX),
		cast<uint8_t>(newY)
		));

    std::vector<Argument> args = {input};
    output.compile_to_static_library("halide_complex_fisheye_gen", args);
    return 0;
}
