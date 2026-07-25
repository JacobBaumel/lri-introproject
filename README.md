# LRI Onboarding Project

Hello, and welcome to LRI Electronics onboarding for 2026! This repository is part of the asynchronous introductory
onboarding materials, so If you have not already, please see
the [electronics wiki](https://github.com/liquid-rocketry-illinois/Electronics-Onboarding-2026/wiki) for instructions.

# Project Purpose

This guided project is intended to help get you started with C++ by giving you a look at real code, as well as a chance
to add your own. You will be investigating and extending a simple notepad app to replace the slow and boring default
Windows notepad (this will work on Mac/Linux too!). To accomplish this, we are relying on the open
source [Dear ImGui](https://github.com/ocornut/imgui) library, which provides a simple to use framework for building
graphical applications. You may be surprised to learn Dear Imgui is used
in [more places than you expect](https://github.com/ocornut/imgui/wiki/Software-using-dear-imgui), including at
companies like Google, Netflix, Microsoft, and NVidia. This goes to say this is not yet another "hello world", but
rather a code framework you can actually use in real production.

## Before Starting

Before jumping in, please make sure you have looked at the [electronics wiki](), which contains important background
information on [setting up your development environment]() and [using Git](). Also, take a look at
the [introductory C++]() materials, as this project builds on the topics present there.

## A note on AI

While generative AI models like Claude, Gemini, etc. can be useful when writing code, try to avoid using a model here.
This project is to help you learn some coding skills, and generating an answer defeats the purpose. If you absolutely
must, using it as a reference source is permissible. If you get to that point though, it would be more useful to you to
first ask one of the electronics leads who can help guide you to an answer we're looking for, or take a gander at
[cppreference](https://en.cppreference.com/).

# First Steps

## Launching the app

To begin, we will set up the development project and make sure you can build and launch the app. First, on this github
repository there is a green "code" button. Click this, and copy the url shown. This URL is how you can clone this
repository with git.

Next, open CLion, click the button to "Clone Repository", and paste the url you copied. You can optionally choose a
location to place the project. Once this is done, click clone. CLion will begin cloning the repository to your computer
so you can edit the project files. Once the clone has finished, CLion will open the project.

The first thing you will be presented with is a CMake configuration screen. Go ahead and accept all the defaults, as
those will work for us. You should now see the CMake text window pop up at the bottom, where it will show the progress
on setting up the project. This should complete without issue.

Once this is done, you are now ready to compile and run the project. Hit the green play button at the top to start the
build process.

After a few moments, the notepad app will open. As is, this project is a fully functional (albeit simplistic) notetaking
app. You should see a window pop up with some text saying to take some notes, and a textbox that fills the rest of the
screen. This is the project we will be what we are adding on to for the rest of this tutorial.

## The Important Code

Now that you are able to build and launch the project, let's take a look at the code. The important pieces are located
in `src/main.cpp`. Before you look at the code, let's talk a little about some of the external dependencies this project
has:

- Most notably as mentioned above, we use the [Dear ImGui](https://github.com/ocornut/imgui) library for drawing the
  graphics we show to the user. It is
  intended for rapid and prototyping development, due to its simplicity of use and very light overhead. It is a little
  more difficult in terms of stylizing the graphics themselves, but we are more concerned with functionality rather than
  graphic design. See their github page for in-depth documentation, or uncomment the line in the project which says
  `ImGui::ShowDemoWindow()` to also show a detailed demonstration of all the library features.
- In order to use ImGui, however, we require a graphics backend which handles communicating with the operating system
  about things like user input, and window control. For this, we use [GLFW](https://github.com/glfw/glfw). GLFW is a
  little more complicated than
  ImGui, so we will not play around much with it. All you need to know for our purposes is that it creates and manages
  the window itself.
- As part of creating a window for us, GLFW will also handle initializing a low level system library called OpenGL.
  OpenGL provides us an interface to interact with the computer's graphics card, and is the final destination for any
  piece of code we call that has anything to do with drawing stuff on the screen. OpenGL is much more complicated than
  either GLFW or ImGui, so we will not talk much about it either. For us, this is simply the underlying system that
  communicates the drawing information to the GPU for us.

Go ahead and scroll through and read the comments. They explain the purpose of each part, and walk you through the
program design.

## Adding More

Now that you've taken some time to understand the existing code, let's work on adding some new functionality.

Writing notes into a textbox is cool and all, but it would be nice if we could at least save what we've written so we
can recall it later. To accomplish this, we will add two buttons below the text box: One will say "Save", and pressing
it
will save our notes to a file in the same folder as the app executable itself. The other button will say "Load", and
pressing it will load the contents of this file. To accomplis the file handling, we will use the C++ `fstream` library,
which provides some convenient utilities for reading and writing from/to files.

First, we will need to make the fstream functions available by including the appropriate header file. At the top of
`main.cpp`, add a line that says `#include <fstream>`. This indicates to the compiler we want to use the code in the
`<fstream>` header.

To use an fstream, we need to know which direction we are going. In other words, we need to decide if we are reading or
writing to a file. Depending on what we are doing, we will either use an `std::ifstream`, or an `std::ofstream` (i for
input, o for output). In either case, we can very easily open a file for reading/writing by declaring a variable of the
appropriate type. For example, if we want to create a file called `notes` to save our notes into, we can simply write:

```c++
std::ofstream outfile("notes");
```

From there, we have a few different ways to write into the file. We can write one char at a time, or we can write a
whole vector of chars:

```c++
// Write the entire vector at once. The write function takes a pointer to the beginning of the buffer we want to write,
//  and the number of elements we want to write:
outfile.write(&notepad_text[0], notepad_text.size());

// Alternatively, we could write chars one at a time:

/*
for(int i = 0; i < notepad_text.size(); i++) {
    outfile << notepad_text[i];
}
*/
```

To read characters from our `notes` file, we can write:

```c++
// Open the file
std::ifstream infile("notes");

// Empty out our text buffer, so it will overwrite the contents of what was already there
notepad_text.clear();

// Test to make sure we successfully opened it
if(infile.is_open()) {
    char c = 0;
    
    // Loop while there are characters left to read. Place each read character into the variable `c`
    while(infile.get(c)) {
        // Place the latest character into our text buffer
        notepad_text.push_back(c);
    }
}
```

Now that we have our fun file-finagling fully formed and functional, we need to somehow trigger this from our UI. To do
this, we will use the `ImGui::Button` function. This function takes one string parameter, and when it is called ImGui
will draw a button on the next line in our window, with the string parameter as it's display text. For example,
`ImGui::Button("Hello World!")` will display a button that shows "Hello World!" inside. Additionally, this function
returns whether the button was pushed in the last frame. So, we can test the return value of this function to determine
if we need to take some kind of action:

```c++
if(ImGui::Button("...text...")) {
    // Do something awesome
}
```

This sounds just like what we need. Now that we have all the parts, let's combine them. In the draw function after the
comment saying to add more code, let's first add a button for saving our text. You will need to add an if-statement with
and ImGui button as shown above, and use the code for writing our notes to a file. Since we want the notes to be written
when the button is pushed, the file writing code should be inside the if-statement for the button.

Next, add another if-statement/button combo for the load button. This should be outside and after the first
if-statement,
since we want to check the buttons separately. Do something similar to the save button with the if-statement for the
load button and the code to read from the file.

Now, if you run the program, you should see two new buttons below the textbox for saving and loading. Try pushing them
to see if they work. After you push "Save", there should be a file in the same folder as the app executable called
"notes" that contains all the text you wrote. If you restart the program, you should be greeted with a blank notepad,
but after pushing "Load" everything you wrote should come back.

If for some reason it isnt working, try adding some printouts (via `std::cout << ` or `printf`, whichever is more
comfortable for you) to see whats happening, or if you're feeling fancy poke around with the debugger.

If you have tried as hard as you could but you cant figure out why your save/loads are not working, there is a
[solution repository]() which does have the answers. However, try to save this as a last resort, since you won't learn
as much by looking at a solution compared to writing it yourself.

# Adding Even More

Now that we've walked you through adding one new basic feature, it is now your turn to figure out how to add some more,
and customize it to your liking! Think of some more features you might want to see out of your notepad app. Try to add
these in yourself. If needed, you can uncomment the line in the main function that says `ImGui::ShowDemoWindow()` to
display a demo window that shows all the different features ImGui offers. See what's available, and try to add some cool
things!

If you need some ideas, here are a few ranked on difficulty that might be interesting:

### Easy

- Change the color scheme
- Automatically save and load notepad content on program close/start
- Show statistics like how many characters have been typed

### Medium

- Allow choosing the name of the file to save the text into
- Allow having multiple notepads/tabs of text
- Basic text autoformatting (removing extra spaces, capitalizing the first letter of each sentence, removing extra
  newlines, simple things)
- Changing the font
    - The [ImGui](https://github.com/ocornut/imgui) repo has lots of documentation available on fonts!

### Hard

- Setting the icon for the window (in the taskbar)
    - This is not too difficult, but will require an outside library. In particular, using `stbimage` to load the image
      data from a file into memory
- Emoji Support
    - Fortunately, documentation on how to do this with ImGui does exist! It's up to you to find and interpret it
      though :)
- Setting the icon for the executable file itself (Windows OS only)
    - As a hint, google is your best friend :). Doing this is fortunately well documented. If you absolutely require an
      existing codebase to refer to, check out [RCI](https://github.com/liquid-rocketry-illinois/lri-rci-cpp).

### Extra Hard

- Markdown rendering
    - [Markdown](https://www.markdownguide.org/getting-started/) is a way to format text using special
      sequences of characters. In fact, this very document is written in Markdown (click the "Code" button on this file
      in github to see the raw source)! In short, it is a way to format text with things like bolding, italics, and
      headers without needing a fancy word processor. There already exist a few different Markdown renderers for ImGui,
      but using existing code is no fun! The challenge here is to write your own parser and renderer. Supporting the
      full extent of Markdown crosses the line into ULTRA Hard territory, but supporting a subset of some of the simpler
      features like italics and bolding isn't too crazy.
- Images in the editor
    - Support embedding images into the text of the editor
- Allow choosing which file to load or the location to save with a file picker window
    - Doing this from raw OS libraries would be an ULTRA Hard. Given its difficulty, there are fortunately libraries out
      there which simplify the process of opening and using file dialogs. In
      particular, [Native File Dialog](https://github.com/mlabbe/nativefiledialog) works well and is easy to use.

### ULTRA Hard

- Custom window titlebar (also called borderless windows) (Windows OS only): DO NOT ATTEMPT unless you have done
  everything else on this list and are *extremely* board. This one is under ULTRA Hard for a reason: a minimum viable
  example of a borderless ImGui window took the author of this exercise (engine lead Jacob) 3 attempts, close to 20/25
  hours, multiple reference codebases, and an unusual tolerance for Microsoft developer documentation. If you get to the
  point where you really are this bored, talk to Jacob for some resources on how to get started.
