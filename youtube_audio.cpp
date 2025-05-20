#include "packet_sender/sleep.h"
#include "display_manager/display_manager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>   // system() 사용
#include <opencv2/opencv.hpp>
#include <utility>
#include <iostream>

using namespace std;

pair<string, string> getYouTubeStreamURLs(const string& youtube_url) {
    // Request 1080p video and best audio separately
    string command = "yt-dlp -f \"bestvideo[height<=1080]+bestaudio\" -g " + youtube_url + " 2>/dev/null";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        cerr << "Error: Failed to run yt-dlp." << endl;
        return {"", ""};
    }

    char buffer[1024];
    vector<string> lines;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        lines.emplace_back(buffer);
    }
    pclose(pipe);

    // Clean up newline characters
    for (auto& line : lines) {
        size_t pos = line.find('\n');
        if (pos != string::npos)
            line = line.substr(0, pos);
    }

    // We expect exactly 2 lines: video URL, then audio URL
    if (lines.size() >= 2) {
        return {lines[0], lines[1]};
    } else {
        cerr << "Error: Could not extract both video and audio URLs." << endl;
        return {"", ""};
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3 || string(argv[1]) != "-url") {
        cerr << "Usage: " << argv[0] << " -url <YouTube URL>" << endl;
        return -1;
    }

    string youtube_url = argv[2];
    cout << "Fetching stream URLs for: " << youtube_url << endl;

    // 1. 비디오 + 오디오 스트림 URL 가져오기
    auto [video_url, audio_url] = getYouTubeStreamURLs(youtube_url);
    if (video_url.empty() || audio_url.empty()) {
        cerr << "Error: Failed to get YouTube stream URLs." << endl;
        return -1;
    }

    cout << "Video URL: " << video_url << endl;
    cout << "Audio URL: " << audio_url << endl;

    // 2. 오디오 재생 시작 (백그라운드에서 ffplay로)
    string audio_command = "ffplay -nodisp -autoexit \"" + audio_url + "\" > /dev/null 2>&1 &";
    system(audio_command.c_str());

    // 3. OpenCV로 비디오 재생
    cv::VideoCapture cap(video_url);
    if (!cap.isOpened()) {
        cerr << "Error: Could not open video stream." << endl;
        return -1;
    }

    cv::Mat frame;
    cap >> frame;

    // LED 디스플레이 초기화
    DisplayManager* display = new DisplayManager(frame.rows, frame.cols, 120, 240, 10, 0, "192.168.50.72");
    cout << frame.rows << "x" << frame.cols << endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            cap.open(video_url);
            continue;
        }

        unsigned char* rgb_data = frame.data;
        display->display_itp(rgb_data, false);

        cv::imshow("YouTube Video", frame);
        usleep(20000);

        if (cv::waitKey(30) == 27) break;  // ESC 누르면 종료
    }

    cap.release();
    cv::destroyAllWindows();

    // ffplay도 종료하고 싶으면 아래 명령어 추가 가능
    // system("pkill ffplay");

    return 0;
}

