#define DEBUG true

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "log.h"



using std::string;

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall() {
	while (GLenum error = glGetError()) {
		log(GLErrorToString(error), ERROR_GL);
		return false;
	}
	return true;
}

//per passare le due shader contemporaneamente
struct ShaderProgramSource {
	string VertexSource;
	string FragmentSource;
};

// Funzione per leggere il file shader
static ShaderProgramSource PraseShader(const string& filepath){

    std::ifstream stream(filepath);

    enum class ShaderType : char{
        NONE     = -1,
        VERTEX   =  0,
        FRAGMENT =  1
    };

    std::stringstream ss[2]; // array che contiene il codice del vertex e fragment shader

    string line;
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)){

        if (line.find("#shader") != string::npos){ // trva la keyword per identificare il tipo di shader

            if (line.find("vertex") != string::npos)
                type = ShaderType::VERTEX;

            else if (line.find("fragment") != string::npos)
                type = ShaderType::FRAGMENT;

        } else { // è una riga di codice quindi la aggiungo alla shader corretta

            ss[(int)type] << line << '\n';
        }
    }
    return {ss[0].str(), ss[1].str()};
}

GLFWwindow* setupOpenGL(int width, int height, const char* title) {
    // Inizializza GLFW
    if (!glfwInit()) {
        std::cerr << "Errore nell'inizializzazione di GLFW" << std::endl;
        return nullptr;
    }

    // Crea la finestra
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Errore nella creazione della finestra" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Imposta il contesto OpenGL per la finestra
    glfwMakeContextCurrent(window);

    // Inizializza GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Errore nell'inizializzazione di GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    return window;
}

static unsigned int CompileShader(unsigned int type, const string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cerr << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int  CreateShader(const string& vertexShader, const string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

int main() {
    // Configura OpenGL e crea la finestra
    GLFWwindow* window = setupOpenGL(1080, 720, "Finestrella");
    if (!window) return -1;

    glfwSwapInterval(1);
    // Vertici di un triangolo
    float posizioni[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
		-0.5f,  0.5f, // 3
    };

    unsigned int indici[] = {
    	0, 1, 2,
		2, 3, 0
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posizioni), posizioni, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indici), indici, GL_STATIC_DRAW);

    ShaderProgramSource source = PraseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_color"); // u_color = è il nome della varibile usata sella sheder
    ASSERT(location != -1); // non trova la variabile o non viene usata
    glUniform3f(location, 1.0f, 0.0f,0.0f); //3 voul dire un vec3 e f voul dire float


    float r = 0.0f;
    float incremento = 0.05f;
    // Loop principale
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform3f(location, 0.5f, 1.0f-r, r);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //numero di indici

        if (r > 1.0f)
        	incremento = -0.05f;
        else if (r < 0.0f)
        	incremento = 0.05f;

        r += incremento;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //pulizzia
    //glDeleteProgram(shader);
    glfwTerminate();

    return 0;
}
