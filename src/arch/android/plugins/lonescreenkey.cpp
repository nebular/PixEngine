//
// Created by rodo on 2020-01-26.
//

#include <lib/olcPGE_runner/PgeRunner.h>
#include "lonescreenkey.h"

LoneScreenKey *LoneScreenKey::currentInstance = nullptr;

LoneScreenKey::LoneScreenKey(int resolution) : fResolution(resolution) {}

int LoneScreenKey::input(int32_t x, int32_t y) {
	x /= fResolution;
	y /= fResolution;
	for (ScreenKey_t key : vKeys)
		if (x > key.x0 && x < key.x0 + key.w && y > key.y0 && y < key.y0 + key.h) return key.code;

	return NO_KEYZ;
}

bool LoneScreenKey::GetFakeMouse(int button) { return aMyMouseStats[button]; }

void LoneScreenKey::add(ScreenKey_t key) {
	vKeys.push_back(key);
}

// convenience to add 4 arrow keys
void LoneScreenKey::addCursors(int PX, int PY) {
	add({olc::Key::UP, PX, PY + 0, 100, 50});
	add({olc::Key::LEFT, PX, PY + 50, 50, 50});
	add({olc::Key::RIGHT, PX + 50, PY + 50, 50, 50});
	add({olc::Key::DOWN, PX, PY + 100, 100, 50});

}

// convenience to add mouse buttons
void LoneScreenKey::addMouse(int PX, int PY) {
	add({0, PX, PY, 50, 50, true});
	add({1, PX + 50, PY, 50, 50, true});
}

// remove all vkeys
void LoneScreenKey::clear() {
	vKeys.clear();
}

void LoneScreenKey::DrawSelf(olc::PixelGameEngine *engine, olc::Pixel color, bool mouse) {

	if (mouse)
		engine->DrawCircle(engine->GetMouseX(), engine->GetMouseY(), 40.0, olc::RED);

	for (ScreenKey_t key : vKeys) {
		if (((!key.mouse && engine->GetKey(static_cast<olc::Key>(key.code)).bHeld)
			 || (key.mouse && aMyMouseStats[key.code])))
			engine->FillRect(key.x0, key.y0, key.w, key.h, color);
		else
			engine->DrawRect(key.x0, key.y0, key.w, key.h, color);
	}

}

#define MAXBUTTONS 2

void LoneScreenKey::reset() {
	for (int i = 0; i < MAXBUTTONS; i++) aMyMouseStats[i] = false;
}

// todo
int
LoneScreenKey::sync(int32_t x0, int32_t y0, int32_t x1, int32_t y1, bool keystats[], bool isUp) {

	x0 /= fResolution;
	y0 /= fResolution;
	x1 /= fResolution;
	y1 /= fResolution;

	bool any = NO_KEYZ;

	for (ScreenKey_t key : vKeys) {
		if (x1 > key.x0 && x1 < key.x0 + key.w && y1 > key.y0 && y1 < key.y0 + key.h) {
			// if the secondary pointer is in the key region
			any = 1;
			if (!key.mouse) keystats[key.code] = !isUp;
			else if (key.code < MAXBUTTONS) aMyMouseStats[key.code] = true;
		} else if (x0 > key.x0 && x0 < key.x0 + key.w && y0 > key.y0 && y0 < key.y0 + key.h) {
			// if the primary pointer is in the key region
			any = 0;
			if (!key.mouse) keystats[key.code] = !isUp;
			else if (key.code < MAXBUTTONS) aMyMouseStats[key.code] = true;
		} else {
			if (key.mouse) aMyMouseStats[key.code] = false;
			else keystats[key.code] = false;
		}
	}


	return any;
}
