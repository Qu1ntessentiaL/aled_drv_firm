#include "images.h"

extern char buff[20];

void im_main(void) {
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    i = rand() % 256;
    j = rand() % 256;
    k = rand() % 256;
    SSD1306_GotoXY(0, 0);
    sprintf(buff, "RED:   %d", i);
    SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(0, 22);
    sprintf(buff, "GREEN: %d", j);
    SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(0, 44);
    sprintf(buff, "BLUE:  %d", k);
    SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
}
