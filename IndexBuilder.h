#ifndef INDEXBUILDER_H_
#define INDEXBUILDER_H_
#include "config.h"
#include "ConfigFile.h"

class CTerm
{//Term
public: 
	string m_word;   //word content
	int m_fileIdx;   //file index
	int m_locIdx;    //location
};

class CPosting
{//posting
public: 
	vector<vector<int>*>* m_postingList;   
	map<int, int>* m_docuList; 
};


class CIndexBuilder
{
public: 
	CIndexBuilder()  {Clear(); }
	~CIndexBuilder() {Clear(); }
	//initialize
	void Initialize(string _configFName);
	//run program based on work mode
	void Run(); 

private:
	//clear
	void Clear();
	//read parameters from file
	void ReadParameters(CConfigFile _config); 
	//load text data from file
	void LoadData(string _dataPath, string _stopListPath); 
	//build word list 
	void BuildWordList(); 
	//build posting list
	void BuildPostingsList(); 
	//save word list and posting list to file
	void SaveList(string _savePath); 
	//test interface
	void TestInterface(); 

private: 
	//config file name
	string m_configFName; 
	//text data folder path
	string m_dataPath; 
	//stop list word path
	string m_stopListPath;
	//save result path
	string m_resultPath; 

	//config 
	CConfigFile m_config;
	//enum work mode: 
	//BUILD: build word list and postings list and save them to file
	//TEST: build word list and postings list and start test interface
	enum EWorkMode
	{
		WORKMODE_BUILD,
		WORKMODE_TEST, 
	} m_workMode;

	//document names
	vectorString m_docuNameVec; 
	//terms
	vector<CTerm> m_termSet; 
	//stop words
	vectorString m_stopWordList; 
	//word list 
	map<string, int> m_wordList; 
	//postings list
	map<string, CPosting> m_postingList; 
	
	//#documents
	int m_numDocus;
	//#terms
	int m_numTerms; 
	//#words
	int m_numWords; 
	//#stop words
	int m_numStopWords; 
};

#endif //INDEXBUILDER_H_