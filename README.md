# Drmix
DragonheartLX的练习集

### 编译
- CMake Version >= 3.19
- Visual Studio 2022 (recommend)
- MinGW x64 13.2
- Only tested on Windows with x64
- Vcpkg

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

    ### 依赖会由vcpkg自动安装

### Projects
- 已完成
    - None
- 未完成
    - RayTracing    光线追踪            参考[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
    - Breakout      打砖块              参考[Learn OpenGL (cn)](https://learnopengl-cn.github.io/06%20In%20Practice/2D-Game/01%20Breakout/)
    - UIDemo        简易的可交互ui示例


### 依赖
|       Dependence name     |    Version(vcpkg version)     |
|       :-------------      |              :------          |
| glfw3                     | 3.3.8#2                       |
| glad                      | 0.1.36                        |
| glm                       | 2023-06-08                    |
| stb                       | 2023-04-11#1                  |
| spdlog                    | 1.12.0                        |
| imgui                     | 1.89.9                        |