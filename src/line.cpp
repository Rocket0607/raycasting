#include <iostream>

struct Line {
    int height;
    int x;
    int side;
    float hit_position;
};

// class Line {
// public:
//     unsigned int *m_VAO;
//     glm::mat4 MVP = glm::mat4(1.0f);
//     glm::vec3 m_color;
//     int *m_shaderProgram;
//     Line(float height, float x, glm::vec3 color, unsigned int *VAO, int *shaderProgram)
//     {
//         m_color = color;
//         // TODO: create MVP
//         MVP = glm::translate(MVP, glm::vec3(x, 0.0f, 0.0f));
//         MVP = glm::scale(MVP, glm::vec3(1.0f, height, 1.0f));
//         m_VAO = VAO;
//         m_shaderProgram = shaderProgram;

//     }
//     int draw()
//     {
//         glUseProgram(*m_shaderProgram);
//         glUniformMatrix4fv(glGetUniformLocation(*m_shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
//         glUniform3fv(glGetUniformLocation(*m_shaderProgram, "color"), 1, &m_color[0]);

//         glBindVertexArray(*m_VAO);
//         glDrawArrays(GL_LINES, 0, 2);
//         return 1;
//     }
// };