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

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent),
    m_camera(Camera{ m_data.cameraData })
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
    free(m_text);
    glDeleteProgram(m_shader);

    exit(EXIT_SUCCESS); // Function call: exit() is a C/C++ function that performs various tasks to help clean up resources.
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

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_defaultFBO = 2;
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    srand(time(0));

    // Initialize GL extension wrangler
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) fprintf(stderr, "Error while initializing GLEW: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Successfully initialized GLEW %s\n", glewGetString(GLEW_VERSION));

    glDisable(GL_CULL_FACE);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); // Anti-aliasing
    glEnable(GL_BLEND); // GL_BLEND for OpenGL transparency which is further set within the fragment shader.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    m_model = rotate(m_model, M_PI / -2.f, glm::vec3(1.0f, 0.f, 0.0f));

    FT_Error error_code = FT_Init_FreeType(&m_free_type);
    if (error_code)
    {
        std::cout << "\n   Error code: " << error_code << " --- " << "An error occurred during initialising the FT_Library";
        int keep_console_open;
        std::cin >> keep_console_open;
    }

    // qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890,.;:'\"?![]-_+={}|/
    m_text = new Text(m_free_type, m_fbo_width, m_fbo_height, settings.text); // Declare a new text object, passing in your chosen alphabet.
    std::string typefaceFilepath = settings.typeface;
    typefaceFilepath.erase(remove_if(typefaceFilepath.begin(), typefaceFilepath.end(), isspace), typefaceFilepath.end());
    m_text->create_text_message(settings.text, 0, 0, "resources/typefaces/" + typefaceFilepath + ".ttf", 130, false);

    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glUseProgram(m_texture_shader);
    glUniform1i(glGetUniformLocation(m_texture_shader, "textureSampler"), 0);
    glUseProgram(0);

    std::vector<GLfloat> fullscreen_quad_data =
        {
            -1.f,  1.f, 0.f,
            0.f, 1.f,
            -1.f, -1.f, 0.f,
            0.f, 0.f,
            1.f, -1.f, 0.f,
            1.f, 0.f,
            1.f,  1.f, 0.f,
            1.f, 1.f,
            -1.f,  1.f, 0.f,
            0.f, 1.f,
            1.f, -1.f, 0.f,
            1.f, 0.f
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

    setUpScene();
}




void Realtime::paintGL() {
    // glm::vec2 dims = m_text->calculate_message_image_size(m_text->messages[m_text->messages.size() - 1]);
    // glViewport(0, 0, dims[0], dims[1]);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    // glm::vec4 topLeft = m_text->messages[m_text->messages.size() - 1].characters_quads[0].top_left_tr1;
    // glm::vec4 topRight = m_text->messages[m_text->messages.size() - 1].characters_quads[m_text->messages[m_text->messages.size() - 1].characters_quads.size() - 1].top_right_tr2;
    // glm::vec4 bottomLeft = m_text->messages[m_text->messages.size() - 1].characters_quads[0].bottom_left_tr1;

    // int width = topRight.x - topLeft.x;
    // int height = bottomLeft.y - topLeft.y;


    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "isText"), true);
    glUniform1i(glGetUniformLocation(m_shader, "alphabet_texture"), 31);
    m_text->draw_messages(m_text->messages.size() - 1);

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
    glViewport(0, 0, m_fbo_width, m_fbo_height);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintTexture(m_fbo_texture, settings.perPixelFilter, settings.kernelBasedFilter);
    glUseProgram(0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // if commented out, will display text bc we painttexture above. if not commented out, then clear screen before drawing city

    for (RenderShapeData shape : m_data.shapes) {
        glBindVertexArray(m_vao_cube);

        // switch (shape.primitive.type) {
        // case PrimitiveType::PRIMITIVE_SPHERE:
        //     glBindVertexArray(m_vao_sphere);
        //     break;
        // case PrimitiveType::PRIMITIVE_CUBE:
        //     glBindVertexArray(m_vao_cube);
        //     break;
        // case PrimitiveType::PRIMITIVE_CONE:
        //     glBindVertexArray(m_vao_cone);
        //     break;
        // case PrimitiveType::PRIMITIVE_CYLINDER:
        //     glBindVertexArray(m_vao_cylinder);
        //     break;
        // case PrimitiveType::PRIMITIVE_MESH:
        //     break;
        // }

        // glm::vec4 cAmbient(shape.primitive.material.cAmbient[0], shape.primitive.material.cAmbient[1], shape.primitive.material.cAmbient[2], shape.primitive.material.cAmbient[3]);
        // glm::vec4 cDiffuse(shape.primitive.material.cDiffuse[0], shape.primitive.material.cDiffuse[1], shape.primitive.material.cDiffuse[2], shape.primitive.material.cDiffuse[3]);
        // glm::vec4 cSpecular(shape.primitive.material.cSpecular[0], shape.primitive.material.cSpecular[1], shape.primitive.material.cSpecular[2], shape.primitive.material.cSpecular[3]);


        glActiveTexture(GL_TEXTURE31);
        // glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

        glm::vec4 cAmbient(1, 1, 1, 1);
        glm::vec4 cDiffuse(1, 1, 1, 1);
        glm::vec4 cSpecular(1, 1, 1, 1);

        glUseProgram(m_shader);
        glUniform1i(glGetUniformLocation(m_shader, "alphabet_texture"), 31);
        glUniform1i(glGetUniformLocation(m_shader, "alphabet_texture_width"), m_text->messages[m_text->messages.size() - 1].alphabet_texture_width);
        glUniform1i(glGetUniformLocation(m_shader, "alphabet_texture_height"), m_text->messages[m_text->messages.size() - 1].alphabet_texture_height);

        glUniformMatrix4fv(glGetUniformLocation(m_shader, "text_model"), 1, GL_FALSE, &m_model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "model"), 1, GL_FALSE, &shape.ctm[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "view"), 1, GL_FALSE, &m_camera.getViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "proj"), 1, GL_FALSE, &m_proj[0][0]);
        glUniform1f(glGetUniformLocation(m_shader, "ka"), m_data.globalData.ka);
        glUniform1f(glGetUniformLocation(m_shader, "kd"), m_data.globalData.kd);
        glUniform1f(glGetUniformLocation(m_shader, "ks"), m_data.globalData.ks);
        glUniform4fv(glGetUniformLocation(m_shader, "lightPos"), 8, &m_lightPos[0][0]);
        glUniform4fv(glGetUniformLocation(m_shader, "lightDir"), 8, &m_lightDir[0][0]);
        glUniform4fv(glGetUniformLocation(m_shader, "lightColors"), 8, &m_lightColors[0][0]);
        glUniform4fv(glGetUniformLocation(m_shader, "cAmbient"), 1, &cAmbient[0]);
        glUniform4fv(glGetUniformLocation(m_shader, "cDiffuse"), 1, &cDiffuse[0]);
        glUniform4fv(glGetUniformLocation(m_shader, "cSpecular"), 1, &cSpecular[0]);
        glUniform1iv(glGetUniformLocation(m_shader, "lightType"), 8, &m_lightType[0]);
        glUniform1fv(glGetUniformLocation(m_shader, "a"), 8, &m_a[0]);
        glUniform1fv(glGetUniformLocation(m_shader, "b"), 8, &m_b[0]);
        glUniform1fv(glGetUniformLocation(m_shader, "c"), 8, &m_c[0]);
        glUniform1fv(glGetUniformLocation(m_shader, "angle"), 8, &m_angle[0]);
        glUniform1fv(glGetUniformLocation(m_shader, "penumbra"), 8, &m_penumbra[0]);
        glUniform1i(glGetUniformLocation(m_shader, "numLights"), m_numLights);
        // glUniform1f(glGetUniformLocation(m_shader, "shininess"), shape.primitive.material.shininess);
        glUniform1f(glGetUniformLocation(m_shader, "shininess"), 20);
        glUniform1i(glGetUniformLocation(m_shader, "isText"), false);
        glUniform4fv(glGetUniformLocation(m_shader, "cameraPos"), 1, &(glm::inverse(m_camera.getViewMatrix()) * glm::vec4(0.f, 0.f, 0.f, 1.f))[0]);

        // switch (shape.primitive.type) {
        // case PrimitiveType::PRIMITIVE_SPHERE:
        //     glDrawArrays(GL_TRIANGLES, 0, sphere->generateShape().size() / 6);
        //     break;
        // case PrimitiveType::PRIMITIVE_CUBE:
        //     glDrawArrays(GL_TRIANGLES, 0, cube->generateShape().size() / 6);
        //     break;
        // case PrimitiveType::PRIMITIVE_CONE:
        //     glDrawArrays(GL_TRIANGLES, 0, cone->generateShape().size() / 6);
        //     break;
        // case PrimitiveType::PRIMITIVE_CYLINDER:
        //     glDrawArrays(GL_TRIANGLES, 0, cylinder->generateShape().size() / 6);
        //     break;
        // case PrimitiveType::PRIMITIVE_MESH:
        //     break;
        // }

        glDrawArrays(GL_TRIANGLES, 0, cube->generateShape().size() / 6);

        glBindVertexArray(0);
    }
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
    float widthAngle = float(size().width()) / float(size().height()) * m_camera.getHeightAngle();
    m_proj = glm::mat4(1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, -2.f, 0.f,
                       0.f, 0.f, -1.f, 1.f) *
             glm::mat4(1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, 1.f / (1.f + c), -1,
                       0.f, 0.f, -c / (1.f + c), 0.f) *
             glm::mat4(1.f / (settings.farPlane * tan(widthAngle / 2.f)), 0.f, 0.f, 0.f,
                       0.f, 1.f / (settings.farPlane * tan(m_camera.getHeightAngle() / 2.f)), 0.f, 0.f,
                       0.f, 0.f, 1.f / settings.farPlane, 0.f,
                       0.f, 0.f, 0.f, 1.f);
}

void Realtime::setUpShapeData(GLuint& shape_vbo, GLuint& shape_vao, std::vector<float> shapeData) {
    // Generate and bind VBO
    glGenBuffers(1, &shape_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, shape_vbo);

    // Send data to VBO
    glBufferData(GL_ARRAY_BUFFER, shapeData.size() * sizeof(GLfloat), shapeData.data(), GL_STATIC_DRAW);
    // Generate, and bind vao
    glGenVertexArrays(1, &shape_vao);
    glBindVertexArray(shape_vao);

    // Enable and define attribute 0 to store vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), reinterpret_cast<void*>(4 * sizeof(GLfloat)));

    // Clean-up bindings
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Realtime::setUpScene() {
    m_data = RenderData();

    m_data.cameraData.pos = glm::vec4(0.f, 0.f, 20.f, 1.f);
    m_data.cameraData.look = glm::vec4(0.f, 0.f, -20.f, 0.f);
    m_data.cameraData.up = glm::vec4(0, 1, 0, 0);
    m_data.cameraData.heightAngle = 30 * M_PI / 180.f;;
    m_data.globalData.ka = 0.5;
    m_data.globalData.kd = 0.5;
    m_data.globalData.ks = 0.5;
    m_data.globalData.kt = 0.5;


    glm::vec4 dir = glm::vec4(-3.f, -2.f, -1.f, 0.f);
    SceneLightData lightData;
    lightData.id = 1;
    lightData.type = LightType::LIGHT_DIRECTIONAL;
    lightData.color = glm::vec4(1, 1, 1, 1);
    lightData.dir = dir;
    m_data.lights.push_back(lightData);

    generateCity();

    RealtimeScene scene{ size().width(), size().height(), m_data };
    m_camera = scene.getCamera();

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
    float widthAngle = float(size().width()) / float(size().height()) * m_camera.getHeightAngle();
    m_proj = glm::mat4(1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, -2.f, 0.f,
                       0.f, 0.f, -1.f, 1.f) *
             glm::mat4(1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, 1.f / (1.f + c), -1,
                       0.f, 0.f, -c / (1.f + c), 0.f) *
             glm::mat4(1.f / (settings.farPlane * tan(widthAngle / 2.f)), 0.f, 0.f, 0.f,
                       0.f, 1.f / (settings.farPlane * tan(m_camera.getHeightAngle() / 2.f)), 0.f, 0.f,
                       0.f, 0.f, 1.f / settings.farPlane, 0.f,
                       0.f, 0.f, 0.f, 1.f);
}

void Realtime::generateCity() {
    m_data.shapes.clear();

    int gridSizeX = m_text->messages[m_text->messages.size() - 1].alphabet_texture_width / 10.f;
    int gridSizeZ = m_text->messages[m_text->messages.size() - 1].alphabet_texture_height / 10.f;
    float gridUpperBoundaryX = gridSizeX / 10.f;
    float gridUpperBoundaryZ = gridSizeZ / 10.f;
    int streetDensityX = gridSizeX / (gridSizeX * (settings.streetDensityX / 100.f));
    int streetDensityZ = gridSizeZ / (gridSizeZ * (settings.streetDensityZ / 100.f));

    for (int i = 0; i < gridSizeX; i++) {
        for (int j = 0; j < gridSizeZ; j++) {
            if (i % streetDensityX != 0 && j % streetDensityZ != 0) {
                ScenePrimitive primitive;
                primitive.type = PrimitiveType::PRIMITIVE_CUBE;
                float r1 = (arc4random_uniform(settings.buildingHeight) + 0.1f) / 10.f;
                float r2 = (arc4random_uniform(settings.buildingHeight) + 0.1f) / 10.f;
                float r3 = (arc4random_uniform(settings.buildingHeight) + 0.1f) / 10.f;

                glm::mat4 ctm = glm::translate(glm::mat4(1.0f), glm::vec3(i / 10.f, r2 / 4.f, j / 10.f));
                ctm *= glm::translate(glm::mat4(1.0f), glm::vec3(gridSizeX / -20.f, 0, gridSizeZ / -20.f));
                float dim1 = r3 / settings.buildingHeight;
                float dim3 = r1 / settings.buildingHeight;
                ctm *= glm::scale(glm::mat4(1.0f), glm::vec3(dim1 + ((0.1 - dim1) / 2), r2 / 2.f, dim3 + ((0.1 - dim3) / 2)));

                RenderShapeData shape;
                shape.ctm = ctm;
                shape.primitive = primitive;
                m_data.shapes.push_back(shape);

                for (int k = 0; k < settings.buildingIrregularity; k++) {
                    ScenePrimitive primitive2;
                    primitive2.type = PrimitiveType::PRIMITIVE_CUBE;
                    r1 = (arc4random_uniform(2) + 1.f) / 10.f;
                    r2 = (arc4random_uniform(3) + 1.f) / 10.f;
                    r3 = (arc4random_uniform(4) + 1.f) / 10.f;

                    dim1 = i / 10.f + r1;
                    dim3 = j / 10.f + r3;

                    if ((int) (dim1 * 10.f) % streetDensityX != 0 && (int) (dim3 * 10.f) % streetDensityZ != 0 && dim1 < gridUpperBoundaryX && dim3 < gridUpperBoundaryZ) {
                        glm::mat4 ctm2 = glm::translate(glm::mat4(1.0f), glm::vec3(dim1, r2 / 6.f, dim3));
                        ctm2 *= glm::translate(glm::mat4(1.0f), glm::vec3(gridSizeX / -20.f, 0, gridSizeZ / -20.f));
                        ctm2 *= glm::scale(glm::mat4(1.0f), glm::vec3(r1 / 3.f, r2 / 3.f, r3 / 3.f));
                        RenderShapeData shape2;
                        shape2.ctm = ctm2;
                        shape2.primitive = primitive2;
                        m_data.shapes.push_back(shape2);
                    }
                }
            }

        }
    }
}

void Realtime::settingsChanged() {
    if (initFinish) {
        free(m_text);
        m_text = new Text(m_free_type, m_fbo_width, m_fbo_height, settings.text); // Declare a new text object, passing in your chosen alphabet.
        std::string typefaceFilepath = settings.typeface;
        typefaceFilepath.erase(remove_if(typefaceFilepath.begin(), typefaceFilepath.end(), isspace), typefaceFilepath.end());
        m_text->create_text_message(settings.text, 0, 0, "resources/typefaces/" + typefaceFilepath + ".ttf", 130, false);

        glm::vec2 dims = m_text->calculate_message_image_size(m_text->messages[m_text->messages.size() - 1]);
        glViewport(0, 0, dims[0], dims[1]);
        makeFBO();
        generateCity();
    }
    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::makeFBO(){
    glGenTextures(1, &m_fbo_texture);
    glActiveTexture(GL_TEXTURE0);
    glm::vec2 dims = m_text->calculate_message_image_size(m_text->messages[m_text->messages.size() - 1]);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dims[0], dims[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &m_fbo_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dims[0], dims[1]);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
}

void Realtime::paintTexture(GLuint texture, bool togglePerPixelTexture, bool toggleKernelTexture){
    glUseProgram(m_texture_shader);

    glBindVertexArray(m_fullscreen_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
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

        float xChange = deltaX * 0.01f;
        float yChange = deltaY * -0.01f;

        m_model = rotate(m_model, xChange, glm::vec3(0.0f, 0.f, 1.0f));
        m_model = rotate(m_model, yChange, glm::vec3(1.0f, 0.0f, 0.0f));

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
    m_camera = scene.getCamera();

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

