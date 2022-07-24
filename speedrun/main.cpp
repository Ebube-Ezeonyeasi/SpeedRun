//#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "includes.h"
#include "GLFW/glfw3.h"
#include <chrono>
#include <windows.h>

#include "Shader.h"
#include "Texture.h"

#include "notmine/glm/glm.hpp"
#include "notmine/glm/glm.hpp"
#include "notmine/glm/gtc/matrix_transform.hpp"
#include "notmine/glm/gtc/type_ptr.hpp"


glm::vec3 forwardVector = glm::vec3(.0f, .0f, -1.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f) * glm::vec3(-10.0f) ;

std::vector<float> circleCoords;
std::vector<unsigned int> indexBufferArray;


float fovy = 45.0f;
double lastX;
double lastY;

bool firstMouse = true;
bool showCursor = true;
bool activateMovement = true;
bool clickTimer = true;
bool afterESC = true;

unsigned int textureIndex = 0;

double pitch;
double yaw;

std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

float deltaTime = 0.0f;
float baseSpeed = 5.0f;


void ProcessInput(GLFWwindow* window);
inline void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void wait(int seconds);
const void* GenSphereWithNormals(long double Radius, long double Zcoord, int numberOfSectors);


int main(void) 
{

    float lastFrame = 0.0f;
    float currentFrame = 0.0f;

    if (!glfwInit())
    {
        std::cout << "Failed to initialize glfw \n ";
        return 0;
    }

    GLFWwindow* window = glfwCreateWindow(960, 540, "I can't believe you just did that step bro...", NULL, NULL);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSwapInterval(1);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if (!window) 
    {
        std::cout << "Failed to create window, sos. \n";
        glfwTerminate();

        return -1;
    }


    glfwMakeContextCurrent(window);



    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) 
    {
        std::cout << "Failed to initialize glew...sos \n ";

        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    float positions[216] = 
    
    {

    -0.5f, -0.5f, -0.5f,           0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,           0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,           0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,           0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,           0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,           0.0f,  0.0f, -1.0f,
                                   
    -0.5f, -0.5f,  0.5f,           0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,           0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,           0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,           0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,           0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,           0.0f,  0.0f, 1.0f,
                                   
    -0.5f,  0.5f,  0.5f,          -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,          -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,          -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,          -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,          -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,          -1.0f,  0.0f,  0.0f,
                                   
     0.5f,  0.5f,  0.5f,           1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,           1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,           1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,           1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,           1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,           1.0f,  0.0f,  0.0f,
                                   
    -0.5f, -0.5f, -0.5f,           0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,           0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,           0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,           0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,           0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,           0.0f, -1.0f,  0.0f,
                                   
    -0.5f,  0.5f, -0.5f,           0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,           0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,           0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,           0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,           0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,           0.0f,  1.0f,  0.0f

    };

    unsigned int offset = 0;

    offset = 0;

    unsigned int va;
    report(glGenVertexArrays(1, &va));
    report(glBindVertexArray(va));


    unsigned int sphereBufferAddress;
    report(glGenBuffers(1, &sphereBufferAddress));
    report(glBindBuffer(GL_ARRAY_BUFFER, sphereBufferAddress));

    report(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * circleCoords.size(), GenSphereWithNormals(10.0f, 0.0f, 80) , GL_STATIC_DRAW));

    report(glEnableVertexAttribArray(0));
    report(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)offset));

    offset += 3 * sizeof(float);

    report(glEnableVertexAttribArray(1));
    report(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)offset));

    offset = 0;

    unsigned int va2;
    report(glGenVertexArrays(1, &va2));
    report(glBindVertexArray(va2));

    unsigned int lightCubeBuffer;
    report(glGenBuffers(1, &lightCubeBuffer));
    report(glBindBuffer(GL_ARRAY_BUFFER, lightCubeBuffer));

    report(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 216, positions, GL_STATIC_DRAW));

    report(glEnableVertexAttribArray(0));
    report(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)offset));

  
    /*for (int i = 0; i <= circleCoords.size(); i++) 
    {
        indexBufferArray.push_back(i);
        indexBufferArray.push_back(i + 1);
        indexBufferArray.push_back(i + 2);

        indexBufferArray.push_back(i + 2);
        indexBufferArray.push_back(i + 1);
        indexBufferArray.push_back(i + 3);

    }*/

    unsigned int ib;
    report(glGenBuffers(1, &ib));
    report(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));

    //report(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indexBufferArray.size(), &indexBufferArray[0], GL_STATIC_DRAW));

    
    glm::mat4 MVP = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 MVPforLight = glm::mat4(1.0f);
    glm::vec3 lightColour = glm::vec3(1.0f);
    glm::mat4 perspective = glm::mat4(1.0f);
    glm::vec3 objectColour = glm::vec3(1.0f);
    glm::mat3 NormalMatrix = glm::mat3(1.0f);
    glm::mat4 modelForLight = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    Shader shader("shader/shader.shader");
    Shader lightShader("shader/shader2.shader");


    shader.Bind();

    glm::vec3 lightRotating = lightPos;


    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

       
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        lightPos = glm::vec3(lightRotating.x * glm::cos(glfwGetTime()), lightPos.y, lightRotating.z * glm::sin(glfwGetTime()));
        
        
        
        shader.Bind();
        report(glBindVertexArray(va));
        report(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));

        ImGui::Begin("Sphere");
        ImGui::ColorEdit4("Change Object Colour", glm::value_ptr(objectColour));
        ImGui::ColorEdit4("Change Light Colour", glm::value_ptr(lightColour));

        perspective = glm::perspective(fovy, 960.0f / 540.0f, 0.1f, 100.0f);
        view = glm::lookAt(cameraPosition, cameraPosition + forwardVector, upVector);
        model = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f)), glm::radians(70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        NormalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));

        MVP = perspective * view * model;


        shader.SetMatrix4("MVP", 1, GL_FALSE, glm::value_ptr(MVP));
        shader.SetMatrix4("Model", 1, GL_FALSE, glm::value_ptr(model));
        shader.SetMatrix3("NormalMatrix", 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        shader.SetVec3p("material.diffuse", 1, glm::value_ptr(cameraPosition));
        shader.SetVec3p("cameraPos", 1, glm::value_ptr(cameraPosition));
        shader.SetVec3f("material.diffuse", 1.0f, 0.5f, 0.31f);
        shader.SetVec3f("material.specular", 0.5f, 0.5f, 0.5f);
        shader.SetVec3f("material.ambient", 1.0f, 0.5f, 0.31f);
        shader.SetFloat("material.shininess", 32.0f);        
        shader.SetVec3f("light.diffuse", 1.0f, 1.0f, 1.0f);
        shader.SetVec3f("light.specular", 1.0f,  1.0f, 1.0f );
        shader.SetVec3f("light.ambient", 0.1f, 0.1f, 0.1f);
        shader.SetVec3p("light.position",1, glm::value_ptr(lightPos));

        report(glDrawArrays(GL_TRIANGLES, 0,circleCoords.size()));
        ImGui::End();

        shader.UnBind();





        lightShader.Bind();
        report(glBindVertexArray(va2));


        modelForLight = glm::translate(glm::mat4(1.0f), lightPos);
        MVPforLight = perspective * view *  modelForLight;
        lightShader.SetMatrix4("transform", 1, GL_FALSE, glm::value_ptr(MVPforLight));
        lightShader.SetVec3p("lightColour", 1, glm::value_ptr(lightColour));

        report(glDrawArrays(GL_TRIANGLES, 0, 3));

        
        lightShader.UnBind();
        
        
        
        
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        
        
        glfwPollEvents();
        ProcessInput(window);

        std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;

        
        if (duration.count() < 0.3)
        {
            clickTimer = false;
        }
        else
        {
            clickTimer = true;
        }


        glfwSwapBuffers(window);

        glfwSetCursorPosCallback(window, mouse_pos_callback);
        glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
        glfwSetScrollCallback(window, scroll_callback);

        currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    report(glDeleteVertexArrays(1, &va));

    glfwTerminate();


    return 0;

}


void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    
    if (activateMovement)
    {

        if (firstMouse || afterESC) 
        {
            lastX = xpos;
            lastY = ypos;

            pitch += 0;
            yaw += 0;

            afterESC = false;
            firstMouse = false;
        }
        else 
        {
            pitch += lastY - ypos;
            yaw += xpos - lastX;
        }

        if (pitch > (85))
            pitch = (85.0f);
        if (pitch < (-85.0f))
            pitch = (-85.0f);

        forwardVector.x += cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        forwardVector.y += sin(glm::radians(pitch));
        forwardVector.z += sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        forwardVector = (glm::normalize(forwardVector));

        lastX = xpos;
        lastY = ypos;
    }
    else
    {
        afterESC = true;
    }
}

void ProcessInput(GLFWwindow* window)
{


        glm::vec3 rightVector = glm::normalize(glm::cross(forwardVector, upVector));
        glm::vec3 up = glm::normalize(glm::cross(forwardVector, rightVector));

        float speed = baseSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            if (clickTimer) 
            {
                showCursor == false ? glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL) : glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                activateMovement = (showCursor == true) ? true : false;

                showCursor = (showCursor == true) ? false : true;
                start = std::chrono::high_resolution_clock::now();

            }

        }

        if(activateMovement)
        {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                cameraPosition += forwardVector * speed;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                cameraPosition += -forwardVector * speed;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                cameraPosition += rightVector * speed;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                cameraPosition += -rightVector * speed;

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                cameraPosition += -up * speed;
             if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                cameraPosition += up * speed;

            if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
                baseSpeed += 1;
            if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
                baseSpeed += -1;

            if (baseSpeed < 1.0f)
                baseSpeed = 1.0f;


        }


}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fovy += glm::radians(yoffset * 0.8f);
}

void wait(int seconds) {


}

void PushBack(const long double& xcoord, const long double& ycoord, const long double& zcoord, const float& normalx, const float& normaly, const float& normalz)
{
    circleCoords.push_back((float)xcoord);
    circleCoords.push_back((float)ycoord);
    circleCoords.push_back((float)zcoord);
    circleCoords.push_back((float)normalx);
    circleCoords.push_back((float)normaly);
    circleCoords.push_back((float)normalz);
}

const void* GenSphereWithNormals(long double Radius, long double Zcoord, int numberOfSectors)
{


    long double radius;
    long double angleIncrement = 180.0 / numberOfSectors;
    long double xcoord;
    long double ycoord;
    long double zcoord;
    long double sectorSpanZ = 0.0;
    long double sectorSpanRadius = 0.0;


    for (long double currentAngleForXYPlane = 0.0; currentAngleForXYPlane <= 360.0; currentAngleForXYPlane += angleIncrement) {

        radius = 0.0;
        zcoord = Zcoord - Radius;

        for (long double currentAngleForZYPlane = 0.0; currentAngleForZYPlane <= 180.0; currentAngleForZYPlane += angleIncrement) {

            sectorSpanRadius = ((sin(currentAngleForZYPlane * toRadians) * Radius) - (sin((currentAngleForZYPlane + angleIncrement) * toRadians) * Radius));
            sectorSpanZ = (cos((currentAngleForZYPlane)*toRadians) * Radius) - (cos((currentAngleForZYPlane + angleIncrement) * toRadians) * Radius); // remove 90's and try

            {

                xcoord = cos(rad(currentAngleForXYPlane)) * radius;
                ycoord = sin(rad(currentAngleForXYPlane)) * radius;

                glm::vec3 coord1(xcoord, ycoord, zcoord);


                xcoord = cos(rad(currentAngleForXYPlane + angleIncrement)) * radius;
                ycoord = sin(rad(currentAngleForXYPlane + angleIncrement)) * radius;

                glm::vec3 coord2(xcoord, ycoord, zcoord);


                xcoord = cos(rad(currentAngleForXYPlane)) * (radius - sectorSpanRadius);
                ycoord = sin(rad(currentAngleForXYPlane)) * (radius - sectorSpanRadius);
                zcoord = zcoord + sectorSpanZ;  // zcoord nearer

                glm::vec3 coord3(xcoord, ycoord, zcoord);


                glm::vec3 normal = glm::normalize((coord1 + coord2 + coord3));


                PushBack(coord1.x, coord1.y, coord1.z, normal.x, normal.y, normal.z);
                PushBack(coord2.x, coord2.y, coord2.z, normal.x, normal.y, normal.z);
                PushBack(coord3.x, coord3.y, coord3.z, normal.x, normal.y, normal.z);

                // first triangle
            }

            {

                glm::vec3 coord1(xcoord, ycoord, zcoord);

                xcoord = cos((currentAngleForXYPlane + angleIncrement) * toRadians) * radius;
                ycoord = sin((currentAngleForXYPlane + angleIncrement) * toRadians) * radius;
                zcoord = zcoord - sectorSpanZ;  // furthest


                glm::vec3 coord2(xcoord, ycoord, zcoord);


                xcoord = cos(rad(currentAngleForXYPlane + angleIncrement)) * (radius - sectorSpanRadius);
                ycoord = sin(rad(currentAngleForXYPlane + angleIncrement)) * (radius - sectorSpanRadius);
                zcoord = zcoord + sectorSpanZ; // nearer

                glm::vec3 coord3(xcoord, ycoord, zcoord);


                glm::vec3 normal = glm::normalize((coord1 + coord2 + coord3));

                PushBack(coord1.x, coord1.y, coord1.z, normal.x, normal.y, normal.z);
                PushBack(coord2.x, coord2.y, coord2.z, normal.x, normal.y, normal.z);
                PushBack(coord3.x, coord3.y, coord3.z, normal.x, normal.y, normal.z);


                // second triangle
            }


            radius -= sectorSpanRadius;




        }





    }

   

    std::cout << circleCoords.size()/2 << " floats have been calculated ! \n";

    return circleCoords.data();
}