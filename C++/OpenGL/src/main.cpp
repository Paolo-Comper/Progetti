#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Funzione per leggere il file shader
static void PraseShader(const std::string& filepath){

    std::ifstream stream(filepath);

    enum class ShaderType{
        NONE     = -1, 
        VERTEX   =  0, 
        FRAGMENT =  1
    };

    std::stringstream ss[2]; // array che contiene il codice del vertex e fragment shader

    std::string line;
    ShaderType type = ShaderType::NONE;
    
    while (getline(stream, line)){

        if (line.find("#shader") != std::string::npos){ // trva la keyword per identificare il tipo di shader

            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;            

        } else { // è una riga di codice quindi la aggiungo alla shader corretta

            ss[(int)type] << line << '\n';
        }
    }
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


static unsigned int CompileShader(unsigned int type, const std::string& source) {
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

static unsigned int  CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
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

    // Vertici di un triangolo
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Sinistra
         0.5f, -0.5f, 0.0f, // Destra
         0.0f,  0.5f, 0.0f  // Alto
    };

    // Creazione e binding del buffer
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::string vertexShader = 
        "#version 330 core                          \n"
        "                                           \n"
        "layout(location = 0) in vec4 position;     \n"
        "                                           \n"
        "void main()                                \n"
        "{                                          \n"
        "   gl_Position = position;                 \n"
        "}                                          \n";

    std::string fragmentShader =    
        "#version 330 core                          \n"
        "                                           \n"
        "layout(location = 0) out vec4 color;       \n"
        "                                           \n"
        "void main()                                \n"
        "{                                          \n"
        "   color = vec4(0.0, 0.0, 1.0, 1.0);       \n"
        "}                                          \n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    // Loop principale
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Pulizia
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
