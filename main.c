/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_display_t * hal_init(int32_t w, int32_t h);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

LV_IMG_DECLARE(avatar);
LV_IMAGE_DECLARE(test_image_cogwheel_i4);
LV_IMAGE_DECLARE(test_image_cogwheel_a8);
LV_IMAGE_DECLARE(test_image_cogwheel_rgb565);
LV_IMAGE_DECLARE(test_image_cogwheel_rgb565a8);
LV_IMAGE_DECLARE(test_image_cogwheel_xrgb8888);
LV_IMAGE_DECLARE(test_image_cogwheel_argb8888);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#include <dirent.h>

static void create_pic_preview(void)
{
    lv_obj_t* img;
    DIR* d;
    struct dirent* dir;

    /*Create a container with COLUMN flex direction*/
    lv_obj_t* cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_all(cont, 50, 50);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(cont, 0, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    static const char* const roots[] = {
        "/home/neo/projects/lvgl/lv_port_pc_eclipse/output/",
    };

    char path[256];

    for (int i = 0; i < sizeof(roots) / sizeof(roots[0]); i++) {
        const char* root;
        root = roots[i];

        d = opendir(root);
        if (!d)
            continue;

        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type != DT_REG)
                continue;

            img = lv_image_create(cont);
            lv_obj_center(img);
            lv_obj_set_style_border_width(img, 1, 0);
            lv_obj_set_style_border_color(img, lv_color_hex(0xff0000), 0);

            snprintf(path, sizeof(path), "%s/%s", root, dir->d_name);
            printf("add img: %s\n", path);
            lv_image_set_src(img, path);
            lv_image_set_rotation(img, 40);
            // lv_obj_set_style_image_recolor_opa(img, LV_OPA_50, 0);
            // lv_obj_set_style_image_recolor(img, lv_color_hex(0xff0000), 0);

            lv_image_header_t header;
            lv_image_decoder_get_info(path, &header);
            if (LV_COLOR_FORMAT_IS_ALPHA_ONLY(header.cf)) {
                lv_obj_set_style_image_recolor(img, lv_color_hex(0xff0000), 0);
                lv_obj_set_style_image_recolor_opa(img, LV_OPA_50, 0);
            }

            lv_obj_t *label = lv_label_create(img);
            lv_obj_set_style_text_font(label, &lv_font_montserrat_8, 0);
            lv_obj_set_style_text_color(label, lv_color_hex(0xff0000), 0);
            lv_label_set_text(label, dir->d_name);
        }
    }

    closedir(d);
}


static void img_create(const char * name, const void * img_src, bool rotate, bool recolor)
{
    lv_obj_t * cont = lv_obj_create(lv_screen_active());
    lv_obj_set_size(cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(cont, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
    lv_obj_set_style_pad_all(cont, 5, 0);

    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * img = lv_image_create(cont);
    lv_image_set_src(img, img_src);
    lv_obj_set_style_image_recolor(img, lv_palette_main(LV_PALETTE_RED), 0);   /*For A8*/
    if(recolor) lv_obj_set_style_image_recolor_opa(img, LV_OPA_70, 0);
    if(rotate) lv_image_set_angle(img, 450);

    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text(label, name);
}

#if LV_USE_OBJ_PROPERTY
static void obj_property_example(void)
{
#define MY_PROPERTIES_COUNT 7

  LV_IMG_DECLARE(img_demo_widgets_avatar);

  /*Pointer type of property value*/
  const lv_point_t pivot_50 = {
    LV_PCT(50),
    LV_PCT(50),
  };

  /*Const properties*/
  const struct _obj_info_t {
    const lv_obj_class_t *clz;
    const int properties_count;

    lv_property_t properties[MY_PROPERTIES_COUNT];
  } my_ui[] = {
    {
      .clz = &lv_image_class,
      .properties_count = MY_PROPERTIES_COUNT,
      .properties = {
        { .id = LV_PROPERTY_OBJ_FLAG_HIDDEN, .num = 1, },
        { .id = LV_PROPERTY_IMAGE_SRC, .ptr = &img_demo_widgets_avatar, },
        { .id = LV_PROPERTY_IMAGE_PIVOT, .ptr = &pivot_50, },
        { .id = LV_PROPERTY_IMAGE_ROTATION, .num = 900, },
        { .id = LV_PROPERTY_IMAGE_SCALE, .num = 128, },
        { .id = LV_PROPERTY_OBJ_FLAG_CLICKABLE, .num = 1, },
        { .id = LV_STYLE_IMAGE_OPA, .num = 128, },
      }
    }
  };

  lv_obj_t *root = lv_obj_create(lv_scr_act());
  lv_obj_center(root);
  lv_obj_set_size(root, 200, 200);

  lv_obj_t * img = lv_image_create(lv_scr_act());
  lv_property_t prop;
  prop.id = LV_PROPERTY_OBJ_PARENT;
  prop.ptr = root;
  lv_obj_set_property(img, &prop);
  printf("expect: %p, got: %p\n", root, lv_obj_get_parent(img));
  printf("expect: %p, got: %p\n", root, lv_obj_get_property(img, LV_PROPERTY_OBJ_PARENT).ptr);

  /*Runtime generated properties*/
  lv_property_t prop_parent = {
    .id = LV_PROPERTY_OBJ_PARENT,
    .ptr = root,
  };

  for (int i = 0; i < sizeof(my_ui)/sizeof(my_ui[0]); i++) {
    const struct _obj_info_t *info = &my_ui[i];
    lv_obj_t * obj = lv_obj_class_create_obj(info->clz, lv_scr_act());
    lv_obj_class_init_obj(obj);

    // LV_OBJ_PROPERTY_ARRAY_SET(obj, info->properties);
    for (int j = 0; j < info->properties_count; j++) {
      lv_obj_set_property(obj, &info->properties[j]);
    }

    lv_obj_set_property(obj, &prop_parent);

    lv_property_t prop = { };
    prop.id = LV_STYLE_TEXT_FONT;
    prop.ptr = LV_FONT_DEFAULT;
    lv_obj_set_property(obj, &prop);
    printf("font: expect: %p, got: %p, real: %p\n", LV_FONT_DEFAULT, lv_obj_get_property(obj, LV_STYLE_TEXT_FONT).ptr, lv_obj_get_style_text_font(obj, LV_PART_MAIN));

    printf("obj %d: %p\n", i, obj);
    printf("clickable: %d\n", lv_obj_has_flag(obj, LV_OBJ_FLAG_CLICKABLE));
    printf("hidden: %d\n", lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN));

    printf("clickable2: %d\n", lv_obj_get_property(obj, LV_PROPERTY_OBJ_FLAG_CLICKABLE).num);
    printf("hidden2: %d\n", lv_obj_get_property(obj, LV_PROPERTY_OBJ_FLAG_HIDDEN).num);
  }
}

static void test_property(void)
{
#define TEST_ASSERT_FALSE(expr) \
  do { \
    if (expr) { \
      printf("assert failed: %s\n", #expr); \
      exit(1); \
    } \
  } while (0)

#define TEST_ASSERT_TRUE(expr) \
  do { \
    if (!(expr)) { \
      printf("assert failed: %s\n", #expr); \
      exit(1); \
    } \
  } while (0)

    const struct {
        uint32_t flag;
        uint32_t id;
    } properties[] = {
        {LV_OBJ_FLAG_HIDDEN,                    LV_PROPERTY_OBJ_FLAG_HIDDEN},
        {LV_OBJ_FLAG_CLICKABLE,                 LV_PROPERTY_OBJ_FLAG_CLICKABLE},
        {LV_OBJ_FLAG_CLICK_FOCUSABLE,           LV_PROPERTY_OBJ_FLAG_CLICK_FOCUSABLE},
        {LV_OBJ_FLAG_CHECKABLE,                 LV_PROPERTY_OBJ_FLAG_CHECKABLE},
        {LV_OBJ_FLAG_SCROLLABLE,                LV_PROPERTY_OBJ_FLAG_SCROLLABLE},
        {LV_OBJ_FLAG_SCROLL_ELASTIC,            LV_PROPERTY_OBJ_FLAG_SCROLL_ELASTIC},
        {LV_OBJ_FLAG_SCROLL_MOMENTUM,           LV_PROPERTY_OBJ_FLAG_SCROLL_MOMENTUM},
        {LV_OBJ_FLAG_SCROLL_ONE,                LV_PROPERTY_OBJ_FLAG_SCROLL_ONE},
        {LV_OBJ_FLAG_SCROLL_CHAIN_HOR,          LV_PROPERTY_OBJ_FLAG_SCROLL_CHAIN_HOR},
        {LV_OBJ_FLAG_SCROLL_CHAIN_VER,          LV_PROPERTY_OBJ_FLAG_SCROLL_CHAIN_VER},
        {LV_OBJ_FLAG_SCROLL_ON_FOCUS,           LV_PROPERTY_OBJ_FLAG_SCROLL_ON_FOCUS},
        {LV_OBJ_FLAG_SCROLL_WITH_ARROW,         LV_PROPERTY_OBJ_FLAG_SCROLL_WITH_ARROW},
        {LV_OBJ_FLAG_SNAPPABLE,                 LV_PROPERTY_OBJ_FLAG_SNAPPABLE},
        {LV_OBJ_FLAG_PRESS_LOCK,                LV_PROPERTY_OBJ_FLAG_PRESS_LOCK},
        {LV_OBJ_FLAG_EVENT_BUBBLE,              LV_PROPERTY_OBJ_FLAG_EVENT_BUBBLE},
        {LV_OBJ_FLAG_GESTURE_BUBBLE,            LV_PROPERTY_OBJ_FLAG_GESTURE_BUBBLE},
        {LV_OBJ_FLAG_ADV_HITTEST,               LV_PROPERTY_OBJ_FLAG_ADV_HITTEST},
        {LV_OBJ_FLAG_IGNORE_LAYOUT,             LV_PROPERTY_OBJ_FLAG_IGNORE_LAYOUT},
        {LV_OBJ_FLAG_FLOATING,                  LV_PROPERTY_OBJ_FLAG_FLOATING},
        {LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS,     LV_PROPERTY_OBJ_FLAG_SEND_DRAW_TASK_EVENTS},
        {LV_OBJ_FLAG_OVERFLOW_VISIBLE,          LV_PROPERTY_OBJ_FLAG_OVERFLOW_VISIBLE},
        {LV_OBJ_FLAG_FLEX_IN_NEW_TRACK,         LV_PROPERTY_OBJ_FLAG_FLEX_IN_NEW_TRACK},
        {LV_OBJ_FLAG_LAYOUT_1,                  LV_PROPERTY_OBJ_FLAG_LAYOUT_1},
        {LV_OBJ_FLAG_LAYOUT_2,                  LV_PROPERTY_OBJ_FLAG_LAYOUT_2},
        {LV_OBJ_FLAG_WIDGET_1,                  LV_PROPERTY_OBJ_FLAG_WIDGET_1},
        {LV_OBJ_FLAG_WIDGET_2,                  LV_PROPERTY_OBJ_FLAG_WIDGET_2},
        {LV_OBJ_FLAG_USER_1,                    LV_PROPERTY_OBJ_FLAG_USER_1},
        {LV_OBJ_FLAG_USER_2,                    LV_PROPERTY_OBJ_FLAG_USER_2},
        {LV_OBJ_FLAG_USER_3,                    LV_PROPERTY_OBJ_FLAG_USER_3},
        {LV_OBJ_FLAG_USER_4,                    LV_PROPERTY_OBJ_FLAG_USER_4},
    };

    lv_obj_t * obj = lv_obj_create(lv_scr_act());
    obj->flags = 0;
    for (unsigned long i = 0; i < sizeof(properties) / sizeof(properties[0]); i++) {

        TEST_ASSERT_FALSE(lv_obj_get_property(obj, properties[i].id).num);
        lv_obj_add_flag(obj, properties[i].flag);
        TEST_ASSERT_TRUE(lv_obj_get_property(obj, properties[i].id).num);

        lv_obj_remove_flag(obj, properties[i].flag);
        TEST_ASSERT_FALSE(lv_obj_get_property(obj, properties[i].id).num);

        lv_property_t prop = { };
        prop.id = properties[i].id;
        prop.num = 1;
        TEST_ASSERT_TRUE(lv_obj_set_property(obj, &prop) == LV_RESULT_OK);
        TEST_ASSERT_TRUE(lv_obj_get_property(obj, properties[i].id).num);
        TEST_ASSERT_TRUE(lv_obj_has_flag(obj, properties[i].flag));

        prop.id = properties[i].id;
        prop.num = 0;
        TEST_ASSERT_TRUE(lv_obj_set_property(obj, &prop) == LV_RESULT_OK);
        TEST_ASSERT_FALSE(lv_obj_get_property(obj, properties[i].id).num);
        TEST_ASSERT_FALSE(lv_obj_has_flag(obj, properties[i].flag));
    }
}
#endif

#if 0
void img_in_ram_test_rotate_recolor(void)
{
  // lv_obj_set_flex_flow(lv_screen_active(), LV_FLEX_FLOW_ROW_WRAP);
  // lv_obj_set_flex_align(lv_screen_active(), LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY);

  // lv_obj_clean(lv_screen_active());

  img_create("I4", &test_image_cogwheel_i4, true, true);
  img_create("A8", &test_image_cogwheel_a8, true, true);
  img_create("RGB565", &test_image_cogwheel_rgb565, true, true);
  img_create("RGB565A8", &test_image_cogwheel_rgb565a8, true, true);
  img_create("XRGB8888", &test_image_cogwheel_xrgb8888, true, true);
  img_create("ARGB8888", &test_image_cogwheel_argb8888, true, true);
}

void img_in_ram_test_recolor(void)
{
  // lv_obj_set_flex_flow(lv_screen_active(), LV_FLEX_FLOW_ROW_WRAP);
  // lv_obj_set_flex_align(lv_screen_active(), LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY);

  // lv_obj_clean(lv_screen_active());

  img_create("I4", &test_image_cogwheel_i4, false, true);
  img_create("A8", &test_image_cogwheel_a8, false, true);
  img_create("RGB565", &test_image_cogwheel_rgb565, false, true);
  img_create("RGB565A8", &test_image_cogwheel_rgb565a8, false, true);
  img_create("XRGB8888", &test_image_cogwheel_xrgb8888, false, true);
  img_create("ARGB8888", &test_image_cogwheel_argb8888, false, true);
}

void img_in_ram_test_rotate(void)
{
  // lv_obj_set_flex_flow(lv_screen_active(), LV_FLEX_FLOW_ROW_WRAP);
  // lv_obj_set_flex_align(lv_screen_active(), LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY);

  // lv_obj_clean(lv_screen_active());

  img_create("I4", &test_image_cogwheel_i4, true, false);
  img_create("A8", &test_image_cogwheel_a8, true, false);
  img_create("RGB565", &test_image_cogwheel_rgb565, true, false);
  img_create("RGB565A8", &test_image_cogwheel_rgb565a8, true, false);
  img_create("XRGB8888", &test_image_cogwheel_xrgb8888, true, false);
  img_create("ARGB8888", &test_image_cogwheel_argb8888, true, false);
}
#endif


void test_barcode_normal(void)
{
    lv_obj_t * barcode = lv_barcode_create(lv_scr_act());
    lv_obj_center(barcode);

    lv_color_t dark_color = lv_color_black();
    lv_color_t light_color = lv_color_white();
    uint16_t scale = 2;

    lv_barcode_set_dark_color(barcode, dark_color);
    lv_barcode_set_light_color(barcode, light_color);
    lv_barcode_set_scale(barcode, scale);

    lv_result_t res;
    lv_image_dsc_t * image_dsc;
    lv_barcode_set_direction(barcode, LV_DIR_HOR);
    res = lv_barcode_update(barcode, "https://lvgl.io");
    image_dsc = lv_canvas_get_image(barcode);

    lv_obj_set_size(barcode, image_dsc->header.w, 50);

    lv_barcode_set_direction(barcode, LV_DIR_VER);
    res = lv_barcode_update(barcode, "https://lvgl.io");

    image_dsc = lv_canvas_get_image(barcode);

    lv_obj_set_size(barcode, 50, image_dsc->header.h);

}

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*To hide the memory and performance indicators in the corners
   *disable `LV_USE_MEM_MONITOR` and `LV_USE_PERF_MONITOR` in `lv_conf.h`*/
  hal_init(800, 480);

  // test_property();
  // obj_property_example();

  LV_IMAGE_DECLARE(cogwheel);

  // img_create("avatar", "/home/neo/projects/lvgl/lv_port_pc_eclipse/output/cogwheel.RGB888.bin", true, false);
  // img_create("avatar", &cogwheel, true, true);
  // test_barcode_normal();
  // lv_example_barcode_1();

  // uint8_t * mm = lv_malloc(16);
  // mm[128] = 0x12;
  // create_pic_preview();

  // lv_obj_t* label = lv_label_create(lv_screen_active());
  // lv_obj_set_style_text_font(label, &lv_font_montserrat_48, 0);
  // lv_label_set_text(label, "HELLO\u200E\u202A WORLD!\u202C\u200E");
  // lv_obj_center(label);

  // lv_obj_set_flex_flow(lv_screen_active(), LV_FLEX_FLOW_ROW_WRAP);
  // lv_obj_set_flex_align(lv_screen_active(), LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY);

  // lv_obj_clean(lv_screen_active());
  // img_in_ram_test_rotate();
  // img_in_ram_test_recolor();
  // img_in_ram_test_rotate_recolor();

  // lv_obj_t * img1 = lv_image_create(lv_scr_act());
  // lv_image_set_src(img1,  "/home/neo/projects/lvgl/lv_port_pc_eclipse/lvgl/tests/src/test_files/binimages/cogwheel.RGB565A8.bin");
  // lv_image_set_rotation(img1, 450);
  // lv_demo_widgets();
  // lv_example_image_1();
  // lv_obj_dump_tree(NULL);
//   lv_obj_t * img = lv_image_create(lv_scr_act());
//   lv_image_set_src(img, "/home/neo/projects/lvgl/lv_port_pc_eclipse/lvgl/tests/src/test_files/binimages/cogwheel.RGB565A8.bin");
  // lv_image_set_src(img, &avatar);

  // lv_obj_set_style_image_recolor_opa(img, LV_OPA_50, 0);
  // lv_obj_set_style_image_recolor(img, lv_color_hex(0xff0000), 0);

  // lv_obj_center(img);

  // lv_obj_set_pos(img, 0, 0);

    // lv_obj_t* parent = lv_obj_create(lv_scr_act());
    // lv_obj_t* child = lv_btn_create(parent);
    // lv_obj_set_pos(parent, 50, 50);
    // lv_obj_set_size(parent, 300, 250);

    // lv_obj_set_pos(child, 0, 0);
    // lv_obj_set_size(child, 1000, 1000);
  // lv_demo_widgets();
  // lv_obj_set_style_opa(lv_screen_active(), LV_OPA_50, 0);
  // lv_demo_render(LV_DEMO_RENDER_SCENE_DECODE_SPEED, LV_OPA_80);

  // lv_snapshot_take(lv_scr_act(), LV_COLOR_FORMAT_ARGB8888);
  while(1) {
      /* Periodically call the lv_task handler.
       * It could be done in a timer interrupt or an OS task too.*/
      lv_timer_handler();
      static int count;
      if (count++ == 1) {
        // lv_snapshot_take(lv_scr_act(), LV_COLOR_FORMAT_ARGB8888);
      }
      usleep(1 * 1000);
  }

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{

//    lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(w, h);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_disp(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  // lv_obj_t * cursor_obj;
  // cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
  // lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  // lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_disp(mousewheel, disp);

  lv_indev_t * keyboard = lv_sdl_keyboard_create();
  lv_indev_set_disp(keyboard, disp);
  lv_indev_set_group(keyboard, lv_group_get_default());

  return disp;
}
