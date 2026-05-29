#include "spi.h"

#include <SPI.h>

#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCLK 18

static SPIClass spi(HSPI);

void spi_init() {
    // Se inicializa SPI
    spi.begin(SPI_SCLK, SPI_MISO, SPI_MOSI);
}

SPIClass* spi_get_class() {
    return &spi;
}
