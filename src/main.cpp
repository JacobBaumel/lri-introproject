// We use a vector to store the notepad text
#include <vector>

// These are the headers we need for the UI
#include "glfw/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// This header is provided so people can add their own stuff
#include "notepad.h"

// This is used to store the actual notepad text itself
std::vector<char> notepad_text;

int main() {
    // Add some null chars so that we have a buffer with space
    notepad_text.insert(notepad_text.begin(), 5, 0);

    // Initialize the graphics backend. If it fails error out
    if(!glfwInit()) return -1;

    // Indicate to the graphics library we want to maximize the next window we create
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    // Create the window itself
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Awesome Notepad", nullptr, nullptr);

    // If window creation fails, error out
    if(window == nullptr) return -2;

    // Set the current window as the selected one to draw to
    glfwMakeContextCurrent(window);

    // Enable vsync so we dont waste computer resources drawing as fast as possible
    glfwSwapInterval(1);

    // Setup the graphics library. All backend initialization stuff
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // Loop while the user hasnt clicked the x to close the window
    while(!glfwWindowShouldClose(window)) {
        // Poll for mouse and keyboard events
        glfwPollEvents();

        // Tell the various pieces of the graphics library we are starting a new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Begin an area we can draw to
        ImGui::SetNextWindowPos(ImVec2{0, 0});
        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
        if(ImGui::Begin("##notepad", nullptr,
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoCollapse)) {
            // Call the draw function, which is where your code goes
            draw();
        }

        // Uncomment this line to also show a demo window that shows all the built in graphics utilities
        // ImGui::ShowDemoWindow();
        ImGui::End();

        // Render the frame and draw it to the window
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Once we have closed out the notepad, cleanup the window and the graphics library
    glfwDestroyWindow(window);
    glfwTerminate();
}

// This function draws all the UI elements we want
void draw() {
    // Get the size of the window for sizing the textbox
    ImVec2 winSize = ImGui::GetWindowSize();

    // A little header line
    ImGui::Text("Jot down some notes!");

    // This is the textbox itself. The parameters are, in order:
    //   - An ID to use for this element. This is needed by the graphics library to keep track of things
    //       between frames
    //   - A pointer to the char buffer where we store the raw text
    //   - The size of the char buffer
    //   - The size we want the textbox to be. We want it to take up the whole window, but subtract off some room for
    //       some blank space around the edges, as well as some room for the line of text from above
    //   - Some extra information this function needs to do what we want. Importantly, we want it to notify us when we
    //       need to make the char buffer larger
    //   - The function the graphics library should call when we have to resize
    ImGui::InputTextMultiline("##textbox", &notepad_text[0], notepad_text.size(),
                              winSize - ImVec2{50, ImGui::GetCursorPosX() + 50},
                              ImGuiInputTextFlags_WordWrap | ImGuiInputTextFlags_CallbackResize, resizeCallback);
}

// This function is called by the graphics library when it determines it needs more space in the buffer
int resizeCallback(ImGuiInputTextCallbackData* data) {
    // Make sure this really is a request for more buffer size
    if(data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        // If it is, the library will tell us how large it wants the buffer to be through the data->BufSize variable. So,
        // we need to make the total size equal to data->BufSize. To do this, we need to add the difference between the
        // current size and the requested size, or 'data->BufSize - notepad_text.size()'
        // We want to add in null characters, so the last parameter here is zero
        notepad_text.insert(notepad_text.end(), data->BufSize - notepad_text.size(), 0);

        // Tell imgui where the new buffer location is, since adding new characters causes notepad_text to reallocate the
        // array, changing the memory location
        data->Buf = &notepad_text[0];
    }

    // Required by the library. Is technically used to signal whether this function succeeded, but for us it always will
    return 0;
}
