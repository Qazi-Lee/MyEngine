// main.cpp
#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <map>
#include <string>
#include <sstream>

// 字符信息结构体：存储字形纹理和测量数据
struct Character {
    GLuint TextureID;    // 字形纹理
    glm::ivec2 Size;     // 字形大小（宽、高）
    glm::ivec2 Bearing;  // 从基准线到左/上边缘的偏移
    GLuint Advance;      // 下一个字形的水平偏移（单位：1/64像素）
};

// 使用动态加载方式缓存字符纹理（键使用 Unicode 编码）
std::map<FT_ULong, Character> Characters;

// VAO 和 VBO 用于绘制字符四边形
GLuint VAO, VBO;

// 顶点着色器（将屏幕坐标转换为 NDC）
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec4 vertex; // (x, y, s, t)
    out vec2 TexCoords;
    uniform mat4 projection;
    void main(){
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
    }
)glsl";

// 片段着色器：采样字符纹理并输出指定颜色（这里假设字符纹理存放在红色通道中）
const char* fragmentShaderSource = R"glsl(
    #version 330 core
    in vec2 TexCoords;
    out vec4 FragColor;
    uniform sampler2D text;
    uniform vec3 textColor;
    void main(){
        float alpha = texture(text, TexCoords).r;
        FragColor = vec4(textColor, alpha);
    }
)glsl";

// 编译和链接着色器程序
GLuint CreateShaderProgram(const char* vShaderSrc, const char* fShaderSrc) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderSrc, nullptr);
    glCompileShader(vertexShader);
    // 检查错误（省略详细检查代码）
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation error:\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderSrc, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation error:\n" << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking error:\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

// 使用 FreeType 动态加载一个字符，返回 true 成功
bool LoadCharacter(FT_Face face, FT_ULong charcode) {
    // 如果字符已经在缓存中，则直接返回
    if (Characters.find(charcode) != Characters.end())
        return true;

    // 加载字符字形（渲染为灰度位图）
    if (FT_Load_Char(face, charcode, FT_LOAD_RENDER)) {
        std::cerr << "Failed to load character: " << charcode << std::endl;
        return false;
    }

    // 生成纹理
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,  // 单通道纹理（只保存灰度值）
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 创建 Character 结构体保存信息
    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<GLuint>(face->glyph->advance.x)
    };
    Characters.insert(std::pair<FT_ULong, Character>(charcode, character));
    return true;
}

// RenderText 动态渲染文本，text 传入宽字符串（支持中文）
void RenderText(GLuint shader, const std::wstring& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    // 激活着色器
    glUseProgram(shader);
    glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // 对于每个字符，从宽字符串中获取 Unicode 编码
    for (wchar_t ch : text) {
        FT_ULong charcode = static_cast<FT_ULong>(ch);
        // 动态加载未缓存的字符
        if (Characters.find(charcode) == Characters.end()) {
            // 假设 FT_Face 保存在全局变量 face（下文会初始化）
            // 如果加载失败，则跳过该字符
            extern FT_Face g_Face;
            if (!LoadCharacter(g_Face, charcode))
                continue;
        }
        Character character = Characters[charcode];

        GLfloat xpos = x + character.Bearing.x * scale;
        GLfloat ypos = y - (character.Size.y - character.Bearing.y) * scale;
        GLfloat w = character.Size.x * scale;
        GLfloat h = character.Size.y * scale;

        // 设置 6 个顶点（两个三角形）构成一个矩形
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // 渲染当前字符
        glBindTexture(GL_TEXTURE_2D, character.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 更新 x 坐标：Advance 单位为 1/64 像素
        x += (character.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// 全局 FT_Face，用于动态加载字符时使用
FT_Face g_Face = nullptr;

int main() {
    // 初始化 GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW 初始化失败" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Chinese Text Rendering with FreeType", nullptr, nullptr);
    if (!window) {
        std::cerr << "创建 GLFW 窗口失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 加载 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD 加载失败" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);

    // 编译着色器程序
    GLuint shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    // 设置正交投影矩阵（屏幕坐标：左下角为 (0,0)）
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // 初始化 FreeType
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "FreeType 初始化失败" << std::endl;
        return -1;
    }
    // 加载支持中文的字体文件（例如 Noto Sans CJK 或 SimSun）
    if (FT_New_Face(ft, "Leefont蒙黑体.ttf", 0, &g_Face)) {
        std::cerr << "加载字体失败，请检查路径" << std::endl;
        return -1;
    }
    // 设置期望的像素大小
    FT_Set_Pixel_Sizes(g_Face, 0, 48);

    // 禁用字节对齐（以确保独立通道数据正确）
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // 配置 VAO/VBO 用于渲染字体
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 每个字符 6 个顶点，每个顶点 4 个浮点数
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 主循环
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 示例文本（包含中文字符）
        std::wstring text = L"Hello, 世界！动态加载中文字符";
        // 渲染文本，起始坐标 (25, 550)
        RenderText(shaderProgram, text, 25.0f, 550.0f, 1.0f, glm::vec3(0.8f, 0.8f, 0.2f));

        glfwSwapBuffers(window);
    }

    // 清理资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    FT_Done_Face(g_Face);
    FT_Done_FreeType(ft);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}