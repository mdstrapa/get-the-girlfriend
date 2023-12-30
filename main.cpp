#include "raylib.h"
#include <string>
#include <random>

struct WindowSize{
    int x;
    int y;
};

enum GirlType{
    GIRLFRIEND,
    WRONGGIRL,
    ALTERNATIVEGIRL
};

struct Girl{
    Texture2D image;
    GirlType type;
};

int generateRandomValue(int start,int end){
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(start, end);
    return distribution(generator);
}

int setGirlfriendXPos(){
    return generateRandomValue(50,700);
}

int setGirVelocity(){
    return generateRandomValue(2,4);
}

int setGirlOfTheTurn(){
    return generateRandomValue(0,9);
}

int main(){

    WindowSize windowSize{800,500};

    InitWindow(windowSize.x,windowSize.y,"Get The Girlfriend Game");

    Texture2D boyfriendTex = LoadTexture("images/boyfriend.png");
    Texture2D girlfriendTex = LoadTexture("images/girlfriend.png");
    Texture2D wrongGirlOneText = LoadTexture("images/wrongGirlOne.png");
    Texture2D wrongGirlTwoTex = LoadTexture("images/wrongGirlTwo.png");
    Texture2D wrongGirlThreeTex = LoadTexture("images/wrongGirlThree.png");
    Texture2D alternativeGirlTex = LoadTexture("images/alternativeGirl.png");
    Texture2D backgroundText = LoadTexture("images/background.png");

    Girl girlfriend{girlfriendTex,GIRLFRIEND};
    Girl wrongGirlOne{wrongGirlOneText,WRONGGIRL};
    Girl wrongGirlTwo{wrongGirlTwoTex,WRONGGIRL};
    Girl wrongGirlThree{wrongGirlThreeTex,WRONGGIRL};
    Girl alternativeGirl{alternativeGirlTex,ALTERNATIVEGIRL};
    Girl girls[] = {girlfriend,girlfriend,girlfriend,girlfriend,girlfriend,girlfriend,wrongGirlOne,wrongGirlTwo,wrongGirlThree,alternativeGirl};

    int girlOfTheTurn = 0;
    
    Vector2 girlfriendPos{};
    girlfriendPos.x = setGirlfriendXPos();
    girlfriendPos.y = -girlfriendTex.height;
    Color girlfriendColor = WHITE;
    int girlfriendVel = 2; //the first time the girl friend will descend with the lowest velocity
    int boyfriendXPos = (windowSize.x / 2) - (boyfriendTex.width / 2);

    int playerPontuation{0};
    bool shouldIncreasePontuation{true};
    bool shouldDecreasePontuation{true};
    bool didntGetGirl{};
    bool gotGirl{};

    int background1YPos{0};
    int background2YPos{-backgroundText.height};

    int focusControl = 5;
    float girlfriendScale{1.0};
    bool shouldChangeScale{};
    float floatTolerance = 0.0001;

    std::string intruction;

    SetTargetFPS(60);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(backgroundText,0,background1YPos,WHITE);
        DrawTexture(backgroundText,0,background2YPos,WHITE);
        background1YPos++;
        background2YPos++;

        DrawRectangle(0,0,800,35,WHITE);

        if(background1YPos == windowSize.y) background1YPos = -backgroundText.height;
        if(background2YPos == windowSize.y) background2YPos = -backgroundText.height;

        switch (girls[girlOfTheTurn].type)
        {
            case GIRLFRIEND:
            intruction = "Pegue sua menina!!";
            break;
            case WRONGGIRL:
            intruction = "Fuja dela!!";
            break;
            case ALTERNATIVEGIRL:
            intruction = "Você que decide o que fazer....";
            break;
        
        default:
            break;
        }
        DrawText(intruction.c_str(),0,5,30,RED);
        
        girlfriendPos.y = girlfriendPos.y + girlfriendVel;

        // reset the girlfriend texture
        if(girlfriendPos.y == windowSize.y) {
            girlfriendPos.y = -girlfriendTex.height;
            girlfriendPos.x = setGirlfriendXPos();
            shouldIncreasePontuation = true;
            shouldDecreasePontuation = true;
            girlfriendVel = setGirVelocity();
            gotGirl = false;
            didntGetGirl = false;
            girlfriendColor = WHITE;
            girlOfTheTurn = setGirlOfTheTurn();
            girlfriendScale = 1.0;
        }

        if(IsKeyPressed(KEY_LEFT)) boyfriendXPos = boyfriendXPos - 15;
        if(IsKeyPressed(KEY_RIGHT)) boyfriendXPos = boyfriendXPos + 15;

        Rectangle recBoyfriend{};
        recBoyfriend.x = boyfriendXPos;
        recBoyfriend.y = windowSize.y - boyfriendTex.height;
        recBoyfriend.width = boyfriendTex.width;
        recBoyfriend.height = boyfriendTex.height;

        Rectangle recGirlfriend{};
        recGirlfriend.x = girlfriendPos.x;
        recGirlfriend.y = girlfriendPos.y;
        recGirlfriend.width = girlfriendTex.width;
        recGirlfriend.height = girlfriendTex.height;

        if (CheckCollisionRecs(recBoyfriend,recGirlfriend)){ 
            if(girls[girlOfTheTurn].type == GIRLFRIEND || girls[girlOfTheTurn].type == ALTERNATIVEGIRL){
                DrawText("Bom trabalho!!!",windowSize.x / 2 - 100,windowSize.y / 2 - 15,30,BLUE);
                if (shouldIncreasePontuation){
                    playerPontuation = playerPontuation + 10;
                    shouldIncreasePontuation = false;
                    gotGirl = true;
                    girlfriendColor = BLUE;
                }
            }else{
                DrawText("Não pode pegar essa menina!!",windowSize.x / 2 - 200,windowSize.y / 2 - 15,30,RED);
                if(shouldDecreasePontuation){
                    playerPontuation = playerPontuation - 5;
                    didntGetGirl = true;
                    gotGirl = true;
                    shouldDecreasePontuation = false;
                    girlfriendColor = RED;
                }
            }
        }

        if((girlfriendPos.y + girlfriendTex.height >= windowSize.y) && !gotGirl){
            if(girls[girlOfTheTurn].type == GIRLFRIEND || girls[girlOfTheTurn].type == ALTERNATIVEGIRL){
                DrawText("Perdeu sua menina!",windowSize.x / 2 - 150,windowSize.y / 2 - 15,30,RED);
                if(shouldDecreasePontuation){
                    playerPontuation = playerPontuation - 5;
                    didntGetGirl = true;
                    shouldDecreasePontuation = false;
                    girlfriendColor = RED;
                }
            }else{
                DrawText("Bom trabalho!!",windowSize.x / 2 - 100,windowSize.y / 2 - 15,30,BLUE);
                if (shouldIncreasePontuation){
                    playerPontuation = playerPontuation + 10;
                    shouldIncreasePontuation = false;
                    gotGirl = false;
                    girlfriendColor = BLUE;
                }
            }
        }


        if(gotGirl) {
                if(focusControl % 5 == 0) shouldChangeScale = true;
                focusControl++;

                if(shouldChangeScale){
                    shouldChangeScale = false;
                    
                    if(std::abs(girlfriendScale - 1.2) < floatTolerance) girlfriendScale = 1.0;
                    else girlfriendScale = 1.2;
                }

                DrawTextureEx(girls[girlOfTheTurn].image,girlfriendPos,0.0,girlfriendScale,girlfriendColor);
            }
        else if (didntGetGirl) DrawTextureEx(girls[girlOfTheTurn].image,girlfriendPos,0.0,girlfriendScale,girlfriendColor);
        else DrawTexture(girls[girlOfTheTurn].image,girlfriendPos.x,girlfriendPos.y,girlfriendColor);

        DrawTexture(boyfriendTex,boyfriendXPos,windowSize.y - boyfriendTex.height,WHITE);
        
        std::string textToDraw = std::to_string(playerPontuation);
        std::string pontuationText = "Pontuação: ";
        pontuationText.append(textToDraw);
        DrawText(pontuationText.c_str(),550,5,30,GREEN);

        EndDrawing();
    }
    UnloadTexture(boyfriendTex);
    UnloadTexture(girlfriendTex);
    UnloadTexture(backgroundText);
    UnloadTexture(wrongGirlOneText);
    UnloadTexture(wrongGirlTwoTex);
    UnloadTexture(wrongGirlThreeTex);
    UnloadTexture(alternativeGirlTex);
    CloseWindow();
}