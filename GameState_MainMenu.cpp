/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       GameState_MainMenu.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		
-------------------------------------------------------------------------------
@author		Mohamed Zafir (m.zafir@digipen.edu)
@role		Part of game's credits
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)
@role		
-------------------------------------------------------------------------------
@author		Desmond Too Wei Kang (d.too@digipen.edu)
@role
*//*_________________________________________________________________________*/

#include "Main.h"

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/


/******************************************************************************/
/*!
	Enums/Struct/Class Definitions
*/
/******************************************************************************/
enum BUTTON_TYPE {
	START_GAME = 1,
	OPTIONS,
	CREDITS,
	EXIT_GAME,
	RETURN,
	TOGGLE_FS,
	TOGGLE_SOUND,
	YES,
	NO,
	BG
};

enum SCREEN_TYPE {
	MAIN_SCREEN = 0,
	CREDIT_SCREEN,
	OPTION_SCREEN,
	EXIT_SCREEN
};


/******************************************************************************/
/*!
	(Static) Variables
*/
/******************************************************************************/
// ----- Textures ------
AEGfxTexture* backgroundTexture{};				// Background texture	
AEGfxTexture* buttonTexture_START{};			// Start game button texture
AEGfxTexture* buttonTexture_QUIT{};				// Quit game button texture
AEGfxTexture* buttonTexture_OPTIONS{};			// Options button texture
AEGfxTexture* buttonTexture_CREDITS{};			// Credits button texture
AEGfxTexture* buttonTexture_TOGGLE_FS{};		// Toggle fullscreen button texture
AEGfxTexture* buttonTexture_TOGGLE_SOUND{};		// Toggle sound button texture
AEGfxTexture* buttonTexture_RETURN{};			// Return button texture
AEGfxTexture* buttonTexture_YES{};				// Yes button texture
AEGfxTexture* buttonTexture_NO{};				// No button texture

// ----- Mesh Sizes & Scales -----
AEVec2		BUTTON_MESHSIZE { 500.0f, 100.0f };		// Button mesh size
AEVec2		BUTTON_SCALE	{ 1.0f, 1.0f };			// Button scale size

// Pointer to specific game object instances
static GameObjInst* ButtonInstance_START;			// Start button object instance
static GameObjInst* ButtonInstance_QUIT;			// Quit button object instance
static GameObjInst* ButtonInstance_OPTIONS;			// Options button object instance
static GameObjInst* ButtonInstance_CREDITS;			// Credits button object instance
static GameObjInst* ButtonInstance_TOGGLE_FS;		// Toggle fullscreen button object instance
static GameObjInst* ButtonInstance_TOGGLE_SOUND;	// Toggle sound button object instance
static GameObjInst* ButtonInstance_RETURN;			// Return button object instance
static GameObjInst* ButtonInstance_YES;				// Yes button object instance
static GameObjInst* ButtonInstance_NO;				// No button object instancce


static enum	SCREEN_TYPE	screen;						// Variable to store the current screen to display

/******************************************************************************/
/*!
	"Load" function of this state
*/
/******************************************************************************/
void GameStateMainMenuLoad() 
{
	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;

	// load/create the mesh data (game objects / Shapes)
	GameObj* pObj;

	// =========================
	// Load textures
	// =========================
	{
		// Background texture
		backgroundTexture = AEGfxTextureLoad(".\\Resources\\Assets\\background.png");
		AE_ASSERT_MESG(backgroundTexture, "failed to create background texture");

		// Start button texture
		buttonTexture_START = AEGfxTextureLoad(".\\Resources\\Assets\\start_button.png");
		AE_ASSERT_MESG(buttonTexture_START, "failed to create start button texture");

		// Options button texture
		buttonTexture_OPTIONS = AEGfxTextureLoad(".\\Resources\\Assets\\option_button.png");
		AE_ASSERT_MESG(buttonTexture_OPTIONS, "failed to create option button texture");

		// Credits button texture
		buttonTexture_CREDITS = AEGfxTextureLoad(".\\Resources\\Assets\\credit_button.png");
		AE_ASSERT_MESG(buttonTexture_CREDITS, "failed to create credit button texture");

		// Quit button texture
		buttonTexture_QUIT = AEGfxTextureLoad(".\\Resources\\Assets\\exit_button.png");
		AE_ASSERT_MESG(buttonTexture_QUIT, "failed to create quit button texture");

		// Toggle fullscreen button texture
		buttonTexture_TOGGLE_FS = AEGfxTextureLoad(".\\Resources\\Assets\\toggle_fs.png");
		AE_ASSERT_MESG(buttonTexture_TOGGLE_FS, "failed to create toggle fullscreen button texture");

		// Toggle sound button texture
		buttonTexture_TOGGLE_SOUND = AEGfxTextureLoad(".\\Resources\\Assets\\toggle_sound.png");
		AE_ASSERT_MESG(buttonTexture_TOGGLE_SOUND, "failed to create toggle sound button texture");

		// Return button texture
		buttonTexture_RETURN = AEGfxTextureLoad(".\\Resources\\Assets\\return_button.png");
		AE_ASSERT_MESG(buttonTexture_TOGGLE_FS, "failed to create toggle fullscreen button texture");

		// Yes button texture
		buttonTexture_YES = AEGfxTextureLoad(".\\Resources\\Assets\\yes_button.png");
		AE_ASSERT_MESG(buttonTexture_YES, "failed to create toggle fullscreen button texture");

		// No button texture
		buttonTexture_NO = AEGfxTextureLoad(".\\Resources\\Assets\\no_button.png");
		AE_ASSERT_MESG(buttonTexture_NO, "failed to create toggle fullscreen button texture");
	}

	// =========================
	// Create Meshes
	// =========================
	{
		// ----- Create the Button Shape -----
		{
			buttonObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_BUTTON;
			AEGfxMeshStart();
			AEGfxTriAdd(
				-BUTTON_MESHSIZE.x / 2, -BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 1.0f,
				BUTTON_MESHSIZE.x / 2, -BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 1.0f,
				-BUTTON_MESHSIZE.x / 2, BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f);
			AEGfxTriAdd(
				BUTTON_MESHSIZE.x / 2, -BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 1.0f,
				BUTTON_MESHSIZE.x / 2, BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 0.0f,
				-BUTTON_MESHSIZE.x / 2, BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ BUTTON_MESHSIZE.x, BUTTON_MESHSIZE.y };
			AE_ASSERT_MESG(pObj->pMesh, "fail to create BUTTON object!!");
		}

		// ----- Create the background mesh ------
		{
			bgObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_BG;
			AEGfxMeshStart();
			AEGfxTriAdd(
				-static_cast<float>(winWidth) / 2, -static_cast<float>(winHeight) / 2, 0x00FFFFFF, 0.0f, 1.0f,
				static_cast<float>(winWidth) / 2, -static_cast<float>(winHeight) / 2, 0x00FFFFFF, 1.0f, 1.0f,
				-static_cast<float>(winWidth) / 2, static_cast<float>(winHeight) / 2, 0x00FFFFFF, 0.0f, 0.0f);
			AEGfxTriAdd(
				static_cast<float>(winWidth) / 2, -static_cast<float>(winHeight) / 2, 0x00FFFFFF, 1.0f, 1.0f,
				static_cast<float>(winWidth) / 2, static_cast<float>(winHeight) / 2, 0x00FFFFFF, 1.0f, 0.0f,
				-static_cast<float>(winWidth) / 2, static_cast<float>(winHeight) / 2, 0x00FFFFFF, 0.0f, 0.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ 1.0f, 1.0f };
			AE_ASSERT_MESG(pObj->pMesh, "Failed to create bgMesh!!");
		}
	}

	// Move camera to 0,0 in event menu is loaded after game
	AEGfxSetCamPosition(0.0f, 0.0f);

	// Play main menu background sound
	fModSys->playSound(mainMenuBG, nullptr, false, &soundChannel);
	// Check if soundVolumnLevel is true
	if (soundVolumeLevel)
		soundChannel->setVolume(0.7f);	// Set volume
	else
		soundChannel->setVolume(0.0f);	// Mute volume
		
	// Set screen variable to main menu display
	screen = SCREEN_TYPE::MAIN_SCREEN;
}

/******************************************************************************/
/*!
	"InIt" function of this state
*/
/******************************************************************************/
void GameStateMainMenuInit() 
{
	// Set background color
	AEGfxSetBackgroundColor(0.2f, 0.2f, 0.2f);

	// Create an instance of the background 
	AEVec2 scaling{ 1.0f, 1.0f }, pos{0.0f, 0.0f};
	GameObjInst* bg{ gameObjInstCreate(&sGameObjList[bgObjIndex], &scaling, &pos,0, 0.0f, STATE_NONE) };
	bg->sub_type = BUTTON_TYPE::BG;

	// ----- Create the 4 main menu buttons -----
	// Start Game button
	scaling = { 1.0f, 1.0f };
	pos = { 0.0f, 0.0f };
	ButtonInstance_START = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_START->sub_type = BUTTON_TYPE::START_GAME;

	// Options button
	scaling = { 1.0f, 1.0f };
	pos = { 0.0f, -100.0f };
	ButtonInstance_OPTIONS = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_OPTIONS->sub_type = BUTTON_TYPE::OPTIONS;

	// Credits button
	scaling = { 1.0f, 1.0f };
	pos = { 0.0f, -200.0f };
	ButtonInstance_CREDITS = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_CREDITS->sub_type = BUTTON_TYPE::CREDITS;

	// Exit game button
	scaling = { 1.0f, 1.0f };
	pos = { 0,-300.0F };
	ButtonInstance_QUIT = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_QUIT->sub_type = BUTTON_TYPE::EXIT_GAME;
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUpdate() 
{
	// Switch logic based on the innerState of the game state
	switch (gGameStateInnerState) {
		// Pause State
		case INNER_GAME_STATE::GAME_PAUSE: {
			// Pause the sound channel
			soundChannel->setPaused(true);

			// Check if window was focused
			if (winFocused)
				gGameStateInnerState = INNER_GAME_STATE::GAME_PLAY;	// Update innerState back to play state

			break;
		}
		// Play State
		case INNER_GAME_STATE::GAME_PLAY: {
			// Check if window was unfocused
			if (winFocused == false)
				gGameStateInnerState = INNER_GAME_STATE::GAME_PAUSE;	// Update innerState to pause state

			// Find out if sound channel was paused; If so, unpause it
			bool result;
			soundChannel->getPaused(&result);
			if (result)
				soundChannel->setPaused(false);

			//	Check input for left mouse click
			if (AEInputCheckReleased(VK_LBUTTON)){
				// Variable declaration & initialization
				AEVec2 scaling{ 1.0f, 1.0f }, pos{ 0.0f, 0.0f };

				// Switch logic based on the current display screen
				switch (screen) {
					// Main Menu
					case SCREEN_TYPE::MAIN_SCREEN: {
						// Check if mouse clicked on Start Game button
						if ( CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_START->boundingBox) ) {
							//	Load level 1
							//g_chosenLevel = 1;
							gGameStateNext = GS_LEVEL1;
						}

						// Check if mouse clicked on Options button
						if ( CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_OPTIONS->boundingBox) ) {
							//	Update current display screen to Options
							screen = SCREEN_TYPE::OPTION_SCREEN;

							// Create instance of background to overlay over the main menu
							GameObjInst* bg{ gameObjInstCreate(&sGameObjList[bgObjIndex], &scaling, &pos, 0, 0.0f, STATE_NONE) };
							bg->sub_type = BUTTON_TYPE::BG;

							// Create Toggle Fullscreen button
							scaling = { 1.0f, 1.0f };
							pos = { 0.0f, 0.0f };
							ButtonInstance_TOGGLE_FS = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
							ButtonInstance_TOGGLE_FS->sub_type = BUTTON_TYPE::TOGGLE_FS;

							// Create Toggle Sound button
							scaling = { 1.0f, 1.0f };
							pos = { 0.0f, -130.0f };
							ButtonInstance_TOGGLE_SOUND = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
							ButtonInstance_TOGGLE_SOUND->sub_type = BUTTON_TYPE::TOGGLE_SOUND;

							// Create Return button
							scaling = { 1.0f, 1.0f };
							pos = { 0,-300.0f };
							ButtonInstance_RETURN = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
							ButtonInstance_RETURN->sub_type = BUTTON_TYPE::RETURN;
						}

						// Check if mosue was clicked on Credits button
						if ( CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_CREDITS->boundingBox) ) {
							// Update current display screen to credits
							screen = SCREEN_TYPE::CREDIT_SCREEN;

							// Create instance of background to overlay over the main menu
							GameObjInst* bg{ gameObjInstCreate(&sGameObjList[bgObjIndex], &scaling, &pos, 0, 0.0f, STATE_NONE) };
							bg->sub_type = BUTTON_TYPE::BG;

							// Create Return button
							scaling = { 1.0f, 1.0f };
							pos = { 0,-280.0f };
							ButtonInstance_RETURN = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
							ButtonInstance_RETURN->sub_type = BUTTON_TYPE::RETURN;
						}

						// Check if mosue was clicked on Credits button
						if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_QUIT->boundingBox)) {
							// Update current display screen to exit confirmation
							screen = SCREEN_TYPE::EXIT_SCREEN;
							
							// Create instance of background to overlay over the main menu
							GameObjInst* bg{ gameObjInstCreate(&sGameObjList[bgObjIndex], &scaling, &pos, 0, 0.0f, STATE_NONE) };
							bg->sub_type = BUTTON_TYPE::BG;

							// Create Yes button
							scaling = { 0.5f, 0.5f };
							pos = { -300.0f,-300.0f };
							ButtonInstance_YES = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
							ButtonInstance_YES->sub_type = BUTTON_TYPE::YES;

							// Create No button
							scaling = { 0.5f, 0.5f };
							pos = { 300.0f ,-300.0f };
							ButtonInstance_NO = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
							ButtonInstance_NO->sub_type = BUTTON_TYPE::NO;
						}
						break;
					}
					// Options
					case SCREEN_TYPE::OPTION_SCREEN: {
						//	Check if mouse was clicked on Toggle Fullscreen button
						if ( CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_TOGGLE_FS->boundingBox) ) {
							// Invert flag variable's current value
							toFullScreen = !toFullScreen;
							// Set fullscreen
							AEToogleFullScreen(toFullScreen);
						}

						//	Check if mouse was clicked on Toggle Sound button
						if ( CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_TOGGLE_SOUND->boundingBox) ) {
							// Invert flag variable's current value
							soundVolumeLevel = !soundVolumeLevel;
							// Based on value, set whether to mute the channel or set volume
							if (soundVolumeLevel)
								soundChannel->setVolume(0.7f);
							else
								soundChannel->setVolume(0.0f);
						}

						//	Check if mouse was clicked on Return button
						if ( CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_RETURN->boundingBox) ) {
							// Update screen display to main menu
							screen = SCREEN_TYPE::MAIN_SCREEN;
							// Restart the state 
							gGameStateNext = GS_RESTART;
						}
						break;
					}
					// Credits
					case CREDIT_SCREEN: {
						//	Check if mouse was clicked on Return button
						if ( CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_RETURN->boundingBox) ) {
							// Update display screen to be main menu
							screen = SCREEN_TYPE::MAIN_SCREEN;
							// Restart state
							gGameStateNext = GS_RESTART;
						}
						break;
					}
					// Exit Confirmation
					case EXIT_SCREEN: {
						// Check if mouse was clicked on Yes button
						if ( CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_YES->boundingBox) )
							gGameStateNext = GS_QUIT;	// Quit the game

						// Check if mouse was clicked on No button
						if ( CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_NO->boundingBox) ) {
							// Update display screen to be main menu
							screen = SCREEN_TYPE::MAIN_SCREEN;
							// Restart state
							gGameStateNext = GS_RESTART;
						}
						break;
					}
				}
			}

			// Call FMOD system update function
			fModSys->update();

			// Variable declaration & initia
			int i{};
			GameObjInst* pInst;

			// Update object instances physics and behavior
			for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
			{
				pInst = sGameObjInstList + i;

				// skip non-active object
				if (0 == (pInst->flag & FLAG_ACTIVE))
					continue;

			}

			//Update object instances positions
			for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
			{
				pInst = sGameObjInstList + i;

				// skip non-active object
				if (0 == (pInst->flag & FLAG_ACTIVE))
					continue;

				/********************************
				update the positions of instances
				********************************/

				// ----- Update Position -----
				pInst->posCurr.x += pInst->velCurr.x * g_dt;
				pInst->posCurr.y += pInst->velCurr.y * g_dt;

				// ----- Update Bounding Box -----
				pInst->boundingBox.min.x = -(pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
				pInst->boundingBox.min.y = -(pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;

				pInst->boundingBox.max.x = (pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
				pInst->boundingBox.max.y = (pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;
			}

			// ====================
			// Check for collision
			// ====================
			for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
			{
				pInst = sGameObjInstList + i;

				// skip non-active object
				if ((pInst->flag & FLAG_ACTIVE) == 0)
					continue;
			}

			// =====================================
			// Calculate the matrix for all objects
			// =====================================
			for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
			{
				pInst = sGameObjInstList + i;
				AEMtx33		 trans, rot, scale;

				// skip non-active object
				if ((pInst->flag & FLAG_ACTIVE) == 0)
					continue;

				// Compute the scaling matrix
				AEMtx33Scale(&scale, pInst->scale.x, pInst->scale.y);
				// Compute the rotation matrix 	
				AEMtx33Rot(&rot, pInst->dirCurr);
				// Compute the translation matrix
				AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
				// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
				AEMtx33Concat(&pInst->transform, &rot, &scale);
				AEMtx33Concat(&pInst->transform, &trans, &pInst->transform);
			}


			// =====================================
			// Mouse Input
			// =====================================
			float cameraX, cameraY;
			AEGfxGetCamPosition(&cameraX, &cameraY);
			worldMouseX = cameraX + (static_cast<float>(g_mouseX) - static_cast<float>(AEGetWindowWidth()) / 2);
			worldMouseY = cameraY + (-1) * (static_cast<float>(g_mouseY) - static_cast<float>(AEGetWindowHeight()) / 2);

			break;
		}
	}
}

/******************************************************************************/
/*!
	"Draw" function of this state
*/
/******************************************************************************/
void GameStateMainMenuDraw() {
	// Set background color
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Set render settings
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);

	// draw all object instances in the list
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE) || 0 == (pInst->flag & FLAG_VISIBLE))
			continue;

		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxSetTransform(pInst->transform.m);
		// ----- Render objects on screen ------
		// Switch texture based on the button type
		switch (pInst->sub_type) {
			// Start Game
			case BUTTON_TYPE::START_GAME: {
				AEGfxTextureSet(buttonTexture_START, 0.0f, 0.0f);
				break;
			}
			// Exit game
			case BUTTON_TYPE::EXIT_GAME: {
				AEGfxTextureSet(buttonTexture_QUIT, 0.0f, 0.0f);
				break;
			}
			// Options
			case BUTTON_TYPE::OPTIONS: {
				AEGfxTextureSet(buttonTexture_OPTIONS, 0.0f, 0.0f);
				break;
			}
			// Credits
			case BUTTON_TYPE::CREDITS: {
				AEGfxTextureSet(buttonTexture_CREDITS, 0.0f, 0.0f);
				break;
			}
			// Toggle Fullscreen
			case BUTTON_TYPE::TOGGLE_FS: {
				AEGfxTextureSet(buttonTexture_TOGGLE_FS, 0.0f, 0.0f);
				break;
			}
			// Toggle Sound
			case BUTTON_TYPE::TOGGLE_SOUND: {
				AEGfxTextureSet(buttonTexture_TOGGLE_SOUND, 0.0f, 0.0f);
				break;
			}
			// Return
			case BUTTON_TYPE::RETURN: {
				AEGfxTextureSet(buttonTexture_RETURN, 0.0f, 0.0f);
				break;
			}
			// Yes
			case BUTTON_TYPE::YES: {
				AEGfxTextureSet(buttonTexture_YES, 0.0f, 0.0f);
				break;
			}
			// No
			case BUTTON_TYPE::NO: {
				AEGfxTextureSet(buttonTexture_NO, 0.0f, 0.0f);
				break;
			}
			// Background
			case BUTTON_TYPE::BG: {
				AEGfxTextureSet(backgroundTexture, 0.0f, 0.0f);
				break;
			}
		}
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}

	//	Drawing for Font for all states
	char strBuffer[100]{};
	f32 TextWidth{ 1.0f }, TextHeight{ 1.0f };
	memset(strBuffer, 0, 100 * sizeof(char));

	// Set render mode
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// Switch logic based on which screen is the current display
	switch (screen) {
		// Credits
		case SCREEN_TYPE::CREDIT_SCREEN: {
			// ----- Created at DigiPen -----
			{
				sprintf_s(strBuffer, "Created at ");
				AEGfxGetPrintSize(g_font30, strBuffer, 0.8f, TextWidth, TextHeight);
				AEGfxPrint(g_font30, strBuffer, -0.35f - TextWidth / 2, 0.27f - TextHeight / 2, 0.8f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "DigiPen Institute of Technology Singapore");
				AEGfxGetPrintSize(g_font30, strBuffer, 0.8f, TextWidth, TextHeight);
				AEGfxPrint(g_font30, strBuffer, -0.35f - TextWidth / 2, 0.2f - TextHeight / 2, 0.8f, 1.f, 1.f, 1.f);
			}
			// ----- Team Composition -----
			{
				// Team Name
				sprintf_s(strBuffer, "By Broken Tilde");
				AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font20, strBuffer, -0.6f - TextWidth / 2, 0.10f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				// Team Members
				sprintf_s(strBuffer, "Mohamed Zafir");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.6f - TextWidth / 2, 0.02f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Lee Hsien Wei, Joachim");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.6f - TextWidth / 2, -0.05f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Leong Wai Kit");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.6f - TextWidth / 2, -0.12f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Desmond Too Wei Kang");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.6f - TextWidth / 2, -0.19f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
			}
			// ----- Oversee-ers ------
			{
				// Instructors
				sprintf_s(strBuffer, "Instructors");
				AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font20, strBuffer, -0.6f - TextWidth / 2, -0.35f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				// Instructor's Names
				sprintf_s(strBuffer, "Cheng Ding Xiang");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.6f - TextWidth / 2, -0.43f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Gerald Wong Han Feng");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.6f - TextWidth / 2, -0.5f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
			}
			// ----- Softwares Used -----
			{
				// Software
				sprintf_s(strBuffer, "Softwares Used");
				AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font20, strBuffer, -0.15f - TextWidth / 2, 0.10f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				// Name of softwares
				sprintf_s(strBuffer, "Visual Studio 2019");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.15f - TextWidth / 2, 0.03f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Alpha Engine");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.15f - TextWidth / 2, -0.04f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "FMOD Sound System");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.15f - TextWidth / 2, -0.11f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "by FireLight Technologies Pty Ltd (1998 - 2020)");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.15f - TextWidth / 2, -0.14f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Paint 3D");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.15f - TextWidth / 2, -0.21f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "OBS Studio");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.15f - TextWidth / 2, -0.28f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "DaVinci Resolve");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, -0.15f - TextWidth / 2, -0.35f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
			}
			// ----- DigiPen Executives -----
			{
				sprintf_s(strBuffer, "President");
				AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font20, strBuffer, 0.45f - TextWidth / 2, 0.25f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Claude COMAIR");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.45f - TextWidth / 2, 0.15f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

				sprintf_s(strBuffer, "Executives");
				AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font20, strBuffer, 0.45f - TextWidth / 2, 0.0f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

				sprintf_s(strBuffer, "Jason CHU");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.25f - TextWidth / 2, -0.1f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Christopher COMAIR");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.45f - TextWidth / 2, -0.1f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Michael GATS");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.65f - TextWidth / 2, -0.1f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

				sprintf_s(strBuffer, "Michele COMAIR");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.25f - TextWidth / 2, -0.2f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Raymond YAN");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.45f - TextWidth / 2, -0.2f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Samir ABOU SAMRA");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.65f - TextWidth / 2, -0.2f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

				sprintf_s(strBuffer, "Prasanna GHALI");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.25f - TextWidth / 2, -0.3f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "John BAUER");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.45f - TextWidth / 2, -0.3f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Dr. Erik MOHRMANN");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.65f - TextWidth / 2, -0.3f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

				sprintf_s(strBuffer, "Melvin GONSALVEZ");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.25f - TextWidth / 2, -0.4f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Angela KUGLER");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.45f - TextWidth / 2, -0.4f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Dr Charles DUBA");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.65f - TextWidth / 2, -0.4f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

				sprintf_s(strBuffer, "Ben ELLINGER");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.35f - TextWidth / 2, -0.5f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
				sprintf_s(strBuffer, "Johnny DEEK");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.55f - TextWidth / 2, -0.5f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
			}
			// ----- Copyright to DigiPen -----
			{
				sprintf_s(strBuffer, "All Content Copyright 2022 DigiPen Institute of Technology Singapore, All Rights Reserved.");
				AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
				AEGfxPrint(g_font12, strBuffer, 0.0f - TextWidth / 2, -0.95f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
			}
			break;
		}
		// Exit Confirmation
		case SCREEN_TYPE::EXIT_SCREEN: {
			// Inform user of exit confirmation
			sprintf_s(strBuffer, "Are you sure you want to exit?");
			AEGfxGetPrintSize(g_font30, strBuffer, 1.0f, TextWidth, TextHeight);
			AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, 0.0f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
			break;
		}
	}
}

/******************************************************************************/
/*!
	"Free" function of this state
*/
/******************************************************************************/
void GameStateMainMenuFree() {
	// kill all object instances in the array using "gameObjInstDestroy"
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;

		gameObjInstDestroy(pInst);
	}
}

/******************************************************************************/
/*!
	"Unload" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUnload() {
	AEGfxTextureUnload(backgroundTexture);
	AEGfxTextureUnload(buttonTexture_START);
	AEGfxTextureUnload(buttonTexture_QUIT);
	AEGfxTextureUnload(buttonTexture_CREDITS);
	AEGfxTextureUnload(buttonTexture_OPTIONS);
	AEGfxTextureUnload(buttonTexture_TOGGLE_FS);
	AEGfxTextureUnload(buttonTexture_RETURN);
	AEGfxTextureUnload(buttonTexture_YES);
	AEGfxTextureUnload(buttonTexture_NO);

	soundChannel->stop(); //stop background music
		
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned long i = 0; i < sGameObjNum; i++) {
		GameObj* pObj = sGameObjList + i;
		if (pObj->pMesh != nullptr)
			AEGfxMeshFree(pObj->pMesh);
	}
}
