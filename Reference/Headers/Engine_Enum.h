#pragma once

namespace Engine
{
	enum class LAYERTAG
	{
		// DEFAULT
		DEFAULT,
		BACKGROUND,
		CAMERA,
		IGNORERAY,
		UI,
		DEFAULT_LAYER_END,

		// DYNAMIC_LAYER
		PLAYER,
		UNIT_AIR,
		UNIT_GROUND,
		EQUIPMENT,	// DYNAMIC이 맞을까 잘 모르겠다.
		DYNAMIC_LAYER_END,

		// STATIC_LAYER
		TERRAIN,
		GROUND,
		WALL,
		TRIGGER,
		INTERACTABLE,
		STATIC_LAYER_END,

		// END
		LAYER_END
	};

	enum
	{
		DEFAULT_LAYER_COUNT = static_cast<_uint>(LAYERTAG::DEFAULT_LAYER_END),
		DYNAMIC_LAYER_COUNT = static_cast<_uint>(LAYERTAG::DYNAMIC_LAYER_END) - DEFAULT_LAYER_COUNT - 1,
		STATIC_LAYER_COUNT = static_cast<_uint>(LAYERTAG::STATIC_LAYER_END) - (DYNAMIC_LAYER_COUNT + 1) - (DEFAULT_LAYER_COUNT + 1)
	};

	static const _char* LayerTag_string[] =
	{
		"DEFAULT",
		"BACKGROUND",
		"CAMERA",
		"IGNORERAY",
		"UI",
		"DEFAULT_LAYER_END",

		"PLAYER",
		"UNIT_AIR",
		"UNIT_GROUND",
		"DYNAMIC_LAYER_END",

		"TERRAIN",
		"GROUND",
		"WALL",
		"TRIGGER",
		"INTERACTABLE",
		"STATIC_LAYER_END",

		"LAYER_END",
	};

	enum class OBJECTTAG
	{
		OBJECT_END
	};

	enum class Axis
	{
		X,
		Y,
		Z,
	};

	enum class KEYSTATE
	{
		NONE,
		TAP,
		HOLD,
		AWAY
	};

	enum class KEY
	{
		LEFT_ARROW,
		RIGHT_ARROW,
		UP_ARROW,
		DOWN_ARROW,
		TAB,
		OPEN_SQUARE_BRACKET,
		CLOSE_SQUARE_BRACKET,
		F1, F2, F3, F4, F5, F6, F7, F8, F9,
		INSERT, DEL, HOME, END_KEY, PAGE_UP, PAGE_DOWN,
		NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0,
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		ALT, CTRL, SHIFT,
		SPACE, ENTER, ESC,
		LBTN, RBTN,
		TYPE_END
	};

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

}