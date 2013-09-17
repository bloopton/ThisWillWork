#include <glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <SOIL.h>

const char* VertexShader=

	"#version 330 \n"//opengl 3.3.
	"layout(location = 0) in vec2 coord;"//inputs information about initial vector location
	"layout(location = 1) in vec2 uvCoord;"
	"out vec2 uv;"
	"void main()"
	"{"
	"	gl_Position = vec4(coord, 0.0, 1.0);" /*gl_Position not gl_position*/
	"	uv = uvCoord;"
	"}";


const char* FragmentShader=

	"#version 330 \n"
	"in vec2 uv;"
	"out vec4 finalcolor;"//exports information about the color to display on screen
	"uniform sampler2D mySampler;"
	"void main()"
	"{"
	"	finalcolor = texture(mySampler, uv).rgb;" /*fail, you used a : instead of a ;*/
	"}";



int main()
{
	glfwInit();
	GLFWwindow* myWindow = glfwCreateWindow(500, 500, "YOLO", NULL, NULL);
	glfwMakeContextCurrent(myWindow);
	glewInit();



	GLuint simpleVertexShader= glCreateShader (GL_VERTEX_SHADER); //creates an GLuint  shader using the vertex shader variable
	GLuint simpleFragmentShader= glCreateShader (GL_FRAGMENT_SHADER);//same but for fragment shader

	glShaderSource(simpleVertexShader, 1, &VertexShader, 0);//tells the newly created vertex shader to go up to where VertexShader is and read the strings
	glCompileShader(simpleVertexShader);//compiles the shader

	glShaderSource(simpleFragmentShader, 1, &FragmentShader, 0);
	glCompileShader(simpleFragmentShader);

	GLuint simpleShaderProgram = glCreateProgram(); //creates a program to process the shaders
	glAttachShader(simpleShaderProgram, simpleVertexShader); //links the Vertex Shader to the shader program
	glAttachShader(simpleShaderProgram, simpleFragmentShader);

	glLinkProgram(simpleShaderProgram);//compiles and creates the actual program out of the two attached shaders

	glDeleteShader(simpleVertexShader);//now that they are in the program, the individual shaders are no longer needed
	glDeleteProgram(simpleFragmentShader);

	glUseProgram(simpleShaderProgram); //sets simpleShaderProgram as the current shader program

	

	
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	int width, height;
	unsigned char* image =
    SOIL_load_image( "yolo.png", &width, &height, 0, SOIL_LOAD_RGB );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );

	GLuint square;
	glGenVertexArrays(1, &square);
	glBindVertexArray(square);

	GLfloat coords[] =
	{
		-1.0f, -1.0f,		 0.0f, 0.0f,
		-1.0f,  1.0f,		 0.0f, 1.0f,
		 1.0f,  1.0f,		 1.0f, 1.0f,

		 1.0f, -1.0f,		 1.0f, 0.0f,
		 1.0f,  1.0f,		 1.0f, 1.0f,
		-1.0f, -1.0f,		 0.0f, 0.0f
	};

	GLuint sqaureBuffer;
	glGenBuffers(1, &sqaureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sqaureBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*) (sizeof(GLfloat) * 2));


	bool running = true;
	while(running == true)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		if(glfwGetKey(myWindow, GLFW_KEY_ESCAPE))
			running = false;

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(myWindow);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}