#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID {0x9B, 0xC4, 0xC2, 0x86, 0x42, 0xFE, 0x47, 0xC6, 0xB6, 0x09, 0x80, 0x05, 0xD5, 0xF0, 0x3D, 0x7D}
PBL_APP_INFO(MY_UUID,
             "Flux", "Igor Vieira",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;

Layer bg_layer;
Layer animation_layer;
Layer top_layer;

TextLayer text_time_layer;

BmpContainer flux_one;
BmpContainer flux_two;
BmpContainer flux_three;
BmpContainer flux_four;


void toplayer_update_callback(Layer *me, GContext* ctx) {
  (void)me;
  (void)ctx;

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(35,67, 75, 29), 0, GCornerNone);
}


void animationlayer_update_callback(Layer *me, GContext* ctx) {
  (void)me;
  (void)ctx;

  PblTm t;
  get_time(&t);

  if(t.tm_min != 0) return;

  switch(t.tm_sec)
  {
    case 0:   graphics_draw_bitmap_in_rect(ctx, &flux_two.bmp, GRect(0, 0, 144, 168));
              break;
    case 2:   graphics_draw_bitmap_in_rect(ctx, &flux_two.bmp, GRect(0, 0, 144, 168));
              break;
    case 3:   graphics_draw_bitmap_in_rect(ctx, &flux_three.bmp, GRect(0, 0, 144, 168));
              break;
    case 4:   graphics_draw_bitmap_in_rect(ctx, &flux_two.bmp, GRect(0, 0, 144, 168));
              break;
    case 5:   graphics_draw_bitmap_in_rect(ctx, &flux_three.bmp, GRect(0, 0, 144, 168));
              break;
    case 6:   graphics_draw_bitmap_in_rect(ctx, &flux_four.bmp, GRect(0, 0, 144, 168));
              break;
    case 7:   graphics_draw_bitmap_in_rect(ctx, &flux_three.bmp, GRect(0, 0, 144, 168));
              break;
    case 8:   graphics_draw_bitmap_in_rect(ctx, &flux_four.bmp, GRect(0, 0, 144, 168));
              break;
    case 9:   graphics_draw_bitmap_in_rect(ctx, &flux_three.bmp, GRect(0, 0, 144, 168));
              break;
    case 10:  graphics_draw_bitmap_in_rect(ctx, &flux_two.bmp, GRect(0, 0, 144, 168));
              break;
    case 12:  graphics_draw_bitmap_in_rect(ctx, &flux_two.bmp, GRect(0, 0, 144, 168));
              break;
  }
}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Flux Capacitor");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  // Init the layer for the minute display
  layer_init(&bg_layer, window.layer.frame);
  layer_add_child(&window.layer, &bg_layer);

  resource_init_current_app(&APP_RESOURCES);
  bmp_init_container(RESOURCE_ID_IMAGE_FLUX_ONE, &flux_one);
  bmp_init_container(RESOURCE_ID_IMAGE_FLUX_TWO, &flux_two);
  bmp_init_container(RESOURCE_ID_IMAGE_FLUX_THREE, &flux_three);
  bmp_init_container(RESOURCE_ID_IMAGE_FLUX_FOUR, &flux_four);

  //Background
  layer_add_child(&bg_layer, &flux_one.layer.layer);

  //Z-Index
  layer_add_child(&bg_layer, &animation_layer);
  animation_layer.update_proc = &animationlayer_update_callback;
  layer_add_child(&window.layer, &top_layer);
  top_layer.update_proc = &toplayer_update_callback;


  //Clock text
  text_layer_init(&text_time_layer, GRect(35, 65, 74, 30));
  text_layer_set_text_color(&text_time_layer, GColorWhite);
  text_layer_set_background_color(&text_time_layer, GColorClear);
  text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGITAL_26)));
  text_layer_set_text_alignment(&text_time_layer, GTextAlignmentCenter);
  text_layer_set_text(&text_time_layer, "00:00");
  layer_add_child(&top_layer, &text_time_layer.layer);
}


void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  bmp_deinit_container(&flux_one);
  bmp_deinit_container(&flux_two);
  bmp_deinit_container(&flux_three);
  bmp_deinit_container(&flux_four);
}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;

  //Redraw layer
  layer_mark_dirty(&animation_layer);

  //Time
  static char time_text[] = "00:00";
  char *time_format;

  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);

  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(&text_time_layer, time_text);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = SECOND_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
