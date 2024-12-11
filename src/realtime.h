#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glut/glut.h>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>
#include <iostream>
#include "realtimescene.h"
#include "shapes/cube.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "text_fonts_glyphs.h"

class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void setUpScene();
    void setUpShapeData(GLuint& shape_vbo, GLuint& shape_vao, std::vector<float> shapeData);
    void settingsChanged();
    void saveViewportImage(std::string filePath);

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void generateCity();
    void makeFBO();
    void paintTexture(GLuint texture, bool togglePerPixelTexture, bool toggleKernelTexture);

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    double m_devicePixelRatio;

    bool initFinish = false;

    RenderData m_data;
    Camera m_camera;

    GLuint m_shader; // Stores id of shader program
    GLuint m_vbo_sphere;
    GLuint m_vbo_cube;
    GLuint m_vbo_cone;
    GLuint m_vbo_cylinder;
    GLuint m_vao_sphere;
    GLuint m_vao_cube;
    GLuint m_vao_cone;
    GLuint m_vao_cylinder;
    std::vector<std::vector<float>> m_shapesData;

    Cube* cube = new Cube();

    glm::mat4 m_mvp = glm::mat4(1);
    glm::mat4 m_text_model = glm::mat4(1);
    glm::mat4 m_view  = glm::mat4(1);
    glm::mat4 m_proj  = glm::mat4(1);

    glm::vec4 m_lightPos[4];
    glm::vec4 m_lightDir[4];
    glm::vec4 m_lightColors[4];
    int m_lightType[4];
    float m_a[4];
    float m_b[4];
    float m_c[4];
    float m_angle[4];
    float m_penumbra[4];
    int m_numLights;

    GLuint m_defaultFBO;
    int m_fbo_width;
    int m_fbo_height;
    int m_screen_width;
    int m_screen_height;
    GLuint m_texture_shader;
    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;
    GLuint m_fbo;
    GLuint m_fbo_texture;
    GLuint m_fbo_renderbuffer;

    FT_Library m_free_type;
    Text* m_text;
    Text::Message_Parent m_latest_message;
    int m_text_size = 120;

    void verifyVAO(std::vector<GLfloat> &triangleData, GLuint index, GLsizei size, GLsizei stride, const void* offset) {

        int newStride = int(stride / 4);
        int groupNum = 0;
        int newOffset = static_cast<int>(reinterpret_cast<intptr_t>(offset)) / 4;

        for (int i = newOffset; i < triangleData.size(); i = i + newStride) {
            std::cout << "Group " << groupNum << " of Values for VAO index " << index << std::endl;
            std::cout << "[";
            for (auto j = i; j < i + size; ++j) {
                if (j != i + size - 1) {
                    std::cout << triangleData[j]<< ", ";
                } else {
                    std::cout << triangleData[j]<< "]" << std::endl;
                }
            }
            groupNum = groupNum + 1;
        }
        std::cout << "" << std::endl;
    }
};
