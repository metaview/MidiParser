// parser2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "parser2.h"
#include <Windows.h>
#include <CommDlg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void RedirectIOToConsole();

#define MAX_LOADSTRING 100
#define IDC_CONVERT_BUTTON 101
#define IDC_PARSE_BUTTON 102

// Global Variables:
HWND hWnd;
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PARSER2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PARSER2));
	
	RedirectIOToConsole();

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PARSER2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PARSER2);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 420, 250, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

FILE *getSaveFile()
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("All\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = _T("Save as...");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	// Display the Open dialog box. 

	if (GetSaveFileName(&ofn)==TRUE)
	{
		// Convert to a char*
		size_t origsize = wcslen(ofn.lpstrFile) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		char nstring[newsize];
		wcstombs_s(&convertedChars, nstring, origsize, ofn.lpstrFile, _TRUNCATE);
		return fopen(nstring, "wb");
	}

	return NULL;
}

void convert()
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260];       // buffer for file name
	HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("All\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE)
	{
		hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES) NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL);
		if (hf)
		{
			FILE *f = getSaveFile();
			if (f)
			{
				fprintf(f, "unsigned char midi[] = \n{\n");
				int i = 0;
				unsigned char buffer = 0;
				DWORD read = 0;
				char line[10];

				while (ReadFile(hf, &buffer, 1, &read, NULL) && (read > 0))
				{
					if( (i % 88) == 0 )
					{
						fprintf(f, "\n/*        ");
						for (int ii = 0; ii < 8; ii++)
							fprintf(f, "[%04x]", i + ii);
							fprintf(f, "*/");
					}
					if ((i % 8) == 0)
					{
						if (i > 0)
						{
							line[8] = 0;
							fprintf(f, " // %s ", line); // add a space at the end to prevent "line continuation character"
						}
						fprintf(f, "\n/*[%04x]*/ ", i);
					}

					fprintf(f, "0x%02x, ", buffer);
					line[i % 8] = (buffer < 0x20) ? '.' : ((buffer > 0x7E) ? '.' : buffer);

					i++;
				}
				fprintf(f, "\n};\n");
				fclose(f);
			}
			CloseHandle(hf);
			MessageBox(hWnd, _T("File saved."), _T("Attention"), MB_OK);
		}
	}
}

struct oneChannel
{
	int note;
	int start;
	int end;
};
#define MAX_NOTES 1000
#define MAX_CHANNELS 16
struct oneChannel arrChannels[MAX_CHANNELS][MAX_NOTES];
int arrayIdx[MAX_CHANNELS];

int channels[MAX_CHANNELS];

#define MAX_INSTRUMENT_NAME 32
char chanInstrument[MAX_CHANNELS][MAX_INSTRUMENT_NAME];

#define NOTE_PAUSE	-1

void stopChannel(int chan, int timeStamp)
{
	channels[chan] = 0;
	if ((arrayIdx[chan] < MAX_NOTES) && (arrChannels[chan][arrayIdx[chan]].note > -2))
	{
		arrChannels[chan][arrayIdx[chan]].end = timeStamp;
		arrayIdx[chan]++;
	}
}

void startChannel(int chan, int note, int timeStamp)
{
	channels[chan] = note;
	if (arrayIdx[chan] < MAX_NOTES)
	{
		arrChannels[chan][arrayIdx[chan]].start = timeStamp;
		arrChannels[chan][arrayIdx[chan]].note = note;
	}
}

void parseMidi(BYTE *dataMidi, DWORD sizeMidi)
{
	DWORD i = 0;
	int tag = 0;
	int header = 0;
	int track = 0;
	int lastChannel = 0;
	int maxChannel = 0;
	WORD type = 0;
	WORD trackCnt = 0;
	WORD speed = 0;
	WORD trackIdx = 0;

	for (int k=0; k<MAX_CHANNELS; k++)
	{
		arrayIdx[k] = 0;
		channels[k] = 0;
		chanInstrument[k][0] = 0;
		for (int j=0; j<MAX_NOTES; j++)
		{
			arrChannels[k][j].note = -2;
			arrChannels[k][j].start = 0;
			arrChannels[k][j].end = 0;
		}
	}

	while (i < sizeMidi)
	{
		unsigned char byte = dataMidi[i++];
		if ((byte == 0x4D) && (tag == 0))
		{
			header = 0;
			track = 0;
			tag++;
		}
		else if ((byte == 0x54) && (tag == 1))
		{
			tag++;
		}
		else if ((byte == 0x68) && (tag == 2))
		{
			tag++;
		}
		else if ((byte == 0x64) && (tag == 3))
		{
			header = 1;
			tag = 0;
		}
		else if ((byte == 0x72) && (tag == 2))
		{
			tag++;
		}
		else if ((byte == 0x6B) && (tag == 3))
		{
			track = 1;
			tag = 0;
		}
		else
		{
			tag = 0;
		}
		if (header)
		{
			DWORD size = 0;
			size += dataMidi[i++] << 24;
			size += dataMidi[i++] << 16;
			size += dataMidi[i++] << 8;
			size += dataMidi[i++] << 0;
			if (size != 6)
			{
				printf("Wrong header size\n");
				break;
			}
			type = 0;
			type += dataMidi[i++] << 8;
			type += dataMidi[i++] << 0;
			trackCnt = 0;
			trackCnt += dataMidi[i++] << 8;
			trackCnt += dataMidi[i++] << 0;
			speed = 0;
			speed += dataMidi[i++] << 8;
			speed += dataMidi[i++] << 0;
		}
		else if (track)
		{
			DWORD size = 0;
			size += dataMidi[i++] << 24;
			size += dataMidi[i++] << 16;
			size += dataMidi[i++] << 8;
			size += dataMidi[i++] << 0;

			DWORD end = i + size;
			if (end > sizeMidi)
			{
				printf("Wrong track (%d) size: %d != %d\n", trackIdx, size, sizeMidi-i);
				break;
			}

			if (trackIdx >= MAX_CHANNELS)
			{
				printf("skipping track %d\n", trackIdx);
				i = end;
				continue;
			}
			int sumTime = 0;
			int cmd = 0;
			while (i < end)
			{
				int k;
				int time = 0;
				int note = 0;
				int velocity = 0;
				byte = dataMidi[i++];
				while (byte > 0x7F)
				{
					time += byte & 0x7F;
					time <<= 7;
					byte = dataMidi[i++];
				}
				time += byte;
				sumTime += time;
				byte = dataMidi[i++];
				if (byte > 0x7F)
					cmd = byte; // new cmd received
				else
					i--; // put byte back, it's part of event load
				switch (cmd & 0xF0)
				{
				case 0x80: // note off
					note = dataMidi[i++];
					velocity = dataMidi[i++];
					if (type == 0)
					{
						for (k=0; k<MAX_CHANNELS; k++)
						{
							if (channels[k] == note)
							{
								stopChannel(k, sumTime);
								break;
							}
						}
					}
					else if (type == 1)
					{
						//stopChannel(trackIdx, sumTime);
						for (k=lastChannel; k<MAX_CHANNELS; k++)
						{
							if (channels[k] == note)
							{
								stopChannel(k, sumTime);
								break;
							}
						}
					}
					break;
				case 0x90: // note on
					note = dataMidi[i++];
					velocity = dataMidi[i++];
					if (type == 0)
					{
						for (k=0; k<MAX_CHANNELS; k++)
						{
							if (channels[k] == note)
							{
								// already playing: stop it or retrigger it
								if (velocity == 0)
									stopChannel(k, sumTime);
								else
									startChannel(k, note, sumTime);
								break;
							}
						}
						if (k == MAX_CHANNELS)
						{
							// play it
							for (int k=0; k<MAX_CHANNELS; k++)
							{
								if (channels[k] == 0)
								{
									startChannel(k, note, sumTime);
									break;
								}
							}
						}
					}
					else if (type == 1)
					{
						for (k=lastChannel; k<MAX_CHANNELS; k++)
						{
							if (channels[k] == note)
							{
								// already playing: stop it or retrigger it
								if (velocity == 0)
									stopChannel(k, sumTime);
								else
									startChannel(k, note, sumTime);
								break;
							}
						}
						if (k == MAX_CHANNELS)
						{
							// play it
							for (int k=lastChannel; k<MAX_CHANNELS; k++)
							{
								if (channels[k] == 0)
								{
									startChannel(k, note, sumTime);
									if (k > maxChannel)
										maxChannel = k;
									break;
								}
							}
						}
						//stopChannel(trackIdx, sumTime);
						//if (velocity > 0)
						//	startChannel(trackIdx, note, sumTime);
					}
					break;
				case 0xA0: // after touch
					// ignoring: note and velocity
					dataMidi[i++];
					dataMidi[i++];
					break;
				case 0xB0: // control change
					// ignoring controller and value
					dataMidi[i++];
					dataMidi[i++];
					break;
				case 0xC0: // patch change
					// ignoring instrument
					dataMidi[i++];
					break;
				case 0xD0: // channel pressure
					// ignoring pressure
					dataMidi[i++];
					break;
				case 0xE0: // pitch wheel
					//ignoring pitch value (2 bytes)
					dataMidi[i++];
					dataMidi[i++];
					break;
				case 0xF0: // system msgs
					if (cmd == 0xF0)
					{
						// ignore everything until 0xF7
						while (i < end)
						{
							if (dataMidi[i++] == 0xF7)
								break;
						}
					}
					else if (cmd == 0xF1)
					{
						// ignoring time code
						dataMidi[i++];
					}
					else if (cmd == 0xF2)
					{
						// ignoring song position (2 bytes)
						dataMidi[i++];
						dataMidi[i++];
					}
					else if (cmd == 0xF3)
					{
						// ignoring song selection
						dataMidi[i++];
					}
					else if (cmd == 0xFF)
					{
						// Meta event
						int cmd2 = dataMidi[i++];
						int length = dataMidi[i++];
						if (cmd2 == 0x03)
						{
							// Instrument name?
							strncpy(chanInstrument[trackIdx], (const char *) &dataMidi[i], max(length, MAX_INSTRUMENT_NAME-1));
							chanInstrument[trackIdx][max(length, MAX_INSTRUMENT_NAME-1)] = 0;
						}
						else if (cmd2 == 0x2F)
						{
							// end of track
							for (k=0; k<MAX_CHANNELS; k++)
							{
								stopChannel(k, sumTime);
							}
						}
						i += length;
						if (cmd2 == 0x2F)
							break;
					}
					// ignoring everything else without data
					break;
				default:
					printf("Unknown event %d at %d\n", cmd, i);
					break;
				}

				/*

				if ((cmd & 0xF0) == 0x80)
					printf("Note Off: %d %d %d\n", time, note, velocity);
				else if ((cmd & 0xF0) == 0x90)
					printf("Note On: %d %d %d\n", time, note, velocity);
				else
					printf("Msg: %d %x\n", time, cmd);
				*/
			}
			if (arrayIdx[trackIdx] > 0)
				lastChannel = maxChannel + 1;
				//trackIdx++;
		}
	}
	for (int k=0; k<MAX_CHANNELS; k++)
	{
		// 2 rounds of data refactoring:
		// 1st round: insert pause
		// 2nd round: split events > 255 ticks into some smaller ones (we use 8-bit arays in the µC)
		for (int j=0; j<arrayIdx[k]; j++)
		{
			if (j < (arrayIdx[k]-1))
			{
				// check for pause
				if (arrChannels[k][j+1].start > arrChannels[k][j].end)
				{
					// insert pause
					for (int n=arrayIdx[k]-1; n>j; n--)
						arrChannels[k][n+1] = arrChannels[k][n];
					arrChannels[k][j+1].note = NOTE_PAUSE;
					arrChannels[k][j+1].start = arrChannels[k][j].end;
					arrChannels[k][j+1].end = arrChannels[k][j+2].start;
				}
			}
		}
		for (int j=0; j<arrayIdx[k]; j++)
		{
			if (((arrChannels[k][j].end - arrChannels[k][j].start) * 64 / speed) > 0xffff)
			{
				// split
				for (int n=arrayIdx[k]-1; n>j; n--)
					arrChannels[k][n+1] = arrChannels[k][n];
				arrChannels[k][j+1].note = arrChannels[k][j].note;
				arrChannels[k][j+1].end = arrChannels[k][j].end;
				printf("%d -> ", arrChannels[k][j].end - arrChannels[k][j].start);
				arrChannels[k][j+1].start = arrChannels[k][j].end = arrChannels[k][j].start + (arrChannels[k][j].end - arrChannels[k][j].start) / 2;
				printf("%d, %d\n", arrChannels[k][j].end - arrChannels[k][j].start, arrChannels[k][j+1].end - arrChannels[k][j+1].start);
			}
		}
	}
	for (int k=0; k<MAX_CHANNELS; k++)
	{
		printf("Channel: %3d Length: %6d\n", k, arrayIdx[k]);
	}
	FILE *f = getSaveFile();
	if (f)
	{
		int channelIdx = 1;
		for (int k=0; k<MAX_CHANNELS; k++)
		{
			int sumTime = 0;
			if (arrayIdx[k] > 0)
			{
				fprintf(f, "// Instrument: %s\n", chanInstrument[k]);
				fprintf(f, "const int8_t notes%d[] PROGMEM = {\n", channelIdx);
				for (int j=0; j<arrayIdx[k]; j++)
				{
					if ((j % 16) == 0)
						fprintf(f, "\n");
					if (arrChannels[k][j].note == NOTE_PAUSE)
						fprintf(f, "PAUSE, ");
					else
						fprintf(f, "%5d, ", arrChannels[k][j].note);
				}
				fprintf(f, " STOP\n};\n\n");
				fprintf(f, "const uint16_t duras%d[] PROGMEM = {\n", channelIdx);
				for (int j=0; j<arrayIdx[k]; j++)
				{
					if ((j % 16) == 0)
						fprintf(f, "\n");
					fprintf(f, "%3d, ", (arrChannels[k][j].end - arrChannels[k][j].start) * 64 / speed);
					sumTime += (arrChannels[k][j].end - arrChannels[k][j].start) * 64 / speed;
				}
				fprintf(f, "  0\n}; // %d\n\n", sumTime);
				channelIdx++;
			}
		}
		fprintf(f, "// type: %d\n", type);
		fprintf(f, "// speed: %d\n", speed);
		fclose(f);
		MessageBox(hWnd, _T("File saved."), _T("Attention"), MB_OK);
	}
}

void openMidi()
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260];       // buffer for file name
	HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("All\0*.*\0Midi\0*.mid\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE)
	{
		hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES) NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL);
		if (hf != INVALID_HANDLE_VALUE)
		{
			DWORD midiSize = GetFileSize(hf, NULL);
			BYTE *midiData = (BYTE *) malloc(midiSize);
			if (midiData)
			{
				DWORD read;
				ReadFile(hf, midiData, midiSize, &read, NULL);
				parseMidi(midiData, midiSize);
				free(midiData);
			}
			CloseHandle(hf);
		}
	}
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
			CreateWindowEx(NULL, _T("BUTTON"), _T("Convert Binary Into Include"), WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, 50, 30, 300, 40, hWnd, (HMENU)IDC_CONVERT_BUTTON, GetModuleHandle(NULL), NULL);
			CreateWindowEx(NULL, _T("BUTTON"), _T("Parse Midi file"), WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, 50, 100, 300, 40, hWnd, (HMENU)IDC_PARSE_BUTTON, GetModuleHandle(NULL), NULL);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDC_CONVERT_BUTTON:
			convert();
			break;
		case IDC_PARSE_BUTTON:
			openMidi();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
