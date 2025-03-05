#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Funzione per inizializzare GLFW, creare la finestra e inizializzare GLEW
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

int main() {
    // Configura OpenGL e crea la finestra
    GLFWwindow* window = setupOpenGL(800, 600, "OpenGL Example");
    if (!window) return -1;

    // Loop principale
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap dei buffer e gestione eventi
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Pulizia
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
