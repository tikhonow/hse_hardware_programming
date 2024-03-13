#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"

static int static_global_var = 0; // Static global variable
int global_var = 0; // Global variable
const int const_global_var = 0; // Const global variable

void print_addresses(void) {
    static int static_local_var = 0; // Static local variable
    int local_var = 0; // Local variable
    const int const_local_var = 0; // Const local variable

    printf("Address of static global variable: %p\n", (void *)&static_global_var);
    printf("Address of global variable: %p\n", (void *)&global_var);
    printf("Address of const global variable: %p\n", (void *)&const_global_var);

    printf("Address of static local variable: %p\n", (void *)&static_local_var);
    printf("Address of local variable: %p\n", (void *)&local_var);
    printf("Address of const local variable: %p\n", (void *)&const_local_var);
}

int main(void) {
    puts("Memory addresses in RIOT OS:");
    printf("FF");
    print_addresses();
    fflush(stdout);
    return 0;

}
