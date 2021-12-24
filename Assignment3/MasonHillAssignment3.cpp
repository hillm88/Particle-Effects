/*Mason Hill Lab3
Novemebr 8th*/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include <windows.h>
#define M_PI 3.14159265358979323846 // Window doesn't seem to define this
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <cmath>
#include <iostream>
#include <array>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
using namespace std;



// XZ position of the camera



float previousX;
float previousY;

float xCoord=0.8;
float yCoord=0.0;
float zCoord = 0.0;

float modifiedx;
float modifiedy;


int CoordinateCount=0;
int CoordinateCountHelper=0;

 float acceleration = -9.8;
  float velocity = 0;
  float position =0;
  float dt;

  float xVelocity=0;
  float yVelocity=0;
  float zVelocity = 0;
  
  bool timeStepLarge = false;

bool flag = false;


bool falling = true;

bool isClicked;

//Gravity in m/s
float gravity = -9.8;


char SimType;

float blackHoleConstant = 10;


//Variable for holding the mouse down
bool MouseHeldDown=false;
//Used to help an array to store the coordinates of the particles
int CoordinateSetter=0;

//An object to hold each particles info
class ParticleInfo{
    public:
    bool particleExists = false;
    
    //x and y coordinate of the mouse
    float xCoordinate;
    float yCoordinate=0;

    //x and y velocity
    float yvelocity = 0;
    float xvelocity = 0;

    float zCoordinate = 0;
    float zvelocity = 0;

  //Setting the particles time to live and its cuttoff point
    float particleTTL = 20;
    float maxTTL = 0;

    
    float particleyVelocity;
    float particlexVelocity;

    float acceleration;

    //method that gets the particles information from the mouse
    void setter(bool exists,float x,float y,float z,float vx, float vy, float vz, float acceleration){
       
       //Capturing all the values
        xCoordinate=x;
        yCoordinate=y;
        zCoordinate = 0;

        zvelocity=0;
       
        yvelocity=vy;
        xvelocity=vx;

    
        particleExists=exists;
    }


};
//Creating a particle array of 500
 ParticleInfo particleArray[500];


//Creating a time seed for random number generation 
void init(){
  srand (time(NULL));
}


//A timer function to use motion
void timer (int i){
 




 
    //Checking if the mouse is held down  
    if(MouseHeldDown==true){
        //Reseting the array if it reaches 500 particles
        if(CoordinateSetter==500){
            CoordinateSetter=0;
            flag = true;
          
        }
        if(CoordinateCount!=500){
            CoordinateCount++;
        }



        //Modifying the x and y velocities to be somewhat random initally
        xVelocity = xVelocity/500.0*2.0 + (float)((float)rand()*2.0/(float)RAND_MAX-1);
        yVelocity = yVelocity/500.0*2.0 + (float)((float)rand()*2.0/(float)RAND_MAX - 1.0);
        


        //Setting the coordinates of each particle
        particleArray[CoordinateSetter].setter(true,xCoord,yCoord,zCoord,xVelocity,yVelocity, zVelocity,acceleration);
        
        //reseting the particles time to live
        particleArray[CoordinateSetter].particleTTL = 20;
      

        CoordinateSetter++;
        if(flag == false){
          CoordinateCountHelper = CoordinateSetter;
        }
        
    }

      //Gravity big time step
      if(SimType =='G'){
        //Changing the time step so that it is larger
        dt = 0.03;
      
        //Looping through each particle
        for(int i = 0; i<CoordinateCountHelper;i++){

          //Decrementing each particles ttl
          particleArray[i].particleTTL = particleArray[i].particleTTL - 1;

          //Making the particle have gravity by modifying its velocity
          particleArray[i].yvelocity = particleArray[i].yvelocity + gravity * dt;

          //Modifying the particles y coordinate to apply the velocity
          particleArray[i].yCoordinate = particleArray[i].yCoordinate + particleArray[i].yvelocity * dt;

          //Modifiying the particles x coordinate to apply the x velocity
          particleArray[i].xCoordinate = particleArray[i].xCoordinate + particleArray[i].xvelocity * dt;

        }
      }

      //Same as above just changed time step
      else if(SimType == 'g'){
        dt = 0.005;
        for(int i = 0; i<CoordinateCountHelper;i++){

          particleArray[i].particleTTL = particleArray[i].particleTTL - 0.2;

          particleArray[i].yvelocity = particleArray[i].yvelocity + gravity * dt;
        

         
          particleArray[i].yCoordinate = particleArray[i].yCoordinate + particleArray[i].yvelocity * dt;
        
          particleArray[i].xCoordinate = particleArray[i].xCoordinate + particleArray[i].xvelocity * dt;

        }
      }

      //Blackhole small timestep
      else if (SimType == 'b'){
        dt = 0.001;

        for(int i = 0; i<CoordinateCountHelper;i++){

          //Decrementing ttl
          particleArray[i].particleTTL = particleArray[i].particleTTL - 0.1;
        
        //Calculating the vector of the particle
        float VectorCalculation = particleArray[i].xCoordinate * particleArray[i].xCoordinate + particleArray[i].yCoordinate* particleArray[i].yCoordinate + particleArray[i].zCoordinate * particleArray[i].zCoordinate;
        
        //Normalizing the vector
         float NormalizedVector = sqrt(particleArray[i].xCoordinate * particleArray[i].xCoordinate/VectorCalculation + particleArray[i].yCoordinate*particleArray[i].yCoordinate/VectorCalculation + particleArray[i].zCoordinate*particleArray[i].zCoordinate/VectorCalculation);

          //accelerating the particle based on the black hole constant and the normalized vector
          particleArray[i].acceleration = blackHoleConstant / NormalizedVector;

          //Getting the new x and y velocities by dividing the x and y position by their normalized vector and multiplying it by the particle acceleration
          particleArray[i].xvelocity += ((-1 * particleArray[i].xCoordinate)/ sqrt(NormalizedVector)) * particleArray[i].acceleration;

           particleArray[i].yvelocity += ((-1 * particleArray[i].yCoordinate)/ sqrt(NormalizedVector)) * particleArray[i].acceleration;

          

 
          //Modifiying the x and y coordinate with its new position
          particleArray[i].yCoordinate = particleArray[i].yCoordinate + particleArray[i].yvelocity * dt;

          particleArray[i].xCoordinate = particleArray[i].xCoordinate + particleArray[i].xvelocity * dt;

        }
      }

      //Same as above sim type but with a different time step
      else if (SimType == 'B'){
        dt = 0.3;
        for(int i = 0; i<CoordinateCountHelper;i++){


          particleArray[i].particleTTL = particleArray[i].particleTTL - 0.1;
          
          float VectorCalculation = particleArray[i].xCoordinate * particleArray[i].xCoordinate + particleArray[i].yCoordinate* particleArray[i].yCoordinate;
        
         float NormalizedVector = sqrt(particleArray[i].xCoordinate * particleArray[i].xCoordinate/VectorCalculation + particleArray[i].yCoordinate*particleArray[i].yCoordinate/VectorCalculation);

          particleArray[i].acceleration = blackHoleConstant / NormalizedVector;

          particleArray[i].xvelocity += ((-1 * particleArray[i].xCoordinate)/ sqrt(NormalizedVector)) * particleArray[i].acceleration;

           particleArray[i].yvelocity += ((-1 * particleArray[i].yCoordinate)/ sqrt(NormalizedVector)) * particleArray[i].acceleration;

        

          particleArray[i].yCoordinate = particleArray[i].yCoordinate + particleArray[i].yvelocity *dt;

          particleArray[i].xCoordinate = particleArray[i].xCoordinate + particleArray[i].xvelocity *dt;

        }
      }
    



    


  glutPostRedisplay();
  //This timer function calls the movement function over and over again.

  glutTimerFunc(4,timer,8);
}


//Main display function
void display(void)
{
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // background colour
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the background
 //Adjusting the light matrix so we are moving the light.

  glColor3f(1.0, 1.0, 0.0);
 


  
glLoadIdentity();

  //Setting the point size
    glPointSize(5.0);
    glBegin(GL_POINTS);
    
   //Drawing each particle using its coordinates
  
    for(int k=0;k<CoordinateCount;k++){
        //If the sim type is o, all the particles will die
        if(SimType!='o'){
          //If the particles time to live is 0, they will die
          if(particleArray[k].particleTTL != 0){

            glColor3f(0,particleArray[k].particleTTL/particleArray[k].maxTTL*100,0);
            
            //Drawing the particles            
            glVertex3f(particleArray[k].xCoordinate,particleArray[k].yCoordinate,0.0);
          }
        }
    }
   

  
    
  
    
    glEnd();
    


      glFlush(); // make sure the polygon is pushed to the graphics card

      glutSwapBuffers(); // flip the buffers so the polygon I just drew is now on the front buffer
}


//Getting user input for the wall collision test and the transparency test
void keyboardFunc(unsigned char key, int x, int y ){

  switch(key){
    case 's' :
      cout << "Stop time" << endl;
      //Used to set the sim type to stop
      SimType = 's';

      if(falling == true){
          falling = false;
      }

      else {
          falling=true;
      }
    

    break;

  case 'S' :

      cout << "Stop time but with a captial s" << endl;
      SimType='S';

      if(falling == true){
          falling = false;
      }

      else {
          falling=true;
      }


    break;

  case 'g' :

      SimType = 'g';
      cout << "Gravity Small time step" << endl;
      
    break;

  case 'G' :
  SimType = 'G';
     
      cout << "Gravity big time step" << endl;

    break;

  case 'b' :
      SimType = 'b';
      cout << "Black hole small time step" << endl;

    break;

  case 'B' :

      SimType = 'B';
      cout << "Black hole big time step" << endl;

    break;

  case 'o' :
    SimType = 'o';
    cout << "Particle Death" << endl;

}}

//Getting the users mouse state info
void mouseActionReceiver(int button, int state, int x, int y) {
    
    //Checking if the user is clicking the left mouse button
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        //Checking if the mouse is held down
        MouseHeldDown=true;
        isClicked = true;
    }
    else{
        MouseHeldDown=false;
    }

    if(x<500 && x>=0 || y<500 && y>=0){
        //Putting the x and y into our screen coordinates of 1 and -1 by dividing it by 250
        xCoord=x/250.0-1.0;
        yCoord=(y/250.0-1.0)*-1;
    }
}

//Capturing the motion of the mouse by getting its x and y
void motionCapture(int x,int y){
    if(x<500 && x>=0 || y<500 && y>=0){
        //Putting the x and y into our screen coordinates of 1 and -1 by dividing it by 250
        xCoord=x/250.0-1.0;
        yCoord=(y/250.0-1.0)*-1;

        //Getting the x and y velocity by subtracting the previous x and y by the current x and y
        xVelocity=(x - previousX)/dt;
        
        yVelocity=(y - previousY)/dt;


        previousX = x;
        previousY = y;
    
    }
    cout << xCoord << " " <<yCoord << endl;
}











//Setting up the program
void begin(){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
//Setting our view
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);


  glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

 
}



int main(int argc, char** argv)
{
    // First set up GLUT
    glutInit( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Make the window
    int windowHandle = glutCreateWindow("Program 1");
    glutSetWindow(windowHandle);

    init();
    begin();
    //ParticleInfo particle;

    // Place and size the window
    glutPositionWindow ( 100, 100 );
    glutReshapeWindow( 500, 500 );

    //Calling our functions for drawing and user input.
    glutMouseFunc(mouseActionReceiver);
    glutMotionFunc(motionCapture);
    glutKeyboardFunc(keyboardFunc);
    glutDisplayFunc(display);

    

    glutTimerFunc(400,timer,30);


    // Go into an infinite loop waiting for messages
    glutMainLoop();
    return 0;
}











