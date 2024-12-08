#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "utils/shaderloader.h"
#include "utils/sceneparser.h"

// ================== Project 5: Lights, Camera

FT_Library m_free_type;
Text* m_text;

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent),
      camera(Camera{ m_data.cameraData })
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here
    glDeleteBuffers(1, &m_vbo_sphere);
    glDeleteBuffers(1, &m_vbo_cube);
    glDeleteBuffers(1, &m_vbo_cone);
    glDeleteBuffers(1, &m_vbo_cylinder);
    glDeleteBuffers(1, &m_vao_sphere);
    glDeleteBuffers(1, &m_vao_cube);
    glDeleteBuffers(1, &m_vao_cone);
    glDeleteBuffers(1, &m_vao_cylinder);
    glDeleteProgram(m_shader);
    this->doneCurrent();

    glDeleteProgram(m_texture_shader);
    glDeleteVertexArrays(1, &m_fullscreen_vao);
    glDeleteBuffers(1, &m_fullscreen_vbo);
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    FT_Done_Face(m_text->face);
    FT_Done_FreeType(m_free_type);
    glDeleteProgram(m_shader);

    exit(EXIT_SUCCESS); // Function call: exit() is a C/C++ function that performs various tasks to help clean up resources.
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_defaultFBO = 2;
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initialize GL extension wrangler
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) fprintf(stderr, "Error while initializing GLEW: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Successfully initialized GLEW %s\n", glewGetString(GLEW_VERSION));

    glEnable(GL_CULL_FACE);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
    m_texture_shader = ShaderLoader::createShaderProgram(":/resources/shaders/texture.vert", ":/resources/shaders/texture.frag");

    sphere->updateParams(5, 5);
    cube->updateParams(5);
    cone->updateParams(5, 5);
    cylinder->updateParams(5, 5);

    setUpShapeData(m_vbo_sphere, m_vao_sphere, sphere->generateShape());
    setUpShapeData(m_vbo_cube, m_vao_cube, cube->generateShape());
    setUpShapeData(m_vbo_cone, m_vao_cone, cone->generateShape());
    setUpShapeData(m_vbo_cylinder, m_vao_cylinder, cylinder->generateShape());

    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glUseProgram(m_texture_shader);
    glUniform1i(glGetUniformLocation(m_texture_shader, "textureSampler"), 0);
    glUseProgram(0);

    std::vector<GLfloat> fullscreen_quad_data =
        {
            -1.f,  1.f, 0.f, 0.f, 1.f,
            -1.f, -1.f, 0.f, 0.f, 0.f,
            1.f, -1.f, 0.f, 1.f, 0.f,
            1.f,  1.f, 0.f, 1.f, 1.f,
            -1.f,  1.f, 0.f, 0.f, 1.f,
            1.f, -1.f, 0.f, 1.f, 0.f
        };

    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    makeFBO();

    initFinish = true;


}

void Realtime::paintGL() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_fbo_width, m_fbo_height);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // for (RenderShapeData shape : m_data.shapes) {
    //     switch (shape.primitive.type) {
    //     case PrimitiveType::PRIMITIVE_SPHERE:
    //         glBindVertexArray(m_vao_sphere);
    //         break;
    //     case PrimitiveType::PRIMITIVE_CUBE:
    //         glBindVertexArray(m_vao_cube);
    //         break;
    //     case PrimitiveType::PRIMITIVE_CONE:
    //         glBindVertexArray(m_vao_cone);
    //         break;
    //     case PrimitiveType::PRIMITIVE_CYLINDER:
    //         glBindVertexArray(m_vao_cylinder);
    //         break;
    //     case PrimitiveType::PRIMITIVE_MESH:
    //         break;
    //     }

    //     glm::vec4 cAmbient(shape.primitive.material.cAmbient[0], shape.primitive.material.cAmbient[1], shape.primitive.material.cAmbient[2], shape.primitive.material.cAmbient[3]);
    //     glm::vec4 cDiffuse(shape.primitive.material.cDiffuse[0], shape.primitive.material.cDiffuse[1], shape.primitive.material.cDiffuse[2], shape.primitive.material.cDiffuse[3]);
    //     glm::vec4 cSpecular(shape.primitive.material.cSpecular[0], shape.primitive.material.cSpecular[1], shape.primitive.material.cSpecular[2], shape.primitive.material.cSpecular[3]);

    //     glUseProgram(m_shader);
    //     glUniformMatrix4fv(glGetUniformLocation(m_shader, "model"), 1, GL_FALSE, &shape.ctm[0][0]);
    //     glUniformMatrix4fv(glGetUniformLocation(m_shader, "view"), 1, GL_FALSE, &camera.getViewMatrix()[0][0]);
    //     glUniformMatrix4fv(glGetUniformLocation(m_shader, "proj"), 1, GL_FALSE, &m_proj[0][0]);
    //     glUniform1f(glGetUniformLocation(m_shader, "ka"), m_data.globalData.ka);
    //     glUniform1f(glGetUniformLocation(m_shader, "kd"), m_data.globalData.kd);
    //     glUniform1f(glGetUniformLocation(m_shader, "ks"), m_data.globalData.ks);
    //     glUniform4fv(glGetUniformLocation(m_shader, "lightPos"), 8, &m_lightPos[0][0]);
    //     glUniform4fv(glGetUniformLocation(m_shader, "lightDir"), 8, &m_lightDir[0][0]);
    //     glUniform4fv(glGetUniformLocation(m_shader, "lightColors"), 8, &m_lightColors[0][0]);
    //     glUniform4fv(glGetUniformLocation(m_shader, "cAmbient"), 1, &cAmbient[0]);
    //     glUniform4fv(glGetUniformLocation(m_shader, "cDiffuse"), 1, &cDiffuse[0]);
    //     glUniform4fv(glGetUniformLocation(m_shader, "cSpecular"), 1, &cSpecular[0]);
    //     glUniform1iv(glGetUniformLocation(m_shader, "lightType"), 8, &m_lightType[0]);
    //     glUniform1fv(glGetUniformLocation(m_shader, "a"), 8, &m_a[0]);
    //     glUniform1fv(glGetUniformLocation(m_shader, "b"), 8, &m_b[0]);
    //     glUniform1fv(glGetUniformLocation(m_shader, "c"), 8, &m_c[0]);
    //     glUniform1fv(glGetUniformLocation(m_shader, "angle"), 8, &m_angle[0]);
    //     glUniform1fv(glGetUniformLocation(m_shader, "penumbra"), 8, &m_penumbra[0]);
    //     glUniform1i(glGetUniformLocation(m_shader, "numLights"), m_numLights);
    //     glUniform1f(glGetUniformLocation(m_shader, "shininess"), shape.primitive.material.shininess);
    //     glUniform4fv(glGetUniformLocation(m_shader, "cameraPos"), 1, &(glm::inverse(camera.getViewMatrix()) * glm::vec4(0.f, 0.f, 0.f, 1.f))[0]);

    //     switch (shape.primitive.type) {
    //     case PrimitiveType::PRIMITIVE_SPHERE:
    //         glDrawArrays(GL_TRIANGLES, 0, sphere->generateShape().size() / 6);
    //         break;
    //     case PrimitiveType::PRIMITIVE_CUBE:
    //         glDrawArrays(GL_TRIANGLES, 0, cube->generateShape().size() / 6);
    //         break;
    //     case PrimitiveType::PRIMITIVE_CONE:
    //         glDrawArrays(GL_TRIANGLES, 0, cone->generateShape().size() / 6);
    //         break;
    //     case PrimitiveType::PRIMITIVE_CYLINDER:
    //         glDrawArrays(GL_TRIANGLES, 0, cylinder->generateShape().size() / 6);
    //         break;
    //     case PrimitiveType::PRIMITIVE_MESH:
    //         break;
    //     }

        ///
        ///
        glUniform1i(glGetUniformLocation(m_shader, "alphabet_texture"), 31);

        // glm::vec3 RGB(10.0f, 120.0f, 105.0f);
        float RGB[3];
        RGB[0] = 10.f;
        RGB[1] = 120.f;
        RGB[2] = 105.f;
        unsigned int font_colour_loc = glGetUniformLocation(m_shader, "font_colour");
        glUniform3fv(font_colour_loc, 1, &RGB[0]);


        int num_replace = 3;
        size_t vec_size = m_text->messages[0].characters_quads.size();
        float start_pos = m_text->messages[0].start_x_current[vec_size - num_replace];

        m_text->draw_messages(0);
        m_text->draw_alphabets();
        glBindVertexArray(0);
    // }
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
    glViewport(0, 0, m_screen_width, m_screen_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintTexture(m_fbo_texture, settings.perPixelFilter, settings.kernelBasedFilter);
    glUseProgram(0);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;
    makeFBO();

    float c = -settings.nearPlane / settings.farPlane;
    float widthAngle = float(size().width()) / float(size().height()) * camera.getHeightAngle();
    m_proj = glm::mat4(1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, -2.f, 0.f,
                       0.f, 0.f, -1.f, 1.f) *
             glm::mat4(1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, 1.f / (1.f + c), -1,
                       0.f, 0.f, -c / (1.f + c), 0.f) *
             glm::mat4(1.f / (settings.farPlane * tan(widthAngle / 2.f)), 0.f, 0.f, 0.f,
                       0.f, 1.f / (settings.farPlane * tan(camera.getHeightAngle() / 2.f)), 0.f, 0.f,
                       0.f, 0.f, 1.f / settings.farPlane, 0.f,
                       0.f, 0.f, 0.f, 1.f);
}

void Realtime::setUpShapeData(GLuint& shape_vbo, GLuint& shape_vao, std::vector<float> shapeData) {
    // Generate and bind VBO
    // glGenBuffers(1, &shape_vbo);
    // glBindBuffer(GL_ARRAY_BUFFER, shape_vbo);

    // // Send data to VBO
    // glBufferData(GL_ARRAY_BUFFER, shapeData.size() * sizeof(GLfloat), shapeData.data(), GL_STATIC_DRAW);
    // // Generate, and bind vao
    // glGenVertexArrays(1, &shape_vao);
    // glBindVertexArray(shape_vao);

    // // Enable and define attribute 0 to store vertex positions
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    // // Clean-up bindings
    // glBindVertexArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER,0);

    FT_Error error_code = FT_Init_FreeType(&m_free_type);
    if (error_code)
    {
        std::cout << "\n   Error code: " << error_code << " --- " << "An error occurred during initialising the FT_Library";
        int keep_console_open;
        std::cin >> keep_console_open;
    }
    m_text = new Text(m_free_type, 300, 300, "01234567890Get Rady.Timr:owns&ClBgfb"); // Declare a new text object, passing in your chosen alphabet.
    m_text->create_text_message("Get Ready... Timer: 000", 150, 100, "/Users/Tiffany/Desktop/csci1230/the-cutie-creators/resources/typefaces/RubikMonoOne-Regular.ttf", 130, true); // True indicates that the message will be modified.

}

void Realtime::setUpScene() {
    m_data = RenderData();
    bool success = SceneParser::parse(settings.sceneFilePath, m_data);
    if (!success) {
        std::cerr << "Error loading scene" << std::endl;
    }

    RealtimeScene scene{ size().width(), size().height(), m_data };
    camera = scene.getCamera();

    m_numLights = m_data.lights.size();
    for (int i = 0; i < m_numLights; i++) {
        SceneLightData currLight = m_data.lights[i];
        m_lightPos[i] = currLight.pos;
        m_lightDir[i] = currLight.dir;
        glm::vec4 color(currLight.color.r, currLight.color.g, currLight.color.b, 1.f);
        m_lightColors[i] = color;
        if (currLight.type == LightType::LIGHT_DIRECTIONAL) {
            m_lightType[i] = 0;
        } else if (currLight.type == LightType::LIGHT_POINT) {
            m_lightType[i] = 1;
            m_a[i] = currLight.function[0];
            m_b[i] = currLight.function[1];
            m_c[i] = currLight.function[2];
        } else if (currLight.type == LightType::LIGHT_SPOT) {
            m_lightType[i] = 2;
            m_a[i] = currLight.function[0];
            m_b[i] = currLight.function[1];
            m_c[i] = currLight.function[2];
            m_angle[i] = currLight.angle;
            m_penumbra[i] = currLight.penumbra;
        }
    }

    float c = -settings.nearPlane / settings.farPlane;
    float widthAngle = float(size().width()) / float(size().height()) * camera.getHeightAngle();
    m_proj = glm::mat4(1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, -2.f, 0.f,
                       0.f, 0.f, -1.f, 1.f) *
             glm::mat4(1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, 1.f / (1.f + c), -1,
                       0.f, 0.f, -c / (1.f + c), 0.f) *
             glm::mat4(1.f / (settings.farPlane * tan(widthAngle / 2.f)), 0.f, 0.f, 0.f,
                       0.f, 1.f / (settings.farPlane * tan(camera.getHeightAngle() / 2.f)), 0.f, 0.f,
                       0.f, 0.f, 1.f / settings.farPlane, 0.f,
                       0.f, 0.f, 0.f, 1.f);
}

void Realtime::sceneChanged() {
    setUpScene();
    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {
    if (initFinish) {
        sphere->clearData();
        cube->clearData();
        cone->clearData();
        cylinder->clearData();

        sphere->updateParams(settings.shapeParameter1, settings.shapeParameter2);
        setUpShapeData(m_vbo_sphere, m_vao_sphere, sphere->generateShape());
        cube->updateParams(settings.shapeParameter1);
        setUpShapeData(m_vbo_cube, m_vao_cube, cube->generateShape());
        cone->updateParams(settings.shapeParameter1, settings.shapeParameter2);
        setUpShapeData(m_vbo_cone, m_vao_cone, cone->generateShape());
        cylinder->updateParams(settings.shapeParameter1, settings.shapeParameter2);
        setUpShapeData(m_vbo_cylinder, m_vao_cylinder, cylinder->generateShape());

        float c = -settings.nearPlane / settings.farPlane;
        float widthAngle = float(size().width()) / float(size().height()) * camera.getHeightAngle();
        m_proj = glm::mat4(1.f, 0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, -2.f, 0.f,
                           0.f, 0.f, -1.f, 1.f) *
                 glm::mat4(1.f, 0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, 1.f / (1.f + c), -1,
                           0.f, 0.f, -c / (1.f + c), 0.f) *
                 glm::mat4(1.f / (settings.farPlane * tan(widthAngle / 2.f)), 0.f, 0.f, 0.f,
                           0.f, 1.f / (settings.farPlane * tan(camera.getHeightAngle() / 2.f)), 0.f, 0.f,
                           0.f, 0.f, 1.f / settings.farPlane, 0.f,
                           0.f, 0.f, 0.f, 1.f);
    }
    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::makeFBO(){
    glGenTextures(1, &m_fbo_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &m_fbo_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
}

void Realtime::paintTexture(GLuint texture, bool togglePerPixelTexture, bool toggleKernelTexture){
    glUseProgram(m_texture_shader);
    glUniform1i(glGetUniformLocation(m_texture_shader, "togglePerPixelTexture"), togglePerPixelTexture);
    glUniform1i(glGetUniformLocation(m_texture_shader, "toggleKernelTexture"), toggleKernelTexture);
    glUniform1i(glGetUniformLocation(m_texture_shader, "fboWidth"), m_fbo_width);
    glUniform1i(glGetUniformLocation(m_texture_shader, "fboHeight"), m_fbo_height);

    glBindVertexArray(m_fullscreen_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

glm::mat4 rotate(const glm::mat4& mat, float angle, const glm::vec3& axis) {
    glm::vec3 normalizedAxis = glm::normalize(axis);
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    float x = normalizedAxis.x;
    float y = normalizedAxis.y;
    float z = normalizedAxis.z;

    glm::mat4 rotationMatrix = glm::mat4(
        cosAngle + x * x * (1 - cosAngle), x * y * (1 - cosAngle) - z * sinAngle, x * z * (1 - cosAngle) + y * sinAngle, 0.f,
        y * x * (1 - cosAngle) + z * sinAngle, cosAngle + y * y * (1 - cosAngle), y * z * (1 - cosAngle) - x * sinAngle, 0.f,
        z * x * (1 - cosAngle) - y * sinAngle, z * y * (1 - cosAngle) + x * sinAngle, cosAngle + z * z * (1 - cosAngle), 0.f,
        0.f, 0.f, 0.f, 1.f
        );

    return mat * rotationMatrix;
}

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        float xChange = deltaX * 0.2f;
        float yChange = deltaY * 0.2f;

        glm::vec3 look = glm::vec3(m_data.cameraData.look);
        glm::mat4 cameraMatrix = glm::mat4(1.0f);
        cameraMatrix = rotate(cameraMatrix, glm::radians(xChange), glm::vec3(0.0f, 1.0f, 0.0f));

        look = glm::vec3(cameraMatrix * glm::vec4(look, 0.0f));
        glm::vec3 up = glm::vec3(m_data.cameraData.up);
        up = glm::vec3(cameraMatrix * glm::vec4(up, 0.0f));

        glm::vec3 rightDirection = glm::normalize(glm::cross(look, up));
        cameraMatrix = rotate(glm::mat4(1.0f), glm::radians(yChange), rightDirection);
        look = glm::vec3(cameraMatrix * glm::vec4(look, 0.0f));
        up = glm::vec3(cameraMatrix * glm::vec4(up, 0.0f));

        m_data.cameraData.look = glm::vec4(glm::normalize(look), 0.0f);
        m_data.cameraData.up = glm::vec4(glm::normalize(up), 0.0f);

        RealtimeScene scene{ size().width(), size().height(), m_data };
        camera = scene.getCamera();

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around
    glm::vec3 move = glm::vec3(0.f);
    glm::vec3 look = glm::normalize(glm::vec3(m_data.cameraData.look));
    glm::vec3 up = glm::normalize(glm::vec3(m_data.cameraData.up));

    if (m_keyMap[Qt::Key_W]) {
        move += look;
    } if (m_keyMap[Qt::Key_S]) {
        move -= look;
    } if (m_keyMap[Qt::Key_A]) {
        move -= glm::normalize(glm::cross(look, up));
    } if (m_keyMap[Qt::Key_D]) {
        move += glm::normalize(glm::cross(look, up));
    } if (m_keyMap[Qt::Key_Space]) {
        move += glm::vec3(0.f, 1.f, 0.f);
    } if (m_keyMap[Qt::Key_Control]) {
        move -= glm::vec3(0.f, 1.f, 0.f);
    }

    if (glm::length(move) != 0.0) move = glm::normalize(move);
    m_data.cameraData.pos += glm::vec4(move * 5.f * deltaTime, 0.f);

    RealtimeScene scene{ size().width(), size().height(), m_data };
    camera = scene.getCamera();

    update(); // asks for a PaintGL() call to occur
}

// DO NOT EDIT
void Realtime::saveViewportImage(std::string filePath) {
    // Make sure we have the right context and everything has been drawn
    makeCurrent();

    int fixedWidth = 1024;
    int fixedHeight = 768;

    // Create Frame Buffer
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create a color attachment texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fixedWidth, fixedHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Optional: Create a depth buffer if your rendering uses depth testing
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fixedWidth, fixedHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    // Render to the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, fixedWidth, fixedHeight);

    // Clear and render your scene here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintGL();

    // Read pixels from framebuffer
    std::vector<unsigned char> pixels(fixedWidth * fixedHeight * 3);
    glReadPixels(0, 0, fixedWidth, fixedHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Unbind the framebuffer to return to default rendering to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Convert to QImage
    QImage image(pixels.data(), fixedWidth, fixedHeight, QImage::Format_RGB888);
    QImage flippedImage = image.mirrored(); // Flip the image vertically

    // Save to file using Qt
    QString qFilePath = QString::fromStdString(filePath);
    if (!flippedImage.save(qFilePath)) {
        std::cerr << "Failed to save image to " << filePath << std::endl;
    }

    // Clean up
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}
