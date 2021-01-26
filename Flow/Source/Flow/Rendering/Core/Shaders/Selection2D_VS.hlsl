cbuffer UIBuffer : register(b1)
{
    float xPosition;
    float yPosition;
    float xWidth;
    float yWidth;
    
    float sizeX;
    float sizeY;
    float padding;
    float padding2;
};

struct VSOut
{
    float4 Pos : SV_Position;
};

VSOut main(float2 pos : Position)
{
    VSOut vso;
       
    //Map to clip space
    float unitX = 1 / xWidth;
    float unitY = -1 / yWidth;
    float x = unitX * xPosition;
    float y = unitY * yPosition;
    float xPos = x + (pos.x * sizeX * unitX);
    float yPos = y + (pos.y * sizeY * unitY);
    
    //Centre to top left    
    vso.Pos = float4((xPos * 2) - 1.0f, (yPos * 2) + 1.0f, 0.0f, 1.0f);
    return vso;
}