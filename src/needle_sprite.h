#ifndef NEEDLE_SPRITE_H
#define NEEDLE_SPRITE_H

#include <stdint.h>
#include "oled.h"
#include "display.h"


typedef struct needle_sprite_t_ {
  sprite_t sprite;
  int8_t column[OLED_HEIGHT];
  int8_t start_column[OLED_PAGES_N];
  int8_t end_column[OLED_PAGES_N];
} needle_sprite_t;


void needle_sprite_init(needle_sprite_t *needle);
void needle_sprite_draw(needle_sprite_t *needle, uint8_t angle);
void needle_sprite_add_to_extents(needle_sprite_t *needle, update_extents_t extents);


#endif /* NEEDLE_SPRITE_H */
