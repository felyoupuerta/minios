#include <stdint.h>
#include "keyboard.h"
#include "screen.h"
#include "commands.h"

// Defino puertos y teclas
#define KEYBOARD_DATA_PORT 0x60  // Puerto pa' leer el teclado
#define ENTER_KEY 0x1C           // Scancode del Enter
#define BACKSPACE_KEY 0x0E       // Scancode del Backspace
#define BUFFER_SIZE 128          // Tamaño del buffer pa' los comandos

char buffer[BUFFER_SIZE];        // Acá guardamos lo que tipea el usuario
unsigned int index = 0;          // Posición actual en el buffer

// Mapeo teclas teclado yankee QWERTY
static const char scancode_to_char[] = {
    [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r', [0x14] = 't',  // Primera fila
    [0x15] = 'y', [0x16] = 'u', [0x17] = 'i', [0x18] = 'o', [0x19] = 'p',  // Segunda fila
    [0x1E] = 'a', [0x1F] = 's', [0x20] = 'd', [0x21] = 'f', [0x22] = 'g',  // Tercera fila
    [0x23] = 'h', [0x24] = 'j', [0x25] = 'k', [0x26] = 'l',                // Tercera fila
    [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v',                // Cuarta fila
    [0x30] = 'b', [0x31] = 'n', [0x32] = 'm',                              // Cuarta fila
    [0x39] = ' '  // Barra espaciadora
};

// Función pa leer del puerto
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Función principal del teclado
void init_keyboard(void) {
    while (1) {  // Loop infinito para escuchar siempre
        uint8_t scancode = inb(KEYBOARD_DATA_PORT);  // Leo tecla
        
        // Si es Enter, procesamos el comando
        if (scancode == ENTER_KEY) {
            buffer[index] = '\0';  // Ponemos fin de string
            handle_command(buffer); // Mandamos el comando
            index = 0;             // Reseteamos el buffer
        }
        // Si es Backspace y hay algo pa borrar
        else if (scancode == BACKSPACE_KEY && index > 0) {
            index--;               // Retrocedemos el índice
            print("\b \b");        // Borramos de la pantalla
        }
        // Si es una tecla mapeada
        else if (scancode <= 0x39 && scancode_to_char[scancode] != 0) {
            // Si no nos pasamos del buffer
            if (index < BUFFER_SIZE - 1) {
                buffer[index++] = scancode_to_char[scancode];  // Guardamos la letra
                char str[2] = {scancode_to_char[scancode], '\0'};
                print(str);  // Mostramos en pantalla
            }
        }
    }
}
