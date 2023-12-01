/**************************************************************
 *
 * Copyright(c) 2022, Beijing Xiaomi Mobile Software Co., Ltd.
 * All Rights Reserved.
 *
 *************************************************************/

#ifndef LV_EXT_WIDGETS_CANVAS_H_
#define LV_EXT_WIDGETS_CANVAS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"

#if 1


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_canvas_t obj;
    lv_img_dsc_t *img;
    lv_obj_t* hit_obj; /* The obj receiving input dev event. */

    lv_disp_t *vdisp; /* virtual display attached to canvas */
    lv_disp_drv_t disp_drv;
    lv_disp_draw_buf_t draw_buf;
} lvx_canvas_t;

extern const lv_obj_class_t lvx_canvas_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_obj_t* lvx_canvas_create(lv_obj_t* parent);

void lvx_canvas_set_disp(lv_obj_t* obj, lv_coord_t w, lv_coord_t h);
const void *lvx_canvas_get_img(lv_obj_t *obj);
lv_disp_t *lvx_canvas_get_disp(lv_obj_t *obj);


/**********************
 *      MACROS
 **********************/

#endif /*  */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_EXT_WIDGETS_CANVAS_H_ */
