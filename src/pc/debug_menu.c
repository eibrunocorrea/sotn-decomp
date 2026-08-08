// SPDX-License-Identifier: AGPL-3.0-or-later
// In-game debug menu ("GameShark") rendered with Dear ImGui on top of the
// PSY-Z SDL3+SDL_GPU backend. Toggle with F1 or the gamepad Create button.
#include <game.h>

#include <SDL3/SDL.h>
#include <psyz/overlay.h>
#include <psyz/overlay_sdl3.h>
#include <psyz/overlay_sdl3_gpu.h>

#include "cimgui.h"
#include "cimgui_impl.h"

// imgui_impl_sdlgpu3 is compiled with IMGUI_IMPL_API=extern "C" but has no
// generated cimgui binding, so its C-linkage API is declared by hand here.
typedef struct {
    SDL_GPUDevice* Device;
    SDL_GPUTextureFormat ColorTargetFormat;
    SDL_GPUSampleCount MSAASamples;
    SDL_GPUSwapchainComposition SwapchainComposition;
    SDL_GPUPresentMode PresentMode;
} DebugImplSDLGPU3_InitInfo;
extern bool ImGui_ImplSDLGPU3_Init(DebugImplSDLGPU3_InitInfo* info);
extern void ImGui_ImplSDLGPU3_Shutdown(void);
extern void ImGui_ImplSDLGPU3_NewFrame(void);
extern void ImGui_ImplSDLGPU3_PrepareDrawData(
    ImDrawData* draw_data, SDL_GPUCommandBuffer* command_buffer);
extern void ImGui_ImplSDLGPU3_RenderDrawData(
    ImDrawData* draw_data, SDL_GPUCommandBuffer* command_buffer,
    SDL_GPURenderPass* render_pass, SDL_GPUGraphicsPipeline* pipeline);

static SDL_GPUDevice* s_device;
static bool s_ready;
static bool s_open;
static bool s_godMode;
static bool s_frameActive;

static const char* s_relicNames[NUM_RELICS] = {
    "Soul of Bat",      "Fire of Bat",     "Echo of Bat",
    "Force of Echo",    "Soul of Wolf",    "Power of Wolf",
    "Skill of Wolf",    "Form of Mist",    "Power of Mist",
    "Gas Cloud",        "Cube of Zoe",     "Spirit Orb",
    "Gravity Boots",    "Leap Stone",      "Holy Symbol",
    "Faerie Scroll",    "Jewel of Open",   "Merman Statue",
    "Bat Card",         "Ghost Card",      "Faerie Card",
    "Demon Card",       "Sword Card",      "Sprite Card (JP)",
    "Nosedevil Card (JP)", "Heart of Vlad", "Tooth of Vlad",
    "Rib of Vlad",      "Ring of Vlad",    "Eye of Vlad",
};

static const char* s_statNames[4] = {"STR", "CON", "INT", "LCK"};

static void InputS32(const char* label, s32* value) {
    int v = (int)*value;
    if (igInputInt(label, &v, 1, 10, 0)) {
        *value = (s32)v;
    }
}

static void InputU32(const char* label, u32* value) {
    int v = (int)*value;
    if (igInputInt(label, &v, 1, 10, 0)) {
        if (v < 0) {
            v = 0;
        }
        *value = (u32)v;
    }
}

static void DrawPanel(void) {
    ImVec2 initSize = {380.0f, 560.0f};
    ImVec2 zero = {0.0f, 0.0f};
    int i;

    igSetNextWindowSize(initSize, ImGuiCond_FirstUseEver);
    if (!igBegin("GameShark", &s_open, 0)) {
        igEnd();
        return;
    }
    igTextDisabled("F1 ou botao Create abre/fecha");

    if (igCollapsingHeader_TreeNodeFlags(
            "Vitals", ImGuiTreeNodeFlags_DefaultOpen)) {
        InputS32("HP", &g_Status.hp);
        InputS32("HP max", &g_Status.hpMax);
        InputS32("MP", &g_Status.mp);
        InputS32("MP max", &g_Status.mpMax);
        InputS32("Hearts", &g_Status.hearts);
        InputS32("Hearts max", &g_Status.heartsMax);
        if (igButton("Encher tudo", zero)) {
            g_Status.hp = g_Status.hpMax;
            g_Status.mp = g_Status.mpMax;
            g_Status.hearts = g_Status.heartsMax;
        }
        igSameLine(0.0f, -1.0f);
        igCheckbox("God Mode", &s_godMode);
    }

    if (igCollapsingHeader_TreeNodeFlags("Stats e progresso", 0)) {
        for (i = 0; i < 4; i++) {
            InputS32(s_statNames[i], &g_Status.statsBase[i]);
        }
        InputS32("Level", &g_Status.level);
        InputU32("EXP", &g_Status.exp);
        InputS32("Gold", &g_Status.gold);
        if (igButton("+100 mil gold", zero)) {
            g_Status.gold += 100000;
        }
        igSameLine(0.0f, -1.0f);
        if (igButton("Aprender todas as magias", zero)) {
            g_Status.spellsLearnt = ~0u;
        }
    }

    if (igCollapsingHeader_TreeNodeFlags("Reliquias", 0)) {
        if (igButton("Todas", zero)) {
            for (i = 0; i < NUM_RELICS; i++) {
                g_Status.relics[i] = RELIC_FLAG_FOUND | RELIC_FLAG_ACTIVE;
            }
        }
        igSameLine(0.0f, -1.0f);
        if (igButton("Nenhuma", zero)) {
            for (i = 0; i < NUM_RELICS; i++) {
                g_Status.relics[i] = 0;
            }
        }
        for (i = 0; i < NUM_RELICS; i++) {
            bool owned = g_Status.relics[i] != 0;
            if (igCheckbox(s_relicNames[i], &owned)) {
                g_Status.relics[i] =
                    owned ? (RELIC_FLAG_FOUND | RELIC_FLAG_ACTIVE) : 0;
            }
        }
    }

    if (igCollapsingHeader_TreeNodeFlags("Equipamento (IDs)", 0)) {
        igTextDisabled("IDs numericos das tabelas do jogo");
        InputU32("Mao direita", &g_Status.equipment[0]);
        InputU32("Mao esquerda", &g_Status.equipment[1]);
        InputU32("Cabeca", &g_Status.wornEquipment[0]);
        InputU32("Corpo", &g_Status.wornEquipment[1]);
        InputU32("Capa", &g_Status.wornEquipment[2]);
        InputU32("Acessorio 1", &g_Status.wornEquipment[3]);
        InputU32("Acessorio 2", &g_Status.wornEquipment[4]);
        InputU32("Subweapon", &g_Status.subWeapon);
    }

    igText("Tempo de jogo: %02d:%02d:%02d", g_Status.timerHours,
        g_Status.timerMinutes, g_Status.timerSeconds);
    igEnd();
}

static void OnInit(SDL_Window* window, SDL_GPUDevice* device) {
    DebugImplSDLGPU3_InitInfo info;

    s_device = device;
    igCreateContext(NULL);
    ImGui_ImplSDL3_InitForSDLGPU(window);
    info.Device = device;
    info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(device, window);
    info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    info.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;
    info.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;
    if (ImGui_ImplSDLGPU3_Init(&info)) {
        s_ready = true;
    }
}

static void OnEvent(const SDL_Event* event) {
    if (!s_ready) {
        return;
    }
    ImGui_ImplSDL3_ProcessEvent(event);
    if (event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat &&
        event->key.key == SDLK_F1) {
        s_open = !s_open;
    }
    if (event->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN &&
        event->gbutton.button == SDL_GAMEPAD_BUTTON_BACK) {
        s_open = !s_open;
    }
}

static void OnFrame(void) {
    ImDrawData* drawData;

    if (!s_ready) {
        return;
    }
    if (s_godMode && g_Status.hpMax > 0) {
        g_Status.hp = g_Status.hpMax;
    }
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    igNewFrame();
    if (s_open) {
        DrawPanel();
    }
    igRender();
    s_frameActive = true;

    // Uploading vertex/index buffers opens a copy pass, which is illegal
    // inside the swapchain render pass the render callback runs in, so the
    // upload is submitted on its own command buffer beforehand.
    drawData = igGetDrawData();
    if (drawData != NULL && drawData->CmdListsCount > 0) {
        SDL_GPUCommandBuffer* upload = SDL_AcquireGPUCommandBuffer(s_device);
        if (upload != NULL) {
            ImGui_ImplSDLGPU3_PrepareDrawData(drawData, upload);
            SDL_SubmitGPUCommandBuffer(upload);
        }
    }
}

static void OnRender(SDL_GPUCommandBuffer* cmd, SDL_GPURenderPass* pass) {
    ImDrawData* drawData;

    if (!s_ready || !s_frameActive) {
        return;
    }
    s_frameActive = false;
    drawData = igGetDrawData();
    if (drawData != NULL && drawData->CmdListsCount > 0) {
        ImGui_ImplSDLGPU3_RenderDrawData(drawData, cmd, pass, NULL);
    }
}

static void OnDestroy(void) {
    if (!s_ready) {
        return;
    }
    s_ready = false;
    ImGui_ImplSDLGPU3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    igDestroyContext(NULL);
}

void DebugMenu_Install(void) {
    Psyz_OverlayInit_SDL3GPU(OnInit);
    Psyz_OverlayEvent_SDL3(OnEvent);
    Psyz_OverlayFrameCB(OnFrame);
    Psyz_OverlayRender_SDL3GPU(OnRender);
    Psyz_OverlayDestroyCB(OnDestroy);
}
