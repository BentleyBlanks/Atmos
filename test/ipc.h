#pragma once
#include <core/messageQueue/a3Message.h>
#include <core/messageQueue/a3MessageQueueIPC.h>
#include <Atmos.h>

a3Scene* generateScene()
{

}

void main()
{
    const int msgMaxNum = 100;
    const int msgMaxSize = sizeof(a3C2SGridBufferMessage);

    a3S2CInitMessage* initMsg = NULL;

    a3MessageQueueIPC ipcC2S, ipcS2C;
    ipcS2C.init(L"Who's Your Daddy S2C", false, 50, 512);
    ipcC2S.init(L"Who's Your Daddy C2S", false, msgMaxNum, msgMaxSize);

    t3Timer timer;
    timer.start();

    // ------------------------------------------------Init Scene------------------------------------------------
    // waiting till initMsg. 60fps
    bool recieved = false;
    while(!recieved)
    {
        // 超时关闭
        //timer.end();
        //if(timer.difference() < 7)
        //{
        //    a3Log::info("Closing.............");
        //    return;
        //}
        if(!ipcS2C.isEmpty())
        {
            // stack maybe full
            char* msg_buffer = new char[msgMaxSize];
            memset(msg_buffer, 0, msgMaxSize);

            a3MessageEntryHead* pMsg = (a3MessageEntryHead*) msg_buffer;
            while(ipcS2C.dequeue(*pMsg))
            {
                if(pMsg->type == A3_S2C_MSG_INIT)
                {
                    const a3S2CInitMessage* msg = (const a3S2CInitMessage*) pMsg;
                    initMsg = new a3S2CInitMessage(*msg);
                    recieved = true;
                }
            }

            delete msg_buffer;
        }

        // 限帧
        Sleep(16.66667);
    }

    a3Log::debug("Init message recieved\n");

    // ------------------------------------------------Scene------------------------------------------------
    //int startX = 0, startY = 0;
    int imageWidth = initMsg->imageWidth, imageHeight = initMsg->imageHeight;

    // grid level
    int levelX = initMsg->levelX, levelY = initMsg->levelY;
    int gridCount = levelX * levelY;

    int gridWidth = imageWidth / levelX, gridHeight = imageHeight / levelY;
    int gridSize = gridWidth * gridHeight * 3;

    a3Log::info("Dimension[%d, %d] Level[%d, %d] Grid[%d, %d] Size:%d\n", imageWidth, imageHeight, levelX, levelY, gridWidth, gridHeight, gridSize);

    // ------------------------------------------------Renderer------------------------------------------------
    // 相机
    a3Sensor* camera;

    // 积分器
    a3Integrator* integrator;

    // 伽马校正开关
    bool enableGammaCorrection;
    bool enableToneMapping;

    // Samples Per Pixels
    int spp;

    // grid buffer pointer
    float** gridBuffer = new float*[gridCount];
    for(int i = 0; i < gridCount; i++)
    {
        gridBuffer[i] = new float[gridSize];
    }

    a3Random r;
    // ------------------------------------------------Render------------------------------------------------
    for(int currentGrid = 0; currentGrid < gridCount; currentGrid++)
    {
        // pointer to the current grid buffer
        float* bufferPointer = gridBuffer[currentGrid];

#pragma omp parallel for schedule(dynamic)
        for(int x = 0; x < gridWidth; x++)
        {
            a3Log::info("Grid[%d / %d] Rendering: %8.2f \r", currentGrid, levelX * levelY, (float) x / gridWidth * 100);
            for(int y = 0; y < gridHeight; y++)
            {
                bufferPointer[(x + y * gridWidth) * 3 + 0] = r.randomFloat();
                bufferPointer[(x + y * gridWidth) * 3 + 1] = r.randomFloat();
                bufferPointer[(x + y * gridWidth) * 3 + 2] = r.randomFloat();
            }
        }
        a3Log::print("\n");

        // ------------------------------------------------Send------------------------------------------------
        if(!ipcC2S.isFull())
        {
            // grid left-up position
            int gridX = (int) (currentGrid % levelX) * gridWidth;
            int gridY = (int) (currentGrid / levelX) * gridHeight;

            // right-down
            int gridEndX = gridX + gridWidth;
            int gridEndY = gridY + gridHeight;

            a3Log::debug("Grid Size:%d, Start[%d, %d], End[%d, %d]\n", gridSize, gridX, gridY, gridEndX, gridEndY);
            if(gridSize > A3_GRIDBUFFER_LENGTH)
                a3Log::warning("Grid size too long\n");

            // send
            a3C2SGridBufferMessage gridMsg;

            gridMsg.currentIndex = currentGrid;
            gridMsg.gridX = gridX;
            gridMsg.gridY = gridY;

            gridMsg.gridWidth = gridSize <= A3_GRIDBUFFER_LENGTH ? gridWidth : A3_GRIDBUFFER_MAX_WIDTH;
            gridMsg.gridHeight = gridSize <= A3_GRIDBUFFER_LENGTH ? gridHeight : A3_GRIDBUFFER_MAX_HEIGHT;

            // copy buffer to msg
            memcpy(gridMsg.buffer, bufferPointer, sizeof(float) * gridSize);

            ipcC2S.enqueue(gridMsg);
        }
        else
            a3Log::warning("Full Message Queue\n");
    }
}