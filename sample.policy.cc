#include <cassert>
#include <string>
#include <iostream>

#include "hyde.hpp"

int main( int argc, char **argv )
{
    {
        // following lines are ok
        hyde::windows::gamepad gamepad1(0);
        hyde::windows::gamepad gamepad2(1);
        hyde::windows::gamepad gamepad3(2);
        hyde::windows::gamepad gamepad4(3);
        // following line is wrong: hyde::windows::gamepad driver supports only 4 instances at once
        hyde::windows::gamepad gamepad5(4);
        // following line is wrong: device id #0 is already being used
        hyde::windows::gamepad gamepad6(0);

        // following line is unreachable
        ;
    }

    {
        // following lines are ok
        hyde::windows::mouse mouse1(0);
        // following line is wrong: hyde::windows::mouse driver supports only 1 instance at once
        hyde::windows::mouse mouse2(1);
        // following line is wrong: device id #0 is already in use
        hyde::windows::mouse mouse3(0);

        // following line is unreachable
        ;
    }

    {
        // following line is ok
        hyde::windows::keyboard keyboard1(0);
        // following line is wrong: hyde::windows::keyboard driver supports only 1 instance at once
        hyde::windows::keyboard keyboard2(1);
        // following line is wrong: device id #0 is already in use
        hyde::windows::keyboard keyboard3(0);

        // following line is unreachable
        ;
    }

    std::cout << "All ok" << std::endl;
    return 0;
}

