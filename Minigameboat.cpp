#include "minigameboat.h"
#include <cmath>
using namespace std;

Minigameboat::Minigameboat(int bspeed,int bforce)
{
    boatspeed=bspeed;
    force = bforce;
    restart();
}


// recursive method that sets all objects to random positions by calling itself 
// and calling itself to run again if its colliding with all other obstacle to make sure they dont overlap
void Minigameboat::SetRandom(int i, int max){
    Rectangle j = {(float)(rand()%700+100),(float)(rand()%700-300),(float)(rand()%50+20),(float)(rand()%200+200)}; 
    if(i==max){
    return;
    }else{
    int s=0;
    for(int i=0;i<max;i++){
        if(CheckCollisionRecs(j,obstacles[i].Collider())){
            s=1;
        }
    }
    if(s==1){
    SetRandom(i,max);
    } else{
    obstacles[i].SetCollider(j);
    int k=(rand()%100+30);
    int n = rand()%200;
    if(n>99){
        k*=-1;
    }
    obstacles[i].SetSpeed(k);
        SetRandom(i+1,max);

    }
    }
    
}

// this sets the stage random amount of objects random positions random ice berg same starting boat position no time to prepare since you are an algorithm you run immeadiltiy
void Minigameboat::restart(){
    boatPosition = {-10, 400};
    obc=rand()%50;
    icebergoffset = rand()%345+100;
    for(int i=0;i<obc;i++){
        obstacles[i].resetoffset();
    }
    SetRandom(0,obc);
}

// from easings.net
float easeOutBack(float x) {
    if(x<=1.15){
    const float c1 = 1.70158;
    const float c3 = c1 + 1;
    return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
    }else{
        return 1;
    }
}
void Minigameboat::update(){
    BeginDrawing();
    mousePosition = GetMousePosition();
    ClearBackground((Color){100,180,200,255});
    // frametime thanks to raylib.com example this controls all effects
    step+=50*GetFrameTime();
    step2+=50*GetFrameTime();
    step3+=50*GetFrameTime();
    // start moving up or down based on where the mouse is clicking
    Color cursor={255,255,255,67};
    Color boat={200,200,200,255};
    Color boat2={200,200,200,255};

    // fliping the signal with a local variable and a boolean i declared in the object
    int signn=1;
    if(fabs(mousePosition.y-boatPosition.y)>5.1f && mousePosition.y<800 && mousePosition.y>0){
        if(mousePosition.y<boatPosition.y){
          signn=1;
            if(fart==false){
                step=0;
                fart=true;
            }
        }else{
            if(fart){
                step=0;
                fart=false;
            }
            signn=-1;
        }
    }else{
    step=0;
    }

    // signal thing to see which direction your going
    if(step!=0){
    for(int i=0;i<3;i++){
    
        DrawRing((Vector2){boatPosition.x,boatPosition.y+signn*easeOutBack(step*0.035)*(-18-i*5)},15+i,11,0,signn*easeOutBack(step*0.035)*(-180),5,
        (Color){
            static_cast<unsigned char>(200-50*easeOutBack(step2*0.05)),
             static_cast<unsigned char>(50+200*easeOutBack(step2*0.05)),
            static_cast<unsigned char>(90+50*easeOutBack(step2*0.05)),
            static_cast<unsigned char>(100-70*sin(step*i*0.05))});
    }
    }



    // press button to change some collors and go in the force of the direction
    if(IsMouseButtonDown(0) && fabs(mousePosition.y-boatPosition.y)>5.1f && mousePosition.y<800 && mousePosition.y>0){
    cursor={255,190,199,100};
    step3=0;
    boat2={static_cast<unsigned char>(200+50*easeOutBack(step2*0.08)),210,200,255};
    if(mousePosition.y<boatPosition.y){
        boatPosition.y-=force*1.0f*GetFrameTime();
    }else{
           boatPosition.y+=force*1.0f*GetFrameTime();
    }
    }else{
        boat2={static_cast<unsigned char>(250-50*easeOutBack(step3*0.05)),200,200,255};
        step2=0;
    }

    // switch to the lose screen
    if(boatPosition.x>1000){
        boatPosition.x=-20;
    }
    

    //cursor
    DrawCircle(mousePosition.x,mousePosition.y, 10, cursor);

    // autonomous cargo ship that moves right on its own!!!!!! thats our current reality in the production section on the calculating empires art by kate crawford
    // the algorithm can only affect the y position but those stupid waves are in the way.
    DrawCircleGradient(boatPosition.x,boatPosition.y, 20,boat,boat2 );
    DrawCircle(boatPosition.x,boatPosition.y, 5, RED);


    boatPosition.x+=boatspeed*resistance*GetFrameTime();


/* i used Inefficient code here that is O(N) where the number of obstacles is N. when i looked up what is effiecent code for detecting collisions for multiple objects i found this forum
https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det and
according to https://www.geeksforgeeks.org/dsa/quad-tree/ it would have been O(log N) which doesnt increase as fast as O(N) making it lag less  
so instead i put it so there can only be a maximum of 50 obstacles becuase i didnt have time to implement 
a data structure that is beyond the scope of this course and i just learned about big o about a week ago so scrolling through that explantion 
it reminded me of the binary Insertion sort algorithm we went over in class because its also recursivle dividing but the quadtree seems to recursively divide 2d space*/
int resistancecheck = 0;
for(int i=0;i<obc;i++){
    obstacles[i].update();
    if(CheckCollisionCircleRec(boatPosition,20,obstacles[i].Collider())){
        boatPosition.y-=obstacles[i].GetSpeed()*GetFrameTime();
        // make sure it doesnt go off the screen since the obstacles can make the boat do that
        if(boatPosition.y<0){
            boatPosition.y=0;
        }
        if(boatPosition.y>800){
            boatPosition.y=800;
        }
        resistance=0.321;
        resistancecheck=1;
    }
}
// go back to normal speed
if(resistancecheck!=1){
    resistance=1;
}

// put winnning state
if(CheckCollisionCircleRec(boatPosition,20,(Rectangle){876,(float)(icebergoffset),50,100})){
    restart();
    won=true;
}

    //ice berg
    DrawRectangle(876,(float)(icebergoffset),50,100,(Color){ 255, 233, 255, 255 } );
    EndDrawing();
    
}




