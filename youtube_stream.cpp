#include "packet_sender/sleep.h"
#include "display_manager/display_manager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>   // system() 사용
#include <opencv2/opencv.hpp>


using namespace std;

// yt-dlp를 실행하여 YouTube 영상의 직접 URL을 가져오는 함수
string getYouTubeStreamURL(const string& youtube_url) {
    string command = "yt-dlp -g " + youtube_url + " 2>/dev/null";  // 오류 출력을 숨김
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

    
    cv::VideoCapture cap(stream_url);
    if (!cap.isOpened()) {
        cerr << " Error: Could not open video stream." << endl;
        return -1;
    }

    int rows[54] = {
        104, 126, 141, 157, 172, 187, 199, 211, 224, 235, 246, 255, 263, 272, 279, 286, 293, 299,
        306, 311, 316, 320, 326, 331, 335, 337, 342, 345, 347, 351, 353, 355, 357, 356, 358, 359,
        359, 359, 359, 358, 358, 356, 355, 353, 351, 348, 345, 342, 338, 334, 330, 325, 319, 314
    };

    const char *ip = "192.168.50.72";
    cv::Mat frame, frame_rgb;
    cap >> frame;

    DisplayManager * display= new DisplayManager(frame.rows, frame.cols, 0, 360, 0, 0, "192.168.50.72");
    cout << frame.rows << " " << frame.cols << endl;
    while (true) {
        cap >> frame;
        if (frame.empty()){
		    cap.open(stream_url);
		    continue;
	    }

        // 🔹 BGR → RGB 변환
        //cv::cvtColor(frame, frame_rgb, cv::COLOR_BGR2RGB);

        // 🔹 unsigned char* 데이터 변환
        unsigned char *rgb_data = frame.data;

        // 🔹 LED 패널로 데이터 매핑 및 전송
        
        display->display(rgb_data);
        
        /*
        cout << "start" << endl;
        for(int i = 0; i < frame.rows * frame.cols * 3; i+=3){
            cout << "(" << (unsigned int)im[i] << " " << (unsigned int)im[i+1] << " " << (unsigned int)im[i+2] << ")";
        }
        cout << endl;
*/
        usleep(30000);


        // 🔹 변환된 RGB 프레임을 다시 BGR로 변환하여 화면에 출력
        // cv::cvtColor(frame_rgb, frame, cv::COLOR_RGB2BGR);


        // 🔹 ESC 키(27)로 종료
        if (cv::waitKey(30) == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
