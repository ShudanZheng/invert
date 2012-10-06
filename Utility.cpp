#include "Utility.h"
const int MAX_FILENAME_LENGTH = 256; 

 int utility::CUtility::GetFilesInFolder(string fName, vector<string>& nameVec)
{
	assert(fName.length() != 0); 

	WIN32_FIND_DATA fileName;
	WCHAR wch_fileName[MAX_FILENAME_LENGTH];

	fName += "*.*";
	MultiByteToWideChar(0, 0, fName.c_str(), MAX_FILENAME_LENGTH-1, wch_fileName, MAX_FILENAME_LENGTH);

	LPCWSTR wstr_fileName = wch_fileName;
	HANDLE handle = FindFirstFile(wstr_fileName,&fileName);
	int nFiles = 0; 

	if(handle != INVALID_HANDLE_VALUE) 
	{
		do
		{
			nFiles++;

			int nLen = lstrlen(fileName.cFileName); 
			char* nPtr = new char [nLen + 1];

			FOR (i, nLen)
			{
				nPtr[i] = char(fileName.cFileName[i]);
			}

			nPtr[lstrlen(fileName.cFileName)] = '\0';

			if (nFiles != 1 && nFiles != 2)
			{
				string strFileName = string(nPtr); 
				nameVec.push_back(strFileName); 
			}
		} 
		while(FindNextFile(handle, &fileName));
	} 

	FindClose(handle);
	return nameVec.size(); 
 }