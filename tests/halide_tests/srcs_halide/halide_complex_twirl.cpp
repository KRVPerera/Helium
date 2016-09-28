#include "Halide.h"
using namespace Halide;

int main(int argc, char **argv) {
    
    ImageParam input(UInt(8), 2);

    Func output("output"),delta("delta"),notes("notes");
    Var x("x"), y("y");

    /*
	algo taken from https://github.com/alkacon/alkacon-simapi/blob/master/src-dump/filter/TwirlFilter.java
	*/   

	Expr centerX=input.width()/2;
    Expr centerY=input.height()/2;
    Expr radius=min(centerX,centerY);
    Expr radiusSqr=pow(radius,2); 

    Expr dx=x-(input.width()/2);
    Expr dy=y-(input.height()/2);
    Expr distance=sqrt(pow(dx,2)+pow(dy,2));
    Expr a=atan2(dy,dx)+(5*(radius - distance))/radius;
   
    output(x,y)=input(
        cast<uint8_t>(min(centerX+distance*cos(a),input.width()-1)),
        cast<uint8_t>(min(centerY+distance*sin(a),input.height()-1)
    ));
    
    output.compile_to_file("halide_complex_twirl_gen", input); 
    return 0;
}
