#include "Halide.h"
using namespace Halide;

int main(int argc, char **argv) {
    
    ImageParam input(UInt(8), 2);

    Func output("output"),delta("delta"),notes("notes");
    Var x("x"), y("y");

    /*
	algo taken from https://github.com/alkacon/alkacon-simapi/blob/master/src-dump/filter/TwirlFilter.java
	*/   

	output(x,y)=input(
		cast<uint8_t>(min((input.width()/2)+(sqrt(pow(x-(input.width()/2),2)+pow(y-(input.height()/2),2)))*cos(atan2((y-(input.height()/2)),(x-(input.width()/2)))+(5*(min(input.width()/2,input.height()/2) - sqrt(pow(x-(input.width()/2),2)+pow(y-(input.height()/2),2))))/min(input.width()/2,input.height()/2)),input.width()-1)),
		cast<uint8_t>(min((input.height()/2)+(sqrt(pow(x-(input.width()/2),2)+pow(y-(input.height()/2),2)))*sin(atan2((y-(input.height()/2)),(x-(input.width()/2)))+(5*(min(input.width()/2,input.height()/2) - sqrt(pow(x-(input.width()/2),2)+pow(y-(input.height()/2),2))))/min(input.width()/2,input.height()/2)),input.height()-1)
	));
    
    output.compile_to_file("halide_complex_twirl_gen", input); 
    return 0;
}
