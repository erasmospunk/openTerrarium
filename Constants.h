
#define RELAY_PIN A0

#define ONE_WIRE_BUS 12

#define BUZZER_PIN A1

#define ZC_INT 0 //pin 2
#define TRIAC_PIN 3


// LCD software params
#define LCD_LINES            4
#define LCD_LINE_SIZE        20
#define LCD_MIN_BRIGHTNESS   95
// LCD hardware params
#define LCD_DATA_PIN         7
#define LCD_CLOCK_PIN        6
#define LCD_BRIGHTNESS_PIN   5
#define LCD_ENABLE_PIN       4


#define BUTTONS_A_PIN        11
#define BUTTONS_B_PIN        10
#define BUTTONS_C_PIN        9
#define BUTTONS_INPUT_PIN    8

#define BUTTONS_SPEED 10

//#define TEMP_UPDATE_PERIOD 789 //ms. the fastest!
#define TEMP_UPDATE_PERIOD 1000 //ms. Must be >750ms

#define PROGRAM_SPEED 10

/* Useful Constants */
#define HOURS_PER_DAY  (24UL)
#define HOURS_PER_HALF_DAY  (12UL)
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * HOURS_PER_DAY)
#define SECS_PER_HALF_DAY  (SECS_PER_HOUR * HOURS_PER_HALF_DAY)

