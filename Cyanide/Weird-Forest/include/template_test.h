
#ifndef TEMPLATE_TEST_H
#define TEMPLATE_TEST_H

#include <utilities/service.h>
#include <graphics/texture_manager.h>
#include <graphics/sprite_handler.h>

#define texture_cache service<texture_manager>::get()

#define sprite_cache service<sprite_handler>::get()
#define create_sprite(Texture, Rect) sprite_cache->create_sprite(Texture, Rect)

#endif // !TEMPLATE_TEST_H

