// uniformֵ��ʹ�÷���
// ��ɫ��֮��ͨ�����������ֵ�ķ���

#include <HEADER/utils.hpp>
#include <HEADER/call_backs.hpp>
#include <HEADER/set_uniform.hpp>

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 colorFromVertexShader;\n" // ָ��һ����������ݸ�fragmentShader
    "uniform vec4 ourColor;\n" // ͨ������Ѱ��uniform��λ�ò�Ϊ������ֵ
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   colorFromVertexShader = ourColor;\n"
    "}\n";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 colorFromVertexShader;\n" // ���ͺ����Ʊ����봫�ݵı�����ͬ
    //"uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    //"   FragColor = ourColor;\n"
    "   FragColor = colorFromVertexShader;\n"
    "}\n";

int main()
{
	GLFWwindow* window = initAndCreateWindow(2000, 1500, "uniform");
    if (NULL == window) {
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeChangeCallback); 


	unsigned int vertexShaderID = createShader("VERTEX", vertexShaderSource);
	unsigned int fragmentShaderID = createShader("FRAGMENT", fragmentShaderSource);
	unsigned int shaderProgramID = createShaderProgram(vertexShaderID, fragmentShaderID);
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);


    float vertices[] = {
        // ������1
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,

    };

    unsigned int VAO = set_VAO_VBO_EBO(
        vertices,
        sizeof vertices,
		nullptr,
        0,
        0,
        3,
        3,
        0
    );


    while (!glfwWindowShouldClose(window)) {
		jobAtRenderLoopStart(window);

        double timeValue = glfwGetTime();
        // ��sin��ȡһ����ʱ��仯��0-1��ֵ
        float greenValue = (float)(sin(timeValue) / 2.0 + 0.5);

		set4float(shaderProgramID, 
            "ourColor",
			0.0f,
			greenValue,
			0.0f,
			1.0f
		);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

		jobBeforRenderLoopEnd(window);
    }

    glfwTerminate();
    return 0;
}


