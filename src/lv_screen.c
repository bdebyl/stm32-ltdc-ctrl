#include <lv_screen.h>
#include <lvgl/lvgl.h>

/* void lv_ex_btn_1() { */
/*     lv_obj_set_style_text_font(lv_scr_act(), &lv_font_montserrat_14, 0); */
/*
 *     lv_obj_t* label;
 *
 *     lv_obj_t* btn1 = lv_btn_create(lv_scr_act());
 *     [> lv_obj_set_event_cb(btn1, NULL); <]
 *     lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);
 *     lv_obj_center(btn1);
 *
 *     label = lv_label_create(btn1);
 *     lv_label_set_text(label, "Kyle Sucks"); */
/* } */
static lv_obj_t* meter;

/**
 * A simple meter
 */
void lv_ex_btn_1(void) {
    meter = lv_meter_create(lv_scr_act());
    lv_obj_center(meter);
    lv_obj_set_size(meter, 200, 200);

    /*Add a scale first*/
    lv_meter_scale_t* scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 41, 2, 10,
                             lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(meter, scale, 5, 4, 15, lv_color_black(),
                                   10);
    lv_meter_set_scale_range(meter, scale, 0, 80, 300, 120);

    lv_meter_indicator_t* indic;

    /*     [>Add a blue arc to the start<]
     *     indic =
     *         lv_meter_add_arc(meter, scale, 3,
     * lv_palette_main(LV_PALETTE_BLUE), 0);
     *     lv_meter_set_indicator_start_value(meter, indic, 0);
     *     lv_meter_set_indicator_end_value(meter, indic, 1000);
     *
     *     [>Make the tick lines blue at the start of the scale<]
     *     indic =
     *         lv_meter_add_scale_lines(meter, scale,
     * lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE),
     * false, 0); lv_meter_set_indicator_start_value(meter, indic, 0);
     *     lv_meter_set_indicator_end_value(meter, indic, 1000); */

    /*Add a red arc to the end*/
    indic =
        lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(meter, indic, 65);
    lv_meter_set_indicator_end_value(meter, indic, 80);

    /*Make the tick lines red at the end of the scale*/
    indic =
        lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_RED),
                                 lv_palette_main(LV_PALETTE_RED), false, 0);
    lv_meter_set_indicator_start_value(meter, indic, 65);
    lv_meter_set_indicator_end_value(meter, indic, 80);

    /*Add a needle line indicator*/
    indic = lv_meter_add_needle_line(meter, scale, 4,
                                     lv_palette_main(LV_PALETTE_GREY), -10);
    lv_meter_set_indicator_value(meter, indic, 8);

    lv_obj_t* label = lv_label_create(meter);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label, "RPM\nx100/min");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 70);
}
