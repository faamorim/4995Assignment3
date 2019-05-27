#include "Utils.h"



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Displays an error message to the user
Parameters:
[in] szFormat - the error message string
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SetError(const char* szFormat, ...) {
    char szBuffer[1024];
    va_list pArgList;

    va_start(pArgList, szFormat);

    _vsntprintf_s(szBuffer, sizeof(szBuffer) / sizeof(char), szFormat, pArgList);

    va_end(pArgList);

    OutputDebugString(szBuffer);
    OutputDebugString("\n");
    printf("error!!");
}
