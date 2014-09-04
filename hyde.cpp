#include "hyde.hpp"

#if 1
#include "deps/manymouse/manymouse.h"
#include "deps/manymouse/manymouse.c"
#include "deps/manymouse/linux_evdev.c"
#include "deps/manymouse/macosx_hidmanager.c"
#include "deps/manymouse/macosx_hidutilities.c"
#include "deps/manymouse/windows_wminput.c"
#include "deps/manymouse/x11_xinput2.c"
#endif

namespace hyde {
    hid::dt global_timer;
}
