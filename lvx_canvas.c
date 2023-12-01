/**************************************************************
 *
 * Copyright(c) 2022, Beijing Xiaomi Mobile Software Co., Ltd.
 * All Rights Reserved.
 *
 *************************************************************/

/*********************
 *      INCLUDES
 *********************/
#include "lvx_canvas.h"
#include "lvgl/src/draw/sw/lv_draw_sw.h"

#ifdef CONFIG_LV_USE_GPU_INTERFACE
#include "lv_porting/gpu/lv_gpu_draw_utils.h"
#endif

#if 1

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lvx_canvas_class
#include <stdio.h>
#define zoomerror(format, ...) printf(format, ##__VA_ARGS__)
#define debug(format, ...) printf(format, ##__VA_ARGS__)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lvx_canvas_constructor(const lv_obj_class_t *class_p,
                                   lv_obj_t *obj);
static void lvx_canvas_destructor(const lv_obj_class_t *class_p, lv_obj_t *obj);
static void lvx_canvas_event(const lv_obj_class_t *class_p, lv_event_t *e);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t lvx_canvas_class = {
    .constructor_cb = lvx_canvas_constructor,
    .destructor_cb = lvx_canvas_destructor,
    .event_cb = lvx_canvas_event,
    .instance_size = sizeof(lvx_canvas_t),
    .base_class = &lv_canvas_class,
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t *lvx_canvas_create(lv_obj_t *parent) {
  lv_obj_t *obj = lv_obj_class_create_obj(MY_CLASS, parent);
  lv_obj_class_init_obj(obj);
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
  return obj;
}

static void clear_cb(struct _lv_disp_drv_t *disp_drv, uint8_t *buf,
                     uint32_t size) {
  /* do nothing (?) */
}

static void flush_cb(struct _lv_disp_drv_t *disp_drv, const lv_area_t *area,
                     lv_color_t *color_p) {
  lv_obj_t *obj = disp_drv->user_data;
  lvx_canvas_t *canvas = disp_drv->user_data;
  lv_disp_t *vdisp = canvas->vdisp;

  /* if not transformed, copy dirty area */
  if (lv_img_get_zoom(obj) == LV_IMG_ZOOM_NONE &&
      lv_obj_get_style_transform_zoom(obj, 0) == LV_IMG_ZOOM_NONE) {

    lv_point_t origin = {obj->coords.x1, obj->coords.y1};
    origin.x += lv_obj_get_style_border_width(obj, LV_PART_MAIN);
    origin.x += lv_obj_get_style_pad_left(obj, LV_PART_MAIN);
    origin.y += lv_obj_get_style_border_width(obj, LV_PART_MAIN);
    origin.y += lv_obj_get_style_pad_top(obj, LV_PART_MAIN);

    for (int i = 0; i < vdisp->inv_p; i++) {
      if (vdisp->inv_area_joined[i] == 0) {
        lv_area_t _area = vdisp->inv_areas[i];
        lv_area_move(&_area, origin.x, origin.y);
        lv_obj_invalidate_area(obj, &_area);
      }
    }
  } else {
    lv_obj_invalidate(obj);
  }

  lv_disp_flush_ready(disp_drv);
}

void lvx_canvas_set_disp(lv_obj_t *obj, lv_coord_t w, lv_coord_t h) {
  lvx_canvas_t *canvas = (lvx_canvas_t *)obj;
  if (canvas->vdisp) {
    /* cannot change disp para on the fly for now. */
    return;
  }

  /* init virtual display */
#if 0
  lv_disp_t *phydisp = lv_obj_get_disp(obj);
#endif

  lv_disp_drv_t *driver = &canvas->disp_drv;
  lv_disp_drv_init(driver);

  driver->user_data = obj;
  driver->hor_res = w;
  driver->ver_res = h;
  driver->flush_cb = flush_cb;
  driver->clear_cb = clear_cb;
  driver->screen_transp = false;
  driver->direct_mode = 1;
  driver->full_refresh = 0; /* no need for full refresh */


  canvas->img = lv_img_buf_alloc(w, h, LV_IMG_CF_TRUE_COLOR);
  void *buf1 = (void*)canvas->img->data;
  lv_disp_draw_buf_init(&canvas->draw_buf, buf1, NULL, w * h);
  driver->draw_buf = &canvas->draw_buf;
  driver->draw_ctx_init = lv_draw_sw_init_ctx;
  driver->draw_ctx_size = sizeof(lv_draw_sw_ctx_t);

  lv_disp_t *vdisp = lv_disp_drv_register(driver);
  canvas->vdisp = vdisp;

  lv_canvas_set_buffer(obj, (void *)canvas->img->data, w, h,
                       LV_IMG_CF_TRUE_COLOR);

#if 0 /* can do further optimize on refresh timer */
    lv_timer_del(vdisp->refr_timer);
    vdisp->refr_timer = NULL;

    /* add our own refresh facility */
    lv_obj_add_event_cb(obj, NULL, LV_EVENT_COVER_CHECK, NULL);
#endif
}

const void *lvx_canvas_get_img(lv_obj_t *obj) {
  lvx_canvas_t *canvas = (lvx_canvas_t *)obj;
  return &canvas->img;
}

lv_disp_t *lvx_canvas_get_disp(lv_obj_t *obj) {
  lvx_canvas_t *canvas = (lvx_canvas_t *)obj;
  return canvas->vdisp;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lvx_canvas_constructor(const lv_obj_class_t *class_p,
                                   lv_obj_t *obj) {
  LV_UNUSED(class_p);
  LV_TRACE_OBJ_CREATE("begin");

  lvx_canvas_t *canvas = (lvx_canvas_t *)obj;
  lv_memset_00(&canvas->img, sizeof(lv_img_dsc_t));
  canvas->hit_obj = NULL;
  LV_TRACE_OBJ_CREATE("finished");
}

static void lvx_canvas_destructor(const lv_obj_class_t *class_p,
                                  lv_obj_t *obj) {
  LV_UNUSED(class_p);
  lvx_canvas_t *canvas = (lvx_canvas_t *)obj;
  if (canvas->vdisp) {
    lv_disp_remove(canvas->vdisp);
    if (canvas->img) {
      lv_img_buf_free(canvas->img);
    }
  }
}

static void lvx_canvas_event(const lv_obj_class_t *class_p, lv_event_t *e) {
  LV_UNUSED(class_p);

  lv_point_t point;

  lv_obj_t *obj = lv_event_get_current_target(e);
  lvx_canvas_t *canvas = (lvx_canvas_t *)obj;
  lv_event_code_t code = lv_event_get_code(e);
  bool call_event_base = true;

  switch (code) {
  /* event pass through to obj in virtual display */
  case LV_EVENT_PRESSED:
  case LV_EVENT_PRESSING:
  case LV_EVENT_PRESS_LOST:
  case LV_EVENT_SHORT_CLICKED:
  case LV_EVENT_LONG_PRESSED:
  case LV_EVENT_LONG_PRESSED_REPEAT:
  case LV_EVENT_CLICKED:
  case LV_EVENT_RELEASED: {
    lv_indev_t *indev = lv_indev_get_act();
    if (indev->driver->type != LV_INDEV_TYPE_POINTER) {
      debug("indev is not pointer.\n");
      break;
    }

    /* Map the point to zoomed space. */
    /* @todo no rotation support */
    uint32_t scale = lv_img_get_zoom(obj);
    scale *= lv_obj_get_style_transform_zoom(obj, 0);
    scale >>= 8;

    lv_point_t pivot;
    lv_img_get_pivot(obj, &pivot);

    lv_point_t origin = {obj->coords.x1, obj->coords.y1};
    origin.x += lv_obj_get_style_border_width(obj, LV_PART_MAIN);
    origin.x += lv_obj_get_style_pad_left(obj, LV_PART_MAIN);

    origin.y += lv_obj_get_style_border_width(obj, LV_PART_MAIN);
    origin.y += lv_obj_get_style_pad_top(obj, LV_PART_MAIN);

    pivot.x += origin.x;
    pivot.y += origin.y;
    lv_point_transform(&origin, 0, scale, &pivot);

    lv_indev_get_point(indev, &point);

    /* Convert to relative position. */
    point.x -= origin.x;
    point.y -= origin.y;
    point.x = ((uint32_t)point.x << 8) / scale;
    point.y = ((uint32_t)point.y << 8) / scale;

    lv_obj_t *scr = lv_disp_get_scr_act(canvas->vdisp);
    if (code == LV_EVENT_CLICKED) {
      debug("Clicked event. hit: %p\n", canvas->hit_obj);
    }

    if (code == LV_EVENT_PRESSED) {
      /* Update hit_obj */
      lv_obj_t *hit_obj;

      /* Find the one hit. */
      hit_obj = lv_indev_search_obj(scr, &point);
      if (hit_obj == NULL || hit_obj == scr) {
        debug("No hit\n");
        break;
      }

      if (!lv_obj_has_flag(hit_obj, LV_OBJ_FLAG_CLICKABLE)) {
        debug("Not clickable\n");
        break;
      }
      canvas->hit_obj = hit_obj;
      debug("hit obj: %p\n", hit_obj);

    } else if (canvas->hit_obj == NULL) {
      /* If no hit_obj found previously, then let parent handle it. */
      break;
    }

    debug("code: %d\n", code);
    /* Let hit_obj handle the event. */
    call_event_base = false;

    lv_point_t act_point;
    lv_obj_t *scroll_obj = indev->proc.types.pointer.scroll_obj;

    act_point = indev->proc.types.pointer.act_point;

    indev->proc.types.pointer.act_point = point;
    indev->proc.types.pointer.act_obj = canvas->hit_obj;
    lv_event_send(canvas->hit_obj, code, indev);
    indev->proc.types.pointer.act_point = act_point;
    indev->proc.types.pointer.act_obj = obj;

    if (code == LV_EVENT_RELEASED) {
      if (scroll_obj != NULL) /* No clicked event. */
        canvas->hit_obj = NULL;
    } else if (code == LV_EVENT_CLICKED) {
      canvas->hit_obj = NULL;
    }
    e->deleted = true;
    break;
  }
  default:
    break;
  }

  if (call_event_base)
    lv_obj_event_base(MY_CLASS, e);
}

#if 1 /* example */

static lv_obj_t *g_canvas1, *g_canvas2;

static void slider_event_cb(lv_event_t *e) {
  lv_obj_t *slider = lv_event_get_target(e);
  lv_obj_t *label = e->user_data;
  lv_label_set_text_fmt(label, "%d%%", (int)lv_slider_get_value(slider));

  int scale = lv_slider_get_value(slider) * 256 / 100;
  lv_img_set_zoom(g_canvas1, scale);
  lv_img_set_zoom(g_canvas2, scale);
  debug("set zoom to %d", scale);
}

static void btn_clicked(lv_event_t *e) {
  lv_coord_t code = lv_event_get_code(e);
  printf("btn: %d\n", code);
}

static void label_update_cb(lv_timer_t *timer) {
  static int count;
  lv_obj_t *obj = timer->user_data;
  lv_label_set_text_fmt(obj, "hello%d", count++);
}

static void create_disp_content(lv_obj_t *canvas, int id) {
  lv_obj_t *root = lv_disp_get_scr_act(lvx_canvas_get_disp(canvas));

#if 1 /* @todo !!! why need another obj here? */
  root = lv_obj_create(root);
  lv_obj_set_size(root, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_pad_all(root, 0, 0);
#endif

  /* Styles can be directly added to canvas widget. */
  lv_obj_set_style_radius(root, 10, 0);
  lv_obj_set_style_pad_all(root, 10, 0);
  lv_obj_set_style_border_opa(root, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(root, lv_color_black(), 0);
  lv_obj_set_style_border_width(root, 2, 0);
  lv_obj_set_style_bg_color(root, lv_color_hex(0x0000ff), 0);
  lv_obj_set_style_bg_opa(root, LV_OPA_10, 0);

  /**
   * Add items to widget to be canvas.
   */
  lv_obj_t *btn = lv_btn_create(root);

  /**
   * Demo to show btn can receive event under zoomed state.
   */
  lv_obj_add_event_cb(btn, btn_clicked, LV_EVENT_PRESSED, NULL);
  lv_obj_add_event_cb(btn, btn_clicked, LV_EVENT_PRESSING, NULL);
  lv_obj_add_event_cb(btn, btn_clicked, LV_EVENT_PRESS_LOST, NULL);
  lv_obj_add_event_cb(btn, btn_clicked, LV_EVENT_SHORT_CLICKED, NULL);
  lv_obj_add_event_cb(btn, btn_clicked, LV_EVENT_LONG_PRESSED, NULL);
  lv_obj_add_event_cb(btn, btn_clicked, LV_EVENT_LONG_PRESSED_REPEAT, NULL);
  lv_obj_add_event_cb(btn, btn_clicked, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(btn, btn_clicked, LV_EVENT_RELEASED, NULL);

  lv_obj_set_size(btn, 100, 40);
  lv_obj_set_style_width(btn, 80, LV_STATE_PRESSED);
  lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text_fmt(btn_label, "button");
  lv_obj_center(btn_label);
  lv_timer_create(label_update_cb, 100, btn_label);

  lv_obj_t *box = lv_obj_create(root);
  lv_obj_t *label = lv_label_create(box);
  lv_label_set_text(label, "Hello");
  lv_obj_clear_flag(box, LV_OBJ_FLAG_CLICKABLE);
  lv_timer_create(label_update_cb, 1000, label);

  lv_obj_set_size(box, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_pos(box, 0, 0);
  lv_obj_set_style_border_opa(box, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(box, lv_color_black(), 0);
  lv_obj_set_style_border_width(box, 1, 0);
  lv_obj_set_style_bg_color(box, lv_color_hex(0x00ff00), 0);
  lv_obj_set_style_bg_opa(box, 50, 0);
  lv_obj_align_to(box, btn, LV_ALIGN_TOP_RIGHT, 0, 0);

  label = lv_label_create(canvas);
  lv_label_set_text_fmt(label, "DISPLAY%d", id);
  lv_obj_center(label);

  /**
   * Normal child can be still added to canvas(physical display).
   */
  lv_obj_t *btn_child = lv_btn_create(canvas);
  lv_obj_set_size(btn_child, LV_SIZE_CONTENT, 40);
  lv_obj_align(btn_child, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_label_set_text(lv_label_create(btn_child), "ABOVE CANVAS");
}

void lvx_canvas_example(void) {
  lv_obj_t *container = lv_obj_create(lv_scr_act());
  lv_obj_set_size(container, LV_HOR_RES, LV_VER_RES);
  lv_obj_center(container);

  lv_obj_t *below = lv_label_create(container);
  lv_label_set_text(below, "BELOW CANVAS OBJECT");
  lv_obj_set_pos(below, 50, 50);

  g_canvas1 = lvx_canvas_create(container);
  lv_obj_set_style_border_opa(g_canvas1, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(g_canvas1, lv_color_hex(0x00ff00), 0);
  lv_obj_set_style_border_width(g_canvas1, 5, 0);
  lv_obj_align(g_canvas1, LV_ALIGN_LEFT_MID, 0, 0);
  lvx_canvas_set_disp(g_canvas1, 180, 400);
  lv_obj_set_style_opa(g_canvas1, LV_OPA_80, 0);

  create_disp_content(g_canvas1, 1);

  g_canvas2 = lvx_canvas_create(container);
  lv_obj_set_style_border_opa(g_canvas2, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(g_canvas2, lv_color_hex(0x00ff00), 0);
  lv_obj_set_style_border_width(g_canvas2, 5, 0);
  lv_obj_align(g_canvas2, LV_ALIGN_RIGHT_MID, 0, 0);
  lvx_canvas_set_disp(g_canvas2, 180, 400);

  create_disp_content(g_canvas2, 2);

  /*Create a slider to manually control the scale of canvas widget.*/
  lv_obj_t *slider = lv_slider_create(lv_scr_act());
  lv_obj_t *slider_label = lv_label_create(lv_scr_act());
  lv_obj_center(slider);
  lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED,
                      slider_label);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 0);
  lv_slider_set_value(slider, 100, LV_ANIM_OFF);
  lv_slider_set_range(slider, 0, 200);
  lv_label_set_text(slider_label, "100%");
  lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}
#endif

#endif