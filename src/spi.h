#pragma once

// Abstraccion de la clase SPI por la libreria
class SPIClass;

// Permiten inicializar configuracion global sobre el SPI
// utilizado por la pantalla y SD.
void spi_init();
SPIClass* spi_get_class();
