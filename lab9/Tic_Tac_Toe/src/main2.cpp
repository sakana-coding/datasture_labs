#define GL_SILENCE_DEPRECATION

#include "ComputerPlayer.h"
#include "TicTacToeBoard.h"

#include <GLFW/glfw3.h>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/ImageIO.h>
#endif

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace {

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 720;

struct Color {
    float r;
    float g;
    float b;
    float a;
};

struct Button {
    float x;
    float y;
    float w;
    float h;
    std::string text;
    std::string action;
};

enum GameMode {
    HUMAN_VS_COMPUTER = 1,
    COMPUTER_VS_COMPUTER = 2
};

enum MenuStep {
    STEP_SIZE,
    STEP_MODE,
    STEP_FIRST,
    STEP_DIFFICULTY1,
    STEP_DIFFICULTY2,
    STEP_READY
};

void setColor(const Color& color) {
    glColor4f(color.r, color.g, color.b, color.a);
}

void drawRect(float x, float y, float w, float h, const Color& color) {
    setColor(color);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawRectLine(float x, float y, float w, float h, const Color& color, float lineWidth = 2.0f) {
    glLineWidth(lineWidth);
    setColor(color);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

const char** glyphFor(char c) {
    static const char* SPACE[] = {"00000","00000","00000","00000","00000","00000","00000"};
    static const char* A[] = {"01110","10001","10001","11111","10001","10001","10001"};
    static const char* B[] = {"11110","10001","10001","11110","10001","10001","11110"};
    static const char* C[] = {"01111","10000","10000","10000","10000","10000","01111"};
    static const char* D[] = {"11110","10001","10001","10001","10001","10001","11110"};
    static const char* E[] = {"11111","10000","10000","11110","10000","10000","11111"};
    static const char* F[] = {"11111","10000","10000","11110","10000","10000","10000"};
    static const char* G[] = {"01111","10000","10000","10111","10001","10001","01111"};
    static const char* H[] = {"10001","10001","10001","11111","10001","10001","10001"};
    static const char* I[] = {"11111","00100","00100","00100","00100","00100","11111"};
    static const char* J[] = {"00111","00010","00010","00010","10010","10010","01100"};
    static const char* K[] = {"10001","10010","10100","11000","10100","10010","10001"};
    static const char* L[] = {"10000","10000","10000","10000","10000","10000","11111"};
    static const char* M[] = {"10001","11011","10101","10101","10001","10001","10001"};
    static const char* N[] = {"10001","11001","10101","10011","10001","10001","10001"};
    static const char* O[] = {"01110","10001","10001","10001","10001","10001","01110"};
    static const char* P[] = {"11110","10001","10001","11110","10000","10000","10000"};
    static const char* Q[] = {"01110","10001","10001","10001","10101","10010","01101"};
    static const char* R[] = {"11110","10001","10001","11110","10100","10010","10001"};
    static const char* S[] = {"01111","10000","10000","01110","00001","00001","11110"};
    static const char* T[] = {"11111","00100","00100","00100","00100","00100","00100"};
    static const char* U[] = {"10001","10001","10001","10001","10001","10001","01110"};
    static const char* V[] = {"10001","10001","10001","10001","10001","01010","00100"};
    static const char* W[] = {"10001","10001","10001","10101","10101","10101","01010"};
    static const char* X[] = {"10001","10001","01010","00100","01010","10001","10001"};
    static const char* Y[] = {"10001","10001","01010","00100","00100","00100","00100"};
    static const char* Z[] = {"11111","00001","00010","00100","01000","10000","11111"};
    static const char* N0[] = {"01110","10001","10011","10101","11001","10001","01110"};
    static const char* N1[] = {"00100","01100","00100","00100","00100","00100","01110"};
    static const char* N2[] = {"01110","10001","00001","00010","00100","01000","11111"};
    static const char* N3[] = {"11110","00001","00001","01110","00001","00001","11110"};
    static const char* N4[] = {"00010","00110","01010","10010","11111","00010","00010"};
    static const char* N5[] = {"11111","10000","10000","11110","00001","00001","11110"};
    static const char* N6[] = {"00111","01000","10000","11110","10001","10001","01110"};
    static const char* N7[] = {"11111","00001","00010","00100","01000","01000","01000"};
    static const char* N8[] = {"01110","10001","10001","01110","10001","10001","01110"};
    static const char* N9[] = {"01110","10001","10001","01111","00001","00010","11100"};
    static const char* COLON[] = {"00000","00100","00100","00000","00100","00100","00000"};
    static const char* DASH[] = {"00000","00000","00000","11111","00000","00000","00000"};
    static const char* PLUS[] = {"00000","00100","00100","11111","00100","00100","00000"};
    static const char* DOT[] = {"00000","00000","00000","00000","00000","00100","00100"};

    c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    switch (c) {
        case 'A': return A; case 'B': return B; case 'C': return C; case 'D': return D;
        case 'E': return E; case 'F': return F; case 'G': return G; case 'H': return H;
        case 'I': return I; case 'J': return J; case 'K': return K; case 'L': return L;
        case 'M': return M; case 'N': return N; case 'O': return O; case 'P': return P;
        case 'Q': return Q; case 'R': return R; case 'S': return S; case 'T': return T;
        case 'U': return U; case 'V': return V; case 'W': return W; case 'X': return X;
        case 'Y': return Y; case 'Z': return Z;
        case '0': return N0; case '1': return N1; case '2': return N2; case '3': return N3;
        case '4': return N4; case '5': return N5; case '6': return N6; case '7': return N7;
        case '8': return N8; case '9': return N9;
        case ':': return COLON; case '-': return DASH; case '+': return PLUS; case '.': return DOT;
        default: return SPACE;
    }
}

float textWidth(const std::string& text, float scale) {
    return static_cast<float>(text.size()) * 6.0f * scale;
}

float fitTextScale(const std::string& text, float w, float h, float maxScale) {
    if (text.empty()) {
        return maxScale;
    }

    // 字符点阵宽 5，高 7，字符间距按 6 计算。留出左右/上下边距避免贴边。
    float widthScale = (w - 18.0f) / (static_cast<float>(text.size()) * 6.0f);
    float heightScale = (h - 12.0f) / 7.0f;
    float fitted = std::min(maxScale, std::min(widthScale, heightScale));
    return std::max(1.2f, fitted);
}

void drawText(float x, float y, const std::string& text, float scale, const Color& color) {
    setColor(color);
    for (std::size_t idx = 0; idx < text.size(); ++idx) {
        const char** glyph = glyphFor(text[idx]);
        float baseX = x + static_cast<float>(idx) * 6.0f * scale;
        for (int row = 0; row < 7; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (glyph[row][col] == '1') {
                    drawRect(baseX + col * scale, y + row * scale, scale, scale, color);
                }
            }
        }
    }
}

void drawCenteredText(float x, float y, float w, float h,
                      const std::string& text, float scale, const Color& color) {
    float actualScale = fitTextScale(text, w, h, scale);
    float tx = x + (w - textWidth(text, actualScale)) / 2.0f;
    float ty = y + (h - 7.0f * actualScale) / 2.0f;
    drawText(tx, ty, text, actualScale, color);
}

bool contains(const Button& button, double x, double y) {
    return x >= button.x && x <= button.x + button.w
        && y >= button.y && y <= button.y + button.h;
}

}  // namespace

class GuiTicTacToe {
public:
    GuiTicTacToe()
        : window_(NULL),
          windowWidth_(WINDOW_WIDTH),
          windowHeight_(WINDOW_HEIGHT),
          framebufferWidth_(WINDOW_WIDTH),
          framebufferHeight_(WINDOW_HEIGHT),
          backgroundTexture_(0),
          hasBackground_(false),
          inMenu_(true),
          menuStep_(STEP_SIZE),
          boardSize_(3),
          mode_(HUMAN_VS_COMPUTER),
          humanFirst_(false),
          difficulty1_(ComputerPlayer::NORMAL),
          difficulty2_(ComputerPlayer::NORMAL),
          board_(3),
          computer1_(ComputerPlayer::NORMAL),
          computer2_(ComputerPlayer::NORMAL),
          currentComputerRole_(TicTacToeBoard::COMPUTER),
          gameOver_(false),
          gameOverTime_(0.0),
          message_("CONFIGURE GAME") {
    }

    ~GuiTicTacToe() {
        if (backgroundTexture_ != 0) {
            glDeleteTextures(1, &backgroundTexture_);
        }
        if (window_ != NULL) {
            glfwDestroyWindow(window_);
        }
        glfwTerminate();
    }

    bool init() {
        if (!glfwInit()) {
            std::cerr << "Failed to init GLFW.\n";
            return false;
        }

        window_ = glfwCreateWindow(windowWidth_, windowHeight_, "Tic Tac Toe GUI", NULL, NULL);
        if (window_ == NULL) {
            std::cerr << "Failed to create GLFW window.\n";
            return false;
        }

        glfwMakeContextCurrent(window_);
        glfwSwapInterval(1);
        glfwSetWindowUserPointer(window_, this);
        glfwSetMouseButtonCallback(window_, mouseCallback);
        glfwSetKeyCallback(window_, keyCallback);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        loadBackgroundTexture("assets/background.png");
        return true;
    }

    void run() {
        while (!glfwWindowShouldClose(window_)) {
            // 鼠标坐标是窗口逻辑坐标；Retina 屏幕 framebuffer 可能是 2 倍。
            // 所以绘制投影使用 windowWidth_/windowHeight_，viewport 使用 framebuffer。
            glfwGetWindowSize(window_, &windowWidth_, &windowHeight_);
            glfwGetFramebufferSize(window_, &framebufferWidth_, &framebufferHeight_);
            render();
            glfwSwapBuffers(window_);
            glfwPollEvents();
        }
    }

private:
    GLFWwindow* window_;
    int windowWidth_;
    int windowHeight_;
    int framebufferWidth_;
    int framebufferHeight_;
    GLuint backgroundTexture_;
    bool hasBackground_;
    bool inMenu_;
    MenuStep menuStep_;
    int boardSize_;
    GameMode mode_;
    bool humanFirst_;
    ComputerPlayer::Difficulty difficulty1_;
    ComputerPlayer::Difficulty difficulty2_;
    TicTacToeBoard board_;
    ComputerPlayer computer1_;
    ComputerPlayer computer2_;
    int currentComputerRole_;
    bool gameOver_;
    double gameOverTime_;
    std::string message_;
    std::vector<Button> buttons_;

    static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
        (void)mods;
        if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) {
            return;
        }

        GuiTicTacToe* app = static_cast<GuiTicTacToe*>(glfwGetWindowUserPointer(window));
        if (app == NULL) {
            return;
        }

        double x = 0.0;
        double y = 0.0;
        glfwGetCursorPos(window, &x, &y);
        app->handleClick(x, y);
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        (void)scancode;
        (void)mods;
        if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    void setupProjection() const {
        glViewport(0, 0, framebufferWidth_, framebufferHeight_);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, windowWidth_, windowHeight_, 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void render() {
        if (!inMenu_ && gameOver_ && glfwGetTime() - gameOverTime_ >= 2.0) {
            returnToMenu();
        }

        setupProjection();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawBackground();
        buttons_.clear();

        if (inMenu_) {
            drawMenu();
        } else {
            drawGame();
        }
    }

    void drawBackground() const {
        if (hasBackground_) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, backgroundTexture_);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(static_cast<float>(windowWidth_), 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(static_cast<float>(windowWidth_), static_cast<float>(windowHeight_));
            glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, static_cast<float>(windowHeight_));
            glEnd();
            glDisable(GL_TEXTURE_2D);
            return;
        }

        glBegin(GL_QUADS);
        glColor3f(0.08f, 0.14f, 0.35f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(static_cast<float>(windowWidth_), 0.0f);
        glColor3f(0.95f, 0.70f, 0.34f);
        glVertex2f(static_cast<float>(windowWidth_), static_cast<float>(windowHeight_) * 0.48f);
        glVertex2f(0.0f, static_cast<float>(windowHeight_) * 0.48f);
        glColor3f(0.05f, 0.42f, 0.55f);
        glVertex2f(0.0f, static_cast<float>(windowHeight_) * 0.40f);
        glVertex2f(static_cast<float>(windowWidth_), static_cast<float>(windowHeight_) * 0.40f);
        glColor3f(0.10f, 0.68f, 0.72f);
        glVertex2f(static_cast<float>(windowWidth_), static_cast<float>(windowHeight_) * 0.72f);
        glVertex2f(0.0f, static_cast<float>(windowHeight_) * 0.72f);
        glColor3f(0.93f, 0.66f, 0.38f);
        glVertex2f(0.0f, static_cast<float>(windowHeight_) * 0.68f);
        glVertex2f(static_cast<float>(windowWidth_), static_cast<float>(windowHeight_) * 0.68f);
        glColor3f(0.98f, 0.80f, 0.50f);
        glVertex2f(static_cast<float>(windowWidth_), static_cast<float>(windowHeight_));
        glVertex2f(0.0f, static_cast<float>(windowHeight_));
        glEnd();
    }

    void drawTitle(const std::string& subtitle) {
        float titleW = std::min(720.0f, static_cast<float>(windowWidth_) - 80.0f);
        float titleX = (windowWidth_ - titleW) / 2.0f;
        drawRect(titleX, 28.0f, titleW, 58.0f, Color{0.0f, 0.95f, 0.05f, 0.88f});
        drawCenteredText(titleX, 28.0f, titleW, 58.0f, "TIC TAC TOE GAME", 6.0f,
                         Color{0.05f, 0.38f, 1.0f, 1.0f});
        if (!subtitle.empty()) {
            drawCenteredText(0.0f, 94.0f, static_cast<float>(windowWidth_), 30.0f,
                             subtitle, 3.0f, Color{1.0f, 1.0f, 1.0f, 1.0f});
        }
    }

    void drawMenu() {
        drawTitle("SETUP STEP BY STEP");

        float panelW = std::min(760.0f, static_cast<float>(windowWidth_) - 80.0f);
        float panelX = (windowWidth_ - panelW) / 2.0f;
        float panelH = std::min(430.0f, static_cast<float>(windowHeight_) - 220.0f);
        panelH = std::max(380.0f, panelH);
        float panelY = std::max(125.0f, (windowHeight_ - panelH) / 2.0f + 12.0f);
        drawRect(panelX, panelY, panelW, panelH, Color{0.0f, 0.0f, 0.0f, 0.46f});
        drawRectLine(panelX, panelY, panelW, panelH, Color{0.50f, 0.50f, 1.0f, 1.0f}, 4.0f);

        float centerX = panelX + panelW / 2.0f;
        float buttonW = std::min(360.0f, panelW - 160.0f);
        float buttonX = centerX - buttonW / 2.0f;
        float titleY = panelY + 32.0f;
        float choiceY = panelY + panelH * 0.40f;
        float infoY = panelY + panelH * 0.61f;
        float navY = std::min(panelY + panelH + 24.0f, static_cast<float>(windowHeight_) - 78.0f);

        if (menuStep_ == STEP_SIZE) {
            drawCenteredText(panelX, titleY, panelW, 44.0f, "STEP 1 BOARD SIZE", 4.0f,
                             Color{1.0f, 1.0f, 1.0f, 1.0f});
            addButton(centerX - 165.0f, choiceY, 90.0f, 58.0f, "-", "SIZE_MINUS", false);
            addButton(centerX - 50.0f, choiceY, 100.0f, 58.0f, std::to_string(boardSize_), "NONE", true);
            addButton(centerX + 75.0f, choiceY, 90.0f, 58.0f, "+", "SIZE_PLUS", false);
            drawCenteredText(panelX, infoY, panelW, 30.0f, "SIZE RANGE 3 TO 9", 3.0f,
                             Color{1.0f, 1.0f, 1.0f, 1.0f});
            addButton(centerX - 90.0f, navY, 180.0f, 56.0f, "NEXT", "MENU_NEXT", false);
        } else if (menuStep_ == STEP_MODE) {
            drawCenteredText(panelX, titleY, panelW, 44.0f, "STEP 2 GAME MODE", 4.0f,
                             Color{1.0f, 1.0f, 1.0f, 1.0f});
            addButton(buttonX, choiceY - 35.0f, buttonW, 58.0f, "HUMAN VS PC", "MODE_HVC",
                      mode_ == HUMAN_VS_COMPUTER);
            addButton(buttonX, choiceY + 45.0f, buttonW, 58.0f, "PC VS PC", "MODE_CVC",
                      mode_ == COMPUTER_VS_COMPUTER);
            drawMenuNav(true, true, navY);
        } else if (menuStep_ == STEP_FIRST) {
            drawCenteredText(panelX, titleY, panelW, 44.0f, "STEP 3 FIRST PLAYER", 4.0f,
                             Color{1.0f, 1.0f, 1.0f, 1.0f});
            addButton(buttonX, choiceY - 35.0f, buttonW, 58.0f, "HUMAN FIRST", "FIRST_HUMAN", humanFirst_);
            addButton(buttonX, choiceY + 45.0f, buttonW, 58.0f, "PC FIRST", "FIRST_COMPUTER", !humanFirst_);
            drawMenuNav(true, true, navY);
        } else if (menuStep_ == STEP_DIFFICULTY1) {
            drawCenteredText(panelX, titleY, panelW, 44.0f, "STEP 4 PC1 LEVEL", 4.0f,
                             Color{1.0f, 1.0f, 1.0f, 1.0f});
            drawDifficultyChoices(buttonX, choiceY - 64.0f, buttonW, "D1");
            drawMenuNav(true, true, navY);
        } else if (menuStep_ == STEP_DIFFICULTY2) {
            drawCenteredText(panelX, titleY, panelW, 44.0f, "STEP 5 PC2 LEVEL", 4.0f,
                             Color{1.0f, 1.0f, 1.0f, 1.0f});
            drawDifficultyChoices(buttonX, choiceY - 64.0f, buttonW, "D2");
            drawMenuNav(true, true, navY);
        } else {
            drawCenteredText(panelX, titleY, panelW, 44.0f, "READY", 4.5f,
                             Color{1.0f, 1.0f, 1.0f, 1.0f});
            drawCenteredText(panelX, choiceY - 56.0f, panelW, 32.0f,
                             "SIZE " + std::to_string(boardSize_), 3.2f,
                             Color{1.0f, 1.0f, 1.0f, 1.0f});
            drawCenteredText(panelX, choiceY - 16.0f, panelW, 32.0f,
                             mode_ == HUMAN_VS_COMPUTER ? "MODE HUMAN VS PC" : "MODE PC VS PC",
                             3.2f, Color{1.0f, 1.0f, 1.0f, 1.0f});
            if (mode_ == HUMAN_VS_COMPUTER) {
                drawCenteredText(panelX, choiceY + 24.0f, panelW, 32.0f,
                                 humanFirst_ ? "FIRST HUMAN" : "FIRST PC",
                                 3.2f, Color{1.0f, 1.0f, 1.0f, 1.0f});
            }
            drawCenteredText(panelX, choiceY + 64.0f, panelW, 32.0f,
                             "PC1 " + difficultyName(difficulty1_), 3.2f,
                             Color{1.0f, 1.0f, 1.0f, 1.0f});
            if (mode_ == COMPUTER_VS_COMPUTER) {
                drawCenteredText(panelX, choiceY + 104.0f, panelW, 32.0f,
                                 "PC2 " + difficultyName(difficulty2_), 3.2f,
                                 Color{1.0f, 1.0f, 1.0f, 1.0f});
            }
            addButton(centerX - 230.0f, navY, 180.0f, 58.0f, "BACK", "MENU_BACK", false);
            addButton(centerX + 50.0f, navY, 180.0f, 58.0f, "START", "START", false);
        }
    }

    void drawMenuNav(bool showBack, bool showNext, float y) {
        float centerX = windowWidth_ / 2.0f;
        if (showBack) {
            addButton(centerX - 230.0f, y, 180.0f, 58.0f, "BACK", "MENU_BACK", false);
        }
        if (showNext) {
            addButton(centerX + 50.0f, y, 180.0f, 58.0f, "NEXT", "MENU_NEXT", false);
        }
    }

    void drawDifficultyChoices(float x, float y, float w, const std::string& prefix) {
        ComputerPlayer::Difficulty selected =
            (prefix == "D1") ? difficulty1_ : difficulty2_;
        addButton(x, y, w, 54.0f, "EASY", prefix + "_EASY", selected == ComputerPlayer::EASY);
        addButton(x, y + 68.0f, w, 54.0f, "NORMAL", prefix + "_NORMAL",
                  selected == ComputerPlayer::NORMAL);
        addButton(x, y + 136.0f, w, 54.0f, "HARD", prefix + "_HARD",
                  selected == ComputerPlayer::HARD);
    }

    void drawGame() {
        drawTitle(message_);

        float maxBoard = std::min(static_cast<float>(windowWidth_) * 0.62f,
                                  static_cast<float>(windowHeight_) * 0.50f);
        float boardPixels = std::min(430.0f, maxBoard);
        float bx = (windowWidth_ - boardPixels) / 2.0f;
        float by = std::max(135.0f, static_cast<float>(windowHeight_) * 0.20f);
        float maxBy = static_cast<float>(windowHeight_) - boardPixels - 145.0f;
        if (mode_ == COMPUTER_VS_COMPUTER && !gameOver_) {
            maxBy = static_cast<float>(windowHeight_) - boardPixels - 205.0f;
        }
        by = std::min(by, std::max(120.0f, maxBy));
        float gap = std::max(4.0f, 10.0f - board_.size() * 0.5f);
        float cell = (boardPixels - gap * (board_.size() + 1)) / board_.size();

        drawRect(bx, by, boardPixels, boardPixels, Color{0.45f, 0.45f, 1.0f, 0.95f});
        for (int r = 0; r < board_.size(); ++r) {
            for (int c = 0; c < board_.size(); ++c) {
                float x = bx + gap + c * (cell + gap);
                float y = by + gap + r * (cell + gap);
                drawRect(x, y, cell, cell, Color{0.0f, 0.95f, 0.05f, 1.0f});
                drawRectLine(x, y, cell, cell, Color{0.15f, 0.15f, 0.15f, 0.55f}, 1.0f);

                int value = board_.getCell(r, c);
                if (value == TicTacToeBoard::COMPUTER) {
                    drawX(x, y, cell);
                } else if (value == TicTacToeBoard::PLAYER) {
                    drawO(x, y, cell);
                }
            }
        }

        float restartY = std::min(by + boardPixels + 28.0f,
                                  static_cast<float>(windowHeight_) - 130.0f);
        if (mode_ == COMPUTER_VS_COMPUTER && !gameOver_) {
            restartY = std::min(by + boardPixels + 18.0f,
                                static_cast<float>(windowHeight_) - 150.0f);
        }
        addButton((windowWidth_ - 220.0f) / 2.0f, restartY,
                  220.0f, 54.0f, "RESTART", "RESTART", false);

        if (mode_ == COMPUTER_VS_COMPUTER && !gameOver_) {
            float nextY = std::min(restartY + 66.0f,
                                   static_cast<float>(windowHeight_) - 58.0f);
            addButton((windowWidth_ - 180.0f) / 2.0f, nextY,
                      180.0f, 46.0f, "NEXT", "NEXT", false);
        }
    }

    void addButton(float x, float y, float w, float h,
                   const std::string& text, const std::string& action, bool selected) {
        Color fill = selected ? Color{1.0f, 0.85f, 0.15f, 0.92f}
                              : Color{0.55f, 0.55f, 1.0f, 0.90f};
        Color border = selected ? Color{1.0f, 0.2f, 0.2f, 1.0f}
                                : Color{0.05f, 0.05f, 0.30f, 1.0f};
        drawRect(x, y, w, h, fill);
        drawRectLine(x, y, w, h, border, 3.0f);
        float scale = text.size() > 16 ? 2.2f : 3.0f;
        drawCenteredText(x, y, w, h, text, scale, Color{0.0f, 0.0f, 0.0f, 1.0f});
        if (action != "NONE") {
            buttons_.push_back(Button{x, y, w, h, text, action});
        }
    }

    void drawX(float x, float y, float cell) const {
        glLineWidth(std::max(5.0f, cell * 0.12f));
        setColor(Color{0.0f, 0.0f, 0.0f, 1.0f});
        float pad = cell * 0.22f;
        glBegin(GL_LINES);
        glVertex2f(x + pad, y + pad);
        glVertex2f(x + cell - pad, y + cell - pad);
        glVertex2f(x + cell - pad, y + pad);
        glVertex2f(x + pad, y + cell - pad);
        glEnd();
    }

    void drawO(float x, float y, float cell) const {
        glLineWidth(std::max(5.0f, cell * 0.10f));
        setColor(Color{1.0f, 1.0f, 1.0f, 1.0f});
        float cx = x + cell / 2.0f;
        float cy = y + cell / 2.0f;
        float radius = cell * 0.30f;
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 64; ++i) {
            float a = static_cast<float>(i) * 6.2831853f / 64.0f;
            glVertex2f(cx + std::cos(a) * radius, cy + std::sin(a) * radius);
        }
        glEnd();
    }

    void handleClick(double x, double y) {
        for (std::size_t i = 0; i < buttons_.size(); ++i) {
            if (contains(buttons_[i], x, y)) {
                handleAction(buttons_[i].action);
                return;
            }
        }

        if (!inMenu_ && mode_ == HUMAN_VS_COMPUTER && !gameOver_) {
            handleBoardClick(x, y);
        }
    }

    void handleAction(const std::string& action) {
        if (action == "SIZE_MINUS") {
            boardSize_ = std::max(TicTacToeBoard::MIN_SIZE, boardSize_ - 1);
        } else if (action == "SIZE_PLUS") {
            boardSize_ = std::min(TicTacToeBoard::MAX_SIZE, boardSize_ + 1);
        } else if (action == "MENU_NEXT") {
            menuNext();
        } else if (action == "MENU_BACK") {
            menuBack();
        } else if (action == "MODE_HVC") {
            mode_ = HUMAN_VS_COMPUTER;
            menuNext();
        } else if (action == "MODE_CVC") {
            mode_ = COMPUTER_VS_COMPUTER;
            menuNext();
        } else if (action == "FIRST_HUMAN") {
            humanFirst_ = true;
            menuNext();
        } else if (action == "FIRST_COMPUTER") {
            humanFirst_ = false;
            menuNext();
        } else if (action == "DIFF1") {
            difficulty1_ = nextDifficulty(difficulty1_);
        } else if (action == "DIFF2") {
            difficulty2_ = nextDifficulty(difficulty2_);
        } else if (action == "D1_EASY") {
            difficulty1_ = ComputerPlayer::EASY;
            menuNext();
        } else if (action == "D1_NORMAL") {
            difficulty1_ = ComputerPlayer::NORMAL;
            menuNext();
        } else if (action == "D1_HARD") {
            difficulty1_ = ComputerPlayer::HARD;
            menuNext();
        } else if (action == "D2_EASY") {
            difficulty2_ = ComputerPlayer::EASY;
            menuNext();
        } else if (action == "D2_NORMAL") {
            difficulty2_ = ComputerPlayer::NORMAL;
            menuNext();
        } else if (action == "D2_HARD") {
            difficulty2_ = ComputerPlayer::HARD;
            menuNext();
        } else if (action == "START") {
            startGame();
        } else if (action == "RESTART") {
            startGame();
        } else if (action == "MENU") {
            // 防御性保留：正常游戏界面不再绘制 MENU 按钮。
            returnToMenu();
        } else if (action == "NEXT") {
            computerVsComputerStep();
        }
    }

    void returnToMenu() {
        inMenu_ = true;
        menuStep_ = STEP_SIZE;
        gameOver_ = false;
        gameOverTime_ = 0.0;
        message_ = "CONFIGURE GAME";
    }

    void menuNext() {
        if (menuStep_ == STEP_SIZE) {
            menuStep_ = STEP_MODE;
        } else if (menuStep_ == STEP_MODE) {
            menuStep_ = (mode_ == HUMAN_VS_COMPUTER) ? STEP_FIRST : STEP_DIFFICULTY1;
        } else if (menuStep_ == STEP_FIRST) {
            menuStep_ = STEP_DIFFICULTY1;
        } else if (menuStep_ == STEP_DIFFICULTY1) {
            menuStep_ = (mode_ == COMPUTER_VS_COMPUTER) ? STEP_DIFFICULTY2 : STEP_READY;
        } else if (menuStep_ == STEP_DIFFICULTY2) {
            menuStep_ = STEP_READY;
        }
    }

    void menuBack() {
        if (menuStep_ == STEP_READY) {
            menuStep_ = (mode_ == COMPUTER_VS_COMPUTER) ? STEP_DIFFICULTY2 : STEP_DIFFICULTY1;
        } else if (menuStep_ == STEP_DIFFICULTY2) {
            menuStep_ = STEP_DIFFICULTY1;
        } else if (menuStep_ == STEP_DIFFICULTY1) {
            menuStep_ = (mode_ == HUMAN_VS_COMPUTER) ? STEP_FIRST : STEP_MODE;
        } else if (menuStep_ == STEP_FIRST) {
            menuStep_ = STEP_MODE;
        } else if (menuStep_ == STEP_MODE) {
            menuStep_ = STEP_SIZE;
        }
    }

    void handleBoardClick(double mx, double my) {
        float maxBoard = std::min(static_cast<float>(windowWidth_) * 0.62f,
                                  static_cast<float>(windowHeight_) * 0.56f);
        float boardPixels = std::min(430.0f, maxBoard);
        float bx = (windowWidth_ - boardPixels) / 2.0f;
        float by = 170.0f;
        float gap = std::max(4.0f, 10.0f - board_.size() * 0.5f);
        float cell = (boardPixels - gap * (board_.size() + 1)) / board_.size();

        if (mx < bx || mx > bx + boardPixels || my < by || my > by + boardPixels) {
            return;
        }

        for (int r = 0; r < board_.size(); ++r) {
            for (int c = 0; c < board_.size(); ++c) {
                float x = bx + gap + c * (cell + gap);
                float y = by + gap + r * (cell + gap);
                if (mx >= x && mx <= x + cell && my >= y && my <= y + cell) {
                    if (board_.place(r, c, TicTacToeBoard::PLAYER)) {
                        updateGameOver();
                        if (!gameOver_) {
                            computerMove(computer1_, TicTacToeBoard::COMPUTER,
                                         TicTacToeBoard::PLAYER, "COMPUTER THINKING");
                        }
                    }
                    return;
                }
            }
        }
    }

    void startGame() {
        board_.reset(boardSize_);
        computer1_.setDifficulty(difficulty1_);
        computer2_.setDifficulty(difficulty2_);
        inMenu_ = false;
        gameOver_ = false;
        gameOverTime_ = 0.0;

        if (mode_ == HUMAN_VS_COMPUTER) {
            if (humanFirst_) {
                message_ = "YOUR TURN";
            } else {
                computerMove(computer1_, TicTacToeBoard::COMPUTER,
                             TicTacToeBoard::PLAYER, "YOUR TURN");
            }
        } else {
            currentComputerRole_ = TicTacToeBoard::COMPUTER;
            message_ = "COMPUTER1 TURN - CLICK NEXT";
        }
    }

    void computerVsComputerStep() {
        if (gameOver_) {
            return;
        }

        if (currentComputerRole_ == TicTacToeBoard::COMPUTER) {
            computerMove(computer1_, TicTacToeBoard::COMPUTER,
                         TicTacToeBoard::PLAYER, "COMPUTER2 TURN - CLICK NEXT");
            currentComputerRole_ = TicTacToeBoard::PLAYER;
        } else {
            computerMove(computer2_, TicTacToeBoard::PLAYER,
                         TicTacToeBoard::COMPUTER, "COMPUTER1 TURN - CLICK NEXT");
            currentComputerRole_ = TicTacToeBoard::COMPUTER;
        }
    }

    void computerMove(ComputerPlayer& computer, int myRole,
                      int opponentRole, const std::string& nextMessage) {
        std::pair<int, int> move = computer.chooseMove(board_, myRole, opponentRole);
        if (move.first != -1) {
            board_.place(move.first, move.second, myRole);
        }
        if (!updateGameOver()) {
            message_ = nextMessage;
        }
    }

    bool updateGameOver() {
        if (board_.hasWon(TicTacToeBoard::COMPUTER)) {
            markGameOver();
            message_ = (mode_ == COMPUTER_VS_COMPUTER) ? "COMPUTER1 WIN" : "COMPUTER WIN";
            return true;
        }
        if (board_.hasWon(TicTacToeBoard::PLAYER)) {
            markGameOver();
            message_ = (mode_ == COMPUTER_VS_COMPUTER) ? "COMPUTER2 WIN" : "YOU WIN";
            return true;
        }
        if (board_.isFull()) {
            markGameOver();
            message_ = "DRAW";
            return true;
        }
        return false;
    }

    void markGameOver() {
        if (!gameOver_) {
            gameOver_ = true;
            gameOverTime_ = glfwGetTime();
        }
    }

    static ComputerPlayer::Difficulty nextDifficulty(ComputerPlayer::Difficulty difficulty) {
        if (difficulty == ComputerPlayer::EASY) {
            return ComputerPlayer::NORMAL;
        }
        if (difficulty == ComputerPlayer::NORMAL) {
            return ComputerPlayer::HARD;
        }
        return ComputerPlayer::EASY;
    }

    static std::string difficultyName(ComputerPlayer::Difficulty difficulty) {
        if (difficulty == ComputerPlayer::EASY) {
            return "EASY";
        }
        if (difficulty == ComputerPlayer::HARD) {
            return "HARD";
        }
        return "NORMAL";
    }

    bool loadBackgroundTexture(const std::string& path) {
#ifdef __APPLE__
        CFStringRef cfPath = CFStringCreateWithCString(NULL, path.c_str(), kCFStringEncodingUTF8);
        if (cfPath == NULL) {
            return false;
        }
        CFURLRef url = CFURLCreateWithFileSystemPath(NULL, cfPath, kCFURLPOSIXPathStyle, false);
        CFRelease(cfPath);
        if (url == NULL) {
            return false;
        }

        CGImageSourceRef source = CGImageSourceCreateWithURL(url, NULL);
        CFRelease(url);
        if (source == NULL) {
            return false;
        }

        CGImageRef image = CGImageSourceCreateImageAtIndex(source, 0, NULL);
        CFRelease(source);
        if (image == NULL) {
            return false;
        }

        std::size_t w = CGImageGetWidth(image);
        std::size_t h = CGImageGetHeight(image);
        std::vector<unsigned char> pixels(w * h * 4, 0);

        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGContextRef context = CGBitmapContextCreate(&pixels[0], w, h, 8, w * 4,
                                                     colorSpace,
                                                     kCGImageAlphaPremultipliedLast
                                                     | kCGBitmapByteOrder32Big);
        CGColorSpaceRelease(colorSpace);
        if (context == NULL) {
            CGImageRelease(image);
            return false;
        }

        CGContextClearRect(context, CGRectMake(0, 0, w, h));
        CGContextDrawImage(context, CGRectMake(0, 0, w, h), image);
        CGContextRelease(context);
        CGImageRelease(image);

        glGenTextures(1, &backgroundTexture_);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     static_cast<GLsizei>(w), static_cast<GLsizei>(h),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
        hasBackground_ = true;
        return true;
#else
        (void)path;
        return false;
#endif
    }
};

int main() {
    GuiTicTacToe app;
    if (!app.init()) {
        return 1;
    }
    app.run();
    return 0;
}
