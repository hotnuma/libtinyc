#include <CString.h>
#include <print.h>

CString testfunc(bool val)
{
    CString buff(512);

    print("addr = %p", buff.data());

    if (val == false)
        return buff;

    buff = "blablie";

    return buff;
}
int main()
{
    CString buff = testfunc(true);

    print("addr = %p", buff.data());

    return 0;
}


