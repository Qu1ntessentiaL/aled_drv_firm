#include "images.h"

extern char buff[20];

void im_main(uint8_t red, uint8_t green, uint8_t blue) {
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    red = rand() % 256;
    green = rand() % 256;
    blue = rand() % 256;
    SSD1306_GotoXY(0, 0);
    sprintf(buff, "RED:   %d", red);
    SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(0, 22);
    sprintf(buff, "GREEN: %d", green);
    SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(0, 44);
    sprintf(buff, "BLUE:  %d", blue);
    SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
}
