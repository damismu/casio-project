#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/uart.h>
#include <string.h>

int main(void) {
    char prompt[256] = "";    
    char response[1024] = "Conectado. Escribi y dale a EXE"; 
    int cursor = 0;

    // Inicializar UART (Jack 2.5mm)
    uart_init(UART_3PIN, 115200);

    while(1) {
        dclear(C_WHITE);
        dtext(10, 10, C_BLACK, "TERMINAL GEMINI - DAMIAN");
        dhline(22, C_BLACK);
        
        dtext(10, 35, C_BLUE, "PREGUNTA:");
        dtext(10, 50, C_BLACK, prompt);
        
        dtext(10, 85, C_RED, "GEMINI:");
        dtext_opt(10, 100, C_BLACK, C_NONE, DTEXT_LEFT, DTEXT_TOP, response, -1);
        
        dupdate();

        key_event_t ev = getkey();
        
        // Borrar con la tecla DEL
        if(ev.key == KEY_DEL && cursor > 0) {
            prompt[--cursor] = '\0';
        }

        // Enviar con EXE
        if(ev.key == KEY_EXE && cursor > 0) {
            uart_write(UART_3PIN, prompt, strlen(prompt));
            uart_write(UART_3PIN, "\n", 1);
            
            strcpy(response, "Pensando...");
            dupdate();
            
            // Esperar respuesta del ESP32
            int len = uart_read(UART_3PIN, response, 1023, 10000); 
            if(len > 0) response[len] = '\0';
            else strcpy(response, "Sin respuesta del ESP32");
            
            memset(prompt, 0, sizeof(prompt));
            cursor = 0;
        }

        if(ev.key == KEY_MENU) break;
    }
    return 0;
}