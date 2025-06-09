<div align="center">
    <img src="https://capsule-render.vercel.app/api?type=rect&color=fcff4d&height=120&text=Sphere%20in%20Handong&animation=twinkling&fontColor=000000&fontSize=50" />
</div>

<div align="center">

<!-- 기술 배지 -->
<img src="https://img.shields.io/badge/GitHub-181717?style=flat-square&logo=github&logoColor=white" />
<img src="https://img.shields.io/badge/C++-00599C?style=flat-square&logo=c%2b%2b&logoColor=white" />
<img src="https://img.shields.io/badge/OpenCV-5C3EE8?style=flat-square&logo=opencv&logoColor=white" />
<img src="https://img.shields.io/badge/SDL2-014C99?style=flat-square&logo=SDL&logoColor=white" />
<img src="https://img.shields.io/badge/Linux-FCC624?style=flat-square&logo=linux&logoColor=black" />
<img src="https://img.shields.io/badge/CMake-064F8C?style=flat-square&logo=cmake&logoColor=white" />

</div>




## Project Overview

**Sphere in Handong**은 구형 디스플레이 장치 위에 2D 이미지 또는 영상을 실시간으로 투영하기 위한 **LED 구 디스플레이 제어 시스템**입니다. 사용자는 간단한 이미지 입력만으로도 Sphere의 전면에 정확하게 맵핑된 결과를 확인할 수 있으며, 실시간 렌더링 및 제어 기능을 제공합니다.


---
## Key Features

- Sphere(구체형 디스플레이)에 2D 이미지 실시간 투영
- 이미지 다운샘플링 및 맵핑 (Nearest, Bilinear, Average Pooling 지원)
- E1.31(sACN) 기반 실시간 조명 패킷 전송
- SDL2 기반 UI와 실시간 전송 상태 시각화
- BeagleBone Black 기반의 하드웨어 연동
- 1만 6천 개 이상의 LED 실시간 동기화 지원

---

## Technology Stack

| 항목        | 설명                                                                 |
|-------------|----------------------------------------------------------------------|
| **언어**     | C++                                                                  |
| **라이브러리** | OpenCV, SDL2, CMake                                                  |
| **하드웨어** | BeagleBone Black, K32A-B Controller                                 |
| **OS**      | Linux 환경 최적화 (Ubuntu, Debian 기반 권장)                        |
| **프로토콜** | E1.31 (Streaming ACN), UDP 소켓 통신 기반                            |

---

## Installation & Execution

### 1. Dependency Installation 
#### 1.1 libe131   
https://github.com/hhromic/libe131
#### 1.2 yt-dlp
https://github.com/yt-dlp/yt-dlp?tab=readme-ov-file#installation
#### 1.3 SDL2
```bash
sudo apt update
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libjsoncpp-dev
```

### 2. Build
```bash
git clone https://github.com/MCNL-HGU/sphere_in_handong.git
cd sphere_in_handong
make
```

### 3. Run
```bash
./sdl_ui
```

---
## Project Structure
```bash
.
├── include/
│   └── image_processor.h      # 이미지 변환 및 LED 맵핑 함수 정의
├── src/
│   ├── image_processor.cpp    # 맵핑 알고리즘 (nearest, bilinear 등)
│   ├── sphere_viewer.cpp      # 메인 렌더링 루프 + 소켓 전송
│   └── sdl_ui.cpp             # SDL2 UI 처리
├── assets/                    # 샘플 이미지 및 리소스
├── CMakeLists.txt
└── README.md
```

---
## Contributors

| 이름 | 역할 | 연락처 |
|------|------|--------|
| 정성호 | 프로젝트 리드, 시스템 설계 | [@sungho](https://github.com/onuyon) |
| 이창건 | yt-dlp 연동, 이미지 유튜브 영상 | [@changun](https://github.com/hami0713) |
| 이온유 | SDL2 기반 UI 설계, LED 제어 | [@onyu](https://github.com/gippeumee) |
| 황수현 | 프로젝트 계획, Sphere 영상 제작 | [@suihun](https://github.com/gippeumee) |

> 🙌 함께한 MCNL 연구실 팀원들께 감사드립니다.
