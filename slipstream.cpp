#include "slipstream.h"

Slipstream::Slipstream(Rectangle i, int sspeed)
{
    stream=i;
    speed=sspeed;
}

int Slipstream::GetSpeed(){
    return speed;
}
void Slipstream::SetSpeed(int hi){
    speed=hi;
}

void Slipstream::resetoffset(){
    for(int i=0;i<10;i++){
        offset[i]=0;
    }
}
Rectangle Slipstream::Collider(){
    return stream;
}

void Slipstream::SetCollider(Rectangle hi){
    stream = hi;
}

//from easings.net
float easeOutQuad(float x) {
    if(x<=1){
return 1 - (1 - x) * (1 - x);
    }else{
        return 1;
    }
}

void Slipstream::update(){

    //DrawRectangle(stream.x,stream.y,stream.width,stream.height,BLACK);
    // this isnt  

    
    // only 10 waves shown proportional to the height
    // offset by the speed to know how its gonna push the boat


    // depending on the direction it will go another way and start at a specific area all proportional to the rectangle itself
    // then offset by an array so that it can be a clean offsetting pattern
    if(speed<0){
    for(int i=0;i<10;i++){
    //start fading in
    step1[i]-=speed*0.01*GetFrameTime();
    // set a color that gets slightly adjusted by the for loop that fades in and out
    Color j = {204,200,static_cast<unsigned char>(i+234),static_cast<unsigned char>(180*easeOutQuad(step1[i])-180*easeOutQuad(step2[i]))};
    offset[i]-=speed*GetFrameTime();
    // when reach 90% of condition then start fading out
    if(offset[i]+i*stream.height*0.10>=stream.height*0.9){
        step2[i]-=speed*0.05*GetFrameTime();
    }
    if(offset[i]+i*stream.height*0.10>=stream.height){
        //reset lerp values
         step2[i]=0;
         step1[i]=0;
        offset[i]=-i*stream.height*0.10;
    }
    // direction lines
    DrawLineEx((Vector2){stream.x,stream.y+offset[i]+(float)(i*stream.height*0.10)},(Vector2){(float)(stream.x+stream.width/2.0),stream.y+(float)(stream.height*0.05)+offset[i]+(float)(i*stream.height*0.10)},4.0,j);
    DrawLineEx((Vector2){(float)(stream.x+stream.width/2.0),stream.y+(float)(stream.height*0.05)+offset[i]+(float)(i*stream.height*0.10)},(Vector2){stream.x+stream.width,stream.y+offset[i]+(float)(i*stream.height*0.10)},4.0,j);
    }
    }else{
    for(int i=0;i<10;i++){

 //start fading in
    step1[i]+=speed*0.01*GetFrameTime();
    // set a color that gets slightly adjusted by the for loop that fades in and out
    Color j = {204,200,static_cast<unsigned char>(i+234),static_cast<unsigned char>(180*easeOutQuad(step1[i])-180*easeOutQuad(step2[i]))};
    offset[i]-=speed*GetFrameTime();

    // when reach 90% of condition then start fading out
    if(offset[i]-i*stream.height*0.10<=-stream.height*0.9){
        step2[i]+=speed*0.05*GetFrameTime();
    }

    if(offset[i]-i*stream.height*0.10<=-stream.height){
        //reset lerp values
         step2[i]=0;
         step1[i]=0;
        offset[i]=i*stream.height*0.10;
    }
    // direction lines
    DrawLineEx((Vector2){stream.x,stream.y+stream.height+offset[i]-(float)(i*stream.height*0.10)},(Vector2){(float)(stream.x+stream.width/2.0),(stream.y+stream.height)-(float)(stream.height*0.05)+offset[i]-(float)(i*stream.height*0.10)},4.0,j);
    DrawLineEx((Vector2){(float)(stream.x+stream.width/2.0),(stream.y+stream.height)-(float)(stream.height*0.05)+offset[i]-(float)(i*stream.height*0.10)},(Vector2){stream.x+stream.width,stream.y+stream.height+offset[i]-(float)(i*stream.height*0.10)},4.0,j);
    }
    }

}

