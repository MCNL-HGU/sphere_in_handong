#include "packet_sender/sleep.h"
#include <opencv2/opencv.hpp>
#include "display_manager/display_manager.h"
#include <SDL2/SDL.h>


#include <iostream>
#include <vector>
#include <unistd.h> // usleep 사용

using namespace std;

int main() {
    std::string video_path = "cry.MOV";

    cv::VideoCapture cap(video_path);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video file." << std::endl;
        return -1;
    }

    //CGAME CONTROLLER INIT SETTING
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    
    SDL_GameController *controller = nullptr;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                std::cout << "Game controller connected: " << SDL_GameControllerName(controller) << std::endl;
                break;
            }
        }
    }
    
    if (!controller) {
        std::cerr << "No compatible game controller found." << std::endl;
        SDL_Quit();
        return -1;
    }
    
    

   
    cv::Mat frame, frame_rgb;

    cap >> frame;
    if (frame.empty()) {
        std::cerr << "Error: Failed to read the first frame." << std::endl;
        return -1;
    }
    const char *ip = "192.168.50.72";
    
    DisplayManager * display= new DisplayManager(frame.rows, frame.cols, 0, 360, 0, 0, "192.168.50.72");
    E131Sender *sender = new E131Sender(ip);
    bool running =true;
    while (running) {
        cap >> frame;
        if (frame.empty()) break;
    
        unsigned char *rgb_data = frame.data;
        display->display_mean(rgb_data,false);
        cv::imshow("Local Video Stream", frame);
    
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_CONTROLLERBUTTONDOWN) {
                if (e.cbutton.button == SDL_CONTROLLER_BUTTON_B) {
                    std::cout << "B button pressed. Exiting..." << std::endl;
                    running = false;
                }
            }
        }
    
        // Avoid burning 100% CPU
        SDL_Delay(10);
    }
    cap.release();
    cv::destroyAllWindows();
    if (controller) SDL_GameControllerClose(controller);
    SDL_Quit();


    return 0;
}
