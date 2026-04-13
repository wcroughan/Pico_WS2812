#ifndef WS2812_H
#define WS2812_H

#include "pico/types.h"
#include "hardware/pio.h"

class WS2812 {
    public:
        enum DataByte {
            NONE=0,
            RED=1,
            GREEN=2,
            BLUE=3,
            WHITE=4
        };
        enum DataFormat {
            FORMAT_RGB=0,
            FORMAT_GRB=1,
            FORMAT_WRGB=2
        };

        WS2812(uint pin, uint length, PIO pio, uint sm);
        WS2812(uint pin, uint length, PIO pio, uint sm, DataFormat format);
        WS2812(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3);
        WS2812(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3, DataByte b4);
        ~WS2812();

        static uint32_t RGB(uint8_t red, uint8_t green, uint8_t blue) {
            return (uint32_t)(blue) << 16 | (uint32_t)(green) << 8 | (uint32_t)(red);
        };

        static uint32_t RGBW(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
            return (uint32_t)(white) << 24 | (uint32_t)(blue) << 16 | (uint32_t)(green) << 8 | (uint32_t)(red);
        }

        void setPixelColor(uint index, uint32_t color);
        void setPixelColor(uint index, uint8_t red, uint8_t green, uint8_t blue);
        void setPixelColor(uint index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
        /**
         * @brief Writes a PIO-ready pixel word with no bounds check (caller must ensure index < length).
         *
         * @param index Pixel index in the strip buffer
         * @param deviceFormatWord Value already permuted for this strip's byte order (output of mapLogicalRgbToBus)
         */
        void setPixelColorUnsafeDevice(uint index, uint32_t deviceFormatWord);
        /**
         * @brief Converts logical 0x00RRGGBB (or RGBW in high byte) to the wire order this strip expects.
         */
        uint32_t mapLogicalRgbToBus(uint32_t logicalRgb) const;
        void fill(uint32_t color);
        void fill(uint32_t color, uint first);
        void fill(uint32_t color, uint first, uint count);
        void show();

    private:
        uint pin;
        uint length;
        PIO pio;
        uint sm;
        DataByte bytes[4];
        uint32_t *data;

        void initialize(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3, DataByte b4);
        uint32_t convertData(uint32_t rgbw) const;

};

#endif