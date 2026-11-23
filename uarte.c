#include "uarte.h"

nrfx_uarte_t uarte0_instance = NRFX_UARTE_INSTANCE(0);
nrfx_uarte_t uarte1_instance = NRFX_UARTE_INSTANCE(1);

char uarte0_rx_buffer;
char uarte1_rx_buffer;
void uarte0_handler(nrfx_uarte_event_t const *p_event, void *p_context) {
    nrfx_uarte_t* p_inst = p_context;

    switch (p_event->type) {
        case NRFX_UARTE_EVT_RX_DONE:
            nrfx_uarte_rx(&uarte0_instance, &uarte0_rx_buffer, sizeof(char));
            break;
        case NRFX_UARTE_EVT_TX_DONE:  
            break;
        case NRFX_UARTE_EVT_ERROR:
            break;
    }
}
void uarte1_handler(nrfx_uarte_event_t const *p_event, void *p_context) {
    nrfx_uarte_t* p_inst = p_context;

    switch (p_event->type) {
        case NRFX_UARTE_EVT_RX_DONE:
            nrfx_uarte_rx(&uarte1_instance, &uarte1_rx_buffer, sizeof(char));
            break;
        case NRFX_UARTE_EVT_TX_DONE:  
            break;
        case NRFX_UARTE_EVT_ERROR:
            break;
    }
}

void uarte_init() {
    const nrfx_uarte_config_t config0 = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD0, PIN_RXD0);
    const nrfx_uarte_config_t config1 = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD1, PIN_RXD1);

    //Initierar UARTE
    nrfx_err_t errr  = nrfx_uarte_init(&uarte0_instance, &config0, uarte0_handler);    
    if (errr != 0){
        //Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
    }

    errr = nrfx_uarte_init(&uarte1_instance, &config1, uarte1_handler);
    /*if (errr != 0){
    
    }*/

    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    nrfx_uarte_rx(&uarte0_instance, &uarte0_rx_buffer, sizeof(char));

    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(1)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(1)));
    nrfx_uarte_rx(&uarte1_instance, &uarte1_rx_buffer, sizeof(char));
}

void uarte0_write(char* tx_buffer, size_t bufferSize) {
    nrfx_uarte_tx(&uarte0_instance, tx_buffer, bufferSize, 0);
    while(nrfx_uarte_tx_in_progress(&uarte0_instance)) {}
}
char uarte0_read() {
    char c = uarte0_rx_buffer;
    uarte0_rx_buffer = '\0';
    return c;
}

void uarte1_write(char* tx_buffer, size_t bufferSize) {
    nrfx_uarte_tx(&uarte1_instance, tx_buffer, bufferSize, 0);
    while(nrfx_uarte_tx_in_progress(&uarte1_instance)) {}
}
char uarte1_read() {
    char c = uarte1_rx_buffer;
    uarte1_rx_buffer = '\0';
    return c;
}

void uart_bb_tx(uint32_t pin, uint8_t data, float baudrate) {
    float period = SystemCoreClock / baudrate;
    float t0 = dwt_get_cycle();// + period/2;

    // Start bit
    nrf_gpio_pin_write(pin, 0);

    for(int i = 0; i < 8; i++) {
        dwt_delay_until_cycle(t0 + period * (i + 1));
        nrf_gpio_pin_write(pin, (data & (1 << i)) != 0);
    }

    // Stop bit
    dwt_delay_until_cycle(t0 + period * 9);
    nrf_gpio_pin_write(pin, 1);
    
    dwt_delay_until_cycle(t0 + period * 10);
}