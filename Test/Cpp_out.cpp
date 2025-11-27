#include <iostream>
int main() {
    int a = 8974536;
    for (int i = 0x4fe23; i > 0; i &= i-1)
        a ^= a >> 16;
    std::cout << a;
    return 0;
}