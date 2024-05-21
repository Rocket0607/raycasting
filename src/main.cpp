#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "line.cpp"
// #include "../raycast.cpp"

#define PI 3.14159

#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


std::vector<struct Line> get_lines(double posX, double posY, double dirX, double dirY);
void init(unsigned int* VAO, unsigned int* VBO, int* shaderProgram);
void glfwError(int id, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

double pX = 22.0;
double pY = 12.0;
double dX = -1.0, dY = 0.0;
double planeX = 0.0, planeY = 0.66;
double speed = 0.005;
double theta = PI / (180.0 * 4);

int main()
{
    glfwSetErrorCallback(&glfwError);    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Raycasting", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load OpengGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    unsigned int VBO, VAO;
    int shaderProgram;
    std::vector<float> vertices = {
        0.0f, 1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
    };
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "out float yPos;\n"
        "void main()\n"
        "{\n"
        "   yPos = aPos.y;\n"
        "   gl_Position = model * view * projection * vec4(aPos, 1.0);\n"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec3 color;\n"
        "uniform sampler2D ourTexture;\n"
        "uniform float texCoord;\n"
        "in float yPos;\n"
        "void main()\n"
        "{\n"
        "   FragColor = texture(ourTexture, vec2(texCoord, yPos)) * vec4(color, 1.0);\n"
        // "   FragColor = vec4(color, 1.0);"
        "}\n\0";

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors

    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // TEXTURES
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("res/kaveen.jpg", &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "error loading texture\n";
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);


    glm::vec3 color;

    GLfloat delta_time = 0.0f;
    GLfloat last_frame = 0.0f;


    while (!glfwWindowShouldClose(window)) {
        GLfloat curr_frame = glfwGetTime();
        delta_time = curr_frame - last_frame;
        last_frame = curr_frame;

        processInput(window);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        std::vector<struct Line> lines = get_lines(pX, pY, dX, dY);

        for (struct Line line : lines) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(1.0f, (line.height/(float)screenHeight), 1.0f));
            model = glm::translate(model, glm::vec3(2*line.x/(float)screenWidth-1, 0.0f, 0.0f));
            glm::mat4 view = glm::mat4(1.0f);
            // glm::mat4 projection = glm::ortho(0.0f, 641.0f, 0.0f, 481.0f, 0.1f, 100.0f);
            glm::mat4 projection = glm::mat4(1.0f);
            color = (line.side == 0) ? glm::vec3(1, 1, 1) : glm::vec3(0.8, 0.8, 0.8);
            glUseProgram(shaderProgram);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &color[0]);
            glUniform1f(glGetUniformLocation(shaderProgram, "texCoord"), line.hit_position);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(VAO);
            glDrawArrays(GL_LINES, 0, 2);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void init(unsigned int* VAO, unsigned int* VBO, int* shaderProgram)
{

}

// error: callback to print errors
void glfwError(int id, const char* description)
{
  std::cout << description << std::endl;
}

// process input:  quary GLFW for relevant key status and react accordingly
void processInput(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (worldMap[(int)(pX + speed * dX)][(int)(pY + speed * dY)] == 0) {
          pY = pY + speed * dY;
          pX = pX + speed * dX;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (worldMap[(int)(pX - speed * dX)][(int)(pY - speed * dY)] == 0) {
          pY -= speed * dY;
          pX -= speed * dX;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      if (worldMap[(int)(pX - speed * dX)][(int)(pY + speed * dY)] == 0) {
        pY += speed * dX;
        pX -= speed * dY;
      }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      if (worldMap[(int)(pX + speed * dX)][(int)(pY - speed * dY)] == 0) {
        pY -= speed * dX;
        pX += speed * dY;
      }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        theta *= -1;
        dX = cos(theta)*dX - sin(theta)*dY;
        dY = sin(theta)*dX + cos(theta)*dY;
        planeX = cos(theta)*planeX - sin(theta)*planeY;
        planeY = sin(theta)*planeX + cos(theta)*planeY;
        theta *= -1;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        dX = cos(theta)*dX - sin(theta)*dY;
        dY = sin(theta)*dX + cos(theta)*dY;
        planeX = cos(theta)*planeX - sin(theta)*planeY;
        planeY = sin(theta)*planeX + cos(theta)*planeY;
    }
}

// window: when window size is changed, this function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

std::vector<struct Line> get_lines(double posX, double posY, double dirX, double dirY)
{

/*
* 1. Init positions, directions and camera plane
* 2. For each ray:
*   1. determine its cameraX, or the point at which it intersects the camera plane
*   2. determine its x and y components using the player's position, direction and the cameraX value found earlier
*   3. calculate delta distance between x and y grid squares using delta distance = abs(1/ray direction)
*   4. calculate stepX and stepY (-1 if direction is negative, 1 if direction is positive)
*   5. calculate distance from current position to closest x and y grid squares  
*   6. perform DDA: while the ray has not hit anything, increment the x or y pos by delta distance depending on which one 
       is currently lower. increment the map coord x and y using step and set side = 0 if x, and side = 1 if y.
*   7. get distance from camera plane to wall sideDist - delta distance
*   8. line height = h / distance to wall
*/

  // int* lines = (int*)malloc(sizeof(int) * screenWidth);
  std::vector<struct Line> lines;


  double cameraX;
  double rayDirX, rayDirY;
  double deltaDistX, deltaDistY;
  double sideDistX, sideDistY;
  int mapX, mapY;
  int stepX, stepY;
  int hit, side;
  double perpWallDist;
  int lineHeight;

  for (int i = 0; i < screenWidth; i++) {
    cameraX = 2 * i / (double)screenWidth - 1;
    rayDirX = dirX + planeX * cameraX;
    rayDirY = dirY + planeY * cameraX;

    deltaDistX = std::abs(1 / rayDirX);
    deltaDistY = std::abs(1 / rayDirY);

    mapX = (int)posX;
    mapY = (int)posY;

    //calculate step and initial sideDist
    if (rayDirX < 0) {
      stepX = -1;
      sideDistX = (posX - mapX) * deltaDistX;
    } else {
      stepX = 1;
      sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    } 
    if (rayDirY < 0) {
      stepY = -1;
      sideDistY = (posY - mapY) * deltaDistY;
    } else {
      stepY = 1;
      sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }

    // calculate y or x movement
    double hit_position;

    hit = 0;
    side = 0;

    while (hit==0) {
      // calculate opposite distance moved per iteration
      // everytime x or y dist is moved, increment y or x movement
      // when a collision is made, the decimal value of the y or x movement is the x value of the texture that has to be used
      if (sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      } else {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }
      if (worldMap[mapX][mapY] > 0) hit = 1;
    }
    if(side == 0) {
      perpWallDist = (sideDistX - deltaDistX);
      hit_position = posY + rayDirY * perpWallDist;
    } else {
      perpWallDist = (sideDistY - deltaDistY);
      hit_position = posX + rayDirX * perpWallDist;
    }

    lineHeight = (int)(screenHeight / perpWallDist);
    struct Line line;
    line.height = lineHeight;
    line.x = i;
    line.side = side;
    line.hit_position = (float)hit_position;
    lines.push_back(line);
  }
  return lines;
}
