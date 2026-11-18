#include "uarte.h"

nrfx_uarte_t uarte0_instance = NRFX_UARTE_INSTANCE(0);
nrfx_uarte_t uarte1_instance = NRFX_UARTE_INSTANCE(1);

char getChar_uarte1() {
    char c;
    nrfx_uarte_rx(&uarte1_instance, &c, sizeof(char));
    //uarte_write(&c, sizeof(char));
    return c;
}
char sendChar_uarte1(char c) {
    nrfx_uarte_tx(&uarte1_instance, &c, sizeof(char), 0);
    return c;
}

char tx_buffer[UARTE_TX_BUFFER_SIZE];
char uarte1_rx_buffer[UARTE1_RX_BUFFER_SIZE];
char* uarte1_rx_buffer_ptr;
bool uarte1_rx_buffer_overflow;
void uarte1_handler(nrfx_uarte_event_t const *p_event, void *p_context) {
    nrfx_uarte_t* p_inst = p_context;

    switch (p_event->type) {
        case NRFX_UARTE_EVT_RX_DONE:
            if(uarte1_rx_buffer_ptr == uarte1_rx_buffer + UARTE1_RX_BUFFER_SIZE - 1) {
                uarte1_rx_buffer_ptr = uarte1_rx_buffer;
                sprintf("UARTE1 overflow error\n\r", tx_buffer, UARTE_TX_BUFFER_SIZE);
                uarte0_write(tx_buffer, strlen(tx_buffer));
            } else
                uarte1_rx_buffer_ptr++;

            *uarte1_rx_buffer_ptr = getChar_uarte1();
            break;
        case NRFX_UARTE_EVT_TX_DONE:  
            break;
        case NRFX_UARTE_EVT_ERROR:
            sprintf("UARTE1 Error\n\r", tx_buffer, UARTE_TX_BUFFER_SIZE);
            uarte0_write(tx_buffer, strlen(tx_buffer));
            break;
    }
}

void uarte_init() {
    const nrfx_uarte_config_t config0 = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD0, PIN_RXD0);
    const nrfx_uarte_config_t config1 = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD1, PIN_RXD1);

    //Initierar UARTE:
    nrfx_err_t errr  = nrfx_uarte_init(&uarte0_instance, &config0, NULL);    
    if (errr != 0){
        //Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
    }

    errr = nrfx_uarte_init(&uarte1_instance, &config1, uarte1_handler);
    /*if (errr != 0){
        sprintf("UARTE1 failed to initialize\n\r", tx_buffer, UARTE_TX_BUFFER_SIZE);
        uarte0_write(tx_buffer, strlen(tx_buffer));
    }*/

    uarte1_rx_buffer_ptr = uarte1_rx_buffer;
}

void uarte0_write(char* string, size_t stringLength) {
    nrfx_uarte_tx(&uarte0_instance, string, stringLength, 0);
}

void uarte1_write(char* string, size_t stringLength) {
    nrfx_uarte_tx(&uarte1_instance, string, stringLength, 0);
}
void uarte1_read(char* rx_buffer, size_t bufferSize) {
    size_t size_rx = bufferSize;
    if(uarte1_rx_buffer_ptr - uarte1_rx_buffer < bufferSize)
        size_rx = uarte1_rx_buffer_ptr - uarte1_rx_buffer + 1;

    for(char* i = rx_buffer + size_rx - 1; i >= rx_buffer; i--) {
        *i = *uarte1_rx_buffer_ptr;
        uarte1_rx_buffer_ptr--;
    }
}