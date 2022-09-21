#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);  // 注册窗口回调函数
void processInput(GLFWwindow *window);  // 输入函数
std::string Shader::dirName;

int main(int argc, char *argv[])
{
    Shader::dirName = argv[1];
    glfwInit();
    // 设置版本和核心模式
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口对象
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL-syc", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 注册监听窗口大小变化的回调函数
    // 这里会自动调用一次glViewport，所以不用再自己写一遍
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    // 初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 定义Shader类，读取Shader.glsl源文件，生成着色器
    Shader ourShader("./shader/vertex.glsl", "./shader/fragment.glsl");
    
    // 定义三角形顶点坐标和颜色
    float vertices[] = {
        // ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
    // 索引数据
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // 创建VBO和VAO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); //  创建VAO
    glGenBuffers(1, &VBO);  // 创建VBO
    glGenBuffers(1, &EBO);
    // 绑定VAO
    glBindVertexArray(VAO);
    // 复制顶点数组到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 填充缓冲内存
    // 填充EBO内存
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 创建第一个纹理
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 设置绑定的纹理对象的环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载纹理并生成
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // 翻转图片y轴，因为OpenGL的y轴0.0在底部，而图片的y轴0.0通常在顶部
    unsigned char *data = stbi_load("./static/texture/mine/hutao3.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);    // 自动生成mipmap
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);  // 记得释放图像内存

    // 创建第二个纹理
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("./static/texture/mine/lycoris.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // 设置uniform采样器所属纹理单元
    ourShader.use();    // 需先激活着色器
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // // 创建变换矩阵
    // // 注意矩阵乘法的顺序，执行时实际上是先缩放再旋转
    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

    // // 将变换矩阵传递给着色器
    // ourShader.use();
    // unsigned int transLocation = glGetUniformLocation(ourShader.ID, "trans");
    // glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(trans));

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        // 渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 状态设置函数
        glClear(GL_COLOR_BUFFER_BIT);   // 状态使用函数

        // 绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // // 创建变换矩阵，并实时更新达到动态效果
        // // 注意矩阵乘法的顺序，一般来说我们希望的矩阵组合是先缩放再旋转最后平移
        // glm::mat4 trans = glm::mat4(1.0f);
        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        // // 将变换矩阵传递给着色器
        // unsigned int transLocation = glGetUniformLocation(ourShader.ID, "trans");
        // glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(trans));

        // 渲染三角形
        glBindVertexArray(VAO); // 绑定顶点数组
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // // 创建第二个三角
        // trans = glm::mat4(1.0f);
        // trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        // float scaleValue = abs(sin(0.5f * float(glfwGetTime())));
        // trans = glm::scale(trans, glm::vec3(scaleValue, scaleValue, scaleValue));
        // // 将变换矩阵传递给着色器
        // transLocation = glGetUniformLocation(ourShader.ID, "trans");
        // glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(trans));
        // // 渲染三角形，这里不再需要绑定VAO
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);    // 交换颜色缓冲
        glfwPollEvents();   // 检查事件触发
    }

    // 删除缓冲对象
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}