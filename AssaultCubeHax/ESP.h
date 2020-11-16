#pragma once
#include <Windows.h>
#include "vector.h"
#include <iostream>
#include "glm2/glm.hpp"



class ESP
{
private:
	int colour;
	HBRUSH brush;
	int screenWidth = 0, screenHeight = 0;
	float modelWidth = 1000.0f;
	float modelHeight = 2500.0f;
	int lineThickness = 2;
	HWND hwnd; // Window handle
	HDC hdc; // Device context handle

	void drawFilledRect(float x, float y, float w, float h) {
		RECT rect = { x,y,x + w,y + h };
		FillRect(hdc,&rect,brush);
	}

	void drawBoundingBox(float x, float y, float w, float h) {
		drawFilledRect(x, y, w, lineThickness);//top side
		drawFilledRect(x, y, lineThickness, h); //left side
		drawFilledRect((x + w), y, lineThickness, h); //right side
		drawFilledRect(x, y + h, w + lineThickness, lineThickness); //bottom side
	}

public:
	enum TEAM { ENEMY, FRIEND };

	void renderESPBox(glm::vec3 entVec, float* viewMatrix, float distanceFromPlayer) {
		glm::vec3 headVec = { 0,0,0 };
		glm::vec3 feetVec = { 0,0,0 };
		float heightModifier = (modelHeight / distanceFromPlayer); 
		float widthModifier = (modelWidth / distanceFromPlayer);
		entVec.z += 1.0f;
		worldToScreen(entVec, headVec, viewMatrix);

		drawBoundingBox(headVec.x-(widthModifier/2), headVec.y, widthModifier, heightModifier);
	}

	// Maths gratuitously ripped from the Internet.
	void worldToScreen(glm::vec3 In, glm::vec3& Out, float* ViewMatrix) {
		Out.x = In.x * ViewMatrix[0] + In.y * ViewMatrix[4] + In.z * ViewMatrix[8] + ViewMatrix[12];
		Out.y = In.x * ViewMatrix[1] + In.y * ViewMatrix[5] + In.z * ViewMatrix[9] + ViewMatrix[13];
		Out.z = In.x * ViewMatrix[2] + In.y * ViewMatrix[6] + In.z * ViewMatrix[10] + ViewMatrix[14];
		float w = In.x * ViewMatrix[3] + In.y * ViewMatrix[7] + In.z * ViewMatrix[11] + ViewMatrix[15];

		if (w < 0.0)
			return;

		Out.x /= w;
		Out.y /= w;
		Out.z /= w;

		Out.x *= screenWidth / 2.0f;
		Out.x += screenWidth / 2.0f;

		Out.y *= -screenHeight / 2.0f;
		Out.y += screenHeight / 2.0f;
	}

	ESP(TEAM team)
	{
		// Setup the brush colours for each team
		if (team == TEAM::ENEMY) {
			colour = 0x0000FF;
		}
		else {
			colour = 0x00FF00;
		}
		brush = CreateSolidBrush(colour);

		// Get the game window's dimensions, required for world-2-screen calculations
		RECT rect;
		hwnd = FindWindowA(NULL, "AssaultCube"); // If we can't get this HANDLE then we're screwed anyway
		hdc = GetDC(hwnd);

		GetWindowRect(hwnd, &rect);
		screenWidth = rect.right - rect.left;
		screenHeight = rect.bottom - rect.top;

		// Debugging garbage
		//std::cout << "Window size: " << screenWidth << " + " << screenHeight << std::endl;
	}
};

