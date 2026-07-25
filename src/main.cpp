/*
This is the main code file for the notepad app project. There are a few different sections to be aware of:
 - Setup: This section sets up the various libraries we need, and brings in the associated header files. It prepares all
    of our data structures to be used
 - Main loop: This section is repeatedly executed while the window is open. It processes the user input, and calls the
    code to draw the UI elements
 - Cleanup: This section runs once we close out the window. It tells the operating system we are finished, and that it
    can release the various computing resources we were using
 - Frame drawing: This section draws all the different parts of the window. This is the section you will be editing

 */

// ----- SECTION: Setup ----- //

// These include statements allow us to access the various libraries we will be using
#include <vector>
#include "glfw/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// This header is code that we write! You are allowed to modify this too if you have other forward declarations to make
#include "notepad.h"

// This variable is used to store the window we are drawing stuff to. See the comments next to the line containing glfwCreateWindow.
GLFWwindow* window = nullptr;

// This is our main function! Hopefully, it is familiar to you as it is the entrypoint to every C or C++ program. This
// is where our program starts executing when we click run.
int main() {
    // Initialize the GLFW graphics backend. If it fails, abort the program since there's not much we can do without graphics
    if(!glfwInit()) return -1;

    // Indicate to GLFW we want to maximize the next window we create
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    // Create the window itself. The GLFWwindow type is what is known as an "opaque type": we are not actually given any
    // information about the structure of the variable itself. It is only useful as a parameter to GLFW library
    // functions. However, internally GLFW does know what to do with it, so it's not our problem.
    window = glfwCreateWindow(1280, 720, "Awesome Notepad", nullptr, nullptr);

    // If window creation fails, abort
    if(window == nullptr) return -2;

    // Set the current window as the selected one to draw to
    glfwMakeContextCurrent(window);

    // Enable vsync so we don't waste computer resources drawing as many frames as possible, since the monitor can't
    // display infinitely fast
    glfwSwapInterval(1);

    // Set up the UI library. More backend initialization stuff involving telling ImGui about OpenGL and GLFW
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // ----- SECTION: Main Loop ----- //

    // Loop while the user hasnt clicked the x to close the window
    while(!glfwWindowShouldClose(window)) {
        // Ask the operating system for any user inputs that have happened. GLFW keeps track of these for us and tells
        // ImGui about them
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

        // Tells ImGui we are done with our drawing area
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

    // ----- SECTION: Cleanup ----- //

    // Once we have closed out the notepad, cleanup the window and the graphics library
    glfwDestroyWindow(window);
    glfwTerminate();
}

// ----- SECTION: Frame Drawing ----- //

// Before we look at the actual graphics code, lets talk more about how the notepad function works. This functionality
// centers around the ImGui function `ImGui::InputTextMultiline`, which will draw a text box we can type in. In order
// for this function to work, ImGui requires that we provide the memory that the typed text is
// placed in to, so we use the following vector to store it. A vector is like an array, in that it stores some
// contiguous sequence of elements. However, unlike an array we are able to resize it at will.
//
// This is helpful because it means the user can type away and not have to worry about a maximum text length. Also note
// the type for this variable. It is declared as a `std::vector<char>`. The part in angle brackets is important, as it
// tells us the type of data the vector is holding. In this case, since we are storing characters, we use the char type.
//
// Importantly, we rely on the null character (represented by 0) to tell imgui that there are blank positions in the
// vector it can use. The null character is specially reserved by ASCII for exactly this purpose.
std::vector<char> notepad_text;

// This is the function which actually draws all the UI elements we want
void draw() {
    // If the vector is empty, we want to add some null characters in so that imgui has somewhere to start placing text
    if(notepad_text.empty()) notepad_text.insert(notepad_text.end(), 5, 0);

    // Get the size of the window for sizing the textbox
    const ImVec2 winSize = ImGui::GetWindowSize();

    // A little header line
    ImGui::Text("Jot down some notes!");

    // This function tells ImGui to draw the textbox itself. The parameters are, in order:
    //   - An ID to use for this element. This is needed by ImGui to keep track of the textbox between frames
    //   - A pointer to the char buffer where we store the raw text (in this case, this starts at the first element in our vector).
    //   - The size of the char buffer (how many spaces we have in our vector)
    //   - The size we want the textbox to be. We want it to take up the whole window, but subtract off some room for
    //       some blank space around the edges, as well as some room for the line of text from above. This amount was somewhat arbitrarily chosen.
    //   - Some extra information this function needs to do what we want. Importantly, we want it to notify us when we
    //       need to make the char buffer larger (add more spaces to the vector)
    //   - The function ImGui should call when we have to add more space
    ImGui::InputTextMultiline("##textbox", &notepad_text[0], notepad_text.size(),
                              winSize - ImVec2{50, 75},
                              ImGuiInputTextFlags_WordWrap | ImGuiInputTextFlags_CallbackResize, resizeCallback);

    // That's it for the drawing! Feel free to add more UI elements here to add to the things your notepad app shows!

}

// This function is called by ImGui when it determines it needs more space. We are passed this data argument, which
// gives us information about how much new space we need
int resizeCallback(ImGuiInputTextCallbackData* data) {
    // Make sure this really is a request for more buffer space
    if(data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        // If it is, ImGui will tell us how large it wants the buffer to be through the data->BufSize variable. So,
        // we need to make the total size of the vector equal to data->BufSize. To do this, we need to add the
        // difference between the current size and the requested size, or 'data->BufSize - notepad_text.size()'.
        // We add in that many null characters, that way we dont have random text showing up.

        // This insert function here takes in 3 parameters:
        //   - Where in the buffer we want to place the new characters. ImGui expects us to add more space at the end,
        //       and the first parameter is how we communicate where in the buffer we want to add more space. In this
        //       case, there is a convenient .end() function which we can use to specify at the end of the buffer
        //   - The number of characters to add. As discussed, we want data->BufSize - notepad_text.size() new characters
        //   - What character to add. Since we want to add null chracters, we specify 0
        notepad_text.insert(notepad_text.end(), data->BufSize - notepad_text.size(), 0);

        // Internally, when we add new spaces to the vector, it has to re-allocate the memory it is using to store our
        // data to be larger. This means the memory location at which ImGui can store chars could potentially change.
        // Thus, we tell ImGui where the new buffer location is.
        data->Buf = &notepad_text[0];
    }

    // Required by the library. Is technically used to signal whether this function succeeded, but for us, it always will
    // so we can just return 0
    return 0;
}
