#include <SDL2/SDL.h>
#include <SDL_video.h>
#include <chrono>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <ostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>




glm::mat4 transformationMatrix(float time, float translationCoefficient, float rotationCoefficient, SDL_Window*& window){
	int width;
	int height;
	SDL_GetWindowSize(window, &width, &height);
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(translationCoefficient*sin(time), translationCoefficient*cos(time), -4.0f));
	//model = glm::rotate(model, rotationCoefficient*time, glm::normalize(glm::vec3(1,1,1)));
	model = glm::translate(model, glm::vec3(cos(time), sin(time), -5 + sin(time)));
	model = glm::rotate(model, time, glm::normalize(glm::vec3(3, 1 ,4)));
	model = glm::perspective((float)M_PI/3.f, (float)width/height, 1.f, 100.f) * model;
	return model;
}

void GetShaderStatus(GLuint object, const std::string type);

bool Initialize(SDL_Window*& window, SDL_GLContext& context, int width, int height){

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "failed to initialize sdl, heres the reason: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow("literally a triangle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if(!window){
		std::cerr << "failed to create window: " << SDL_GetError() << std::endl;
		return false;
	}

	context = SDL_GL_CreateContext(window);
	if(!context){
		std::cerr << "failed to put in context: " << SDL_GetError() << std::endl;
		return false;
	}

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		std::cerr << "failed to initialize glad" << std::endl;
		return false;
	}

	glad_glViewport(0, 0, width, height);
	return true;
}
GLuint CreateShaderProgram(const char* vertexSource, const char* fragmentSource){
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;

	vertexShader = glad_glCreateShader(GL_VERTEX_SHADER);
	glad_glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glad_glCompileShader(vertexShader);
	GetShaderStatus(vertexShader, "SHADER");

	fragmentShader = glad_glCreateShader(GL_FRAGMENT_SHADER);
	glad_glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glad_glCompileShader(fragmentShader);
	GetShaderStatus(fragmentShader, "SHADER");

	program = glad_glCreateProgram();
	glad_glAttachShader(program, vertexShader);
	glad_glAttachShader(program, fragmentShader);
	glad_glLinkProgram(program);
	GetShaderStatus(program, "PROGRAM");

	glad_glDeleteShader(vertexShader);
	glad_glDeleteShader(fragmentShader);

	return program;
}
void GetShaderStatus(GLuint object, const std::string type){
	
	GLint success;
	GLint success2;
	GLchar infolog[1024];

	if (type == "PROGRAM"){
		glad_glValidateProgram(object);
		glad_glGetProgramiv(object, GL_VALIDATE_STATUS, &success);
		glad_glGetProgramiv(object, GL_LINK_STATUS, &success2);
		success = success & success2;
		if (!success){
			glad_glGetProgramInfoLog(object, 1024, nullptr, infolog);
			std::cerr << "program failed to thing\n" << infolog << std::endl;
			return;
		}
	}

	if (type == "SHADER"){
		glad_glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if(!success){
			glad_glGetShaderInfoLog(object, 1024, nullptr, infolog);
			std::cerr << "shader failed to compile\n" << infolog << std::endl;
			return;
		}
	}
	return;
	
}
char* readFileToCharPointer(const std::string filePath) {
    // Open the file in binary mode
    std::ifstream file(filePath, std::ios::binary);

    // Check if file is open
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return nullptr;
    }

    // Get the length of the file
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate memory for the file content
    char* buffer = new char[fileSize + 1];

    // Read the file into the buffer
    file.read(buffer, fileSize);
    buffer[fileSize] = '\0'; // Null-terminate the string

    file.close();
    return buffer;
}

void SetupBuffers(GLuint& VAO, GLuint& VBO){
    float vertices[] = {

    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    
    1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,

    // Back face (Z = -1)
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    
    1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,

    // Left face (X = -1)
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,

    // Right face (X = 1)
    1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,

    // Top face (Y = 1)
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,

    // Bottom face (Y = -1)
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f

    };

    glad_glGenVertexArrays(1, &VAO);
    glad_glGenBuffers(1, &VBO);

    glad_glBindVertexArray(VAO);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glad_glEnableVertexAttribArray(0);

    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
    glad_glBindVertexArray(0);
}


//void UpdateBuffer(GLuint& VBO){}






//main loop for the game
void MainLoop(SDL_Window*& window, SDL_GLContext& context, GLuint& shaderProgram){







	//predeclaring variables
	SDL_Event e;
	bool running = true;
	GLuint VAO;
	GLuint VBO;
	SetupBuffers(VAO, VBO);
	glad_glBindVertexArray(VAO);
	static auto start = std::chrono::steady_clock::now();  



	while (running){

		//events
		while(SDL_PollEvent(&e)){
			switch (e.type){

				case SDL_QUIT:
					{running = false; break;}
				
				default:
					break;
			}
		}

		
	float time = std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();

		glad_glUniform1f(glad_glGetUniformLocation(shaderProgram, "uTime"), time);

		glm::mat4 trans = transformationMatrix(time, 0.6, 1, window);
		glad_glUniformMatrix4fv(glad_glGetUniformLocation(shaderProgram, "transformationMatrix"), 1, GL_FALSE, &trans[0][0]);

		//rendering

		glad_glDrawArrays(GL_TRIANGLES, 0, 36);
		SDL_GL_SwapWindow(window);
		glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	}
}


int main(){

	SDL_Window* window = nullptr;
	SDL_GLContext context;
	int width = 800; int height = 400;
	char* vertexSource = readFileToCharPointer("/home/blue_birb/codingProjects/graphics/triangle/src/vertex.glsl");
	char* fragmentSource = readFileToCharPointer("/home/blue_birb/codingProjects/graphics/triangle/src/fragment.glsl");

	if(!Initialize(window, context, width, height)) return -1;
	
	//setup
	GLuint shaderprogram = CreateShaderProgram(vertexSource, fragmentSource);
	glad_glUseProgram(shaderprogram);
	glad_glClearColor(0.3, 0.3, 0.3,1);
	glad_glDisable(GL_CULL_FACE);
	glad_glEnable(GL_DEPTH_TEST);
	

	MainLoop(window, context, shaderprogram);
	std::cout << "buh bye!" << std::endl;
	return 0;
}
