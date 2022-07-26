#pragma once
#include <stdio.h>
#include <string.h>
#include "./tinyfiledialogs/tinyfiledialogs.h"
//#include <shapes_loader_saver.h>
#include <imgui/imgui.h>

#include <list>
#include <memory>
//#include "shape.h"



#include <filename.h>
//#include <status.h>

#ifdef _MSC_VER
#pragma warning(disable:4996) /* silences warning about wcscpy*/
#endif

//using namespace std;

class FileManager
{

public: 

	int lIntValue;
	//char const* lPassword;
	char const* lTheSaveFileName;
	char const* lTheOpenFileName;
	//char const* lTheSelectFolderName;
	//char const* lTheHexColor;
	//char const* lWillBeGraphicMode;
	//unsigned char lRgbColor[3];
	FILE* lIn;
	char lBuffer[1024];
	char const* lFilterPatterns[2] = { "*.txt", "*.text" };

	FileManager() {
		//(void)argv; /*to silence stupid visual studio warning*/

		//tinyfd_verbose = argc - 1;  /* default is 0 */
		tinyfd_verbose = 0;  /* default is 0 */
		tinyfd_silent = 1;  /* default is 1 */

		tinyfd_forceConsole = 0; /* default is 0 */
		tinyfd_assumeGraphicDisplay = 0; /* default is 0 */

#ifdef _WIN32
		tinyfd_winUtf8 = 1; /* default is 1 */
   /* On windows, you decide if char holds 1:UTF-8(default) or 0:MBCS */
   /* Windows is not ready to handle UTF-8 as many char functions like fopen() expect MBCS filenames.*/
   /* This hello.c file has been prepared, on windows, to convert the filenames from UTF-8 to UTF-16
	  and pass them passed to _wfopen() instead of fopen() */
#endif
		printf("Se creo un FileManager\n");
	}

	~FileManager() {
		printf("Se destruyo un FileManager\n");
	}

	int init(void) /* WINDOWS ONLY */
	{
		;
	}

	int open() /* WINDOWS ONLY */
	{
	
		lTheOpenFileName = tinyfd_openFileDialog(
			"Open Shapes File",
			"",
			2,
			lFilterPatterns,
			NULL,
			0);

		if (!lTheOpenFileName)
		{
			tinyfd_messageBox(
				"Error",
				"Open file name is NULL",
				"ok",
				"error",
				0);
			return 1;
		}

		/*
#ifdef _WIN32
		if (tinyfd_winUtf8)
			lIn = _wfopen(tinyfd_utf8to16(lTheOpenFileName), L"r"); // the UTF-8 filename is converted to UTF-16 
		else
#endif
			lIn = fopen(lTheOpenFileName, "r");
		
		if (!lIn)
		{
			tinyfd_messageBox(
				"Error",
				"Can not open this file in read mode",
				"ok",
				"error",
				1);
			return(1);
		}

		lBuffer[0] = '\0';
		fgets(lBuffer, sizeof(lBuffer), lIn);
		fclose(lIn);
		*/

		printf("The select Shapes File to load is: %s\n", tinyfd_utf8toMbcs(lTheOpenFileName));
		current_file_name = tinyfd_utf8toMbcs(lTheOpenFileName);
		current_file_mode = 1;
		printf("[%d] current_file_name: %s\n", current_file_mode, current_file_name.c_str());
		//tinyfd_beep();

		return 0;
	}

	int save(bool current) /* WINDOWS ONLY */
	{
		if (!current) {
			lTheSaveFileName = tinyfd_saveFileDialog(
				"Lets save ur canvas on a Shapes File",
				"ShapesFile.txt",
				2,
				lFilterPatterns,
				NULL);
		}
		else {
			lTheSaveFileName = current_file_name.c_str();
		}
		if (!lTheSaveFileName)
		{
			tinyfd_messageBox(
				"Error",
				"Save file name is NULL",
				"ok",
				"error",
				1);
			return 1;
		}
		/*
#ifdef _WIN32
		if (tinyfd_winUtf8)
			lIn = _wfopen(tinyfd_utf8to16(lTheSaveFileName), L"w"); // the UTF-8 filename is converted to UTF-16 to open the file
		else
#endif
			lIn = fopen(lTheSaveFileName, "w");

		if (!lIn)
		{
			tinyfd_messageBox(
				"Error",
				"Can not open this file in write mode",
				"ok",
				"error",
				1);
			return 1;
		}
		//fputs(lPassword, lIn); // Handle saving file
		fclose(lIn);
		*/

		printf("The selected Shapes File to save is: %s\n", tinyfd_utf8toMbcs(lTheSaveFileName));
		current_file_name = tinyfd_utf8toMbcs(lTheSaveFileName);
		current_file_mode = 2;
		printf("[%d] current_file_name: %s\n", current_file_mode, current_file_name.c_str());
	}

};


#ifdef _MSC_VER
#pragma warning(default:4996)
#endif
