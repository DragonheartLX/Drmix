# Drmix
DragonheartLX的练习集

### 编译
- CMake Version >= 3.13
- Visual Studio 2022 (recommend)
- MinGW Version 12.2.0
- Only tested on Windows with x64

1. 克隆仓库
    ```cmd 
    git clone https://github.com/DragonheartLX/Drmix.git
    cd Drmix 
    ```
2. 使用 CMake 编译
    ```cmd
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

### Projects
- 已完成
    - None
- 未完成
    - RayTracing    光线追踪            参考[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
    - Breakout      打砖块              参考[Learn OpenGL (cn)](https://learnopengl-cn.github.io/06%20In%20Practice/2D-Game/01%20Breakout/)
    - UIDemo        简易的可交互ui示例


### 依赖
|       Dependence name     |    Version    |
|       :-------------      |    :------    |
| GLFW                      | 3.3.8         |
| glad                      | 4.6 core      |
| glm                       | 0.9.9.8       |
| stb_image.h               | 2.27x         |
| stb_image_write.h         | 1.16          |
| spdlog                    | 1.10.0        |