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

Layer layer;

BmpContainer flux_one;


void layer_update_callback(Layer *me, GContext* ctx) {
  (void)me;
  (void)ctx;

  layer_add_child(&window.layer, &flux_one.layer.layer);
}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Flux Capacitor");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);


  // Init the layer for the minute display
  layer_init(&layer, window.layer.frame);
  layer.update_proc = &layer_update_callback;
  layer_add_child(&window.layer, &layer);

  resource_init_current_app(&APP_RESOURCES);

  // Note: This needs to be "de-inited" in the application's
  //       deinit handler.
  bmp_init_container(RESOURCE_ID_IMAGE_FLUX_ONE, &flux_one);
}


void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  bmp_deinit_container(&flux_one);
}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
