/*
 * The code written in this file is a modified version of LookingGlass' open source 6x6x6 L3D Cube program.
 * The original program's GitHub location is linked in this project's GitHub location.
 */

#include <beta-cube-library.h>
//#include "beta-cube-library.h" 
#include "L3D_CubePixel.h" 
//#include <vector>
#include "The_Cube_Of_Love.h"
#include <math.h> 





void setup() {
/*************************************
 * init global variables             *
 *************************************/
  delayVal=10;
//  demo=FFT_JOY;
  demo=STATIC_COLOR_GANNON;
  animationType = still;
  
  frameCount=0;
  brightness=50;
  //maxBrightness=50;
  //initMicrophone();  
  
/*************************************
 * init Cube object                  *
 *************************************/
  cube.begin();

/*************************************
 * init FFTJoy variables             *
 *************************************/
  maxValue=255; // Maybe set this value to 120 or 255 to avoid peaking at startup

/*************************************
 * init GoldenRain variables         *
 *************************************
  runningAverage=previousRunningAverage=0;
  readTime=colorTime=0;
  mic=readSampleNum=amplitude=0;
  */

/*************************************
 * init Purple/Golden Rain variables *
 *************************************/
  maximum=minimum=0;
  aboveThreshhold=false;
  fadingMax=25;
  sensitivityPurpleRain=.1;
  initSalvos();

/*************************************
 * init 3D Audio Rainbow variables   *
 *************************************
  currentBg=0;
  sensitivityAudioRainbow=0.98;
  maxAmplitude=0, 
  vr = random(cube.size, 256);
  */

/********************************************
 * init Samtim's global variables           *
 ********************************************/
trendAverage=0;
runningAverage=0;
averageSampleNum=30;
averageAmplitude=0;
colorInc=0;

initSquarePointB();
initRing();
initFireworks();

/*************************************
 * init flip variables               *
 *************************************/
  lastAutoCycle=lastChange=0;
  upsideDown=sideways=false;
  autoCycle=true;    //start on autocycle by default
  upsideDownTime=-1*FLIP_TIMEOUT;
  lastFaceplant=-1*FLIP_TIMEOUT;
  lastLeft=-1*FLIP_TIMEOUT;
  lastRight=-1*FLIP_TIMEOUT;

/*************************************
 * init fade variables               *
 *************************************/
  fading=false;
  fadeValue=255;
  fadeSpeed=1;


   
}

void initMicrophone() {
  pinMode(GAIN_CONTROL, OUTPUT);
  analogWrite(GAIN_CONTROL, INPUT_GAIN);  //digitalWrite(GAIN_CONTROL, LOW);
}

void loop() {
  if(fading) {
    fadeValue--;
    if(fadeValue>0)
      fade(2);
    //if we're done fading
    else {
      fading=false;
      fadeValue=255;
    }
    cube.show();
  }
  else {
    /*****************************************/
    /* THE LINES BELOW SHOULD BE UNCOMMENTED */
    /* WHEN COMPILING EACH MODE INDIVIDUALLY */
    /* TO CREATE THE VISUALIZATIONS          */
    /*****************************************/
//        autoCycle=false;
//        demo=STATIC_S;
     /*****************************************/
    
    if((demo==FIREWORKS && exploded))
            fade(); //cube.background(black);
        else if (demo!=FFT_JOY)
            smoothFade();
    
    switch(demo) {
      case(FIREWORKS):
        updateFireworks();
  //      Serial.println("F");
        break;
      case(PLASMA):
        zPlasma();
     //     Serial.println("P");
        break;
      case(RAINBOW3D):
        Rainbow3D();
        break;
        
      case(ZEROGRAIN):
        ZeroGRainCycle();
        break;
                       
      case(CUBESPANDER):
        CubeSpander();
        break;

      
      case(FFT_JOY):
        FFTJoy();
      //     Serial.println("FT");
        break;

      case(GOLDENRAIN):
        //GoldenRain();
        startAt=TOP; // Where do we want our raindrops to start from?
        fadeSmooth(0, cube.size, 0.3);
        checkMicPurpleRain();
        updateSalvos();
        drawSalvos();
        delay(20);
        break;
      
      case(PURPLERAIN):
        startAt=RANDOM; // Where do we want our raindrops to start from?
        fadeSmooth(0, cube.size, 0.5);
        checkMicPurpleRain();
        updateSalvos();
        drawSalvos();
        delay(15);
        break;
      
      case(ACIDRAIN):
        startAt=TOP; // Where do we want our raindrops to start from?
        fadeSmooth(1, cube.size, 0.25);
        checkMicPurpleRain();
        updateSalvos();
        drawSalvos();
        ledColor++;
        if(ledColor>400) ledColor=0;
        if((ledColor%5)==0) 
          fadeSmooth(0, 1, 0.0625);
        else 
          fadeSmooth(0, 1, -0.073);
        delay(25);
        break;
      
    /********************************************
     * Samtim's modes                           *
     ********************************************/
        case(FFT_FIRE):
          FFTJoy2();
          ledColor++;
          if(ledColor>400) ledColor=0;
          //delay(15);
          break;
        
        case(MUSICTOWER):
          musicTower();
          break;  

        //jtennies10
        //beginning of user-made functions
        case(HEARTATTACK):
          heartColor = heartAttack(heartColor);
          break;


        case(STATIC_HEART):
          staticHeart();
          break;    

        case(STATIC_COLOR_GANNON):
          staticColorGannon();
          break;

        case(STATIC_COLOR_PURPLE):
          staticColorPurple();
          break;
        
        default:
          break;
    }
    
    frameCount++;
  }
  
  //check to see how if the cube has been flipped
  checkFlipState();
  
  cube.show();
}

/********************************
 * Fireworks functions *
 * *****************************/
void updateFireworks()
{
  if(showRocket)
  {
      cube.sphere(rocketX,rocketY,rocketZ,radius, rocketColor);
      rocketX+=xInc*.25;
      rocketY+=yInc*.25;
      rocketZ+=zInc*.25;
  }
  else if(exploded)
  {
      cube.shell(rocketX,rocketY,rocketZ,radius,fireworkColor);
      //if our sphere gets too large, restart the animation in another random spot
      if(radius>maxSize) {
        prepRocket();
      }
      else 
        radius+=speed;  //the sphere gets bigger
  }
  
  if(abs(distance(centerX,centerY,centerZ,rocketX, rocketY, rocketZ)-radius)<2)
  {
      showRocket=false;
      exploded=true;
  }
}

void prepRocket()
{
            showRocket=true;
            exploded=false;
            radius=0;
            centerX=rand()%8;
            centerY=rand()%8;
            centerZ=rand()%8;
            fireworkColor.red=rand()%brightness;
            fireworkColor.green=rand()%brightness;
            fireworkColor.blue=rand()%brightness;
            launchX=rand()%4;
            launchZ=rand()%4;
            rocketX=launchX;
            rocketY=0;
            rocketZ=launchZ;
            launchTime=1+rand()%15;
            xInc=(centerX-rocketX)/launchTime;
            yInc=(centerY-rocketY)/launchTime;
            zInc=(centerZ-rocketZ)/launchTime;
            speed=0.08;
            maxSize=2+rand()%6;
            //speed=rand()%5;
            //speed*=0.1;
}

void initFireworks()
{
    rocketColor.red=255;
    rocketColor.green=150;
    rocketColor.blue=100;
    prepRocket();
}

/********************************
 * zplasma functions *
 * *****************************/
 
void zPlasma()
{
Point p1,p2,p3,dist1,dist2,dist3;
phase += phaseIncrement;
// The two points move along Lissajious curves, see: http://en.wikipedia.org/wiki/Lissajous_curve
// We want values that fit the LED grid: x values between 0..6, y values between 0..6, z values between 0...6
// The sin() function returns values in the range of -1.0..1.0, so scale these to our desired ranges.
// The phase value is multiplied by various constants; I chose these semi-randomly, to produce a nice motion.
 p1 = { (sin(phase*1.000)+1.0) * cube.size, (sin(phase*1.310)+1.0) * cube.size,  (sin(phase*1.380)+1.0) * cube.size};
 p2 = { (sin(phase*1.770)+1.0) * cube.size, (sin(phase*2.865)+1.0) * cube.size,  (sin(phase*1.410)+1.0) * cube.size};
 p3 = { (sin(phase*0.250)+1.0) * cube.size, (sin(phase*0.750)+1.0) * cube.size,  (sin(phase*0.380)+1.0) * cube.size};

byte row, col, dep;

// For each row
for(row=0; row<cube.size; row++) {
float row_f = float(row); // Optimization: Keep a floating point value of the row number, instead of recasting it repeatedly.

// For each column
for(col=0; col<cube.size; col++) {
float col_f = float(col); // Optimization.

// For each depth
for(dep=0; dep<cube.size; dep++) {
float dep_f = float(dep); // Optimization.

// Calculate the distance between this LED, and p1.
 dist1 = { col_f - p1.x, row_f - p1.y,  dep_f - p1.z }; // The vector from p1 to this LED.
float distance1 = sqrt( dist1.x*dist1.x + dist1.y*dist1.y + dist1.z*dist1.z);

// Calculate the distance between this LED, and p2.
 dist2 = { col_f - p2.x, row_f - p2.y,  dep_f - p2.z}; // The vector from p2 to this LED.
float distance2 = sqrt( dist2.x*dist2.x + dist2.y*dist2.y + dist2.z*dist2.z);

// Calculate the distance between this LED, and p3.
 dist3 = { col_f - p3.x, row_f - p3.y,  dep_f - p3.z}; // The vector from p3 to this LED.
float distance3 = sqrt( dist3.x*dist3.x + dist3.y*dist3.y + dist3.z*dist3.z);

// Warp the distance with a sin() function. As the distance value increases, the LEDs will get light,dark,light,dark,etc...
// You can use a cos() for slightly different shading, or experiment with other functions.
float color_1 = distance1; // range: 0.0...1.0
float color_2 = distance2;
float color_3 = distance3;
float color_4 = (sin( distance1 * distance2 * colorStretch )) + 2.0 * 0.5;
// Square the color_f value to weight it towards 0. The image will be darker and have higher contrast.
color_1 *= color_1 * color_4;
color_2 *= color_2 * color_4;
color_3 *= color_3 * color_4;
color_4 *= color_4;
// Scale the color up to 0..7 . Max brightness is 7.
//strip.setPixelColor(col + (8 * row), strip.Color(color_4, 0, 0) );
plasmaColor.red=color_1*plasmaBrightness;
plasmaColor.green=color_2*plasmaBrightness;
plasmaColor.blue=color_3*plasmaBrightness;

cube.setVoxel(row,col,dep,plasmaColor);       
}
}
}
}


void Rainbow3D()
{
  uint16_t j;
  uint16_t x,y,z;
  for(j=0;j<256;j+=3){
    for(z=0;z<cube.size;z++)
     for(x=0;x<cube.size;x++)
       for(y=0;y<cube.size;y++)
       {
         cube.setVoxel(x,y,z,Wheel(((z*36+x*6+y)/2+j)&255));
       }
    cube.show();
    delay(10);
    checkFlipState();
    //if(demo!=RAINBOW3D) return;
  }
  
}

/****************************************
 * CubeSpander functions *
 * **************************************/
void CubeSpander()
{
   Point topLeft=Point(0, 0, 0);
   switch(mode) {
    case(0):
    topLeft=Point(0, 0, 0);
    cubeCol=Color(255, 0, 0);
    break;
    case(1):
    topLeft=Point(cube.size-1-side, 0, 0);
    cubeCol=Color(255, 255, 0);
    break;
    case(2):
    topLeft=Point(cube.size-1-side, cube.size-1-side, 0);
    cubeCol=Color(0, 255, 0);
    break;
    case(3):
    topLeft=Point(0, cube.size-1-side, 0);
    cubeCol=Color(0, 0, 255);
    break;
    case(4):
    topLeft=Point(0, 0, cube.size-1-side);
    cubeCol=Color(255, 0, 255);
    break;
    case(5):
    topLeft=Point(cube.size-1-side, 0, cube.size-1-side);
    cubeCol=Color(0, 255, 255);
    break;
    case(6):
    topLeft=Point(cube.size-1-side, cube.size-1-side, cube.size-1-side);
    cubeCol=Color(255, 255, 255);
    break;
    case(7):
    topLeft=Point(0, cube.size-1-side, cube.size-1-side);
    cubeCol=Color(0, 180, 130);
    break;
  }
  drawCube(topLeft, side, cubeCol);
  //if (frame%5==0)
    cubeInc();
  //frame++;
  cube.show();
  delay(100);
}

void drawCube(Point topLeft, int side, Color col)
{
  Point topPoints[4];
  Point bottomPoints[4];
  topPoints[0]=topLeft;
  topPoints[1]=Point(topLeft.x+side, topLeft.y, topLeft.z);
  topPoints[2]=Point(topLeft.x+side, topLeft.y+side, topLeft.z);
  topPoints[3]=Point(topLeft.x, topLeft.y+side, topLeft.z);
  Point bottomLeft=Point(topLeft.x, topLeft.y, topLeft.z+side);
  bottomPoints[0]=bottomLeft;
  bottomPoints[1]=Point(bottomLeft.x+side, bottomLeft.y, bottomLeft.z);
  bottomPoints[2]=Point(bottomLeft.x+side, bottomLeft.y+side, bottomLeft.z);
  bottomPoints[3]=Point(bottomLeft.x, bottomLeft.y+side, bottomLeft.z);
  for (int i=0; i<4; i++)
  {
    drawLine(topPoints[i], bottomPoints[i], col);
    drawLine(topPoints[i], topPoints[(i+1)%4], col);
    drawLine(bottomPoints[i], bottomPoints[(i+1)%4], col);
  }
  Color comp=complement(col);
  for (int i=0; i<4; i++)
  {
    cube.setVoxel(topPoints[i], comp);
    cube.setVoxel(bottomPoints[i], comp);
  }
}

void cubeInc()
{
  side+=inc;
  if ((side==cube.size-1)||(side==0))
    inc*=-1;
  if (side==0)
    mode++;
  if (mode>7)  
    mode=0;
}  

/****************************************
 * ZeroGRain functions *
 * **************************************/
 void setVoxel( int x, int y, int z, Color col ){
  
  int t;  // a temp int used to swap two values.
  switch( Gaxis ){
    case 0: t = y;  y = z;  z = t;  break;
    case 1: t = x;  x = z;  z = t;  break;
    case 2: t = x;  x = y;  y = t;  break;
  }

  cube.setVoxel( x, y, z, col );
}

 Color getVoxel( int x, int y, int z){
  
  int t;  // a temp int used to swap two values.
  switch( Gaxis ){
    case 0: t = y;  y = z;  z = t;  break;
    case 1: t = x;  x = z;  z = t;  break;
    case 2: t = x;  x = y;  y = t;  break;
  }

  return cube.getVoxel( x, y, z);
}

void ZeroGRainCycle(){
  
  if( ++flips>=NUM_FLIPS ){
    if( ++Gaxis>2 )
      Gaxis = 0;
    flips = 0;
  }

Color clearColor = Color( 0, 0, 0 );
Color mainColor = Color( 0, rand()%3*50, rand()%2*50 );
cube.background(black);
uint sum=true;
  for( int x=0; x<cube.size; x++ )
    for( int y=0; y<cube.size; y++ ){
        setVoxel( x, y, 0, mainColor );
        }
     cube.show();
     checkFlipState();
     //if(demo!=ZEROGRAIN) return;
  
  while(sum){   
    int x = rand()%6;
    int y = rand()%6;

    readVoxel=getVoxel(x,y,0);   
    if(readVoxel.green!=0||readVoxel.red!=0||readVoxel.blue!=0)
    for(int z=1;z<cube.size;z++){
    setVoxel( x, y, z, mainColor );
    setVoxel(x,y,z-1,clearColor);
    cube.show();
    checkFlipState();
    //if(demo!=ZEROGRAIN) return;
    }
    sum=0;
    for( x=0;x<cube.size;x++)
      for(y=0;y<cube.size;y++){
      readVoxel=getVoxel(x,y,0);
      sum+=readVoxel.green;
      sum+=readVoxel.blue;
      sum+=readVoxel.red;
      }
     
  }
  
//===============Flip===================
cube.background(black);
sum=true;
  for( int x=0; x<cube.size; x++ )
    for( int y=0; y<cube.size; y++ ){
        setVoxel( x, y, 5, mainColor );
        }
     cube.show();
     checkFlipState();
     //if(demo!=ZEROGRAIN) return;
  
  while(sum){   
    int x = rand()%6;
    int y = rand()%6;

    readVoxel=getVoxel(x,y,5);   
    if(readVoxel.green!=0||readVoxel.red!=0||readVoxel.blue!=0)
    for(int z=4;z>=0;z--){
    setVoxel( x, y, z, mainColor );
    setVoxel(x,y,z+1,clearColor);
    cube.show();
    checkFlipState();
    //if(demo!=ZEROGRAIN) return;
    }
    sum=0;
    for( x=0;x<cube.size;x++)
      for(y=0;y<cube.size;y++){
      readVoxel=getVoxel(x,y,5);
      sum+=readVoxel.green;
      sum+=readVoxel.blue;
      sum+=readVoxel.red;
      }
      checkFlipState();
  }
  
  delay(100);
}




//jtennies10
//creates a heart on the led that move in a 
//stagger motion from the back to front to back 
//of the cube and continually repeats
Color heartAttack(Color voxelColor) {

  heartAttackCounter++;
  if(heartAttackCounter > 10) {
    heartAttackCounter = 0;
    heartForward = !heartForward;
  }

  //only change led color every other movement to slow it down
  if(heartAttackCounter % 2 == 0) {
    if(voxelColor.red == MIN_HEART_RED) colorChange *= -1;
    else if(voxelColor.red == MAX_HEART_RED) colorChange *= -1;
    
    voxelColor.red-=(2 * colorChange);
    voxelColor.green-=(1 * colorChange);
    voxelColor.blue-=(1 * colorChange);
  }


  for(int x=0; x < cube.size; x++){
    for(int y=0; y < cube.size; y++) {

      //check if given (x,y) is part of heart
      //if so, then determine what z plane led should light up
      if(heart[x][y] == 1) {
        int z = heartAttackCounter - y;

        //if heart is moving backward, set z equal to its complement
        if(!heartForward) z = 5 - z;
        
        if(z < 0) z = 0;
        if(z > 5) z = 5;

        
        cube.setVoxel(x,y,z, voxelColor);

        //turn off last z plane if applicable
        if(z != 0) cube.setVoxel(x,y,(z-1), black);
        if(z != 5) cube.setVoxel(x,y,(z+1), black);
      }
    }
  }         
  delay(50);
  return voxelColor;
}

//jtennies10
//static displays
void staticHeart() {
  for(int x = 0; x < cube.size; x++) {
    for(int y = 0; y < cube.size; y++) {
      if(heart[x][y] == 1) { //only light up the front-most plane
        cube.setVoxel(x, y, 5, red);
      }
    }
  }
}

//jtennies10
void staticColorGannon() {
  Color voxelMaroon(5, 0, 0);
  Color voxelGold(204, 102, 0);
  voxelMaroon.red %= brightness;
  voxelMaroon.green %= brightness;
  voxelMaroon.blue %= brightness;
  voxelGold.red %= brightness;
  voxelGold.green %= brightness;
  voxelGold.blue %= brightness;
  for(int x = 0; x < cube.size; x++) {
    for(int y = 0; y < cube.size; y++) {
      for(int z = 0; z < cube.size; z++) {
        if(y % 2 == 0) {
          if((z % 2 == 0 && x % 2 == 0) || (z % 2 != 0 && x % 2 != 0)) {
            cube.setVoxel(x,y,z, voxelMaroon);
          }
        } else {
          if((z % 2 == 0 && x % 2 != 0) || (z % 2 != 0 && x % 2 == 0)) {
            cube.setVoxel(x,y,z, voxelGold);
          } 
        }
      }
    }
  }
}

//jtennies10
void staticColorPurple() {
  Color voxelColor(102, 0, 51);
  voxelColor.red %= brightness;
  voxelColor.green %= brightness;
  voxelColor.blue %= brightness;
  for(int x = 0; x < cube.size; x++) {
    for(int y = 0; y < cube.size; y++) {
      for(int z = 0; z < cube.size; z++) {
        if(z % 2 == 0) {
          if((y % 2 == 0 && x % 2 == 0) || (y % 2 != 0 && x % 2 != 0)) {
            cube.setVoxel(x,y,z, voxelColor);
          }
        } else {
          if((y % 2 == 0 && x % 2 != 0) || (y % 2 != 0 && x % 2 == 0)) {
            cube.setVoxel(x,y,z, voxelColor);
          }
        }
      }
    }
  }
}


void fade() {
  Color voxelColor;
  
  for(int x=0;x<cube.size;x++)
    for(int y=0;y<cube.size;y++)
      for(int z=0;z<cube.size;z++) {
        voxelColor=cube.getVoxel(x,y,z);
        if(voxelColor.red>0)
            voxelColor.red--;
        if(voxelColor.green>0)
             voxelColor.green--;
        if(voxelColor.blue>0)
            voxelColor.blue--;
        cube.setVoxel(x,y,z, voxelColor);   
   //     delayMicroseconds(100); 
      }
}

void fade(float speed) {
  Color voxelColor;
 
  for(int x=0;x<cube.size;x++)
    for(int y=0;y<cube.size;y++)
      for(int z=0;z<cube.size;z++) {
        voxelColor=cube.getVoxel(x,y,z);
        if(voxelColor.red>0)
          voxelColor.red-=speed;
        if(voxelColor.green>0)
          voxelColor.green-=speed;
        if(voxelColor.blue>0)
          voxelColor.blue-=speed;
        cube.setVoxel(x,y,z, voxelColor);    
      }
}

void smoothFade()
{
    Color voxelColor;
    for(int x=0;x<cube.size;x++)
        for(int y=0;y<cube.size;y++)
            for(int z=0;z<cube.size;z++)
            {
                voxelColor=cube.getVoxel(x,y,z);
                if(voxelColor.red>0)
                  voxelColor.red = voxelColor.red*.5;
                if(voxelColor.green>0)
                  voxelColor.green = voxelColor.green*.5;
                if(voxelColor.blue>0)
                  voxelColor.blue = voxelColor.blue*.5;
                cube.setVoxel(x,y,z, voxelColor);   
           //     delayMicroseconds(100); 
            }
}


/** Fade out all voxels to black
  @param scaleFactor is a percentage of the current color in decimal form, i.e 0.125 */
void fadeSmooth(char lowerLim, char upperLim, float scaleFactor) {
  for(int x=0;x<cube.size;x++)
    for(int y=lowerLim;y<upperLim;y++)
      for(int z=0;z<cube.size;z++) {
        Color voxelColor=cube.getVoxel(x,y,z);
        if(voxelColor.red > 0) voxelColor.red-=voxelColor.red*scaleFactor;
        //voxelColor.red=constrain(voxelColor.red*scaleFactor, 0, brightness);
        if(voxelColor.green > 0) voxelColor.green-=voxelColor.green*scaleFactor;
        //voxelColor.green=constrain(voxelColor.green*scaleFactor, 0, brightness);
        if(voxelColor.blue > 0) voxelColor.blue-=voxelColor.blue*scaleFactor;
        //voxelColor.blue=constrain(voxelColor.blue*scaleFactor, 0, brightness);
        cube.setVoxel(x,y,z, voxelColor);   
      }
}

/*********************
 * FFT JOY functions *
 *********************/
void FFTJoy() {
  for(int i=0; i<ARRAY_SIZE; i++) {
    real[i]=analogRead(MICROPHONE)-SAMPLES;
    delayMicroseconds(170);  /* 210 this sets our 'sample rate'.  I went through a bunch of trial and error to 
                              * find a good sample rate to put a soprano's vocal range in the spectrum of the cube
                              * 120 gets the entire range from bass to soprano within the cube's spectrum
                              * From bass to soprano: https://youtu.be/6jqCuE7C3rg */
    imaginary[i]=0;
  }
  
  FFT(1, M, real, imaginary);
  
  /* In this loop we can trim our 'viewing window', in regards to
   * what range of the audio spectrum we want to see at all times.
   * We do that by means of slightly 'shifting' the array index in
   * both imaginary[] and real[] arrays - the greater the index,
   * the more our 'window' will shift towards high frequencies. */
  for(int i=0; i<ARRAY_SIZE; i++) {
    int ii = (i+1) < ARRAY_SIZE ? i+1 : i;
    imaginary[i]=sqrt(pow(imaginary[ii],2)+pow(real[ii],2));
    if(imaginary[i]>maxValue)
      maxValue=imaginary[i];
  }

  //maxValue = (maxValue>=120) ? maxValue-2 : (maxValue<8) ? 8 : maxValue-.8;
  if(maxValue>=450) maxValue--;
  
  for(int i=0; i<pow(2,M)/2; i++) {
    imaginary[i]=cube.size*imaginary[i]/maxValue;
    Color pixelColor;
    int y, pixIdx, pixUppIdx, pixLowIdx;
    for(y=0; y<=imaginary[i]; y++) {
      pixelColor=y<cube.size-1 ? cube.colorMap(y,0,cube.size+1) : Color(191,0,7);
      pixIdx=(((cube.size-1)*cube.size*cube.size) + (i*cube.size) + y);
      cube.setVoxel(pixIdx, Color(pixelColor.red, pixelColor.green, pixelColor.blue));
      
      //Down-top fade/blanking
      pixLowIdx=pixIdx-(pixIdx%cube.size);
      for(int j=pixLowIdx; j<pixIdx; j++) {
        pixelColor=cube.getVoxel(j);
        //This causes a nice and smooth 'trailing' effect
        //from the base of the cube (y-axis) towards the peak
        if(pixelColor.red > 0) pixelColor.red-=pixelColor.red*(map(j%cube.size, 0, cube.size-1, cube.size-1, 1)*.07);
        if(pixelColor.green > 0) pixelColor.green-=pixelColor.green*(map(j%cube.size, 0, cube.size-1, cube.size-1, 1)*.07);
        if(pixelColor.blue > 0) pixelColor.blue-=pixelColor.blue*(map(j%cube.size, 0, cube.size-1, cube.size-1, 1)*.07);
        cube.setVoxel(j, Color(pixelColor.red, pixelColor.green, pixelColor.blue));
      }
    }
    
    //Topdown fade/blanking
    pixUppIdx=pixIdx+(cube.size-(pixIdx%cube.size));
    for(int j=pixUppIdx-1; j>pixIdx; j--) {
      pixelColor=cube.getVoxel(j);
      //This causes a nice and smooth 'trailing' effect
      //from the top of the cube (y-axis) towards the peak
      if(pixelColor.red > 0) pixelColor.red-=pixelColor.red*(map(j%cube.size, 0, cube.size-1, 1, cube.size-1)*.07);
      if(pixelColor.green > 0) pixelColor.green-=pixelColor.green*(map(j%cube.size, 0, cube.size-1, 1, cube.size-1)*.07);
      if(pixelColor.blue > 0) pixelColor.blue-=pixelColor.blue*(map(j%cube.size, 0, cube.size-1, 1, cube.size-1)*.07);
      cube.setVoxel(j, Color(pixelColor.red, pixelColor.green, pixelColor.blue));
    }
  }
    
  //Fade the 'trail' to black over the length of the cube's z-axis
  for(int x=0; x<cube.size; x++)
    for(int y=0; y<cube.size; y++)
      for(int z=0; z<cube.size-1; z++) {
        int pixIdx=((z+1)*cube.size*cube.size) + (x*cube.size) + y;
        Color trailColor=cube.getVoxel(pixIdx);
        
        //This is responsible for the 'meteors' shooting towards the back of 
        //the cube; otherwise it would look like they were 'going backwards'
        if(trailColor.red > 0) trailColor.red-=trailColor.red*(map(z%cube.size, 0, cube.size-1, 1, cube.size-1)*.09); //.125;
        if(trailColor.green > 0) trailColor.green-=trailColor.green*(map(z%cube.size, 0, cube.size-1, 1, cube.size-1)*.09); //.125;
        if(trailColor.blue > 0) trailColor.blue-=trailColor.blue*(map(z%cube.size, 0, cube.size-1, 1, cube.size-1)*.09); //.125;
        cube.setVoxel((z*cube.size*cube.size) + (x*cube.size) + y, Color(trailColor.red, trailColor.green, trailColor.blue));
        delayMicroseconds(30);  //introducing a little bit of delay to 'smoothen-out' transitions
      }
  
  //cube.show();
}

short FFT(short int dir,int m,float *x,float *y) {
   int n,i,i1,j,k,i2,l,l1,l2;
   float c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   n = 1;
   for (i=0;i<m;i++) 
      n *= 2;

   /* Do the bit reversal */
   i2 = n >> 1;
   j = 0;
   for (i=0;i<n-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0; 
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0; 
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<n;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1; 
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1) 
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == 1) {
      for (i=0;i<n;i++) {
         x[i] /= n;
         y[i] /= n;
      }
   }

   return(0);
}

/*************************
 * Purple Rain functions *
 *************************/
void initSalvos() {
  // Seed the random number generator
  srand(millis()); 
  if(demo==ACIDRAIN) ledColor=0;
  
  for(int i=0;i<cube.size;i++) {
    for(int j=0;j<MAX_POINTS;j++) {
      salvos[i].raindrops[j].raindrop.x=-1;
      salvos[i].raindrops[j].raindrop.z=-1;
      salvos[i].raindrops[j].speed=0;
      salvos[i].raindrops[j].flipped=false;
      salvos[i].raindrops[j].color=black;
      salvos[i].raindrops[j].dead=true;
    }
    salvos[i].dead=true;
  }
}

void checkMicPurpleRain() {
  int mic=analogRead(MICROPHONE);
  if(mic<minimum)
      minimum=mic;
  if(mic>maximum)
      maximum=mic;
  float range=maximum-minimum;
  int mean=range*.5;
  /*
  if(minimum<mean)
      minimum++;
  if(maximum>mean)
      maximum--;
      */
  threshhold=mean+sensitivityPurpleRain*(range/2);

  if(mic>threshhold) {
    if((!aboveThreshhold)&&((timeAboveThreshhold-millis())>MIN_SALVO_SPACING)) {
      launchRain(mic-threshhold);
      aboveThreshhold=true;
      timeAboveThreshhold=millis();
    }
  }
  else
    aboveThreshhold=false;
/*
    Serial.print(mic);
    Serial.print(":  ");
    Serial.print(threshhold);
    Serial.print(" - above threshhold: ");
    Serial.println(aboveThreshhold);
    */
}

void updateSalvos() {
  for(int i=0;i<cube.size;i++) {
    for(int j=0;j<MAX_POINTS;j++) {
      salvos[i].raindrops[j].raindrop.y+=salvos[i].raindrops[j].speed;
      if(salvos[i].raindrops[j].speed>0) {
        if(salvos[i].raindrops[j].raindrop.y<cube.size) {
          if(salvos[i].raindrops[j].flipped) {
            //salvos[i].raindrops[j].color=cube.lerpColor(salvos[i].raindrops[j].color, cube.colorMap(abs(j-i),0,cube.size+1), abs(j-i), abs(salvos[i].raindrops[j].speed), fadingMax);
            switch(demo) {
              case PURPLERAIN:
                mixVoxel(salvos[i].raindrops[j].raindrop, cube.lerpColor(salvos[i].raindrops[j].color, cube.colorMap(abs(j-i),0,cube.size+1), abs(j-i), abs(salvos[i].raindrops[j].speed), fadingMax), .9);
                break;
              case GOLDENRAIN:
              case ACIDRAIN:
                /*if(ledColor<200) {
                  if(salvos[i].raindrops[j].raindrop.y>=5) salvos[i].raindrops[j].color=Color( 0, 5, 15 );
                  if(salvos[i].raindrops[j].raindrop.y>=4 && salvos[i].raindrops[j].raindrop.y<5) salvos[i].raindrops[j].color=Color( 0, 1, 9 );
                  if(salvos[i].raindrops[j].raindrop.y>=3 && salvos[i].raindrops[j].raindrop.y<4) salvos[i].raindrops[j].color=Color( 1, 0, 11 );
                  if(salvos[i].raindrops[j].raindrop.y>=2 && salvos[i].raindrops[j].raindrop.y<3) salvos[i].raindrops[j].color=Color( 3, 0, 12 );
                  if(salvos[i].raindrops[j].raindrop.y>=1 && salvos[i].raindrops[j].raindrop.y<2) salvos[i].raindrops[j].color=Color( 10, 0, 13 );
                  if(salvos[i].raindrops[j].raindrop.y>=0 && salvos[i].raindrops[j].raindrop.y<1) salvos[i].raindrops[j].color=Color( random(10, 16), 0, 1 );
                }                                     
                if(ledColor>=200 && ledColor<400) {   
                  if(salvos[i].raindrops[j].raindrop.y>=5) salvos[i].raindrops[j].color=Color( 13, 13, 0 );
                  if(salvos[i].raindrops[j].raindrop.y>=4 && salvos[i].raindrops[j].raindrop.y<5) salvos[i].raindrops[j].color=Color( 15, 5, 0 );
                  if(salvos[i].raindrops[j].raindrop.y>=3 && salvos[i].raindrops[j].raindrop.y<4) salvos[i].raindrops[j].color=Color( 15, 2, 0 );
                  if(salvos[i].raindrops[j].raindrop.y>=2 && salvos[i].raindrops[j].raindrop.y<3) salvos[i].raindrops[j].color=Color( 15, 0, 0 );
                  if(salvos[i].raindrops[j].raindrop.y>=1 && salvos[i].raindrops[j].raindrop.y<2) salvos[i].raindrops[j].color=Color( 12, 0, 0 );
                  if(salvos[i].raindrops[j].raindrop.y>=0 && salvos[i].raindrops[j].raindrop.y<1) salvos[i].raindrops[j].color=Color( random(10, 16), random(0, 2), 0 );
                }*/
                break;
            }
          }
        }
        else {
          if(salvos[i].raindrops[j].flipped)
            salvos[i].raindrops[j].dead=true;
          else {
            salvos[i].raindrops[j].speed=((demo==PURPLERAIN) ? -salvos[i].raindrops[j].speed : salvos[i].raindrops[j].speed);
            salvos[i].raindrops[j].flipped=true;
          }
        }
      }
      else {
        if(salvos[i].raindrops[j].raindrop.y>0) {
          if(salvos[i].raindrops[j].flipped) {
            //salvos[i].raindrops[j].color=cube.lerpColor(salvos[i].raindrops[j].color, cube.colorMap(abs(j-i),0,cube.size+1), abs(j-i), abs(salvos[i].raindrops[j].speed), fadingMax);
            mixVoxel(salvos[i].raindrops[j].raindrop, cube.lerpColor(salvos[i].raindrops[j].color, cube.colorMap(abs(j-i),0,cube.size+1), abs(j-i), abs(salvos[i].raindrops[j].speed), fadingMax), .9);
          }
        }
        else {
          if(salvos[i].raindrops[j].flipped)
            salvos[i].raindrops[j].dead=true;
          else {
            salvos[i].raindrops[j].speed=((demo==PURPLERAIN) ? -salvos[i].raindrops[j].speed : salvos[i].raindrops[j].speed);
            salvos[i].raindrops[j].flipped=true;
          }
        }
      }
    }
      
    int offCube=true;
    for(int j=0;j<MAX_POINTS;j++) {
      if(!salvos[i].raindrops[j].dead) {
        offCube=false;
        break;
      }
    }
    if(offCube)
      salvos[i].dead=true;
  }
}

void drawSalvos() {
  for(int i=0;i<cube.size;i++)
    if(!salvos[i].dead)
      for(int j=0;j<MAX_POINTS;j++)
        if(!salvos[i].raindrops[j].dead) {
          switch(demo) {
            case PURPLERAIN:
              salvos[i].raindrops[j].color=cube.lerpColor(salvos[i].raindrops[j].color, cube.colorMap(abs(j-i),0,cube.size+1), abs(j-i), abs(salvos[i].raindrops[j].speed), fadingMax);
              break;
            case GOLDENRAIN:
              salvos[i].raindrops[j].color=cube.lerpColor(salvos[i].raindrops[j].color, Color( 95, 95, 25 ), abs(j-i), abs(salvos[i].raindrops[j].speed), fadingMax);
              //salvos[i].raindrops[j].color=cube.lerpColor(Color( 127, 79, 15 ), Color( 95, 95, 25 ), random((abs(salvos[i].raindrops[j].raindrop.z-salvos[i].raindrops[j].raindrop.y))+1), random(salvos[i].raindrops[j].speed+1), cube.size+1);
              break;
            case ACIDRAIN:
              if(ledColor<200) {
                if(salvos[i].raindrops[j].raindrop.y>=5) salvos[i].raindrops[j].color=Color( 0, 5, 15 );
                if(salvos[i].raindrops[j].raindrop.y>=4 && salvos[i].raindrops[j].raindrop.y<5) salvos[i].raindrops[j].color=Color( 0, 1, 9 );
                if(salvos[i].raindrops[j].raindrop.y>=3 && salvos[i].raindrops[j].raindrop.y<4) salvos[i].raindrops[j].color=Color( 1, 0, 11 );
                if(salvos[i].raindrops[j].raindrop.y>=2 && salvos[i].raindrops[j].raindrop.y<3) salvos[i].raindrops[j].color=Color( 3, 0, 12 );
                if(salvos[i].raindrops[j].raindrop.y>=1 && salvos[i].raindrops[j].raindrop.y<2) salvos[i].raindrops[j].color=Color( 10, 0, 13 );
                if(salvos[i].raindrops[j].raindrop.y>=0 && salvos[i].raindrops[j].raindrop.y<1) salvos[i].raindrops[j].color=Color( random(10, 16), 0, 1 );
              }                                     
              if(ledColor>=200 && ledColor<=400) {   
                if(salvos[i].raindrops[j].raindrop.y>=5) salvos[i].raindrops[j].color=Color( 13, 13, 0 );
                if(salvos[i].raindrops[j].raindrop.y>=4 && salvos[i].raindrops[j].raindrop.y<5) salvos[i].raindrops[j].color=Color( 15, 5, 0 );
                if(salvos[i].raindrops[j].raindrop.y>=3 && salvos[i].raindrops[j].raindrop.y<4) salvos[i].raindrops[j].color=Color( 15, 2, 0 );
                if(salvos[i].raindrops[j].raindrop.y>=2 && salvos[i].raindrops[j].raindrop.y<3) salvos[i].raindrops[j].color=Color( 15, 0, 0 );
                if(salvos[i].raindrops[j].raindrop.y>=1 && salvos[i].raindrops[j].raindrop.y<2) salvos[i].raindrops[j].color=Color( 12, 0, 0 );
                if(salvos[i].raindrops[j].raindrop.y>=0 && salvos[i].raindrops[j].raindrop.y<1) salvos[i].raindrops[j].color=Color( random(10, 16), random(0, 2), 0 );
              }
              break;
            default:
              break;
          }
          cube.setVoxel(salvos[i].raindrops[j].raindrop.x, salvos[i].raindrops[j].raindrop.y, salvos[i].raindrops[j].raindrop.z, salvos[i].raindrops[j].color);
        }
}

float setNewSpeed() {
  float ret;
  int rndSpeed=0+(rand()%(cube.size));
  switch(rndSpeed) {
    case 0:
      ret=0.5;
      break;
    case 1:
      ret=-0.5;
      break;
    case 2:
      ret=0.25;
      break;
    case 3:
      ret=-0.25;
      break;
    case 4:
      ret=0.2;
      break;
    case 5:
      ret=-0.2;
      break;
    default:
      ret=SPEED;
      break;
  }
  return ret;
}
 
void launchRain(int amplitude) {
  int i;
  for(i=0;((i<cube.size)&&(!salvos[i].dead));i++)
      ;
  if(i<cube.size) {
    if(amplitude>maxAmplitude)
      maxAmplitude=amplitude;
      
    int numDrops=map(amplitude,0, maxAmplitude,0, MAX_POINTS);
    for(int j=0;j<numDrops;j++) {
      salvos[i].dead=false;
      salvos[i].raindrops[j].dead=false;
      salvos[i].raindrops[j].flipped=false;
      salvos[i].raindrops[j].speed=setNewSpeed();
      salvos[i].raindrops[j].raindrop.x=rand()%cube.size;
      salvos[i].raindrops[j].raindrop.z=rand()%cube.size;

      // Here we decide which point across the y-axis
      // will be our start location for the raindrop,
      // based on the value 'startAt' was initialized
      switch(startAt) {
        case BASE:
          salvos[i].raindrops[j].raindrop.y=((rand()%10)-5)/10;
          break;
        case CENTER:
          salvos[i].raindrops[j].raindrop.y=cube.size/2;
          break;
        case TOP:
          salvos[i].raindrops[j].raindrop.y=cube.size;
          break;
        case RANDOM:
          salvos[i].raindrops[j].raindrop.y=rand()%cube.size;
          break;
      }
      
      // Here's some cool combinations to try with cube.lerpColor():
      // purple, magenta
      // blue, pink
      // purple, pink
      // ..
      // ..
      // Go wild...
      //salvos[i].raindrops[j].color=cube.lerpColor(purple, magenta, j, SPEED, MAX_POINTS);
      switch(demo) {
        case PURPLERAIN:
          salvos[i].raindrops[j].color=cube.colorMap(abs(salvos[i].raindrops[j].raindrop.z-salvos[i].raindrops[j].raindrop.y),0,cube.size);
          break;
        case GOLDENRAIN:
          /*salvos[i].raindrops[j].color=cube.lerpColor(Color( 127, 79, 15 ), Color( 95, 95, 25 ), 
            random((abs(salvos[i].raindrops[j].raindrop.z-salvos[i].raindrops[j].raindrop.y))+1), 
            random(salvos[i].raindrops[j].speed+1), cube.size+1);*/
          salvos[i].raindrops[j].color=Color( 127, 80, 16 );
          break;
        case ACIDRAIN:
              if(ledColor<200) {
                if(salvos[i].raindrops[j].raindrop.y>=5) salvos[i].raindrops[j].color=Color( 0, 5, 15 );
                if(salvos[i].raindrops[j].raindrop.y>=4 && salvos[i].raindrops[j].raindrop.y<5) salvos[i].raindrops[j].color=Color( 0, 1, 9 );
                if(salvos[i].raindrops[j].raindrop.y>=3 && salvos[i].raindrops[j].raindrop.y<4) salvos[i].raindrops[j].color=Color( 1, 0, 11 );
                if(salvos[i].raindrops[j].raindrop.y>=2 && salvos[i].raindrops[j].raindrop.y<3) salvos[i].raindrops[j].color=Color( 3, 0, 12 );
                if(salvos[i].raindrops[j].raindrop.y>=1 && salvos[i].raindrops[j].raindrop.y<2) salvos[i].raindrops[j].color=Color( 10, 0, 13 );
                if(salvos[i].raindrops[j].raindrop.y>=0 && salvos[i].raindrops[j].raindrop.y<1) salvos[i].raindrops[j].color=Color( random(10, 16), 0, 1 );
              }                                     
              if(ledColor>=200 && ledColor<=400) {   
                if(salvos[i].raindrops[j].raindrop.y>=5) salvos[i].raindrops[j].color=Color( 13, 13, 0 );
                if(salvos[i].raindrops[j].raindrop.y>=4 && salvos[i].raindrops[j].raindrop.y<5) salvos[i].raindrops[j].color=Color( 15, 5, 0 );
                if(salvos[i].raindrops[j].raindrop.y>=3 && salvos[i].raindrops[j].raindrop.y<4) salvos[i].raindrops[j].color=Color( 15, 2, 0 );
                if(salvos[i].raindrops[j].raindrop.y>=2 && salvos[i].raindrops[j].raindrop.y<3) salvos[i].raindrops[j].color=Color( 15, 0, 0 );
                if(salvos[i].raindrops[j].raindrop.y>=1 && salvos[i].raindrops[j].raindrop.y<2) salvos[i].raindrops[j].color=Color( 12, 0, 0 );
                if(salvos[i].raindrops[j].raindrop.y>=0 && salvos[i].raindrops[j].raindrop.y<1) salvos[i].raindrops[j].color=Color( random(10, 16), random(0, 2), 0 );
              }
          //salvos[i].raindrops[j].color=cube.colorMap(abs(salvos[i].raindrops[j].raindrop.z-salvos[i].raindrops[j].raindrop.y),0,cube.size);
          break;
      }
    }
    
    for(int j=numDrops;j<MAX_POINTS;j++) {
      salvos[i].raindrops[j].raindrop.x=-1;
      salvos[i].raindrops[j].raindrop.z=-1;
    }
  }
}

/****************************************
 * FFTJoy2 functions *
 * **************************************/
 void FFTJoy2() {
  
  Sinewave mySinewave;

    static long t=0;
  
    Color clearColor = Color( 0, 0, 0 );
    Color fftStripColor[5];
    fftStripColor[0] = Color( random(11, 14), random(10, 14), 0 );  //Color(0,0,50);
    fftStripColor[1] = Color( random(11, 14), random(10), 0 );      //Color(0,40,5);
    fftStripColor[2] = Color( random(11, 14), random(8), 0 );       //Color(0,40,10);
    fftStripColor[3] = Color( random(10, 14), random(4), 0 );       //Color(10,30,0);
    fftStripColor[4] = Color( random(10, 16), random(2), 0 );       //Color(20,20,0);
    fftStripColor[5] = Color( random(10, 16), 0, 0 );               //Color(40,5,0);
    delay(5);
  runningPeriod=0;
  for(int i=0;i<getSinewaveSampleNum;i++){
  mySinewave=cube.getSinewave(1000);
  runningAverage+=mySinewave.amplitude;
  runningPeriod+=mySinewave.timePeriod;
}
  runningAverage/=getSinewaveSampleNum;
  runningPeriod/=getSinewaveSampleNum;
  
  addAverageToChain(runningAverage);
  trendAverage=getTrendAverage(30);
  
  if(runningAverage>trendAverage) t=(runningAverage-trendAverage)/sqrt(getAverageAmplitude(30))+sqrt(runningAverage)/10;
  else t-=(trendAverage-runningAverage)/sqrt(getAverageAmplitude(30));
  /*
  Serial.print(runningPeriod);
  Serial.print("   "); 
  Serial.print(trendAverage);
  Serial.print("   "); 
  Serial.print(runningAverage-trendAverage);
  Serial.print("   "); 
  Serial.println(t); 
  */
  if(t<0) t=0;
  if(t>5) t=5;

  if(trendAverage<400) t*=0.3;
  
  char column;
  if(runningPeriod<25) column=0; 
  else column=(runningPeriod-20)/10;
  if (column>5) column=5;
    
    /*for( int x=0; x<cube.size; x++)
      for( int y=0; y<cube.size; y++){
        cube.setVoxel( x, y, cube.size-1, clearColor );
      }*/
    
    //This causes a nice and smooth 'trailing' effect
    //from the top of the cube (y-axis) towards the peak
    if((ledColor%3)==0) 
      fadeSmooth(0, 1, 0.0625);
    for( int x=0; x<cube.size; x++)
      for(int y=cube.size-1; y>=0; y--)
        for( int z=0; z<cube.size; z++){
          Color readColor=cube.getVoxel(x,y,z);
          if(readColor.red > 0) readColor.red-=readColor.red*(map(y%cube.size, 0, cube.size-1, 1, cube.size-1)*.03);
          if(readColor.green > 0) readColor.green-=readColor.green*(map(y%cube.size, 0, cube.size-1, 1, cube.size-1)*.03);
          if(readColor.blue > 0) readColor.blue-=readColor.blue*(map(y%cube.size, 0, cube.size-1, 1, cube.size-1)*.03);
          cube.setVoxel(x,y,z, Color(readColor.red, readColor.green, readColor.blue));
        }
    
    /*for( int x=0; x<cube.size; x++)
        for(int y=0; y<cube.size; y++)
         for( int z=0; z<cube.size; z++){
          readColor=cube.getVoxel(x,y,z);
          cube.setVoxel(x,y,z,readColor); 
         }*/
    for( int y=0;y<=t; y++){
        int z=random(cube.size);
        cube.setVoxel( column, y, z, fftStripColor[y] );
        if((t-1>0)&&(y>1)) {
          if(column>0) cube.setVoxel( column-1, y-2, (z>0 ? z-1 : z), fftStripColor[y-2] );
          if(column<5) cube.setVoxel( column+1, y-2, (z<cube.size-1 ? z+1 : z), fftStripColor[y-2] );
        }
        if((t-3>=0)&&(y>3)) {
          if(column>1) cube.setVoxel( column-2, y-4, (z>=3 ? z-3 : z), fftStripColor[y-4] );
          if(column<4) cube.setVoxel( column+2, y-4, (z<cube.size-3 ? z+3 : z), fftStripColor[y-4] );
        }
        if((t-5>=0)&&(y>=5)) {
          if(column>2) cube.setVoxel( column-3, y-5, (z>=5 ? z-5 : z), fftStripColor[y-5] );
          if(column<3) cube.setVoxel( column+3, y-5, (z<cube.size-5 ? z+5 : z), fftStripColor[y-5] );
        }
    }

}

/****************************************
 * MusicTower functions *
 * **************************************/
 void musicTower() {
  
  Color readColor;
  Sinewave mySinewave;

  long t=0;
//  Point ring[24];
    Color clearColor = Color( 0, 0, 0 );
    Color musicTowerColor=Color(0,0,0);
    Color fftStripColor[5];
    fftStripColor[0] = Color(0,0,50 );
    fftStripColor[1] = Color(0,40,5);
    fftStripColor[2] = Color(0,40,10);
    fftStripColor[3] = Color(10,30,0);
    fftStripColor[4] = Color(20,20,0);
    fftStripColor[5] = Color(40,5,0);
    delay(10);
  for(int i=0;i<getSinewaveSampleNum;i++){
  mySinewave=cube.getSinewave(1000);
  runningAverage+=mySinewave.amplitude;
  runningPeriod+=mySinewave.timePeriod;
}
  runningAverage/=getSinewaveSampleNum;
  runningPeriod/=getSinewaveSampleNum;
  addAverageToChain(runningAverage);
  trendAverage=getTrendAverage(30);
  
  if(runningAverage>trendAverage) t=8+(runningAverage-trendAverage)/15+sqrt(runningAverage)/4;
  else t=8-(trendAverage-runningAverage)/55;
  /*
  Serial.print(runningAverage);
  Serial.print("   "); 
  Serial.print(runningAverage-trendAverage);
  Serial.print("   "); 
  Serial.println(t); 
  */
  if(t>47) t=47;
  else if(t<0) t=0;
  
  if(trendAverage<400) t*=0.3;
  
  Point towerPoint[48];
  initRing();
  for(int i=0;i<12;i++)
    towerPoint[i]=ring[12+i];
  for(int i=12;i<24;i++){
    towerPoint[i]=ring[i];
    towerPoint[i].y+=1;
  }
  for(int i=24;i<32;i++){
    towerPoint[i]=ring[4+i-24];
    towerPoint[i].y+=2;
  }
  for(int i=32;i<40;i++){
    towerPoint[i]=ring[4+i-32];
    towerPoint[i].y+=3;
  }
  for(int i=40;i<44;i++){
    towerPoint[i]=ring[i-40];
    towerPoint[i].y+=4;
  }
  for(int i=44;i<48;i++){
    towerPoint[i]=ring[i-44];
    towerPoint[i].y+=5;
  }
 /* 
  for(int i=0;i<24;i++){
    float m;
    m=towerPoint[i].y;
    towerPoint[i].y=towerPoint[i].z;
    towerPoint[i].z=m;
  }
*/
//  cube.background(black); 
  for( int i=0;i<t;i++){
     musicTowerColor = Color( 50*sin(colorInc+i*0.1)>0?50*sin(colorInc+i*0.1):0, 50*sin(colorInc+pi*2/3+i*0.1)>0?50*sin(colorInc+pi*2/3+i*0.1):0, 50*sin(colorInc+pi*4/3+i*0.1)>0?50*sin(colorInc+pi*4/3+i*0.1):0 );
     cube.setVoxel(towerPoint[i],musicTowerColor);
  }
  for(int i=t;i<48;i++){
    readColor=cube.getVoxel(towerPoint[i]);
   if(readColor.red>0) readColor.red--;
   if(readColor.green>0) readColor.green--;
   if(readColor.blue>0)  readColor.blue--;
   if(readColor.red>0) readColor.red--;
   if(readColor.green>0) readColor.green--;
   if(readColor.blue>0)  readColor.blue--;
    cube.setVoxel(towerPoint[i],readColor);
  }
  colorInc+=colorSpeed*5;
}


Color Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return Color(WheelPos, 85 - WheelPos, 0);
  }
  else if(WheelPos < 170) {
   WheelPos -= 85;
   return Color(85 - WheelPos, 0, WheelPos);
  } 
  else {
   WheelPos -= 170;
   return Color(0, WheelPos, 85 - WheelPos);
  }
}

/******************
 * flip functions *
 ******************/
void checkFlipState() {
  updateAccelerometer();
//  Serial.println(accelerometer[2]);
//  
//  if(accelerometer[2]>FACEPLANT)  //if the cube is upside-down, set the upside-down flag and mark the time when it was flipped, this is different from 8x8x8 Cube as the chip is on the bottom side
//  {
//    //  upsideDownTime=millis();
//      lastFaceplant=millis();
//Serial.println("I'm upside-down!");
//  }
  
  if(accelerometer[0]<LOCK) {  //if the cube is upside-down, set the upside-down flag and mark the time when it was flipped
    autoCycle=false;
   Serial.println("LOCK!"); 
   lastFaceplant = millis();
//    delay(200);
  }
  if(accelerometer[1]>LEFT_SIDE) {  //if the cube is flipped to either side
    lastLeft=millis();
  // Serial.println("I'm on my left side");
  }
  if(accelerometer[1]<RIGHT_SIDE) {
    lastRight=millis();
  // Serial.println("I'm on my right side");
  }
  if(accelerometer[2]<FACEPLANT) {
    /*
  //   Serial.println("Right side up");
    if(((millis()-lastFaceplant)<FLIP_TIMEOUT)&&(millis()-lastFaceplant>FLIP_DEBOUNCE)) {
      autoCycle=false;
      lastFaceplant=millis()-FLIP_TIMEOUT;
      Color flash;
      flash.red=cube.maxBrightness;
      flash.green=cube.maxBrightness;
      flash.blue=cube.maxBrightness;
      cube.background(flash);
    }*/
    if(((millis()-lastLeft)<FLIP_TIMEOUT)&&(millis()-lastLeft>FLIP_DEBOUNCE)) {
    // Serial.println("turned to the left and back");
      autoCycle=false;
      lastChange=millis();
      decrementDemo();
      lastLeft=millis()-FLIP_TIMEOUT;
      return;
    }
    if(((millis()-lastRight)<FLIP_TIMEOUT)&&(millis()-lastRight>FLIP_DEBOUNCE)) {
    // Serial.println("turned to the right and back");
      autoCycle=false;
      lastChange=millis();
      incrementDemo();
      lastRight=millis()-FLIP_TIMEOUT;
      return;
    }

    if((millis() - lastFaceplant < FLIP_TIMEOUT)&&(millis()-lastFaceplant>FLIP_DEBOUNCE)) {
    switch (animationType) {
    case still: 
      Serial.println("Still");
      animationType = dynamic;
      demo = animationType;
      setFadeSpeed();
      fading=true;
      break;

    case dynamic:
      Serial.println("Dynamic");
      animationType = music;
      demo = animationType;
      setFadeSpeed();
      fading=true;
      break;

    case music:
    Serial.println("Music");
      animationType = still;
      demo = still;
      setFadeSpeed();
      fading=true;
      break;
    } 

    lastChange=millis();
      lastFaceplant=millis()-FLIP_TIMEOUT;
    }
  }

  if(autoCycle)
      if(millis()-lastAutoCycle>AUTOCYCLE_TIME) { //in autocycle, change demos every 15 seconds
        incrementDemo();
        //  Serial.print("autocycling...Demo is ");
        //  Serial.println(demo);
        lastAutoCycle=millis();
      }
}
 
void updateAccelerometer() {
  accelerometer[0]=analogRead(Y);
  accelerometer[1]=analogRead(X);
  accelerometer[2]=analogRead(Z);
}

/********************
 * Helper functions *
 ********************/
void setFadeSpeed() {
  if(autoCycle)
    fadeSpeed=2;
  else
    fadeSpeed=20;
}
 
void incrementDemo() {
  demo++;
  if(demo == STATIC_START || demo == DYNAMIC_START || demo == MUSIC_START) {
    demo = animationType;
  }
  setFadeSpeed();
  fading=true;
  if(demo==GOLDENRAIN || demo==PURPLERAIN || demo==ACIDRAIN)
    initSalvos();
  if(demo>=DEMO_ROUTINES)
    demo=MUSIC_START;
}
 
void decrementDemo() {
  demo--;
  if(demo == STATIC_START-1) demo = DYNAMIC_START-1;
  else if(demo == DYNAMIC_START-1) demo = MUSIC_START-1;
  else if(demo == MUSIC_START-1) demo = DEMO_ROUTINES-1;
  
  fading=true;
  if(demo==GOLDENRAIN || demo==PURPLERAIN || demo==ACIDRAIN)
    initSalvos();
}

float distance(float x, float y, float z, float x1, float y1, float z1) {
  return(sqrt(pow(x-x1,2)+pow(y-y1,2)+pow(z-z1,2)));
}

void add(Point& a, Point& b) {
  a.x+=b.x;
  a.y+=b.y;
  a.z+=b.z;
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
Color Wheel(byte WheelPos, float opacity) {
  if(WheelPos < 85) {
    return Color(constrain(map((WheelPos * 3) * opacity, 0, 255, 0, cube.maxBrightness), 0, cube.maxBrightness), 
    constrain(map((255 - WheelPos * 3) * opacity, 0, 255, 0, cube.maxBrightness), 0, cube.maxBrightness), 0);
  }
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return Color(constrain(map((255 - WheelPos * 3) * opacity, 0, 255, 0, cube.maxBrightness), 0, cube.maxBrightness), 
    0, constrain(map((WheelPos * 3) * opacity, 0, 255, 0, cube.maxBrightness), 0, cube.maxBrightness));
  }
  else {
    WheelPos -= 170;
    return Color(0, constrain(map((WheelPos * 3) * opacity, 0, 255, 0, cube.maxBrightness), 0, cube.maxBrightness), 
    constrain(map((255 - WheelPos * 3) * opacity, 0, 255, 0, cube.maxBrightness), 0, cube.maxBrightness));
    }
}
  
Color adjustGamma(Color col, float opacity) {
  col.red = constrain(map(col.red * opacity, 0, 255, 0, cube.maxBrightness), 0, cube.maxBrightness);
  col.green = constrain(map(col.green * opacity, 0, 255, 0, cube.maxBrightness), 0, cube.maxBrightness);
  col.blue = constrain(map(col.blue * opacity, 0, 255, 0, cube.maxBrightness), 0, cube.maxBrightness);

  return col;
}

//void drawCube(Point topLeft, int side, Color col) {
//  Point topPoints[4];
//  Point bottomPoints[4];
//  topPoints[0]=topLeft;
//  topPoints[1]=Point(topLeft.x+side, topLeft.y, topLeft.z);
//  topPoints[2]=Point(topLeft.x+side, topLeft.y+side, topLeft.z);
//  topPoints[3]=Point(topLeft.x, topLeft.y+side, topLeft.z);
//  Point bottomLeft=Point(topLeft.x, topLeft.y, topLeft.z+side);
//  bottomPoints[0]=bottomLeft;
//  bottomPoints[1]=Point(bottomLeft.x+side, bottomLeft.y, bottomLeft.z);
//  bottomPoints[2]=Point(bottomLeft.x+side, bottomLeft.y+side, bottomLeft.z);
//  bottomPoints[3]=Point(bottomLeft.x, bottomLeft.y+side, bottomLeft.z);
//  
//  for (int i=0; i<4; i++) {
//    drawLine(topPoints[i], bottomPoints[i], col);
//    drawLine(topPoints[i], topPoints[(i+1)%4], col);
//    drawLine(bottomPoints[i], bottomPoints[(i+1)%4], col);
//  }
//  
//  Color comp=complement(col);
//  for (int i=0; i<4; i++) {
//    cube.setVoxel(topPoints[i], comp);
//    cube.setVoxel(bottomPoints[i], comp);
//  }
//}

// draws a line from point p1 to p2 and colors each of the points according
// to the col parameter
// p1 and p2 can be outside of the cube, but it will only draw the parts of
// the line that fall
// inside the cube
void drawLine(Point p1, Point p2, Color col) {
  // thanks to Anthony Thyssen for the original write of Bresenham's line
  // algorithm in 3D
  // http://www.ict.griffith.edu.au/anthony/info/graphics/bresenham.procs

  float dx, dy, dz, l, m, n, dx2, dy2, dz2, i, x_inc, y_inc, z_inc, err_1, err_2;
  Point currentPoint = Point(p1.x, p1.y, p1.z);
  dx = p2.x - p1.x;
  dy = p2.y - p1.y;
  dz = p2.z - p1.z;
  x_inc = (dx < 0) ? -1 : 1;
  l = abs(dx);
  y_inc = (dy < 0) ? -1 : 1;
  m = abs(dy);
  z_inc = (dz < 0) ? -1 : 1;
  n = abs(dz);
  dx2 = l * 2;
  dy2 = m * 2;
  dz2 = n * 2;

  if ((l >= m) && (l >= n)) {
    err_1 = dy2 - l;
    err_2 = dz2 - l;
    for (i = 0; i < l; i++) {
      mixVoxel(currentPoint, col);
      if (err_1 > 0) {
        currentPoint.y += y_inc;
        err_1 -= dx2;
      }
      if (err_2 > 0) {
        currentPoint.z += z_inc;
        err_2 -= dx2;
      }
      err_1 += dy2;
      err_2 += dz2;
      currentPoint.x += x_inc;
    }
  } 
  else if ((m >= l) && (m >= n)) {
    err_1 = dx2 - m;
    err_2 = dz2 - m;
    for (i = 0; i < m; i++) {
      mixVoxel(currentPoint, col);
      if (err_1 > 0) {
        currentPoint.x += x_inc;
        err_1 -= dy2;
      }
      if (err_2 > 0) {
        currentPoint.z += z_inc;
        err_2 -= dy2;
      }
      err_1 += dx2;
      err_2 += dz2;
      currentPoint.y += y_inc;
    }
  } 
  else {
    err_1 = dy2 - n;
    err_2 = dx2 - n;
    for (i = 0; i < n; i++) {
      mixVoxel(currentPoint, col);
      if (err_1 > 0) {
        currentPoint.y += y_inc;
        err_1 -= dz2;
      }
      if (err_2 > 0) {
        currentPoint.x += x_inc;
        err_2 -= dz2;
      }
      
      err_1 += dy2;
      err_2 += dx2;
      currentPoint.z += z_inc;
    }
  }
  
  mixVoxel(currentPoint, col);
}

void mixVoxel(Point currentPoint, Color col) {
  Color currentCol=cube.getVoxel(currentPoint);
  Color newCol=Color(currentCol.red+col.red, currentCol.green+col.green, currentCol.blue+col.blue);
  cube.setVoxel(currentPoint, newCol);
}


void mixVoxel(Point currentPoint, Color col, float step) {
  Color currentCol=cube.getVoxel(currentPoint);
  Color newCol; //=Color(currentCol.red+col.red, currentCol.green+col.green, currentCol.blue+col.blue);
  
  if(currentCol.red>col.red)          newCol.red=currentCol.red-step;
  else if(currentCol.red<col.red)     newCol.red=currentCol.red+step;
  if(currentCol.green>col.green)      newCol.green=currentCol.green-step;
  else if(currentCol.green<col.green) newCol.green=currentCol.green+step;
  if(currentCol.blue>col.blue)        newCol.blue=currentCol.blue-step;
  else if(currentCol.blue<col.blue)   newCol.blue=currentCol.blue+step;
  
  cube.setVoxel(currentPoint, newCol);
}

Color complement(Color original) {
  float R = original.red;
  float G = original.green;
  float B = original.blue;
  float minRGB = min(R, min(G, B));
  float maxRGB = max(R, max(G, B));
  float minPlusMax = minRGB + maxRGB;
  Color complement = Color(minPlusMax-R, minPlusMax-G, minPlusMax-B);
  return complement;
}

void initSquarePointB(){
  squarePointB[0]=Point(2,0,2);
  squarePointB[1]=Point(3,0,2);
  squarePointB[2]=Point(3,0,3);
  squarePointB[3]=Point(2,0,3);
  squarePointB[4]=Point(1,0,1);
  squarePointB[5]=Point(2,0,1);
  squarePointB[6]=Point(3,0,1);
  squarePointB[7]=Point(4,0,1);
  squarePointB[8]=Point(4,0,2);
  squarePointB[9]=Point(4,0,3);
  squarePointB[10]=Point(4,0,4);
  squarePointB[11]=Point(3,0,4);
  squarePointB[12]=Point(2,0,4);
  squarePointB[13]=Point(1,0,4);
  squarePointB[14]=Point(1,0,3);
  squarePointB[15]=Point(1,0,2);
  squarePointB[16]=Point(0,0,0);
  squarePointB[17]=Point(1,0,0);
  squarePointB[18]=Point(2,0,0);
  squarePointB[19]=Point(3,0,0);
  squarePointB[20]=Point(4,0,0);
  squarePointB[21]=Point(5,0,0);
  squarePointB[22]=Point(5,0,1);
  squarePointB[23]=Point(5,0,2);
  squarePointB[24]=Point(5,0,3);
  squarePointB[25]=Point(5,0,4);
  squarePointB[26]=Point(5,0,5);
  squarePointB[27]=Point(4,0,5);
  squarePointB[28]=Point(3,0,5);
  squarePointB[29]=Point(2,0,5);
  squarePointB[30]=Point(1,0,5);
  squarePointB[31]=Point(0,0,5);
  squarePointB[32]=Point(0,0,4);
  squarePointB[33]=Point(0,0,3);
  squarePointB[34]=Point(0,0,2);
  squarePointB[35]=Point(0,0,1);
}
void initRing(){
  ring[0]=Point(2,0,3);
  ring[1]=Point(3,0,3);
  ring[2]=Point(3,0,2);
  ring[3]=Point(2,0,2);
  ring[4]=Point(1,0,3);
  ring[5]=Point(2,0,4);
  ring[6]=Point(3,0,4);
  ring[7]=Point(4,0,3);
  ring[8]=Point(4,0,2);
  ring[9]=Point(3,0,1);
  ring[10]=Point(2,0,1);
  ring[11]=Point(1,0,2);
  ring[12]=Point(0,0,3);
  ring[13]=Point(1,0,4);
  ring[14]=Point(2,0,5);
  ring[15]=Point(3,0,5);
  ring[16]=Point(4,0,4);
  ring[17]=Point(5,0,3);
  ring[18]=Point(5,0,2);
  ring[19]=Point(4,0,1);
  ring[20]=Point(3,0,0);
  ring[21]=Point(2,0,0);
  ring[22]=Point(1,0,1);
  ring[23]=Point(0,0,2);
}

int getTrendAverage(int sampleNum){
  long average=0;
  for(int i=0;i<sampleNum;i++) 
    average+=wholeAverage[i];
  average/=sampleNum;
  return average;
 }

 int getAverageAmplitude(int sampleNum){
  int averageMax=0,averageMin=wholeAverage[0];
  for(int i=0;i<sampleNum;i++){
    if(wholeAverage[i]>averageMax) averageMax=wholeAverage[i];
    if(wholeAverage[i]<averageMin) averageMin=wholeAverage[i];
  }
  return(averageMax-averageMin);
 }

 void addAverageToChain(int arg){
    for(int i=averageSampleNum-1;i>0;i--)
    wholeAverage[i]=wholeAverage[i-1]; 
    wholeAverage[0]=arg;
 }
