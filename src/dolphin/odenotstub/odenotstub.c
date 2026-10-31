#include <dolphin/OdemuExi2/DebuggerDriver.h>

// Reports that no debugger stub is linked. Weak: a debugger stub library replaces it.
__declspec(weak) BOOL Hu_IsStub(void)
{
    return FALSE;
}
