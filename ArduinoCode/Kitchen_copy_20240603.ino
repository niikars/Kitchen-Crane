/*
Kitchen Crane by Nikolay Krasilnikov
created 14-01-2024
updated 20.01.2024-21.01.2024 25.01.2024 27.01.2024 04.02.2024 04.03.2024
        08.03.2024 14.03.2024-18.03.2024 04.04.2024 06.04.2024-07.04.2024
        31.05.2024 03.06.2024
*/
  //ВРЕМЯ
// Пауза для основной петли
#define MAIN_TOUT 300
//Пауза между операциями
#define OPER_TOUT 100
//Максимальное время удерживания крана для сброса касания
#define TUCH_TOUT 1000
//Минимальное время удерживания крана для сброса касания
#define MIN_TUCH_TOUT 500
// Пауза для компенсации звона контактов
#define BOUNCE 10000
// Время работы
#define Mill_TOUT 5000 
#define DEAD_LOCK 1000
  //Разъёмы
// Разъём РГ6_1 Кнопки
#define B_Stop 40  //Чёрный стоп
#define B_Cr   38  //Синий кран 
#define B_Cold 42  //Красный холодная
#define B_FCr  48  //Жёлтыйполный кран
#define B_Dr   52  //Зелёный питьевая
#define B_FDr  50  //Белый полный питьевая
// Разъём РГ6_2 Светодиоды
#define D_Stop  9  //Красный стоп
#define D_Cold  4  //Зелёный холодная
#define D_Cr   15  //Синий кран
#define D_FCr   5  //Жёлтый полный кран
#define D_Dr   11  //Чёрный питьевая
#define D_FDr   6  //Белый полный питьевая

// Разъём РГ6_3 Клапана
#define V_M    35  //Синий мельница
#define V_Cold 32  //Жёлтый холодная
#define V_Cr   33  //Зелёный кран
#define V_FCr  36  //Белый полный кран
#define V_Dr   37  //Красный питьевая
#define V_FDr  34  //Чёрный полный питьевая

// Разъём РГ6_4 Питание
//Зелёный земля
//Жёлтый питание клапанов 12вольт
#define U_Int 2   //Синий Int3 прерывание по подаче силового питания (разрешение на работу)
#define B_Int 3   //Белый Int5 прерывание по нажатию одной из кнопок управления
//Красный питание 5вольт
//Чёрный земля

// Разъём РГ6_5 Сенсоры
// При отсоединённых сенсорах необходимо установить заглушку 
// выходов S_Cr, S_Dr на землю!
#define S_Cr 20    //Зелёный датчик крана Int1
#define S_Dr 21    //Жёлтый датчик питьевая Int0
//Синий земля
#define S_Cr_5 51  //Белый питание сенсора крана
#define S_Dr_5 53  //Красный питание сенсора питьевой воды
//Чёрный земля
//Индикотор работы мельницы
#define M_I 19     // мельница Int2

//Константы управления
//Управление клапанами
#define OPEN LOW
#define CLOSED HIGH
//Индикатор включения мельницы
#define MILL_ON LOW

const char * IntBut_s = "BUTTONS";
const char * IntWorkPermition_s = "WorkPermition";
const char * IntMillWorking_s= "MillWorking";
const char * IntSensorDr_s = "SensorDr";
const char * IntSensorCrane_s = "SensorCrane";
const char * NoInt = 0;

char * IntBut = (char *)NoInt;
char * IntWorkPermition = (char *)NoInt;
char * IntMillWorking = (char *)NoInt;
char * IntSensorDr = (char *)NoInt;
char * IntSensorCrane = (char *)NoInt;

// Состояния кнопок
// При нажатии кнопки её режим изменяется на противоположный
// Кнопка проверяется на звон,
// а так же повторное нажатие детектируется лишь спустя DEAD_LOCK
volatile bool State_Cr = false;
volatile bool State_FCr = false;
volatile bool State_Dr = false;
volatile bool State_FDr = false;
volatile bool State_Cold = false;
// Переменные для анализа касания или отпускания крана
volatile int Old_SCr = LOW;
volatile int Old_SDr = LOW;
// Касание кухонного крана (факт касания кухонного крана)
volatile bool State_S_Crane = false;
// Касание питьевого крана (факт касания питьевого крана)
volatile bool State_S_Dr = false;
//Состояние режима клапанов 
volatile bool State_V_Cr = false;
volatile bool State_V_FCr = false;
volatile bool State_V_Dr = false;
volatile bool State_V_FDr = false;
volatile bool State_V_Cold = false;
//Признак наличия напряжения на клапанах - разрешение на работу.
volatile bool State_W12 = false;
volatile bool OldState_W12 = false;
//Признак Работы клапана мельницы.
volatile bool State_V_M = false;
//Признак Работы мельницы.
volatile bool State_M = false;
// Время работы канала мельницы
volatile long W_V_M = 0;

// Системное время изменения состояния кнопки
volatile unsigned long mi_B_Cr = 0;
volatile unsigned long mi_B_FCr = 0;
volatile unsigned long mi_B_Cold = 0;
volatile unsigned long mi_B_Dr = 0;
volatile unsigned long mi_B_FDr = 0;

// Системное время текущего цикла
volatile unsigned long curMi = 0;

// Системное время включения питания
volatile unsigned long mi_12 = 0;

// Системное время состояния касания кухонного крана
volatile unsigned long mi_S_Cr = 0;

// Блокировка всех функций при первоначальной настройке.
volatile bool Starting = true;

// Отладочные переменные.
volatile bool debug1 = false;
volatile bool debug2 = false;
