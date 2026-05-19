#include "BOARD.h"
#include "TestShell.h"

int main(void)
{
    BOARD_Init();
    TestShell_Init();
    TestShell_Run();

    return 0;
}
