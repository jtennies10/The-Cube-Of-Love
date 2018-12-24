/*
 * The code written in this file is a modified version of LookingGlass' open source 6x6x6 L3D Cube program.
 * Specifically, the original file was x6CubeMusicPack.h
 * The original program's GitHub location is linked in this project's GitHub location.
 */


#include <vector>
using namespace std;
/********************************************
 * Global variables & functions             *
 ********************************************/
#define ON              1
#define OFF             0
#define CUBE_SIZE       6
#define MICROPHONE      1
#define GAIN_CONTROL    3
#define INPUT_GAIN      0

#define STATIC_HEART    0
#define STATIC_S        1

#define  FIREWORKS      2
#define  PLASMA         3
#define  RAINBOW3D      4
#define CUBESPANDER     5
#define ZEROGRAIN       6
#define HEARTATTACK     7

#define FFT_JOY         8
#define GOLDENRAIN      9
#define PURPLERAIN      10
#define ACIDRAIN        11
#define FFT_FIRE        12
#define MUSICTOWER      13

#define DEMO_ROUTINES   14




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

//jtennies10
const int STATIC_START = 0;
const int DYNAMIC_START = 2;
const int MUSIC_START = 8;

enum Categories{still=STATIC_START, dynamic=DYNAMIC_START, music=MUSIC_START};
Categories animationType;


/*******************************
 * fireworks variables and functions*
 * ****************************/
 int centerX, centerY, centerZ;
int launchX, launchZ, fadeTimer;
float radius=0;
float speed;
bool showRocket;
bool exploded;
float xInc, yInc, zInc;
float rocketX, rocketY, rocketZ;
float launchTime;
int maxSize;
Color rocketColor, fireworkColor;
void initFireworks();
void prepRocket();
void updateFireworks();

/********************************
 * zplasma variables and functions*
 * *****************************/
float phase = 0.0;
float phaseIncrement = 0.03; // Controls the speed of the moving points. Higher == faster
float colorStretch = 0.1; // Higher numbers will produce tighter color bands 
float plasmaBrightness = 0.2;
Color plasmaColor;
void zPlasma();

/*********************************
 * CubeSpander variables and functions*
 * *******************************/

int side=0;
int inc=1;
int mode=0;
//int frame=0;
Color cubeCol;
void drawCube(Point topLeft, int side, Color col);
void drawLine(Point p1, Point p2, Color col);
void mixVoxel(Point currentPoint, Color col);
Color complement(Color original);
void cubeInc();
void CubeSpander();

/*********************************
 * ZeroGRain variables and function*
 * *******************************/
int Gaxis = 1;    // Which axis are we crumbling through?  0=x, 1=y, 2=z.
int flips;
const int NUM_FLIPS = 5;
Color readVoxel;
void resetZeroGRainCycle();
void ZeroGRainCycle();

/*********************************
 * Rainbow3D variables and function*
 * *******************************/
void Rainbow3D();

/***********************************
 * jtennies10                      *
 * heartAttack                     *
 ***********************************/
 const short heart[][6] = {
  {0, 0, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 0, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0}
 };
int heartAttackCounter = 0;
bool heartForward = true;
Color heartColor(198, 40, 40);
const int MIN_HEART_RED = 120;
const int MAX_HEART_RED = 200;
int colorChange = 1; //used to control whether color values are rising or falling
Color heartAttack(Color voxelColor);



/********************************************
 * jtennies10                        
 * staticHeart*
 ********************************************/
 void staticHeart();

/********************************************
 * jtennies10                        
 * staticS
 ********************************************/
 void staticS();
 const short sMatrix[][6] = {
  {0, 0, 0, 0, 0, 0},
  {1, 0, 0, 1, 0, 0},
  {1, 0, 1, 0, 1, 0},
  {1, 0, 1, 0, 1, 0},
  {0, 1, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0}
 };
 



/********************************************
 * Global functions                         *
 ********************************************/
void fade();
void initMicrophone();
void add(Point& a, Point& b);
void fadeSmooth(char lowerLim, char upperLim, float scaleFactor);
void smoothFade();
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
void musicTower();
void flyingCurve();
void initSquarePointB();
void initRing();




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
