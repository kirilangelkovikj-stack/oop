#include "raylib.h"
#include <cstdlib>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <vector>
#include <string>
#include <fstream>

const int FONT_SIZE = 20;
const int LINE_HEIGHT = 24;
const int GUTTER = 50;

int main() {
    InitWindow(1000, 700, "Raygui C++ Notepad");
    SetTargetFPS(60);

    GuiLoadStyleDefault();

    std::vector<std::string> lines(1, "");
    int row = 0, col = 0;
    int scroll = 0;
    std::string filename = "notes.txt";
    std::string statusMessage = "Ready";

    while (!WindowShouldClose()) {
        int ch;
        while ((ch = GetCharPressed()) > 0) {
            if (ch >= 32 && ch <= 126) {
                lines[row].insert(col, 1, (char)ch);
                col++;
            }
        }

        if (IsKeyPressed(KEY_ENTER)) {
            std::string rest = lines[row].substr(col);
            lines[row].erase(col);
            lines.insert(lines.begin() + row + 1, rest);
            row++;
            col = 0;
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (col > 0) {
                lines[row].erase(col - 1, 1);
                col--;
            } else if (row > 0) {
                col = lines[row - 1].size();
                lines[row - 1] += lines[row];
                lines.erase(lines.begin() + row);
                row--;
            }
        }

        if (IsKeyPressed(KEY_LEFT) && col > 0) col--;
        if (IsKeyPressed(KEY_RIGHT) && col < (int)lines[row].size()) col++;
        if (IsKeyPressed(KEY_UP) && row > 0) {
            row--;
            if (col > (int)lines[row].size()) col = lines[row].size();
        }
        if (IsKeyPressed(KEY_DOWN) && row < (int)lines.size() - 1) {
            row++;
            if (col > (int)lines[row].size()) col = lines[row].size();
        }

        scroll -= (int)GetMouseWheelMove() * 3;
        if (scroll < 0) scroll = 0;

        bool triggerSave = (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S));
        bool triggerOpen = (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_O));

        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        int visibleStart = scroll;
        for (int i = visibleStart; i < (int)lines.size(); i++) {
            int y = 50 + (i - visibleStart) * LINE_HEIGHT;
            if (y > GetScreenHeight() - 40) break;

            DrawText(TextFormat("%d", i + 1), 10, y, FONT_SIZE, GRAY);  
            DrawText(lines[i].c_str(), GUTTER, y, FONT_SIZE, BLACK);
        }

        int cx = GUTTER + MeasureText(lines[row].substr(0, col).c_str(), FONT_SIZE);
        int cy = 50 + (row - visibleStart) * LINE_HEIGHT;
        if (cy >= 50 && cy < GetScreenHeight() - 40) {
            if (((int)(GetTime() * 2)) % 2 == 0)
                DrawRectangle(cx, cy, 2, LINE_HEIGHT, WHITE);
        }

        GuiPanel(Rectangle{ 0, 0, (float)GetScreenWidth(), 40 }, NULL);

        if (GuiButton(Rectangle{ 10, 5, 80, 30 }, "#02# Open") || triggerOpen) {
            std::ifstream in(filename);
            if (in) {
                lines.clear();
                std::string s;
                while (std::getline(in, s)) lines.push_back(s);
                if (lines.empty()) lines.push_back("");
                row = 0; col = 0;
                statusMessage = "File opened successfully!";
            } else {
                statusMessage = "Error: File not found!";
            }
        }

        if (GuiButton(Rectangle{ 100, 5, 80, 30 }, "#01# Save") || triggerSave) {
            std::ofstream out(filename);
            for (size_t i = 0; i < lines.size(); i++) {
                out << lines[i];
                if (i + 1 < lines.size()) out << "\n";
            }
            statusMessage = "Saved to " + filename;
        }

        GuiLabel(Rectangle{ 200, 5, 200, 30 }, TextFormat("Current file: %s", filename.c_str()));

        int statusBarY = GetScreenHeight() - 30;
        GuiStatusBar(Rectangle{ 0, (float)statusBarY, (float)GetScreenWidth(), 30 }, NULL);

        DrawText(TextFormat("Ln %d  |  Col %d  |  Lines %d", row + 1, col + 1, (int)lines.size()), 10, statusBarY + 6, 15, DARKGRAY);
        DrawText(statusMessage.c_str(), GetScreenWidth() - MeasureText(statusMessage.c_str(), 15) - 20, statusBarY + 6, 15, MAROON);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}