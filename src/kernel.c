#include "screen.h"
#include "keyboard.h"
#include "memory.h"
#include "commands.h"
#include "multiboot.h"

void kernel_main() {
    clear_screen();
    print("MiniOS v0.1\n");
    print("Type 'help' for available commands.\n\n");

    init_memory();
    init_keyboard();
}
