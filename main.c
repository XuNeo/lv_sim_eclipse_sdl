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
#include <math.h>

/*********************
 *      DEFINES
 *********************/
#ifndef FLT_EPSILON
#define FLT_EPSILON 0x1.0p-23f
#endif

#define MATH_PI  3.14159265358979323846f
#define MATH_HALF_PI 1.57079632679489661923f

#define DEG_TO_RAD 0.017453292519943295769236907684886f
#define RAD_TO_DEG 57.295779513082320876798154814105f

#define MATH_RADIANS(deg) ((deg) * DEG_TO_RAD)
#define MATH_DEGRESS(rad) ((rad) * RAD_TO_DEG)

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
        "/home/neo/projects/lvgl/lv_port_pc_eclipse/output/rle_compressed/",
    };

    char path[PATH_MAX];

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
            // lv_image_set_rotation(img, 40);
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
    // if(rotate) lv_image_set_angle(img, 450);

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

    // LV_OBJ_SET_PROPERTY_ARRAY(obj, info->properties);
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
static void event_cb(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * label = lv_event_get_user_data(e);

  switch(code) {
      case LV_EVENT_PRESSED:
          lv_label_set_text(label, "The last button event:\nLV_EVENT_PRESSED");
          LV_LOG_USER("PRESSED");
          break;
      case LV_EVENT_CLICKED:
          lv_label_set_text(label, "The last button event:\nLV_EVENT_CLICKED");
          LV_LOG_USER("LV_EVENT_CLICKED");
          break;
      case LV_EVENT_LONG_PRESSED:
          lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED");
          LV_LOG_USER("LV_EVENT_LONG_PRESSED");
          break;
      case LV_EVENT_LONG_PRESSED_REPEAT:
          lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED_REPEAT");
          LV_LOG_USER("LV_EVENT_LONG_PRESSED_REPEAT");
          break;
      default:
          break;
  }
}

void event_test(void)
{
  lv_obj_t * btn = lv_button_create(lv_screen_active());
  lv_obj_set_size(btn, 100, 50);
  lv_obj_center(btn);

  lv_obj_t * btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Click me!");
  lv_obj_center(btn_label);

  lv_obj_t * info_label = lv_label_create(lv_screen_active());
  lv_label_set_text(info_label, "The last button event:\nNone");

  lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, info_label);
}

void lv_image_test(void){
   int ww = 200;
   lv_obj_t* native = lv_obj_create(lv_scr_act());
   lv_obj_remove_style_all(native);
   lv_obj_set_width(native,ww);
   lv_obj_set_height(native,ww);
   lv_obj_set_pos(native, 0, 0);

   lv_obj_t* img_ = lv_img_create(native);
   lv_obj_remove_style_all(img_);
   lv_obj_set_pos(img_, 0, 0);
   lv_img_set_src(img_, "/home/neo/projects/lvgl/lv_port_pc_eclipse/00.png");
   lv_obj_set_width(img_,ww);
   lv_obj_set_height(img_,ww);

   lv_obj_set_style_bg_color(img_, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
   lv_obj_set_style_bg_opa(img_, LV_OPA_80, LV_PART_MAIN);

  // lv_img_set_pivot(img_, 0, 0);
   lv_image_set_scale(img_, 512);
}


static lv_obj_t * create_panel(int32_t radius, bool transform)
{
    lv_obj_t * parent = lv_obj_create(lv_screen_active());
    lv_obj_set_style_pad_all(parent, 3, 0);
    lv_obj_set_style_radius(parent, radius, 0);
    lv_obj_set_style_clip_corner(parent, true, 0);
    if(transform) lv_obj_set_style_transform_rotation(parent, 300, 0);

    lv_obj_t * label = lv_label_create(parent);
    lv_obj_set_width(label, lv_pct(200));
    lv_label_set_text(label,
                      "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus dignissim quam id eros iaculis dapibus. Mauris nisl orci, vulputate sed eleifend a, consectetur et nulla.");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_bg_color(label, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_bg_opa(label, LV_OPA_20, 0);

    lv_obj_update_layout(parent);
    lv_obj_scroll_by(parent, -15, -15, LV_ANIM_OFF);

    return parent;
}

void test_func_1(void)
{
    lv_obj_set_flex_flow(lv_screen_active(), LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(lv_screen_active(), LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_obj_set_style_pad_column(lv_screen_active(), 40, 0);

    create_panel(0, false);
    create_panel(10, false);
    create_panel(30, false);
    create_panel(100, false);

    lv_obj_t * cont = create_panel(0, true);
    lv_obj_add_flag(cont, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    create_panel(10, true);
    create_panel(30, true);
    create_panel(100, true);
}

void test_image_normal_align(void)
{
    lv_obj_t * img;
    uint32_t i;
    lv_image_align_t aligns[] = {
        LV_IMAGE_ALIGN_TOP_LEFT, LV_IMAGE_ALIGN_TOP_MID, LV_IMAGE_ALIGN_TOP_RIGHT,
        LV_IMAGE_ALIGN_LEFT_MID, LV_IMAGE_ALIGN_CENTER, LV_IMAGE_ALIGN_RIGHT_MID,
        LV_IMAGE_ALIGN_BOTTOM_LEFT, LV_IMAGE_ALIGN_BOTTOM_MID, LV_IMAGE_ALIGN_BOTTOM_RIGHT,
    };

    for(i = 0; i < 9; i++) {
        lv_obj_t * img = lv_image_create(lv_screen_active());
        lv_image_set_src(img, "A:lvgl/tests/src/test_assets/test_img_lvgl_logo.png");
        lv_obj_set_style_bg_opa(img, LV_OPA_20, 0);
        lv_obj_set_style_shadow_width(img, 10, 0);
        lv_obj_set_style_shadow_color(img, lv_color_hex(0xff0000), 0);

        lv_obj_set_size(img, 200, 120);
        lv_obj_set_pos(img, 30 + (i % 3) * 260, 40 + (i / 3) * 150);
        lv_image_set_align(img, aligns[i]);
    }

}


// void test_draw_buf_stride_adjust(void)
// {
//     const char * img_src = "A:lvgl/tests/test_images/stride_align64/UNCOMPRESSED/test_ARGB8888.bin";
//     lv_obj_t * img = lv_image_create(lv_screen_active());
//     lv_obj_center(img);
//     lv_image_set_src(img, img_src);

//     lv_image_decoder_args_t args = {
//         .no_cache = false,
//         .premultiply = false,
//         .stride_align = false,
//         .use_indexed = false,
//     };

//     lv_image_decoder_dsc_t decoder_dsc;
//     lv_result_t res = lv_image_decoder_open(&decoder_dsc, img_src, &args);

//     const lv_image_header_t * header = &decoder_dsc.decoded->header;

//     lv_draw_buf_t * dup = lv_draw_buf_dup(decoder_dsc.decoded);

//     /* Shrink stride to below minimal stride(by -1 in code below) should fail */
//     res = lv_draw_buf_adjust_stride(dup, header->w * 4 - 1);

//     /*The image should still look same*/
//     lv_image_set_src(img, dup);

//     /* Shrink stride to minimal stride should be OK */
//     res = lv_draw_buf_adjust_stride(dup, header->w * 4);

//     /*The image should still look same*/
//     lv_image_set_src(img, dup);

//     /* Expand the stride should work*/
//     res = lv_draw_buf_adjust_stride(dup, (header->stride + header->w * 4) / 2);
//     lv_image_set_src(img, dup);

//     lv_image_decoder_close(&decoder_dsc);
// }

void test_image_zoom(void)
{
  lv_obj_t * root = lv_obj_create(lv_scr_act());
  lv_obj_center(root);
  lv_obj_set_style_pad_all(root, 0, 0);
  lv_obj_set_size(root, 200, 200);
  lv_obj_set_style_outline_width(root, 1, 0);

  lv_obj_t * img = lv_image_create(root);
  lv_obj_set_style_outline_width(img, 1, 0);
  const char* img_path = "A:lvgl/tests/test_images/stride_align64/UNCOMPRESSED/test_ARGB8888.bin";
  lv_image_set_src(img, img_path);
  lv_image_header_t header;
  lv_image_decoder_get_info(img_path, &header);

  uint32_t zoom = 128;
  // lv_obj_set_pos(img, 0, 0);
  lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 100);
  // lv_image_set_pivot(img, 0, 0);
  // lv_obj_set_size(img, header.w / 2, header.h / 2);
  lv_image_set_scale(img, zoom);
}


void draw_buf_stride_adjust_test(void)
{
    const char * img_src = "A:lvgl/tests/test_images/stride_align1/UNCOMPRESSED/test_I8.bin";
    lv_obj_t * img = lv_image_create(lv_screen_active());
    lv_obj_center(img);
    lv_image_set_src(img, img_src);

    lv_image_decoder_args_t args = {
        .no_cache = true,
        .premultiply = false,
        .stride_align = false,
        .use_indexed = true,
    };

    lv_image_decoder_dsc_t decoder_dsc;
    lv_result_t res = lv_image_decoder_open(&decoder_dsc, img_src, &args);
    lv_draw_buf_t * dup = lv_draw_buf_dup(decoder_dsc.decoded);
    lv_image_decoder_close(&decoder_dsc);

    lv_draw_buf_t * adjusted = lv_draw_buf_adjust_stride(dup, 80 + 64);

    /*The image should still look same*/
    img = lv_image_create(lv_screen_active());
    lv_obj_set_style_outline_width(img, 1, 0);
    lv_obj_set_style_outline_color(img, lv_color_black(), 0);

    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 0);
    lv_image_set_src(img, adjusted);

}

#if 0
void test_snapshot(void)
{
    lv_obj_t* root = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_color(root, lv_color_hex(0x00), 0);
    lv_obj_set_style_bg_opa(root, LV_OPA_COVER, 0);

    lv_obj_set_size(root, 150, 100);
    lv_obj_t* label = lv_obj_create(root);
    lv_obj_set_style_bg_opa(label, LV_OPA_50, 0);
    lv_obj_set_style_bg_color(label, lv_color_hex(0xff0000), 0);
    lv_obj_set_style_border_width(label, 0, 0);
    lv_obj_set_size(label, 100, 50);
    lv_obj_center(label);
    lv_obj_align(root, LV_ALIGN_TOP_MID, 0, 100);

    lv_obj_t* root2 = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_color(root2, lv_color_hex(0x00), 0);
    lv_obj_set_style_bg_opa(root2, LV_OPA_COVER, 0);
    lv_obj_align(root2, LV_ALIGN_BOTTOM_MID, 0, -100);

    lv_draw_buf_t* draw_buf = lv_snapshot_take(label, LV_COLOR_FORMAT_ARGB8888);
    lv_draw_buf_save_file(draw_buf, "A:test.bin");
    lv_obj_t* image = lv_image_create(root2);
    lv_image_set_src(image, draw_buf);
    lv_obj_center(image);
    lv_obj_align(root2, LV_ALIGN_BOTTOM_MID, 0, -100);
}
#endif

void test_image_bar(void)
{

}

void draw_arc(lv_layer_t * layer,  const lv_area_t * coords, const char* img_src, float center_x, float center_y, float start_angle, float end_angle, float radius, float width, bool rounded)
{
    if(width <= 0)
        return;
    if(start_angle == end_angle)
        return;

    lv_area_t clip_area;
    if(!_lv_area_intersect(&clip_area, coords,  &layer->_clip_area)) {
        /*Fully clipped, nothing to do*/
        return;
    }

    float sweep_angle = end_angle - start_angle;

    while(sweep_angle < 0) {
        sweep_angle += 360;
    }

    while(sweep_angle > 360) {
        sweep_angle -= 360;
    }

    /*If the angles are the same then there is nothing to draw*/
    if(fabsf(sweep_angle)  < FLT_EPSILON) {
        return;
    }

    lv_vector_dsc_t * ctx = lv_vector_dsc_create(layer);
    lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);

    float radius_out = radius;
    float radius_in = radius - width;
    float half_width = width * 0.5f;
    float radius_center = radius_out - half_width;
    float cx = center_x;
    float cy = center_y;

    if(fabs(sweep_angle - 360) < FLT_EPSILON) {
        lv_vector_path_append_circle(path, &(lv_fpoint_t){cx, cy}, radius_out, radius_out);
        lv_vector_path_append_circle(path, &(lv_fpoint_t){cx, cy}, radius_in, radius_in);
    }
    else {
        /* radius_out start point */
        float start_angle_rad = MATH_RADIANS(start_angle);
        float start_x = radius_out * cosf(start_angle_rad) + cx;
        float start_y = radius_out * sinf(start_angle_rad) + cy;

        /* radius_in start point */
        float end_angle_rad = MATH_RADIANS(end_angle);
        float end_x = radius_in * cosf(end_angle_rad) + cx;
        float end_y = radius_in * sinf(end_angle_rad) + cy;

        /* radius_out arc */
        lv_vector_path_append_arc(path,
                                  &(lv_fpoint_t){cx, cy},
                                  radius_out,
                                  start_angle,
                                  sweep_angle,
                                  false);

        /* line to radius_in */
        lv_vector_path_line_to(path, &(lv_fpoint_t){end_x, end_y});

        /* radius_in arc */
        lv_vector_path_append_arc(path,
                                  &(lv_fpoint_t){cx, cy},
                                  radius_in,
                                  end_angle,
                                  -sweep_angle,
                                  false);

        /* close arc */
        lv_vector_path_line_to(path, &(lv_fpoint_t){start_x, start_y});
        lv_vector_path_close(path);
        /* draw round */
        if(rounded && half_width > 0) {
            float rcx1 = cx + radius_center * cosf(end_angle_rad);
            float rcy1 = cy + radius_center * sinf(end_angle_rad);
            lv_vector_path_append_circle(path, &(lv_fpoint_t){rcx1, rcy1}, half_width, half_width);

            float rcx2 = cx + radius_center * cosf(start_angle_rad);
            float rcy2 = cy + radius_center * sinf(start_angle_rad);
            lv_vector_path_append_circle(path, &(lv_fpoint_t){rcx2, rcy2}, half_width, half_width);
        }
    }

    lv_vector_dsc_translate(ctx, coords->x1, coords->y1);
    lv_draw_image_dsc_t draw_img_dsc;
    lv_draw_image_dsc_init(&draw_img_dsc);
    draw_img_dsc.src = img_src;
    lv_vector_dsc_set_fill_image(ctx, &draw_img_dsc);
    lv_area_t bound;
    lv_vector_path_get_bounding(path, &bound);
    lv_matrix_t m;
    lv_matrix_identity(&m);
    lv_matrix_translate(&m, -bound.x1, -bound.y1);
    lv_vector_dsc_set_fill_transform(ctx, &m);
    lv_vector_dsc_set_fill_rule(ctx, LV_VECTOR_FILL_EVENODD);
    lv_vector_dsc_add_path(ctx, path);

    lv_vector_path_clear(path);
    lv_vector_path_append_rect(path, coords, 0, 0);
    lv_vector_dsc_set_fill_color(ctx, lv_color_hex(0xff0000));
    lv_vector_dsc_set_fill_opa(ctx, LV_OPA_50);
    lv_vector_dsc_add_path(ctx, path);

    lv_draw_vector(ctx); /* submit draw */

    lv_vector_path_delete(path);
    lv_vector_dsc_delete(ctx);
}

static void timer_cb(lv_timer_t * timer)
{
  lv_obj_t * canvas = timer->user_data;
    lv_layer_t layer;
    lv_canvas_fill_bg(canvas, lv_color_hex(0xffffff), LV_OPA_COVER);
    lv_canvas_init_layer(canvas, &layer);

    static int32_t y = 150;
    lv_area_t coords = {
      .x1 = 20,
      .y1 = 20,
      .x2 = y,
      .y2 = y,
    };
     y += 1;
     if (y > 479) y = 1;

    layer._clip_area = coords;
    draw_arc(&layer, &coords, "A:testimg.png", 240, 240, 0, 360, 100, 50, false);
    lv_canvas_finish_layer(canvas, &layer);
}

void test_draw_arc(void)
{
    // lv_obj_t * parent = lv_obj_create(lv_scr_act());
    // lv_obj_center(parent);
    // lv_obj_set_size(parent, 100, 100);
    lv_draw_buf_t * draw_buf = lv_draw_buf_create(480, 480, LV_COLOR_FORMAT_ARGB8888, LV_STRIDE_AUTO);
    lv_obj_t * canvas = lv_canvas_create(lv_scr_act());
    lv_canvas_set_draw_buf(canvas, draw_buf);
    lv_obj_center(canvas);
    lv_obj_set_style_outline_width(canvas, 1, 0);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_area_t coords = {
      .x1 = 20,
      .y1 = 20,
      .x2 = 479,
      .y2 = 479,
    };

    draw_arc(&layer, &coords, "A:testimg.png", 240, 240, 0, 360, 100, 50, false);

    lv_canvas_finish_layer(canvas, &layer);

    lv_timer_create(timer_cb, 1, canvas);
}

#include "lvx_imgbar.h"

void test_imgbar(void)
{
  lv_obj_t * bar = lvx_imgbar_create(lv_scr_act());
  lv_obj_center(bar);
  lv_obj_set_pos(bar, 0, 0);

  lvx_imgbar_set_src(bar, "A:testimg.png");
  lvx_imgbar_set_angle(bar, 45, 270);
  lvx_imgbar_set_center(bar, 240, 240);
  lvx_imgbar_set_radius(bar, 230);
  lvx_imgbar_set_bar_width(bar, 50);

  lv_obj_set_style_pad_all(bar, 0, 0);
  lv_obj_set_style_outline_width(bar, 1, 0);
  lv_obj_set_style_outline_color(bar, lv_color_black(), 0);
  lv_obj_set_style_outline_opa(bar, LV_OPA_COVER, 0);
}

void test_arc(void)
{
  lv_obj_t * obj = lv_obj_create(lv_scr_act());
  lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0000), 0);
  lv_obj_set_size(obj, 50, 50);
  lv_obj_center(obj);

  lv_obj_t * bar = lv_arc_create(lv_scr_act());
  lv_obj_center(bar);
  lv_obj_set_style_arc_image_src(bar, "A:testimg.png", LV_PART_INDICATOR);
  lv_obj_set_size(bar, 466, 466);
  lv_obj_set_style_line_width(bar, 20, 0);
  lv_obj_set_style_arc_width(bar, 50, LV_PART_INDICATOR);
  lv_arc_set_start_angle(bar, 45);
  lv_arc_set_end_angle(bar, 270);
  lv_arc_set_value(bar, 50);
}

#include "lvx_img_line_bar.h"
void test_img_line_bar(void)
{
  lv_obj_t * bar = lvx_img_line_bar_create(lv_scr_act());
  lv_obj_center(bar);
  lvx_imgbar_set_src(bar, "A:testimg.png");
  lv_obj_set_style_outline_width(bar, 1, 0);
  lvx_img_line_bar_set_para(bar, (lv_point_precise_t){
    50, 50,
  },
  (lv_point_precise_t){
    100,100,
  }, 50, 1
  );
}

void test_arc_issue(void)
{
  lv_obj_t *arc = lv_arc_create(lv_scr_act());
  // lv_obj_set_style_bg_opa(arc, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_pad_all(arc, 4, LV_PART_MAIN);

  lv_obj_set_style_arc_image_src(arc,"A:lvgl/tests/test_images/stride_align1/UNCOMPRESSED/test_RGB565A8.bin", LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc, 16, LV_PART_MAIN);

  // lv_obj_set_style_arc_image_src(arc, "A:lvgl/tests/test_images/stride_align1/UNCOMPRESSED/test_RGB565A8.bin", LV_PART_INDICATOR);
  // lv_obj_set_style_arc_width(arc, 16, LV_PART_INDICATOR);

  // lv_obj_set_style_arc_color(arc, theme->home.out.text2, LV_PART_INDICATOR);
  // lv_obj_set_style_arc_opa(arc, LV_OPA_COVER, LV_PART_INDICATOR);
  lv_obj_set_size(arc, 62 + 6, 62 + 6);
  lv_arc_set_rotation(arc, 180);
  lv_arc_set_bg_angles(arc, 0, 180);
  lv_arc_set_range(arc, 0, 1000);
  lv_arc_set_value(arc, 00);
}

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the display, and the input devices*/

  /*Open a demo or an example*/
  // if (argc == 0) {
  //   lv_demo_widgets();
  //   //  lv_example_chart_1();
  // } else {
  //   if (!lv_demos_create(&argv[1], argc - 1)) {
  //     lv_demos_show_help();
  //     goto demo_end;
  //   }
  // }

  /*To hide the memory and performance indicators in the corners
   *disable `LV_USE_MEM_MONITOR` and `LV_USE_PERF_MONITOR` in `lv_conf.h`*/
  hal_init(800, 600);

  // test_property();
  // obj_property_example();

  LV_IMAGE_DECLARE(cogwheel);
  LV_IMAGE_DECLARE(cogwheel_i8_stride112_RLE);
  LV_IMAGE_DECLARE(cogwheel_rgb888_stride304_RLE);
  LV_IMAGE_DECLARE(cogwheel_xrgb8888_RLE);
  LV_IMAGE_DECLARE(cogwheel_rgb565a8_stride208_RLE);
  LV_IMAGE_DECLARE(cogwheel_a4_stride64_RLE);
  LV_IMAGE_DECLARE(cogwheel_i8_stride112);
  LV_IMAGE_DECLARE(cogwheel_rgb565_stride208);
  LV_IMAGE_DECLARE(cogwheel_I1);
  LV_IMAGE_DECLARE(cogwheel_ARGB8888);

  // lv_demo_brightness();
  // test_arc_issue();
  // lv_example_slider_1();
  // lv_example_switch_1();
  // test_arc();
  // test_imgbar();
  // test_img_line_bar();
  // test_draw_arc();
  // test_snapshot();
  // lv_demo_vector_graphic();
  // draw_buf_stride_adjust_test();
  // lv_example_calendar_1();
  // test_image_zoom();
  // test_barcode_normal();
  // test_draw_buf_stride_adjust();
  // lv_demo_widgets();
  // test_image_normal_align();
  // lv_example_snapshot_1();
  // lv_example_ffmpeg_2();
  // test_func_1();
  img_create("avatar", "/home/neo/projects/lvgl/lv_port_pc_eclipse/output/cogwheel.ARGB8565.bin", false, false);
  // img_create("avatar", "A:lvgl/tests/test_images/stride_align1/RLE/test_ARGB8888.bin", false, false);
  // lv_demo_render(LV_DEMO_RENDER_SCENE_DECODE_SPEED, 0);
  // img_create("avatar", "/home/neo/projects/lvgl/lv_port_pc_eclipse/mouse_cursor_icon.png", false, false);
  // img_create("avatar", "/home/neo/projects/lvgl/lv_port_pc_eclipse/flower.jpg", false, false);
  // img_create("avatar", &cogwheel_I1, false, false);
  // test_barcode_normal();
  // lv_image_test();
  // lv_example_canvas_3();
  // event_test();
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
    //  lv_demo_music();
    //  lv_demo_widgets();
  // lv_example_image_1();
  // lv_example_spinner_1();
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
  // lv_demo_render(LV_DEMO_RENDER_SCENE_ARC_IMAGE, LV_OPA_80);
  // lv_example_qrcode_1();
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

// demo_end:
  lv_deinit();
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

  LV_IMAGE_DECLARE(cogwheel_I1);
  LV_IMAGE_DECLARE(cogwheel_ARGB8888);
  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  // lv_obj_set_style_outline_color(cursor_obj, lv_color_black(), 0);
  // lv_obj_set_style_outline_width(cursor_obj, 1, 0);
  lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_disp(mousewheel, disp);

  lv_indev_t * keyboard = lv_sdl_keyboard_create();
  lv_indev_set_disp(keyboard, disp);
  lv_indev_set_group(keyboard, lv_group_get_default());

  return disp;
}
