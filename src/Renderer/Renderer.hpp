//
// Created by 이재현 on 2021/05/29.
//

#ifndef LEEFRAMEWORK_RENDERER_HPP
#define LEEFRAMEWORK_RENDERER_HPP

#include "Camera.hpp"
#include "Light.hpp"
#include "Shader.hpp"
#include "GUIwrapper.hpp"
#include <GL/glew.h>

#include <GLFW/glfw3.h>

class Renderer {
//TODO: all the responsible class for rendering. the most big class
//TODO: made by java builder pattern
public:


    class Builder {

    public:
        friend class Renderer;

        Builder();//essential?
        Builder &camera(Camera camera); // TODO: need camera explicitly?

        Builder &light(Light light);

        Builder &gui();

        Builder &init();

        Renderer *build();


        //TODO: essential member
    private:
        Camera *m_builder_camera;
        Light *m_builder_light;
        GUIwrapper *m_builder_gui;
        GLFWwindow *m_builder_window;



    };




    Camera *getCamera();
    Light  *getLight();

    void addEntity();

    void render();


private:

    explicit Renderer(const Builder& builder);
    Camera *m_camera_; //TODO: camera can be many?
    Shader *m_shader;
    GLFWwindow* m_window;
    glm::vec3 m_default_color;


};


#endif //LEEFRAMEWORK_RENDERER_HPP
