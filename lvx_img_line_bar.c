/**************************************************************
 *
 * Copyright(c) 2022, Beijing Xiaomi Mobile Software Co., Ltd.
 * All Rights Reserved.
 *
 *************************************************************/

/*********************
 *      INCLUDES
 *********************/
#include "lvx_img_line_bar.h"
#include <math.h>

#if LVX_USE_IMG_LINE_BAR != 0

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lvx_img_line_bar_class
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lvx_img_line_bar_constructor(const lv_obj_class_t* class_p,
                                         lv_obj_t* obj);
static void lvx_img_line_bar_destructor(const lv_obj_class_t* class_p,
                                        lv_obj_t* obj);
static void lvx_img_line_bar_event(const lv_obj_class_t* class_p,
                                   lv_event_t* e);

static void update_mask_async(lvx_img_line_bar_t* imgbar);
//static int alloc_mask(lvx_img_line_bar_t* imgbar);
static void update_mask_cb(void* arg);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lvx_img_line_bar_class = {
    .constructor_cb = lvx_img_line_bar_constructor,
    .destructor_cb = lvx_img_line_bar_destructor,
    .event_cb = lvx_img_line_bar_event,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lvx_img_line_bar_t),
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
lv_obj_t* lvx_img_line_bar_create(lv_obj_t* parent)
{
    lv_obj_class_create_obj(MY_CLASS, parent);
}

void lvx_img_line_bar_set_para(lv_obj_t* obj, lv_point_precise_t start, lv_point_precise_t end,
                               lv_coord_t width, bool round)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_img_line_bar_t* imgbar = (lvx_img_line_bar_t*)obj;
    imgbar->start = start;
    imgbar->end = end;
    lv_obj_set_style_line_width(obj, width, 0);
    lv_obj_set_style_line_rounded(obj, round, 0);
    update_mask_async(imgbar);
}

void lvx_img_line_bar_set_start_x(lv_obj_t* obj, lv_coord_t value)
{
    lvx_img_line_bar_t* imgbar = (lvx_img_line_bar_t*)obj;
    imgbar->start.x = value;
    update_mask_async(imgbar);
}

void lvx_img_line_bar_set_start_y(lv_obj_t* obj, lv_coord_t value)
{
    lvx_img_line_bar_t* imgbar = (lvx_img_line_bar_t*)obj;
    imgbar->start.y = value;
    update_mask_async(imgbar);
}

void lvx_img_line_bar_set_end_x(lv_obj_t* obj, lv_coord_t value)
{
    lvx_img_line_bar_t* imgbar = (lvx_img_line_bar_t*)obj;
    imgbar->end.x = value;
    update_mask_async(imgbar);
}

void lvx_img_line_bar_set_end_y(lv_obj_t* obj, lv_coord_t value)
{
    lvx_img_line_bar_t* imgbar = (lvx_img_line_bar_t*)obj;
    imgbar->end.y = value;
    update_mask_async(imgbar);
}

void lvx_img_line_bar_set_end(lv_obj_t* obj, lv_point_precise_t end)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lvx_img_line_bar_t* imgbar = (lvx_img_line_bar_t*)obj;
    imgbar->end = end;
    update_mask_async(imgbar);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lvx_img_line_bar_constructor(const lv_obj_class_t* class_p,
                                         lv_obj_t* obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lvx_img_line_bar_t* imgbar = (lvx_img_line_bar_t*)obj;

    imgbar->start.x = 0;
    imgbar->start.y = 0;
    imgbar->end.x = 0;
    imgbar->end.y = 0;

    /*Initialize the allocated 'imgbar'*/
    imgbar->mask_id = -1;
    imgbar->mask_map = NULL;
    imgbar->mask_w = 0;
    imgbar->mask_h = 0;

#if 1
    lv_obj_set_style_image_opa((lv_obj_t*)imgbar, 0, 0);
#endif

    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    LV_TRACE_OBJ_CREATE("finished");
}

static void lvx_img_line_bar_destructor(const lv_obj_class_t* class_p,
                                        lv_obj_t* obj)
{
    LV_UNUSED(class_p);

    LV_LOG_TRACE("_lvx_img_line_bar_signal LV_SIGNAL_CLEANUP");
    lvx_img_line_bar_t* imgbar = (lvx_img_line_bar_t*)obj;
    if (imgbar && imgbar->mask_map) {
        lv_free(imgbar->mask_map);
        imgbar->mask_map = NULL;
    }

    if (imgbar->update_mask)
        lv_async_call_cancel(update_mask_cb, imgbar);
}

static inline void lvx_img_line_bar_add_mask(lvx_img_line_bar_t* imgbar)
{
    // lv_area_t a = ((lv_obj_t*)imgbar)->coords;
    // lv_draw_sw_mask_map_init(&imgbar->m, &a, imgbar->mask_map);
    // imgbar->mask_id = lv_draw_mask_add(&imgbar->m, NULL);
}

/**
 * Called in event callback when start to draw image and either image source
 * changed or newly added.
 */
// static int alloc_mask(lvx_img_line_bar_t* imgbar)
// {
//     lv_image_t* img = &imgbar->img;

//     if (img->w == 0 || img->h == 0)
//         return ERROR;

//     lv_opa_t* mask_map
//         = lv_realloc(imgbar->mask_map, sizeof(lv_opa_t) * img->w * img->h);
//     if (mask_map == NULL)
//         return ERROR; /*Return without modify the mask parameter.*/

//     /*Update current mask map info.*/
//     imgbar->mask_map = mask_map;
//     imgbar->mask_w = img->w;
//     imgbar->mask_h = img->h;
//     return OK;
// }

static inline bool math_zero(float a)
{
    return (fabs(a) < (1.19209289550781250000000000000000000e-7F));
}

#define SIGN(x) (math_zero(x) ? 0 : ((x) > 0 ? 1 : -1))

static void append_quarter_circle(lv_vector_path_t * path, lv_fpoint_t *start, lv_fpoint_t * center, lv_fpoint_t * end)
{
    float dx1 = center->x - start->x;
    float dy1 = center->y - start->y;
    float dx2 = end->x - center->x;
    float dy2 = end->y - center->y;

    float c = SIGN(dx1 * dy2 - dx2 * dy1) * 0.55191502449351f;

    lv_fpoint_t p1 = {.x = start->x - c * dy1, .y = start->y + c * dx1};
    lv_fpoint_t p2 = {.x = end->x - c * dy2, .y = end->y + c * dx2};
    lv_vector_path_cubic_to(path, &p1, &p2, end);
}

static void lvx_init_linebar_path(const lv_draw_line_dsc_t * dsc, lv_vector_path_t * path)
{
    float p1_x = dsc->p1.x;
    float p1_y = dsc->p1.y;
    float p2_x = dsc->p2.x;
    float p2_y = dsc->p2.y;

    if(p1_x == p2_x && p1_y == p2_y)
        return;

    float dx = p2_x - p1_x;
    float dy = p2_y - p1_y;
    float inv_dl = 1.0f / sqrtf((dx * dx) + (dy * dy));
    float w_dx = dsc->width * dy * inv_dl;
    float w_dy = dsc->width * dx * inv_dl;
    float w2_dx = w_dx / 2;
    float w2_dy = w_dy / 2;

    /* head point */

    lv_fpoint_t head_start = {.x = p1_x + w2_dx, .y = p1_y - w2_dy};
    lv_fpoint_t head_end = {.x = p1_x - w2_dx, .y = p1_y + w2_dy};

    /* tail point */
    lv_fpoint_t tail_start = {.x = p2_x - w2_dx, .y = p2_y + w2_dy};
    lv_fpoint_t tail_end = {.x = p2_x + w2_dx, .y = p2_y - w2_dy};

    /*
          head_start        tail_end
              *-----------------*
             /|                 |\
            / |                 | \
    arc_c *(  *p1             p2*  )* arc_c
            \ |                 | /
             \|                 |/
              *-----------------*
          head_end          tail_start
    */

    /* move to start point */
    lv_vector_path_move_to(path, &head_start);

    /* draw line head */
    if(dsc->round_start) {
        lv_fpoint_t arc_cs = {.x = p1_x - w2_dy, .y = p1_y - w2_dx};
        lv_fpoint_t center_cs = {.x = dsc->p1.x, .y = dsc->p1.y};
        /* start 90deg arc */
        append_quarter_circle(path, &head_start, &center_cs, &arc_cs);

        /* end 90deg arc */
        append_quarter_circle(path, &arc_cs, &center_cs, &head_end);
    }
    else {
        lv_vector_path_line_to(path, &head_end);
    }

    /* draw line body */
    lv_vector_path_line_to(path, &tail_start);

    /* draw line tail */
    if(dsc->round_end) {
         lv_fpoint_t arc_ce = {.x = p2_x + w2_dy, .y = p2_y + w2_dx};
         lv_fpoint_t center_ce = {.x = dsc->p2.x, .y = dsc->p2.y};
         /* start 90deg arc */
          append_quarter_circle(path, &tail_start, &center_ce, &arc_ce);

          /* end 90deg arc */
          append_quarter_circle(path, &arc_ce, &center_ce, &tail_end);
    }
    else {
        lv_vector_path_line_to(path, &tail_end);
    }

    /* close draw line body */
    lv_vector_path_line_to(path, &head_start);

    lv_vector_path_close(path);
}

static void update_mask_cb(void* arg)
{
    lvx_img_line_bar_t* imgbar = arg;
    if (!imgbar->update_mask)
        return;

    /* Mask not supported for now */
#if 0
    if (imgbar->mask_map == NULL) {
        int ret;
        ret = alloc_mask(imgbar);
        if (ret != OK)
            return;
    }

    /*Create a "8 bit alpha" canvas and clear it*/
    lv_image_t* img = &imgbar->img;
    lv_obj_t* canvas = lv_canvas_create((lv_obj_t*)imgbar);
    lv_canvas_set_buffer(canvas, imgbar->mask_map, img->w, img->h,
                         LV_COLOR_FORMAT_A8);
    /*0x00 in mask map means block the content.*/
    lv_memset(imgbar->mask_map, 0, img->w * img->h * sizeof(lv_opa_t));

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    /*Now draw the line mask.*/

    lv_draw_line_dsc_t dsc;
    lv_obj_init_draw_line_dsc((lv_obj_t*)imgbar, 0, &dsc);
    dsc.color = lv_color_white();
    dsc.p1 = imgbar->start;
    dsc.p2 = imgbar->end;

    lv_draw_line(&layer, &dsc);

    lv_canvas_finish_layer(canvas, &layer);

    /*The mask is ready, the canvas is not required anymore*/
    lv_obj_del(canvas);

    lv_obj_invalidate((lv_obj_t*)imgbar);
#endif
    imgbar->update_mask = false;
}

static void lvx_img_line_bar_event(const lv_obj_class_t* class_p, lv_event_t* e)
{
    LV_UNUSED(class_p);
    lv_result_t res = lv_obj_event_base(MY_CLASS, e);
    if (res != LV_RESULT_OK)
        return;

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    lvx_img_line_bar_t* imgbar = (lvx_img_line_bar_t*)obj;

#if 1
    if (code == LV_EVENT_DRAW_MAIN) {
        lv_area_t clip_area;
        lv_layer_t * layer = lv_event_get_layer(e);

        if (_lv_area_intersect(&clip_area, &layer->_clip_area, &obj->coords) == false) {
            return;
        }

        lv_coord_t width = lv_obj_get_style_line_width(obj, LV_PART_MAIN);
        if (width == 0) {
            return;
        }

        lv_draw_line_dsc_t dsc;
        lv_draw_line_dsc_init(&dsc);

        // dsc.p1.x = imgbar->start.x + obj->coords.x1;
        // dsc.p1.y = imgbar->start.y + obj->coords.y1;
        // dsc.p2.x = imgbar->end.x + obj->coords.x1;
        // dsc.p2.y = imgbar->end.y + obj->coords.y1;

        dsc.p1.x = imgbar->start.x;
        dsc.p1.y = imgbar->start.y;
        dsc.p2.x = imgbar->end.x;
        dsc.p2.y = imgbar->end.y;

        dsc.width = width;
        if (lv_obj_get_style_line_rounded(obj, LV_PART_MAIN)) {
            dsc.round_start = dsc.round_end = 1;
        }

        lv_vector_dsc_t * ctx = lv_vector_dsc_create(layer);
        lv_vector_dsc_identity(ctx);
        lv_vector_dsc_translate(ctx, obj->coords.x1, obj->coords.y1);

        lv_vector_dsc_set_fill_color(ctx, lv_color_black());
        lv_vector_dsc_set_fill_opa(ctx, LV_OPA_50);

        // lv_draw_image_dsc_t img_dsc;
        // lv_draw_image_dsc_init(&img_dsc);
        // img_dsc.src = imgbar->img.src;
        // lv_vector_dsc_set_fill_image(ctx, &img_dsc);

        lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);

        /* generate path */
        lvx_init_linebar_path(&dsc, path);

        lv_area_t bound;
        lv_vector_path_get_bounding(path, &bound);
        lv_matrix_t m;
        lv_matrix_identity(&m);
        lv_matrix_translate(&m, -bound.x1, -bound.y1);
        lv_vector_dsc_set_fill_transform(ctx, &m);
        lv_vector_dsc_add_path(ctx, path);

        lv_draw_vector(ctx);
        lv_vector_path_delete(path);
        lv_vector_dsc_delete(ctx);
        return;
    }
#else
    int ret;
    if (code == LV_EVENT_DRAW_MAIN_BEGIN) {
        /*Add arc progress bar mask to image.*/
        lv_image_t* img = &imgbar->img;
        if (img->src == NULL)
            return;

        if (imgbar->mask_w != img->w || imgbar->mask_h != img->h) {
            /* Image source changed, allocated mask map before add mask.*/
            ret = alloc_mask(imgbar);
            if (ret != OK)
                return;

            update_mask_async(imgbar);
        }

        // lvx_img_line_bar_add_mask(imgbar);
        return;
    }

    if (code == LV_EVENT_DRAW_POST_END) {
        if (imgbar->mask_id >= 0) {
            // lv_draw_mask_remove_id(imgbar->mask_id);
            imgbar->mask_id = -1;
        }
        return;
    }
#endif
}

static void update_mask_async(lvx_img_line_bar_t* imgbar)
{
#if 1
    return;
#else
    if (imgbar->update_mask) {
        /* already send async */
        return;
    }

    imgbar->update_mask = true;
    lv_async_call(update_mask_cb, imgbar);
#endif
}

#endif
