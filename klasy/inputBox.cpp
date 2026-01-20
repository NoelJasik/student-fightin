//
// Created by mateu on 20.01.2026.
//

#include "../headers/inputBox.h"
#include "../headers/TextRenderer.h"

// proste rounded rect
static void drawBackground(SDL_Renderer* renderer, const SDL_Rect& r)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 220);
    SDL_RenderFillRect(renderer, &r);
}

InputBox::InputBox(TextRenderer* textRenderer)
    : textRenderer(textRenderer)
{
}

void InputBox::open(int px, int py, int type)
{
    active = true;
    confirmed = false;
    text.clear();

    x = px;
    y = py;
    contextType = type;

    SDL_StartTextInput();
}

void InputBox::handleEvent(const SDL_Event& e)
{
    if (!active) return;

    if (e.type == SDL_TEXTINPUT)
    {
        int w, h;
        textRenderer->measure(text + e.text.text + "_", w, h);

        const int MAX_TEXT_WIDTH = 180; // dopasuj do szerokości boxa

        if (w < MAX_TEXT_WIDTH)
            text += e.text.text;
    }

    if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_BACKSPACE && !text.empty())
            text.pop_back();

        if (e.key.keysym.sym == SDLK_RETURN)
        {
            confirmed = true;
            active = false;
            SDL_StopTextInput();
        }

        if (e.key.keysym.sym == SDLK_ESCAPE)
        {
            active = false;
            text.clear();
            SDL_StopTextInput();
        }
    }
}

void InputBox::render(SDL_Renderer* renderer)
{
    if (!active) return;

    SDL_Rect box {
        1280 / 2 - 200,
        720  / 2 - 60,
        200,
        100
    };

    drawBackground(renderer, box);

    textRenderer->render(
        "Podaj nazwe:",
        box.x + 15,
        box.y + 15
    );

    textRenderer->render(
        text + "_",
        box.x + 15,
        box.y + 55
    );
}

bool InputBox::isActive() const
{
    return active;
}

bool InputBox::isConfirmed() const
{
    return confirmed;
}

std::string InputBox::getText() const
{
    return text.empty() ? "Object" : text;
}

int InputBox::getX() const { return x; }
int InputBox::getY() const { return y; }
int InputBox::getContextType() const { return contextType; }

void InputBox::reset()
{
    confirmed = false;
}