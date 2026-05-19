#include "BOARD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"

int main(void)
{
    BOARD_Init();
    ES_Initialize();
    ES_Run();

    return 0;
}
