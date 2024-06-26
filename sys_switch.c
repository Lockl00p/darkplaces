/*
 * Include this BEFORE darkplaces.h because it breaks wrapping
 * _Static_assert. Cloudwalk has no idea how or why so don't ask.
 */
#include <SDL.h>

#include "darkplaces.h"
#include <switch.h>
#include <sys/stat.h>
#include "quakedef.h"
// =======================================================================
// General routines
// =======================================================================

void Sys_SDL_Shutdown(void)
{
	socketExit();
	accountExit();
	SDL_Quit();
}

// Sys_Error early in startup might screw with automated
// workflows or something if we show the dialog by default.
static qbool nocrashdialog = true;
void Sys_SDL_Dialog(const char *title, const char *string)
{
	if(!nocrashdialog)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, string, NULL);
}

char *Sys_SDL_GetClipboardData (void)
{
	char *data = NULL;
	char *cliptext;

	cliptext = SDL_GetClipboardText();
	if (cliptext != NULL) {
		size_t allocsize;
		allocsize = min(MAX_INPUTLINE, strlen(cliptext) + 1);
		data = (char *)Z_Malloc (allocsize);
		dp_strlcpy (data, cliptext, allocsize);
		SDL_free(cliptext);
	}

	return data;
}

void Sys_SDL_Init(void)
{
	// we don't know which systems we'll want to init, yet...
	if (SDL_Init(0) < 0)
		Sys_Error("SDL_Init failed: %s\n", SDL_GetError());

	// COMMANDLINEOPTION: sdl: -nocrashdialog disables "Engine Error" crash dialog boxes
	if(!Sys_CheckParm("-nocrashdialog"))
		nocrashdialog = false;
}

qbool sys_supportsdlgetticks = true;
unsigned int Sys_SDL_GetTicks (void)
{
	return SDL_GetTicks();
}
void Sys_SDL_Delay (unsigned int milliseconds)
{
	SDL_Delay(milliseconds);
}

int StartupError(char message[]){
	consoleInit(NULL);
	printf(message);
	printf("\nPlease exit the app");
	while(appletMainLoop()){

		consoleUpdate(NULL);
	}
}

void menu_enter_f(cmd_state_t *cmd)
{
	Key_Event(K_ENTER,0,true);
	Key_Event(K_ENTER,0,false);
}

void Sys_Register_Commands(){
	Cmd_AddCommand(CF_SHARED, "emulate_enter", menu_enter_f, "Emulate pressing Enter");
}
int main(int argc, char *argv[])
{
	
	socketInitializeDefault();
	Result result = accountInitialize(AccountServiceType_Application);
	if (R_FAILED(result))
	{
		StartupError("Unable to initialize account system\nApplet Mode is not supported.");
		

	}
	
	return Sys_Main(argc, argv);
}
