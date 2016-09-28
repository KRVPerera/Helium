#include "Halide.h"
using namespace Halide;

int main(int argc, char **argv) {
    
    ImageParam input(UInt(8), 2);

    Func output("output");
    Var x("x"), y("y");
	
	Func clamped("clamped");
	Expr clamped_x = clamp(x, 0, input.width() - 1);
	Expr clamped_y = clamp(y, 0, input.height() - 1);
	clamped(x, y) = cast<double>(input(clamped_x, clamped_y));

	Expr centerX=input.width()/2;
    Expr centerY=input.height()/2;
    Expr radius=min(centerX,centerY);
    Expr radiusSqr=pow(radius,2); 

    Expr dx=x-(input.width()/2);
    Expr dy=y-(input.height()/2);
    Expr distance=sqrt(pow(dx,2)+pow(dy,2));
    Expr a=atan2(dy,dx)+(5*(radius - distance))/radius;
   
    output(x,y)=cast<uint8_t>(clamped(
        cast<uint8_t>(min(centerX+distance*cos(a),input.width()-1)),
        cast<uint8_t>(min(centerY+distance*sin(a),input.height()-1)
    )));
    
    output.compile_to_file("halide_complex_twirl_gen", input); 
    return 0;
}
