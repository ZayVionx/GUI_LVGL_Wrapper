/**
 * @file lv_port_disp.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp_template.h"
#include <stdbool.h>

/*********************
 *    LCD INCLUDES
 *********************/
#include "lcd.h"

/*********************
 *      DEFINES
 *********************/
/* 屏幕大小 */
#define MY_DISP_HOR_RES     (320)
#define MY_DISP_VER_RES     (240)

/* 缓冲区大小 (双缓冲: 每个缓冲区占半屏, 总内存与原单缓冲相同) */
#define __BUF_PX_SIZE__     (MY_DISP_HOR_RES * 60)

/* 缓冲区选择 */
#define ONE_BUFFER          0
#define TWO_BUFFER          1
#define DOUBLE_BUFFER       2
#define __USE_BUFFER__      TWO_BUFFER

/* Flush method:
 * 0: CPU blocking flush (safer for debugging)
 * 1: DMA flush (faster, requires correct DMA config/IRQ)
 */
#ifndef LV_PORT_DISP_USE_DMA
    #define LV_PORT_DISP_USE_DMA 1
#endif

#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    (320)
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_VER_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    (240)
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

#if __USE_BUFFER__ == ONE_BUFFER
    /* Example for 1) */
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[__BUF_PX_SIZE__];                          /*A buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, __BUF_PX_SIZE__);   /*Initialize the display buffer*/

#elif (__USE_BUFFER__ == TWO_BUFFER)
    /* Example for 2) */
    static lv_disp_draw_buf_t draw_buf_dsc_2;
    static lv_color_t buf_2_1[__BUF_PX_SIZE__];                        /*A buffer for 10 rows*/
    static lv_color_t buf_2_2[__BUF_PX_SIZE__];                        /*An other buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, __BUF_PX_SIZE__);   /*Initialize the display buffer*/

#elif (__USE_BUFFER__ == DOUBLE_BUFFER)
    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
    static lv_disp_draw_buf_t draw_buf_dsc_3;
    static lv_color_t buf_3_1[__BUF_PX_SIZE__];            /*A screen sized buffer*/
    static lv_color_t buf_3_2[__BUF_PX_SIZE__];            /*Another screen sized buffer*/
    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
                          __BUF_PX_SIZE__);   /*Initialize the display buffer*/

#endif

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                  /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
#if (__USE_BUFFER__ == ONE_BUFFER)
    disp_drv.draw_buf = &draw_buf_dsc_1;

#elif (__USE_BUFFER__ == TWO_BUFFER)
    disp_drv.draw_buf = &draw_buf_dsc_2;

#elif (__USE_BUFFER__ == DOUBLE_BUFFER)
    disp_drv.draw_buf = &draw_buf_dsc_3;

#endif

#if (__USE_BUFFER__ == DOUBLE_BUFFER)
    /*Required for Example 3)*/
    disp_drv.full_refresh = 1;
#endif

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
    /**
     * @note 请提供LCD初始化函数
     */
    // lcd_dma_init();

	dma_init_type dma_init_struct;
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
    dma_reset(DMA1_CHANNEL5);
    dma_init_struct.buffer_size = 65535;
    dma_init_struct.direction = DMA_DIR_MEMORY_TO_MEMORY;
    dma_init_struct.memory_base_addr = (__IO uint32_t)XFMC_Addr_ILI9341_DATA; /* as  destination address */
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma_init_struct.memory_inc_enable = FALSE;
    dma_init_struct.peripheral_base_addr = (__IO uint32_t)XFMC_Addr_ILI9341_DATA; /* as  source address */
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    dma_init_struct.peripheral_inc_enable = TRUE;
    dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
    dma_init_struct.loop_mode_enable = FALSE;
    dma_init(DMA1_CHANNEL5, &dma_init_struct);

    nvic_irq_enable(DMA1_Channel5_IRQn, 1, 0);
    dma_interrupt_enable(DMA1_CHANNEL5, DMA_FDT_INT, TRUE);
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}
static lv_disp_drv_t *lv_disp_drv_p = NULL;
static volatile uint8_t dma_flush_busy = 0; /* DMA传输忙标志 */

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    if(disp_flush_enabled) {
#if LV_PORT_DISP_USE_DMA
        /* 等待上一次DMA传输完成 (防护: 理论上LVGL会等flush_ready, 但多加一层保险) */
        while(dma_flush_busy);

        lv_disp_drv_p = disp_drv;
        dma_flush_busy = 1;
        uint32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
        lcd_set_window(area->x1, area->y1, lv_area_get_width(area), lv_area_get_height(area));
        lcd_write_ram_prepare();
        DMA1_CHANNEL5->ctrl   &= ~(uint16_t)1;
        DMA1_CHANNEL5->paddr   = (uint32_t)color_p;
        DMA1_CHANNEL5->dtcnt   = size;
        DMA1_CHANNEL5->ctrl   |= (uint16_t)1;
#else
        /* Debug-safe blocking flush. */
        lcd_color_fill(area->x1, area->y1, area->x2, area->y2, (uint16_t *)color_p);
        lv_disp_flush_ready(disp_drv);
#endif
    }
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}
void DMA1_Channel5_IRQHandler()
{
    dma_flag_clear(DMA1_FDT5_FLAG);
    dma_channel_enable(DMA1_CHANNEL5, FALSE);
    dma_flush_busy = 0; /* 清除忙标志 */
    if (lv_disp_drv_p != NULL)
    {
       lv_disp_flush_ready(lv_disp_drv_p); /* tell lvgl that flushing is done */
    }
}
#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
