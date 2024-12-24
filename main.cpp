#include <cstdlib>
#include <ctime>
#include "interface.h"

drawer* drawer::instance{nullptr};
std::mutex drawer::mtx;

int main()
{
    interface i;
    i.operate();
    return 0;
}
