#include<raylib.h>

Color green = Color{38,185,154,255};
Color dark_green = Color{20,160,133,255};
Color light_green = Color{129,204,184,255};
Color yellow = Color{243,213,91,255};

int player_score=0;
int cpu_score=0;

class Ball{
    public:
    float x,y;
    float x_speed,y_speed;
    int radius;

    void draw(){
        DrawCircle(x,y,radius,yellow);
    }

    void update(){
        x += x_speed;
        y += y_speed;
        if(y>=600-radius || y<=0+radius )
        {
            y_speed *= -1;
        }
        if(x>=1000-radius)//cpu scores
        {
            cpu_score++;
            resetball();
            x_speed *=1;
        }
        if(x<=0+radius)//player scores
        {
            player_score++;
            resetball();
            x_speed *=-1;
        }    
    }
    void resetball()
        {
            x = GetScreenWidth()/2;
            y = GetScreenHeight()/2;
            int speed_dir[2]={1,-1};
            y_speed *= speed_dir[GetRandomValue(0,1)];
        }
};

class Paddle{
    protected:
    void limit_paddle_movement()
    {
        if(y<=5)
        {
            y = 5;
        }
        if(y>=GetScreenHeight()-height-5)
        {
            y = GetScreenHeight()-height-5;
        }
    }
    public:
    float x,y;
    float speed;
    float width,height;

    void draw(){
       DrawRectangleRounded(Rectangle{x,y,width,height},0.8,0,WHITE);
    }

    void update(){
        if(IsKeyDown(KEY_DOWN))
        {
            y += speed;
        }
        if(IsKeyDown(KEY_UP))
        {
            y -= speed;
        }
        limit_paddle_movement();
    }
};

class cpuaddle: public Paddle{
    public:

    void update(int ball_y)
    {
        if(y + height/2 > ball_y)
        {
            y -= speed;
        }
        if(y + height/2 < ball_y)
        {
            y += speed;
        }
        limit_paddle_movement();
    }
};

Ball ball;
Paddle p1;
cpuaddle cpu;

int main()
{
    const int screen_width = 1000, screen_height = 600;
    InitWindow(screen_width,screen_height,"Ping-Pong");
    SetTargetFPS(240);

    ball.radius = 10;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.x_speed = 8/4;
    ball.y_speed = 8/4;

    p1.x = screen_width-30;
    p1.y = screen_height/2 - 60;
    p1.width = 25;
    p1.height = 120;
    p1.speed = 8/3;

    cpu.x = 5;
    cpu.y = screen_height/2 - 60;
    cpu.width = 25;
    cpu.height = 120;
    cpu.speed = 1;

    //game loop
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
        //updates
        ball.update();
        p1.update();
        cpu.update(ball.y);
        ClearBackground(dark_green);
        DrawRectangle(screen_width/2,0,screen_width/2,screen_height,green);
        DrawCircle(screen_width/2,screen_height/2,150,light_green);

        //collision check
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{p1.x,p1.y,p1.width,p1.height}))
        {
            ball.x_speed *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.width,cpu.height}))
        {
            ball.x_speed *= -1;
        }

        //draw static objects
        DrawLine(screen_width/2,0,screen_width/2,screen_height,WHITE);

        //draw non-static objects
        ball.draw();
        p1.draw();
        cpu.draw();
        DrawText(TextFormat("%i",cpu_score),screen_width/4-20,20,80,WHITE); 
        DrawText(TextFormat("%i",player_score),3*screen_width/4-20,20,80,WHITE); 
       
        EndDrawing();
    }

    CloseWindow();
    return 0;
}