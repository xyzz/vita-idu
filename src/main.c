#include <string.h>
#include <stdlib.h>

#include <psp2/ctrl.h>
#include <psp2/kernel/threadmgr.h>

#include <sys/time.h>

#include "graphics.h"

#define printf psvDebugScreenPrintf

int get_key(void) {
	static unsigned buttons[] = {
		SCE_CTRL_SELECT,
		SCE_CTRL_START,
		SCE_CTRL_UP,
		SCE_CTRL_RIGHT,
		SCE_CTRL_DOWN,
		SCE_CTRL_LEFT,
		SCE_CTRL_LTRIGGER,
		SCE_CTRL_RTRIGGER,
		SCE_CTRL_TRIANGLE,
		SCE_CTRL_CIRCLE,
		SCE_CTRL_CROSS,
		SCE_CTRL_SQUARE,
	};

	static unsigned prev = 0;
	SceCtrlData pad;
	while (1) {
		memset(&pad, 0, sizeof(pad));
		sceCtrlPeekBufferPositive(0, &pad, 1);
		unsigned newb = prev ^ (pad.buttons & prev);
		prev = pad.buttons;
		for (int i = 0; i < sizeof(buttons)/sizeof(*buttons); ++i)
			if (newb & buttons[i])
				return buttons[i];

		sceKernelDelayThread(1000); // 1ms
	}
}

void press_exit(void) {
	printf("Press any key to exit this application.\n");
	get_key();
	exit(0);
}

int main(void) {
	psvDebugScreenInit();

	printf("IDU stuffs\n");

	int ret = vshSysconIsIduMode();
	printf("IDU mode status: 0x%x\n", ret);

again:
	printf("\n");
	printf("Press X to set IDU mode\n");
	printf("Press O to clear IDU mode\n");
	printf("EXIT THIS APP IMMEDIATELY IF YOU DO NOT KNOW WHAT YOU ARE DOING OR WHAT IDU MODE IS\n");

	switch(get_key()) {
	case SCE_CTRL_CROSS:
		ret = vshSysconIduModeSet();
		printf("vshSysconIduModeSet: 0x%x\n", ret);
		break;
	case SCE_CTRL_CIRCLE:
		ret = vshSysconIduModeClear();
		printf("vshSysconIduModeClear: 0x%x\n", ret);
		break;
	default:
		goto again;
	}

	printf("Reboot PS Vita for changes to take effect\n");

	press_exit();
	
	return 0;
}
