/**************************************************************
 *
 * Copyright(c) 2022, Beijing Xiaomi Mobile Software Co., Ltd.
 * All Rights Reserved.
 *
 *************************************************************/

#ifndef LV_EXTENSIONS_WIDGETS_IMGBAR_H_
#define LV_EXTENSIONS_WIDGETS_IMGBAR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

#define LVX_USE_IMGBAR 1

#if LVX_USE_IMGBAR != 0

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/*Parts of the image bar*/
enum {
    LV_IMGBAR_PART_MAIN = LV_PART_MAIN,
};
typedef uint8_t lvx_imgbar_part_t;

/*Imgbar linecap style*/
enum {
    LVX_IMGBAR_LINECAP_BUTT = 0,
    LVX_IMGBAR_LINECAP_ROUND,
};
typedef uint8_t lvx_imgbar_lincap_t;

typedef struct _lvx_imgbar_para_t {
    lv_point_t center;
    lv_coord_t radius;
    lv_coord_t width;

    lv_coord_t start_angle; /*angle in degree, same as lv_arc*/
    lv_coord_t end_angle;

    lvx_imgbar_lincap_t linecap;
} lvx_imgbar_para_t;

/*Data of image bar*/
typedef struct {
    lv_image_t img;

    uint16_t zoom;
    lvx_imgbar_para_t para;
} lvx_imgbar_t;

extern const lv_obj_class_t lvx_imgbar_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_obj_t* lvx_imgbar_create(lv_obj_t* parent);
void lvx_imgbar_set_src(lv_obj_t* obj, const void * src);

void lvx_imgbar_set_para(lv_obj_t* obj, const lvx_imgbar_para_t* para);
void lvx_imgbar_set_angle(lv_obj_t* obj, uint16_t start, uint16_t end);
void lvx_imgbar_set_zoom(lv_obj_t* obj, uint16_t zoom);
void lvx_imgbar_set_start_angle(lv_obj_t* obj, lv_coord_t start);
void lvx_imgbar_set_end_angle(lv_obj_t* obj, lv_coord_t end);
void lvx_imgbar_set_center(lv_obj_t* obj, lv_coord_t x, lv_coord_t y);
void lvx_imgbar_set_radius(lv_obj_t* obj, lv_coord_t radius);
void lvx_imgbar_set_bar_width(lv_obj_t* obj, lv_coord_t width);
void lvx_imgbar_set_linecap(lv_obj_t* obj, lvx_imgbar_lincap_t linecap);

static inline lv_coord_t lvx_imgbar_get_start_angle(lv_obj_t* obj)
{
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    return imgbar->para.start_angle;
}

static inline lv_coord_t lvx_imgbar_get_end_angle(lv_obj_t* obj)
{
    lvx_imgbar_t* imgbar = (lvx_imgbar_t*)obj;
    return imgbar->para.end_angle;
}


/**********************
 *      MACROS
 **********************/

#endif /* LVX_USE_IMGBAR */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_EXTENSIONS_WIDGETS_SELECTOR_H_ */
