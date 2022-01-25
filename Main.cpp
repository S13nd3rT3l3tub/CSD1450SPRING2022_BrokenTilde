// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include <Windows.h>


// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	///////////////////////
	// Variable declaration

	int gGameRunning = 1;


	AEGfxVertexList* pTankMesh(0);

	AEGfxTexture* pTankTexture;

	float tankPosX(0.0f), tankPosY(0.0f);

	float camX, camY;		// Used to temporary store camera position

	// Variable declaration end
	///////////////////////////


	/////////////////
	// Initialization
	
	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN), 1, 60, true, NULL);
	
	// Change background color
	AEGfxSetBackgroundColor(255.0f, 255.0f, 255.0f);

	// Changing the window title
	AESysSetWindowTitle("Assignment 1");

	// reset the system modules
	AESysReset();

	// Initialization end
	/////////////////////



	////////////////////////////////
	// Creating the objects (Shapes)

	AEGfxMeshStart();
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);
	pTankMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pTankMesh, "Failed to create pTankMesh");

	// Creating the objects (Shapes) end
	////////////////////////////////////



	////////////////////////////
	// Loading textures (images)

	pTankTexture = AEGfxTextureLoad("tank.png");

	// Loading textures (images) end
	//////////////////////////////////

	//////////////////////////////////
	// Creating Fonts	

	// Creating Fonts end
	//////////////////////////////////


	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		///////////////////
		// Game loop update

		// Move the camera
		AEGfxGetCamPosition(&camX, &camY);
		if (AEInputCheckCurr(AEVK_W))
			AEGfxSetCamPosition(camX, camY + 2);
		else if (AEInputCheckCurr(AEVK_S))
			AEGfxSetCamPosition(camX, camY - 2);

		AEGfxGetCamPosition(&camX, &camY);
		if (AEInputCheckCurr(AEVK_A))
			AEGfxSetCamPosition(camX - 2, camY);
		else if (AEInputCheckCurr(AEVK_D))
			AEGfxSetCamPosition(camX + 2, camY);
		// Game loop update end
		///////////////////////


		//////////////////
		// Game loop draw

		// Drawing object 1
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set position for object 1
		AEGfxSetPosition(tankPosX, tankPosY);
		// No texture for object 1
		AEGfxTextureSet(pTankTexture, 0, 0);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(pTankMesh, AE_GFX_MDM_TRIANGLES);

		// Game loop draw end
		/////////////////////


		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	//////////////////////////////////
	// Free-ing Assets Used	

	// Free-ing Assets Used
	//////////////////////////////////

	// free the system
	AESysExit();
}