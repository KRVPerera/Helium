#include "Halide.h"
using namespace Halide;

int main(int argc, char **argv) {
    //TODO : 45 degrees angle of images gets a error, cpp code works for the similar code
    ImageParam input(UInt(8), 2);

    Func output("output");
	Var x("x"), y("y");;

	Func clamped("clamped");
	Expr clamped_x = clamp(x, 0, input.width() - 1);
	Expr clamped_y = clamp(y, 0, input.height() - 1);
	clamped(x, y) = cast<double>(input(clamped_x, clamped_y));

	Expr M_PI = cast<double>(3.14159265f);
	Expr centerX = input.width() / 2;
	Expr centerY = input.height() / 2;
	Expr R = cast<double>(sqrt(pow(centerX, 2) + pow(centerY, 2)));

	Expr dx = x - centerX;
	Expr dy = y - centerY;

	Expr distance = cast<double>(sqrt(pow(dx, 2) + pow(dy, 2)));
	Expr xout = atan2(dx, dy)* centerX / M_PI;
	Expr yout = distance * input.height() / R;

	Expr newX = ceil(xout + centerX);
	Expr newY = ceil(yout);

	output(x, y) = cast<uint8_t>(clamped(
		cast<uint8_t>(newX),
		cast<uint8_t>(newY)
		));
    
    output.compile_to_file("halide_complex_cylinder_gen", input);
  
    return 0;
}