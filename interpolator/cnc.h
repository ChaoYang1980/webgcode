#pragma once

typedef struct __attribute__((__packed__)) {
    uint32_t stepsPerMillimeter, maxSpeed, maxAcceleration, clockFrequency;
} parameters_t;

typedef struct __attribute__((__packed__)) {
    int32_t x, y, z, speed;
} position_t;

typedef struct __attribute__((packed)) {
    uint8_t xStep, xDirection;
    uint8_t yStep, yDirection;
    uint8_t zStep, zDirection;
} axes_t;

typedef struct {
    uint16_t duration;
    axes_t axes;
} step_t;

typedef struct {
    float32_t x;
    float32_t y;
    float32_t z;
} vec3f_t;

typedef enum {
    NOT_RUNNING = 0,
    DIRECTION_SET = 1,
    STEP_SET = 2
} stepper_state_t;

typedef struct {
    int run: 1;
} __attribute__((packed)) spindle_output_t;

typedef union {
    uint8_t n;
    spindle_output_t s;
} spindle_output_serializer_t;

typedef struct {
    position_t position;
    parameters_t parameters;
    uint16_t state;
    int32_t lastEvent[4];
    step_t currentStep;
    uint64_t tick;
    spindle_output_t spindleOutput;
    uint8_t unfilteredSpindleInput;
    uint8_t spindleInput;
} cnc_memory_t;

typedef enum {
    EP_IN = 0b10000000U,
    EP_OUT = 0b00000000U
} EndpointDirection_t;

typedef enum {
    READY = 0,
    RUNNING_PROGRAM = 1,
    MANUAL_CONTROL = 2,
    ABORTING_PROGRAM = 3,
    PAUSED_PROGRAM = 4
} cnc_state_t;

enum {
    NULL_EVENT = 0,
    PROGRAM_END = 1,
    PROGRAM_START = 2,
    MOVED = 3,
    ENTER_MANUAL_MODE = 4,
    EXIT_MANUAL_MODE = 5
};

//http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html

#define crBegin static int state=0; switch(state) { default:break; case 0:
#define crFinish state=0;}
#define crReturn do { state=0; return; } while (0)
#define crComeBackLater do { state=__LINE__; return; \
                         case __LINE__:; } while (0)

#define INTERRUPT_PACKET_SIZE         24
#define INTERRUPT_ENDPOINT_NUM        1
#define INTERRUPT_ENDPOINT_DIR        EP_IN
#define INTERRUPT_ENDPOINT            (INTERRUPT_ENDPOINT_DIR|INTERRUPT_ENDPOINT_NUM)

#define BULK_PACKET_SIZE              64
#define BULK_ENDPOINT_NUM             1
#define BULK_ENDPOINT_DIR             EP_OUT
#define BULK_ENDPOINT                 (BULK_ENDPOINT_DIR|BULK_ENDPOINT_NUM)

extern volatile cnc_memory_t cncMemory;

extern uint32_t isEmergencyStopped();

extern uint32_t isToolProbeTripped();

extern void initUSB();

extern int32_t readFromProgram(uint32_t count, uint8_t *array);

extern void checkProgramEnd();

extern uint8_t *cncGetCfgDesc(uint8_t speed, uint16_t *length);

extern void zeroJoystick();

extern step_t nextManualStep();

extern void initManualControls();

extern uint32_t toggleManualMode();

extern void periodicUICallback();

extern void copyUSBufferIfPossible();

extern void tryToStartProgram();