#include <lv_screen.h>
#include <lvgl/lvgl.h>

void lv_ex_btn_1() {
    lv_obj_set_style_text_font(lv_scr_act(), &lv_font_montserrat_14, 0);

    lv_obj_t* label;

    lv_obj_t* btn1 = lv_btn_create(lv_scr_act());
    /* lv_obj_set_event_cb(btn1, NULL); */
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_center(btn1);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Kyle Sucks");
}
