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

    #### 依赖会由vcpkg自动安装

### 依赖
|       Dependence name     |    Version(vcpkg version)     |
|       :-------------      |              :------          |
| glfw3                     | 3.4                           |
| glm                       | 1.0.1#2                       |
| stb                       | 2023-04-11#1                  |
| spdlog                    | 1.12.0                        |
| imgui                     | 1.90.7                        |
| vulkan                    | 2023-12-17                    |
| fmt                       | 10.2.1#2                      |
| tomlplusplus              | 3.4.0                         |
| glslang                   | 14.2.0                        |