/**************************************************************
 *
 * Copyright(c) 2022, Beijing Xiaomi Mobile Software Co., Ltd.
 * All Rights Reserved.
 *
 *************************************************************/

/*********************
 *      INCLUDES
 *********************/
#include "lvx_imgbar.h"
#include <float.h>
#include <lvgl.h>
#include <math.h>

#define MATH_PI 3.14159265358979323846f
#define MATH_HALF_PI 1.57079632679489661923f

#define DEG_TO_RAD 0.017453292519943295769236907684886f
#define RAD_TO_DEG 57.295779513082320876798154814105f

#define MATH_RADIANS(deg) ((deg)*DEG_TO_RAD)
#define MATH_DEGRESS(rad) ((rad)*RAD_TO_DEG)

#if LVX_USE_IMGBAR != 0

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lvx_imgbar_class
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lvx_imgbar_constructor(const lv_obj_class_t* class_p,
                                   lv_obj_t* obj);
static void lvx_imgbar_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lvx_imgbar_event(const lv_obj_class_t* class_p, lv_event_t* e);
static void lvx_imgbar_draw_event(lv_event_t* e);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lvx_imgbar_class = {
    .constructor_cb = lvx_imgbar_constructor,
    .destructor_cb = lvx_imgbar_destructor,
    .event_cb = lvx_imgbar_event,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lvx_imgbar_t),
    .base_class = &lv_image_class,
};
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Return with a pointer to the selector
 * @param parent pointer to parent
 * @return pointer to the selector
 */
lv_obj_t* lvx_imgbar_create(lv_obj_t* parent)
{
    return lv_obj_class_create_obj(MY_CLASS, parent);
}

void lvx_imgbar_set_src(lv_obj_t* obj, const void* src)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    lv_image_set_src((lv_obj_t*)&imgbar->img, src);
    lv_obj_invalidate(obj);
}

void lvx_imgbar_set_para(lv_obj_t* obj, const lvx_imgbar_para_t* para)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    imgbar->para = *para;
    lv_obj_invalidate(obj);
}

/**
 * Always draw arc counterwise from start to end. Value must be positive.
 */
void lvx_imgbar_set_angle(lv_obj_t* obj, uint16_t start, uint16_t end)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    imgbar->para.start_angle = start;
    imgbar->para.end_angle = end;
    lv_obj_invalidate(obj);
}

void lvx_imgbar_set_start_angle(lv_obj_t* obj, lv_coord_t start)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    imgbar->para.start_angle = start;
    lv_obj_invalidate(obj);
}

void lvx_imgbar_set_end_angle(lv_obj_t* obj, lv_coord_t end)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    imgbar->para.end_angle = end;
    lv_obj_invalidate(obj);
}

void lvx_imgbar_set_center(lv_obj_t* obj, lv_coord_t x, lv_coord_t y)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    imgbar->para.center.x = x;
    imgbar->para.center.y = y;
    lv_obj_invalidate(obj);
}

void lvx_imgbar_set_radius(lv_obj_t* obj, lv_coord_t radius)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    imgbar->para.radius = radius;
    lv_obj_invalidate(obj);
}

void lvx_imgbar_set_bar_width(lv_obj_t* obj, lv_coord_t width)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    imgbar->para.width = width;
    lv_obj_invalidate(obj);
}

void lvx_imgbar_set_linecap(lv_obj_t* obj, lvx_imgbar_lincap_t linecap)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    imgbar->para.linecap = linecap;
    lv_obj_invalidate(obj);
}

void lvx_imgbar_set_zoom(lv_obj_t* obj, uint16_t zoom)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    imgbar->zoom = zoom;
    lv_image_set_scale(obj, zoom);
    lv_obj_invalidate(obj);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lvx_imgbar_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    imgbar->zoom = LV_SCALE_NONE;

    lv_obj_set_style_img_opa(obj, LV_OPA_10, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    LV_TRACE_OBJ_CREATE("finished");
}

static void lvx_imgbar_destructor(const lv_obj_class_t* class_p, lv_obj_t* obj)
{
    LV_UNUSED(class_p);
}

static void lvx_imgbar_event(const lv_obj_class_t* class_p, lv_event_t* e)
{
    LV_UNUSED(class_p);

    if (e->code == LV_EVENT_DRAW_MAIN) {
        lvx_imgbar_draw_event(e);
        return;
    }

    lv_result_t res = lv_obj_event_base(MY_CLASS, e);
    if (res != LV_RESULT_OK)
        return;
}

#if LV_USE_VECTOR_GRAPHIC
static void draw_arc(lv_layer_t* layer, const lv_area_t* coords,
                     const char* img_src, lv_fpoint_t c, float start_angle,
                     float end_angle, float radius, float width, bool rounded)
{
    if (width <= 0)
        return;
    if (start_angle == end_angle)
        return;

    lv_area_t clip_area;
    if (!_lv_area_intersect(&clip_area, coords, &layer->_clip_area)) {
        /*Fully clipped, nothing to do*/
        return;
    }

    float sweep_angle = end_angle - start_angle;

    while (sweep_angle < 0) {
        sweep_angle += 360;
    }

    while (sweep_angle > 360) {
        sweep_angle -= 360;
    }

    /*If the angles are the same then there is nothing to draw*/
    if (fabsf(sweep_angle) < FLT_EPSILON) {
        return;
    }

    lv_vector_dsc_t* ctx = lv_vector_dsc_create(layer);
    lv_vector_path_t* path
        = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);

    float radius_out = radius;
    float radius_in = radius - width;
    float half_width = width * 0.5f;
    float radius_center = radius_out - half_width;

    if (fabs(sweep_angle - 360) < FLT_EPSILON) {
        lv_vector_path_append_circle(path, &c, radius_out, radius_out);
        lv_vector_path_append_circle(path, &c, radius_in, radius_in);
    } else {
        /* radius_out start point */
        float start_angle_rad = MATH_RADIANS(start_angle);
        lv_fpoint_t start;
        start.x = radius_out * cosf(start_angle_rad) + c.x;
        start.y = radius_out * sinf(start_angle_rad) + c.y;

        /* radius_in start point */
        float end_angle_rad = MATH_RADIANS(end_angle);
        lv_fpoint_t end;
        end.x = radius_in * cosf(end_angle_rad) + c.x;
        end.y = radius_in * sinf(end_angle_rad) + c.y;

        /* radius_out arc */
        lv_vector_path_append_arc(path, &c, radius_out, start_angle,
                                  sweep_angle, false);

        /* line to radius_in */
        lv_vector_path_line_to(path, &end);

        /* radius_in arc */
        lv_vector_path_append_arc(path, &c, radius_in, end_angle, -sweep_angle,
                                  false);

        /* close arc */
        lv_vector_path_line_to(path, &start);
        lv_vector_path_close(path);
        /* draw round */
        if (rounded && half_width > 0) {
            lv_fpoint_t rc1;
            rc1.x = c.x + radius_center * cosf(end_angle_rad);
            rc1.y = c.y + radius_center * sinf(end_angle_rad);
            lv_vector_path_append_circle(path, &rc1, half_width, half_width);

            lv_fpoint_t rc2;
            rc2.x = c.x + radius_center * cosf(start_angle_rad);
            rc2.y = c.y + radius_center * sinf(start_angle_rad);
            lv_vector_path_append_circle(path, &rc2, half_width, half_width);
        }
    }

    lv_vector_dsc_translate(ctx, coords->x1, coords->y1);
    lv_draw_image_dsc_t draw_img_dsc;
    lv_draw_image_dsc_init(&draw_img_dsc);
    draw_img_dsc.src = img_src;
    lv_vector_dsc_set_fill_image(ctx, &draw_img_dsc);
    // lv_vector_dsc_set_fill_color(ctx, lv_color_hex(0xff0000));
    // lv_vector_dsc_set_fill_opa(ctx, LV_OPA_50);

    lv_area_t bound;
    lv_vector_path_get_bounding(path, &bound);
    lv_matrix_t m;
    lv_matrix_identity(&m);
    lv_matrix_translate(&m, -bound.x1, -bound.y1);
    lv_vector_dsc_set_fill_transform(ctx, &m);
    lv_vector_dsc_set_fill_rule(ctx, LV_VECTOR_FILL_EVENODD);
    lv_vector_dsc_add_path(ctx, path);

    lv_draw_vector(ctx); /* submit draw */

    lv_vector_path_delete(path);
    lv_vector_dsc_delete(ctx);
}
#endif

static void lvx_imgbar_draw_event(lv_event_t* e)
{
#if LV_USE_VECTOR_GRAPHIC
    /* call base event */
    lv_obj_event_base(MY_CLASS, e);
    lv_obj_t* obj = lv_event_get_target(e);
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;

    lv_layer_t* layer = lv_event_get_layer(e);
    const char* img_src = lv_image_get_src(obj);

    lv_fpoint_t center;
    center.x = imgbar->para.center.x;
    center.y = imgbar->para.center.y;

    draw_arc(layer, &obj->coords, img_src, center, imgbar->para.start_angle,
             imgbar->para.end_angle, imgbar->para.radius, imgbar->para.width,
             imgbar->para.linecap == LVX_IMGBAR_LINECAP_ROUND);
#else
    /* call base event */
    lv_obj_event_base(MY_CLASS, e);
#endif
}

#endif
