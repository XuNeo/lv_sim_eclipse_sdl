/**************************************************************
 *
 * Copyright(c) 2022, Beijing Xiaomi Mobile Software Co., Ltd.
 * All Rights Reserved.
 *
 *************************************************************/

#ifndef LV_EXTENSIONS_WIDGETS_IMG_LINE_BAR_H_
#define LV_EXTENSIONS_WIDGETS_IMG_LINE_BAR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"
#define LVX_USE_IMG_LINE_BAR 1

#if LVX_USE_IMG_LINE_BAR != 0
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/*Parts of the image line progrss bar*/
enum {
    LV_IMG_LINE_BAR_PART_MAIN = LV_PART_MAIN,
};
typedef uint8_t lvx_img_line_bar_part_t;

/*Imgbar linecap style*/
enum {
    LVX_IMG_LINE_BAR_LINECAP_BUTT = 0,
    LVX_IMG_LINE_BAR_LINECAP_ROUND,
};
typedef uint8_t lvx_img_line_bar_lincap_t;

/*Data of image bar*/
typedef struct {
    lv_image_t img;

    lv_draw_sw_mask_map_param_t m;
    lv_opa_t* mask_map;
    lv_coord_t mask_w;
    lv_coord_t mask_h;
    int16_t mask_id;

    lv_point_precise_t start;
    lv_point_precise_t end;
    bool update_mask;
} lvx_img_line_bar_t;

extern const lv_obj_class_t lvx_img_line_bar_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_obj_t* lvx_img_line_bar_create(lv_obj_t* parent);

void lvx_img_line_bar_set_para(lv_obj_t* obj, lv_point_precise_t start, lv_point_precise_t end,
                               lv_coord_t width, bool round);

void lvx_img_line_bar_set_start_x(lv_obj_t* obj, lv_coord_t value);
void lvx_img_line_bar_set_start_y(lv_obj_t* obj, lv_coord_t value);
void lvx_img_line_bar_set_end_x(lv_obj_t* obj, lv_coord_t value);
void lvx_img_line_bar_set_end_y(lv_obj_t* obj, lv_coord_t value);

void lvx_img_line_bar_set_end(lv_obj_t* obj, lv_point_precise_t end);

/**********************
 *      MACROS
 **********************/

#endif /* LVX_USE_IMG_LINE_BAR */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_EXTENSIONS_WIDGETS_SELECTOR_H_ */
