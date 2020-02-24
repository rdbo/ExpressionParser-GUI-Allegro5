#include "expr_parser.h"
#include "roboto_regular.h"

void ImGuiSetupStyle();
void GetDisplayWindow();
bool resized = true;

double PMath::root(double num, double rt)
{
    return pow(num, 1 / rt);
}

void ExpressionParser::Init()
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    display = al_create_display(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    al_set_window_title(display, "ExpressionParser-GUI");
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_set_window_constraints(display, MIN_WIDTH, MIN_HEIGHT, MAX_WIDTH, MAX_HEIGHT);
    al_apply_window_constraints(display, true);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFontConfig font_cfg;
	font_cfg.FontDataOwnedByAtlas = false;
	roboto = io.Fonts->AddFontFromMemoryTTF(roboto_regular, sizeof(roboto_regular), FONT_SIZE, &font_cfg);
	roboto2 = io.Fonts->AddFontFromMemoryTTF(roboto_regular, sizeof(roboto_regular), FONT_SIZE*2, &font_cfg);
    ImGui_ImplAllegro5_Init(display);
    ExpressionParser::Run();
}

void ExpressionParser::Run()
{
    ImGuiSetupStyle();
    bool running = true;
    while(running)
    {
        ALLEGRO_EVENT ev;
        while (al_get_next_event(queue, &ev))
        {
            ImGui_ImplAllegro5_ProcessEvent(&ev);
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                running = false;
            if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            {
                ImGui_ImplAllegro5_InvalidateDeviceObjects();
                al_acknowledge_resize(display);
                ImGui_ImplAllegro5_CreateDeviceObjects();
                resized = true;
            }
        }

        ImGui_ImplAllegro5_NewFrame();
        ImGui::NewFrame();

        ExpressionParser::AdjustWindow();
        ExpressionParser::Draw();
        
        ImGui::Render();
        al_clear_to_color(al_map_rgba_f(COLOR_CLEAR_R, COLOR_CLEAR_G, COLOR_CLEAR_B, COLOR_CLEAR_A));
        ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
        al_flip_display();
    }

    ExpressionParser::Shutdown();
}

void ExpressionParser::AdjustWindow()
{
    if(resized)
    {
        ImVec2 wpos = ImGui::GetWindowPos();
        ImVec2 wsz = ImGui::GetWindowSize();
        GetDisplayWindow();
        if(wpos.x != wrect.x || wpos.y != wrect.y)
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
        }

        if(wsz.x != wrect.w || wsz.y != wrect.h)
        {
            ImGui::SetNextWindowSize(ImVec2(wrect.w, wrect.h));
        }
        resized = false;
    }
}

void ExpressionParser::Shutdown()
{
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_event_queue(queue);
    al_destroy_display(display);
}

void ImGuiSetupStyle()
{
    wf |= ImGuiWindowFlags_NoTitleBar;
	wf |= ImGuiWindowFlags_NoMove;
	wf |= ImGuiWindowFlags_NoResize;
	wf |= ImGuiWindowFlags_NoCollapse;
	wf |= ImGuiWindowFlags_NoSavedSettings;
	wf |= ImGuiWindowFlags_NoBackground;
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;
	style.WindowRounding = 0.f;
	colors[ImGuiCol_Text] = COLOR_WHITE;
	colors[ImGuiCol_FrameBg] =
    colors[ImGuiCol_FrameBgHovered] =
    colors[ImGuiCol_FrameBgActive] = COLOR_GRAY;
	colors[ImGuiCol_Button] = COLOR_DARK_BLUE;
	colors[ImGuiCol_ButtonHovered] = COLOR_BLUE;
	colors[ImGuiCol_ButtonActive] = COLOR_LIGHT_BLUE;
	colors[ImGuiCol_ScrollbarBg] = COLOR_DARK_GRAY;
	colors[ImGuiCol_ScrollbarGrab] = COLOR_LIGHT_GRAY;
	colors[ImGuiCol_ScrollbarGrabHovered] = COLOR_LIGHTER_GRAY;
	colors[ImGuiCol_ScrollbarGrabActive] = COLOR_LIGHTER_GRAY_2;
}

void GetDisplayWindow()
{
    al_get_window_position(display, &wrect.x, &wrect.y);
    wrect.w = al_get_display_width(display);
    wrect.h = al_get_display_height(display);
}