#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/uart.h>
#include <string.h>

void main(void) {
    char prompt[256] = "";    
    char response[1024] = "Conectado al ESP32..."; 
    int cursor = 0;

    // Configuración de UART (Puerto de 3 pines, 115200 baudios)
    uart_init(UART_3PIN, 115200);

    while(1) {
        dclear(C_WHITE);
        dtext(10, 10, C_BLACK, "TERMINAL GEMINI AI");
        dhline(22, C_BLACK);
        
        dtext(10, 30, C_BLUE, "PREGUNTA:");
        dtext(10, 45, C_BLACK, prompt);
        
        dtext(10, 80, C_RED, "RESPUESTA:");
        // dtext_opt permite que el texto largo se ajuste a la pantalla
        dtext_opt(10, 95, C_BLACK, C_NONE, DTEXT_LEFT, DTEXT_TOP, response, -1);
        
        dupdate();

        key_event_t ev = getkey();
        
        // Tecla DEL para borrar caracteres
        if(ev.key == KEY_DEL && cursor > 0) {
            prompt[--cursor] = '\0';
        }

        // Tecla EXE para enviar al ESP32
        if(ev.key == KEY_EXE && cursor > 0) {
            uart_write(UART_3PIN, prompt, strlen(prompt));
            uart_write(UART_3PIN, "\n", 1); // Indicador de fin de mensaje para el ESP32
            
            strcpy(response, "Pensando...");
            dupdate();
            
            // Leer respuesta (espera hasta 12 segundos)
            int len = uart_read(UART_3PIN, response, 1023, 12000); 
            if(len > 0) response[len] = '\0';
            else strcpy(response, "TIMEOUT: Sin respuesta del ESP32");
            
            memset(prompt, 0, sizeof(prompt));
            cursor = 0;
        }

        if(ev.key == KEY_MENU) break;
    }
}