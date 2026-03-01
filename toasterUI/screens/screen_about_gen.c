/**
 * @file screen_about_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen_about_gen.h"
#include "../toasterUI.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * screen_about_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    static bool style_inited = false;

    if (!style_inited) {

        style_inited = true;
    }

    lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
    lv_obj_set_name_static(lv_obj_0, "screen_about_#");

    lv_obj_t * lv_label_0 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_0, "About screen");
    lv_obj_set_align(lv_label_0, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_0, 10);
    
    lv_obj_t * lv_label_1 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_1, "Add your project details here");
    lv_obj_set_align(lv_label_1, LV_ALIGN_CENTER);
    lv_obj_set_style_text_align(lv_label_1, LV_TEXT_ALIGN_CENTER, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

