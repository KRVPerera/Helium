#include <windows.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

#include "image_manipulation.h"

extern "C" {
#include "halide_complex_fisheye_gen.h"
}
//"F:\\FYP2\\Test\\SundayMeeting\\images\\a.png"
//"F:\\FYP2\\Test\\SundayMeeting\\images\\out.png"
//"C:\\Users\\FYP\\Downloads\\SundayMeeting\\a.png"
//"C:\\Users\\FYP\\Downloads\\SundayMeeting\\out.png"
int main(int argc, char **argv) {

	ULONG_PTR token = initialize_image_subsystem();
	char value = getchar();
	
	Image<uint8_t> input = load_image<uint8_t>("C:\\Users\\FYP\\Downloads\\SundayMeeting\\images\\a.png");
	Image<uint8_t> output(input.width() - 2, input.height() - 2);

	if (value == 'a'){
		halide_complex_fisheye_gen(input, output);
	}
	save_image<uint8_t>("C:\\Users\\FYP\\Downloads\\SundayMeeting\\images\\out.png", output);
	shutdown_image_subsystem(token);

	return 0;
}