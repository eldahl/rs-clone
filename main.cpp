#include <cassert>

#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "vertex_data.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera vectors
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 16.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Mouse
bool firstMouse = false;
bool leftMousePressed = false;
float mouseX = 0.0f;
float mouseY = 0.0f;
float lastX = 400;
float lastY = 300;
glm::vec2 mouseNDC = glm::vec2(0.0f, 0.0f);

// Camera euler angles
float pitch = 0.0f;
float yaw = -90.0f;

// Zoom
float fov = 45.0f;

// Delta tiem
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// FPS
float frameTime = 0.0f;
int frameCount = 0;
float fps = 0.0f;

// Champ coords
float champX = 4.5f;
float champY = 4.5f;

bool doMoveTowards = false;
float targetX = 0.0f;
float targetY = 0.0f;

// Transformation matrices
glm::mat4 projection;
glm::mat4 view = glm::mat4(1.0f);


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
bool rayPlaneIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, glm::vec3& intersectionPoint, float planeZ = 0.5f);
glm::vec2 mouseToNDC(float xpos, float ypos);

int main()
{
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RS Clone", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Mouse disable and movement callback
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // build and compile our shader program
    // ------------------------------------
    Shader groundShader("../res/shaders/ground.vs", "../res/shaders/ground.fs"); 
    Shader champShader("../res/shaders/vertex.vs", "../res/shaders/fragment.fs"); 

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    float champVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f
    };

    float groundVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f
    };

    VertexData groundVD = VertexData();
    groundVD.bind(groundVertices, sizeof(groundVertices));
    groundVD.setVertexAttrib(0, 3, 5, 0);
    groundVD.setVertexAttrib(1, 2, 5, 3);
    
    VertexData champVD = VertexData();
    champVD.bind(champVertices, sizeof(champVertices));
    champVD.setVertexAttrib(0, 3, 5, 0);
    champVD.setVertexAttrib(1, 2, 5, 3);

    // Flip images loaded from stb_image
    stbi_set_flip_vertically_on_load(true);

    Texture champ = Texture("../res/textures/champ.png", true);
    Texture ground = Texture("../res/textures/ground.jpg", false);
    
    ground.genAndBindAndLoad();
    champ.genAndBindAndLoad();

    // Use shader
    groundShader.use();
    // Set uniform for shaders
    groundShader.setInt("groundTex", 0);

    champShader.use();
    champShader.setInt("champTex", 1);

    // render the triangle
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ground.ID);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, champ.ID);

    // Arrange stuff before rendering
    /*
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;
    */

    // Tranformations
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));    
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    
    projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // input
        // -----
        processInput(window);

        groundShader.use();
        // Assign uniform to shader with transformation matricies
        groundShader.setMat4("view", view);
        groundShader.setMat4("projection", projection);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(groundVD.vaoID);
        for(int x = -5; x < 5; x++)
        {
            for(int y = -5; y < 5; y++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)x + 0.5f, (float)y + 0.5f, 0.0f));
                groundShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        champShader.use();
        // Assign uniform to shader with transformation matricies
        champShader.setMat4("view", view);
        champShader.setMat4("projection", projection);

        glBindVertexArray(champVD.vaoID);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(champX, champY, 1.0f));
        model = glm::scale(model, glm::vec3(1.08f, 1.85f, 0.0f));
        champShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        frameTime += deltaTime;
        if (frameTime <= 1) {
            frameCount++;
        }
        else {
            fps = frameCount;
            frameCount = 0;
            frameTime = 0.0f;
            std::cout << "FPS: " << fps << std::endl;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    glDeleteVertexArrays(1, &champVD.vaoID);
    glDeleteBuffers(1, &champVD.vboID);
    glDeleteVertexArrays(1, &groundVD.vaoID);
    glDeleteBuffers(1, &groundVD.vboID);
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

bool rayPlaneIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, glm::vec3& intersectionPoint, float planeZ) {
    if (rayDirection.z == 0) return false;  // Avoid division by zero

    float t = (planeZ - rayOrigin.z) / rayDirection.z;
    if (t < 0.0f) return false;  // Intersection is behind the camera

    intersectionPoint = rayOrigin + t * rayDirection;  // Compute intersection point
    return true;
}

glm::vec2 mouseToNDC(float xpos, float ypos) {
    float x = (2.0f * xpos) / SCR_WIDTH - 1.0f;
    float y = 1.0f - (2.0f * ypos) / SCR_HEIGHT;
    return glm::vec2(x, y);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraUp;
        champY += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraUp;
        champY -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        champX -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        champX += cameraSpeed;
    }

    if (!leftMousePressed && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {       
        // Mouse to NDC
        mouseNDC = mouseToNDC(mouseX, mouseY);
        std::cout << "mouse pos: " << mouseNDC.x << "-" << mouseNDC.y << std::endl;

        // Reverse view projection
        glm::mat4 viewProjection = projection * view;
        glm::mat4 inverseViewProjection = glm::inverse(viewProjection);

        // Unproject near and far points in NDC
        glm::vec4 nearPointNDC(mouseNDC.x, mouseNDC.y, -1.0f, 1.0f);  // z = -1 for near plane
        glm::vec4 farPointNDC(mouseNDC.x, mouseNDC.y, 1.0f, 1.0f);    // z = 1 for far plane

        glm::vec4 nearPointWorld = inverseViewProjection * nearPointNDC;
        glm::vec4 farPointWorld = inverseViewProjection * farPointNDC; 

        // Normalize the points (divide by w)
        nearPointWorld /= nearPointWorld.w;
        farPointWorld /= farPointWorld.w;

        // Create the ray from near to far point
        glm::vec3 rayOrigin = glm::vec3(nearPointWorld);
        glm::vec3 rayDirection = glm::normalize(glm::vec3(farPointWorld - nearPointWorld));
        glm::vec3 intersectionPoint;
  
        if (rayPlaneIntersection(rayOrigin, rayDirection, intersectionPoint)) {
            std::cout << "Intersection at: " << intersectionPoint.x << ", " << intersectionPoint.y << ", " << intersectionPoint.z << std::endl;

            // Move your object to this intersection point or do other operations
            champX = intersectionPoint.x;
            champY = intersectionPoint.y;
            //moveObjectTo(intersectionPoint);
        }

        leftMousePressed = true;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        leftMousePressed = false;
    }

    // Automated movement
    if(doMoveTowards) {
        
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    
    // Set global mouse location
    mouseX = xpos;
    mouseY = ypos;

    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch -= yoffset;

    if(pitch > 89.0f)
      pitch =  89.0f;
    if(pitch < -89.0f)
      pitch = -89.0f;
    
    // Calculate camera direction
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    //cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f; 
}
