#include "packet_sender/sleep.h"
#include "display_manager/display_manager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>   // system() 사용
#include <opencv2/opencv.hpp>
#include <SDL2/SDL.h>


using namespace std;

// yt-dlp를 실행하여 YouTube 영상의 직접 URL을 가져오는 함수
string getYouTubeStreamURL(const string& youtube_url) {
    string command = "yt-dlp -S \"res:1080\" -g " + youtube_url + " 2>/dev/null";  // 오류 출력을 숨김
    FILE* pipe = popen(command.c_str(), "r");  // yt-dlp 실행 결과를 읽기 위한 파이프
    if (!pipe) {
        cerr << " Error: Failed to run yt-dlp." << endl;
        return "";
    }

    char buffer[1024];
    string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;  // 결과를 문자열로 저장
    }
    pclose(pipe);

    // 개행 문자 제거 (출력값이 여러 줄일 경우 첫 번째 줄만 사용)
    size_t pos = result.find('\n');
    if (pos != string::npos) {
        result = result.substr(0, pos);
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 3 || string(argv[1]) != "-url") {
        cerr << " Usage: " << argv[0] << " -url <YouTube URL>" << endl;
        return -1;
    }
    

    string youtube_url = argv[2];
    cout << " Fetching stream URL for: " << youtube_url << endl;


    string stream_url = getYouTubeStreamURL(youtube_url);
    if (stream_url.empty()) {
        cerr << " Error: Failed to get YouTube stream URL." << endl;
        return -1;
    }

    cout << " Stream URL: " << stream_url << endl;

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
    
    cv::VideoCapture cap(stream_url);
    if (!cap.isOpened()) {
        cerr << " Error: Could not open video stream." << endl;
        return -1;
    }

    const char *ip = "192.168.50.72";
    cv::Mat frame, frame_rgb;
    cap >> frame;

    DisplayManager * display= new DisplayManager(frame.rows, frame.cols, 90, 270, 0, 0, "192.168.50.72");
    cout << frame.rows << " " << frame.cols << endl;
    bool running =true;
    while (running) {
        cap >> frame;
        if (frame.empty()){
		    cap.open(stream_url);
		    continue;
	    }
        unsigned char *rgb_data = frame.data;

        display->display_itp(rgb_data, false);
        
	//cv::imshow("youtube", frame);
       // usleep(10000);

        if (cv::waitKey(30) == 27) break;

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
        SDL_Delay(50);
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
