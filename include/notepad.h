#ifndef NOTEPAD_APP_NOTEPAD_H
#define NOTEPAD_APP_NOTEPAD_H

#include <vector>

extern std::vector<char> notepad_text;

void draw();
int resizeCallback(ImGuiInputTextCallbackData* data);

#endif // NOTEPAD_APP_NOTEPAD_H
