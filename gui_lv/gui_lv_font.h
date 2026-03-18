#ifndef __UI_FONT_H__
#define __UI_FONT_H__

/*================================= INCLUDES =================================*/
#ifdef __GUI_LVGL_WRAPPER__
#   include <gui_lv_sys_data.h>
#else
#   include "gui_lv_sys_data.h"
#endif

#ifdef   __cplusplus
extern "C" {
#endif

// 思源黑体 中等 18px
// 基本测量基本測量Basic Test基本測定Базовые измерения
// 分选测量分選測量Sorting Test選別測定Сортировка
// 历史记录歷史記錄History Rec履歴記録История
// 系统设置系統設置Settings設定Настройки
/* 主功能选择界面 */
static const char *const FONT_OPTION_TEXT[][GUI_LV_LANGUAGE_MAX]={
    {"基本测量" , "基本測量" , "Basic Test"   , "基本測定", "Базовые измерения"},
    {"分选测量" , "分選測量" , "Sorting Test" , "選別測定", "Сортировка"       },
    {"历史记录" , "歷史記錄" , "History Rec"  , "履歴記録", "История"          },
    {"系统设置" , "系統設置" , "Settings"     , "設定"    , "Настройки"        },
};

/*----------------------------------------------------------------------------*/
// 上下限:  UpperLowerじょうげんかげんВерх. пред.Ниж. пред.

/* 分选测试界面 */
static const char *const FONT_SORT_MEAS_RANGE_TEXT[][GUI_LV_LANGUAGE_MAX]={
    {"上限:" , "上限:" , "Upper:" , "じょうげん:" , "Верх. пред.:" },
    {"下限:" , "下限:" , "Lower:" , "かげん:"     , "Ниж. пред.:"  },
};

/*----------------------------------------------------------------------------*/
/* SourceHanSansSC Regular 18 */
// 删除文件判定条件确认取消削除文件判定條件確認取消?
// 0123456789 ./-:：ΩabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
// Delete fileSorting CriteriaConfirmCancel
// ファイルを削除しますか選別条件確認キャンセル
// Удалить файлУсловия сортировкиПодтвердитьОтмена

/* SourceHanSansSC Reg 20 */
// 磁盘名称磁盤名稱文件夹文件夾
// abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
// フォルダ
// Имя дискаПапка

/* Roboto Condensed Reg 16 */
// VOLRES

/* Roboto Condensed Reg 20 */
// 0123456789 ./-:：≤ΩabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ


/* 历史记录界面 */
static const char *const FONT_HIST_REC_TEXT[][GUI_LV_LANGUAGE_MAX]={
    {"磁盘名称" , "磁盘名稱" , "Disk Name" , "ディスク名" , "Имя диска" },
    {"文件夹"   , "文件夾"   , "Document1" , "フォルダ"   , "Папка"}
    
};
/* 弹窗 */
static const char *const FONT_POPUP_TEXT[][GUI_LV_LANGUAGE_MAX]={
    {"删除文件?" , "刪除文件?" , "Delete file?" , "ファイルを削除しますか?" , "Удалить файл?" },
    {"删除"      , "刪除"      , "Delete"      , "削除"                    , "Удалить"      },
    {"确认"      , "確認"      , "Yes"     , "確認"                    , "Подтвердить"  },
    {"取消"      , "取消"      , "No"      , "キャンセル"              , "Отмена"       },
    {"判定条件"  , "判定條件"  , "Sorting Criteria" , "選別条件"             , "Условия сортировки"}
};
/*----------------------------------------------------------------------------*/
// [1. Simplified Chinese (SC)]
//  !-.0123456789:?ABEKNSTVZghilnsРийксу不中主于亮从他件低体关其册出分动厂取号
// 否商型基复子屏州市幕广度开恢户手文日时星是暗期本机测消版用电界础硬
// 确简繁置翼自言語认设语软选重量钟闭间面音高
//
// [2. Traditional Chinese (TC)]
//  !-.0123456789:?ABEKNSTVZghilnsРийксу不中主亮他件低体其冊出分動取否商型基子
// 屏州市幕度廠廣從復恢戶手文於日星是時暗期本機消測版用界硬確礎简繁置
// 翼自號言設認語軟選重量鐘閉開間關電面音高
//
// [3. English (EN)]
//  !-.0123456789:?ABCDEFGHKLMNOPRSTUVWXYZabcdeghilmnoprstuvwyzРийксу中体文日本简繁語
//
// [4. Japanese (JP)]
//  !-.0123456789:?ABEKNSTVZeghilnrsРийксуいえかさしすそでなのはまるアオカソト
// ドニハフホマムメュルー中他付低体元分初別動化型基子定州市広復文日明
// 星時期本測源画番简約繁翼自言設語起選量間電面音高
//
// [5. Russian (RU)]
//  !-.0123456789:?ABEKNSTVXZceghilnsyАБВГДИМНОПРСЯабвгдезийклмнопрсту
// цчыьэюя

/* 设置界面 */
static const char *const FONT_SETTINGS_TEXT[][GUI_LV_LANGUAGE_MAX]={
    {"设置"       , "設置"       ,  "Settings"      ,"設定"        , "Настройки"},
    {"1.开机界面" , "1.開機界面" ,  "1.Startpage"   ,"1.起動画面",            "1.Начальный экран"},
	{"2.语言设置" , "2.語言設置" ,  "2.Language"    ,"2.言語設定",            "2.Настройки языка"},
	{"3.音量设置" , "3.音量設置" ,  "3.Vol Set"     ,"3.音量設定",            "3.Настройки громкости"},
	{"4.屏幕亮度" , "4.屏幕亮度" ,  "4.Brightness"  ,"4.画面の明るさ",        "4.Яркость экрана"},
    {"5.暗屏时间" , "5.暗屏時間" ,  "5.Screensaver" ,"5.画面オフまでの時間",  "5.Время затемнения экрана"},
    {"6.自动关机" , "6.自動關機" ,  "6.Power OFF"   ,"6.自動電源オフ",        "6.Автоматическое выключение"},
    {"7.日期设置" , "7.日期設置" ,  "7.Date Set"    ,"7.日付設定",            "7.Настройка даты"},
    {"8.时间设置" , "8.時間設置" ,  "8.Time Set"    ,"8.時間設定",            "8.Настройка времени"},
    {"9.其他设置" , "9.其他設置" ,  "9.Other"       ,"9.その他設定",          "9.Другие настройки"},
    {"10.关于"    , "10.關於"    ,  "10.About"      ,"10.約",                 "10.О программе"},
};

/* 1、启动界面选项 */
static const char *const  FONT_STARTPAGE_TEXT[][GUI_LV_LANGUAGE_MAX] = {
    {"主界面"   , "主界面"   , "Home"         , "ホーム", "Главный экран"},
	{"基本测量" , "基本測量" , "Basic Test"   , "基本測定", "Базовые измерения"},
	{"分选测量" , "分選測量" , "Sorting Test" , "選別測定", "Сортировка"},
};
/* 2、语言选项 */
static const char *const FONT_LANG_TEXT[GUI_LV_LANGUAGE_MAX] = {
	"简体中文", "繁体中文", "English", "日本語", "Русский"
};
/* 3、音量选项 */
static const char *const  FONT_VOLUME_TEXT[][GUI_LV_LANGUAGE_MAX] = {
    {"低"  , "低 " , "Low"   , "低",   "Низк."},
    {"中"  , "中 " , "Mid"   , "中",   "Сред."},
    {"高"  , "高 " , "High"  , "高",   "Высок."},
    {"关闭", "關閉", "Close" , "オフ", "Выкл."},
};
/* 4、屏幕亮度 */
static const char *const FONT_BRIGHTNESS_TEXT[GUI_LV_LANGUAGE_MAX] = {
    "屏幕亮度" , "屏幕亮度" , "Brightness"  , "画面の明るさ",  "Яркость экрана"
};
/* 5、暗屏时间 */
static const char *const  FONT_SCREENSAVE_TEXT[][GUI_LV_LANGUAGE_MAX] = {
    {"1分钟"    , "1分鐘"    , "1 Min"    , "1分",   "1 мин."},
    {"5分钟"    , "5分鐘"    , "5 Min"    , "5分",   "5 мин."},
    {"10分钟"   , "10分鐘"   , "10 Min"   , "10分",  "10 мин."},
    {"30分钟"   , "30分鐘"   , "30 Min"   , "30分",  "30 мин."},
    {"从不"     , "從不"     , "Never"    , "なし",  "Никогда"},
};
/* 6、关机时间 */
static const char *const  FONT_OFFTIME_TEXT[][GUI_LV_LANGUAGE_MAX] = {
    {"5分钟"    , "5分鐘"    , "5 Min"    , "5分",   "5 мин."},
    {"10分钟"   , "10分鐘"   , "10 Min"   , "10分",  "10 мин."},
    {"30分钟"   , "30分鐘"   , "30 Min"   , "30分",  "30 мин."},
    {"60分钟"   , "60分鐘"   , "60 Min"   , "60分",  "60 мин."},
    {"从不"     , "從不"     , "Never"    , "なし",  "Никогда"},
};
/* 7、日期设置 */
static const char *const FONT_DATESET_TEXT[][GUI_LV_LANGUAGE_MAX] = {
    {"年" , "年" , "Year" , "年", "Год"},
    {"月" , "月" , "Mon"  , "月", "Месяц"},
    {"日" , "日" , "Day"  , "日", "День"},
};
/* 8、时间设置 */
static const char *const FONT_TIMESET_TEXT[][GUI_LV_LANGUAGE_MAX] = {
    {"时" , "時" , "Hour" , "時", "Час"},
    {"分" , "分" , "Min"  , "分", "Мин"},
    {"秒" , "秒" , "Sec"  , "秒", "Сек"},
};

/* 9、其他设置 */
static const char *const FONT_OTHER_TEXT[][GUI_LV_LANGUAGE_MAX]={
    {"恢复出厂" , "恢復出廠" , "Factory Reset" , "初期化",   "Сброс"},
    {"用户手册" , "用戶手冊" , "User Manual"   , "マニュアル", "Инструкция"},
};
//(1) 恢复出厂 (2) 用户手册
/* 恢复出厂选项 */
static const char *const FONT_RESET_TEXT[][GUI_LV_LANGUAGE_MAX]={
    {"注意!"     , "注意!"     , "Warning!"         , "注意!",        "Внимание!"},
    {"恢复出厂设置会重置之前所有的设置!是否恢复?" , "恢復出廠設置會重置之前所有的設置!是否恢復?" , "Factory reset will reset all previous settings! Sure to restore?" , "初期化すると、これまでの設定がすべてリセットされます！復元しますか？", "Сброс настроек приведет к удалению всех предыдущих настроек! Восстановить?"},
    {"确认"      , "確認"      , "Yes"              , "はい",          "Да"},
    {"取消"      , "取消"      , "No"               , "いいえ",        "Нет"},
};
/* 用户手册 */
static const char *const FONT_MANUAL_TEXT[][GUI_LV_LANGUAGE_MAX]={
    {"用户手册" , "用戶手冊" , "User Manual" , "マニュアル", "Инструкция"},
};
/* 10、关于*/
static const char *const  FONT_ABOUT_TEXT[][GUI_LV_LANGUAGE_MAX] = {
	{"厂商: 广州市星翼电子"   , "廠商: 廣州市星翼電子"   , "MFR: Guangzhou Xingyi Electronic" , "メーカー: 広州市星翼電子", "Произв.: Xingyi Elec."},
    {"型号: ATK-BZ"           , "型號: ATK-BZ"           , "Model: ATK-BZ"                    , "型番: ATK-BZ",              "Модель: ATK-BZ"},
    {"SN: 123456789"          , "SN: 123456789"          , "SN: 123456789"                    , "SN: 123456789",             "SN: 123456789"},
	{"软件版本: V"            , "軟件版本: V"            , "SW Ver: V"                        , "ソフトVer: V",              "ПО Вер.: V"},
	{"硬件版本: V"            , "硬件版本: V"            , "HW Ver: V"                        , "ハードVer: V",              "Апп. Вер.: V"},
};

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif
