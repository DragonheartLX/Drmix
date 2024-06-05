# Drmix
DragonheartLX的练习集

### 编译
- CMake Version >= 3.19
- Visual Studio 2022 (clang-cl support)
- Ninja
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
    cmake .. -GNinja
    cmake --build .
    ```

    ### 依赖会由vcpkg自动安装

### 依赖
|       Dependence name     |    Version(vcpkg version)     |
|       :-------------      |              :------          |
| glfw3                     | 3.3.8#2                       |
| glm                       | 2023-06-08                    |
| stb                       | 2023-04-11#1                  |
| spdlog                    | 1.12.0                        |
| imgui                     | 1.89.9                        |
| vulkan                    | 1.1.82.1#6                    |
| fmt                       | 10.1.1                        |