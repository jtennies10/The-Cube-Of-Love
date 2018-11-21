/*
 * The code written in this file is a modified version of LookingGlass' open source 6x6x6 L3D Cube program.
 * Specifically, the original file was x6CubeMusicPack.h
 * The original program's GitHub location is linked in this project's GitHub location.
 */

/********************************************
 * Global variables & functions             *
 ********************************************/
#define ON              1
#define OFF             0
#define CUBE_SIZE       6
#define MICROPHONE      1
#define GAIN_CONTROL    3
#define INPUT_GAIN      0

#define FFT_JOY         0
#define GOLDENRAIN      1
#define PURPLERAIN      2
#define ACIDRAIN        3
#define FFT_FIRE        4
#define MUSICTOWER      5
#define MUSICRING       6
#define FFT_JOY3        7
#define MUSICRAINBOW    8

#define DEMO_ROUTINES   9


/********************************************
 * Global variables                         *
 ********************************************/
Cube cube=Cube(CUBE_SIZE,80);  //cube=Cube();
int mic;
int demo;
int delayVal;
int brightness;
int frameCount;
int maxAmplitude;

/********************************************
 * Global functions                         *
 ********************************************/
void fade();
void initMicrophone();
void add(Point& a, Point& b);
void fadeSmooth(char lowerLim, char upperLim, float scaleFactor);
//void smoothFade(float scaleFactor);
void drawLine(Point p1, Point p2, Color col);
void mixVoxel(Point currentPoint, Color col);
void setVoxel(int x, int y, int z, bool clear);
void drawCube(Point topLeft, int side, Color col);
void mixVoxel(Point currentPoint, Color col, float opacity);
int draw();
bool shift();
float distance(float x, float y, float z, float x1, float y1, float z1);
Color Wheel(byte WheelPos);
Color complement(Color original);


/********************************************
 * Samtim's global variables                *
 ********************************************/
long trendAverage, runningAverage;
int runningPeriod;
int averageSampleNum, averageAmplitude;
static int wholeAverage[30];
static float colorInc;
Color readColor;
Point squarePointB[36];
Point ring[24];

/********************************************
 * Samtim's global functions                *
 ********************************************/
//int getTrendAverage(int sampleNum);
//int getAverageAmplitude(int sampleNum);
//void addAverageToChain( int arg);

/********************************************
 * Samtim's modes' functions                *
 ********************************************/
void FFTJoy2();
void FFTJoy3();
void musicRing();
void musicTower();
void musicRainbow();
void flyingCurve();
void initSquarePointB();


/********************************************
 * FFTJoy variables & functions             *
 ********************************************/
#define SAMPLES         2048
#define M               4   // If the M value changes, then the 'ARRAY_SIZE' constant also needs changing
#define ARRAY_SIZE      16  // to be changed to reflect the result of the formula: pow(2,M)
#define fftSampleNum    2

float real[ARRAY_SIZE], imaginary[ARRAY_SIZE];  //real[REA];
//float imaginary[IMA];
float maxValue;
float sample;

void FFTJoy();
short FFT(short int dir,int m,float *x,float *y);


/********************************************
 * Purple/Golden Rain variables & functions *
 ********************************************/
#define MAX_POINTS            36
#define SPEED                 0.1 //0.22
#define MIN_SALVO_SPACING     0.25
//0=BASE|1=CENTER|2=TOP|3=RANDOM
#define BASE                  0
#define CENTER                1
#define TOP                   2
#define RANDOM                3
//Samtim's modes' defines
#define pi                    3.1416
#define colorSpeed            0.005
#define getSinewaveSampleNum  2

int maximum, minimum;
int threshhold, timeAboveThreshhold;
int fadingMax, ledColor;
char startAt;    // [0=BASE|1=CENTER|2=TOP|3=RANDOM]
float sensitivityPurpleRain;
bool aboveThreshhold;

/* type definitions */
typedef struct {
  Point raindrop;
    float speed;
    Color color;
    bool flipped;
    bool dead;
} raindrop;

typedef struct {
    raindrop raindrops[MAX_POINTS];
    bool dead;
} salvo;
 
salvo salvos[CUBE_SIZE];

void initSalvos();
void drawSalvos();
void updateSalvos();
void checkMicPurpleRain();
void launchRain(int amplitude);
void fadeBase(float scaleFactor);
float setNewSpeed();


/********************************************
 * 3D Audio Rainbow variables & functions   *
 ********************************************
#define UPDATE_RATE       15
#define NUM_SAMPLES       25

// Rainbow variables
unsigned char SAMTIM_MODE, VU_MODE, PEAK_HOLD, DOT_MODE;
byte currentBg, vr;
uint16_t ySpan;
float sensitivityAudioRainbow;
Color colorA, colorB, cubeColor;

// Function declarations
Color Wheel(byte WheelPos, float opacity);
Color adjustGamma(Color col, float opacity);
void doBlanking();
void fade(float speed);
void fadeTopDown(float speed, bool wipe);
void checkMicAudioRainbow();
void initMicrophone();
*/

/********************************************
 * flip variables & functions               *
 ********************************************/
 //accelerometer pinout
#define X                 10
#define Y                 12
#define Z                 11
#define AUTOCYCLE_TIME    32222
#define FACEPLANT         1700
#define LOCK              1700
#define UPSIDE_DOWN       1850
#define RIGHTSIDE_UP      1670   //this is different from 8x8x8 Cube, as the accelerometer is on the bottom side.
#define LEFT_SIDE         2270
#define RIGHT_SIDE        1730
#define FLIP_TIMEOUT      5000
#define FLIP_DEBOUNCE     200

long lastFaceplant;
bool upsideDown;
bool sideways;
bool autoCycle;
int upsideDownTime;
long lastAutoCycle;
int lastLeft;
int lastRight;
int accelerometer[3];
long lastChange;

void checkFlipState();
void updateAccelerometer();
void incrementDemo();
void decrementDemo();


/********************************************
 * fade variables & functions               *
 ********************************************/
bool fading;
int fadeValue;
int fadeSpeed;

void setFadeSpeed();
