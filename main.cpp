#include "main.h"
#include <iostream>
#include "raylib.h"
using namespace std;

int main() {

    // Starting
    InitWindow(1000, 800, "hackathon example");
    SetTargetFPS(30);

    Vector2 mousePosition;
    Vector2 boatPosition= { 100, 400};
    Color blue={120,240,200,255};
    Rectangle stream ={500,300,50,200};
    // run every frame
    while (!WindowShouldClose()) {
    BeginDrawing();
    mousePosition = GetMousePosition();
    //background color
    ClearBackground(blue);
    // position= Vector2MoveTowards(Vector2 v, Vector2 target, float maxDistance)
    if(IsMouseButtonDown(0) && abs(mousePosition.y-boatPosition.y)>5.1f && mousePosition.y<800 && mousePosition.y>0){
    if(mousePosition.y<boatPosition.y){
        boatPosition.y-=120.0f*GetFrameTime();
    }else{
        boatPosition.y+=120.0f*GetFrameTime();
    }

    }
    if(boatPosition.x>1000){
        boatPosition.x=-20;
    }

    //slipstream
    DrawRectangle(stream.x,stream.y,stream.width,stream.height,(Color){ 58, 79, 23, 255 } );

    //cursor
    DrawCircle(mousePosition.x,mousePosition.y, 20, (Color){255,255,255,100});

    //boat
    DrawCircle(boatPosition.x,boatPosition.y, 20, WHITE);

    if(CheckCollisionCircleRec(boatPosition,20,stream)){
        boatPosition.y-=100*GetFrameTime();
            boatPosition.x+=20.5*GetFrameTime();
    }else{
    boatPosition.x+=70.5*GetFrameTime();

    }



    //ice berg
    DrawRectangle(800,300,20,100,(Color){ 219, 233, 255, 255 } );
    EndDrawing();
    }
    /***
    //from raylibs example
deltaCircle.x += GetFrameTime()*6.0f*speed;
     your project code here!

     ***/

  // To see graphics for minigame 2, right now no gameplay loop
  // minigame2_sample();


    return 0;
}