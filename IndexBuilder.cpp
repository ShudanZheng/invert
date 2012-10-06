#include "IndexBuilder.h"
#include "Utility.h"

void CIndexBuilder::Clear()
{
	m_workMode = WORKMODE_TEST; 
	m_termSet.clear(); 
	m_wordList.clear(); 
	m_postingList.clear(); 
	m_stopWordList.clear(); 
	m_docuNameVec.clear(); 

	m_numDocus = 0; 
	m_numTerms = 0; 
	m_numStopWords = 0; 
	m_numWords = 0; 
}

void CIndexBuilder::Initialize( string _configFName )
{
	m_configFName = _configFName;
	m_config = CConfigFile(m_configFName); 
	ReadParameters(m_config); 
}

void CIndexBuilder::Run()
{
	switch (m_workMode)
	{
	case WORKMODE_BUILD: 
		printf("Build and save an index.\n"); 
		LoadData(m_dataPath, m_stopListPath); 
		BuildPostingsList(); 
		BuildWordList(); 
		SaveList(m_resultPath); 
		break; 
	case WORKMODE_TEST: 
		LoadData(m_dataPath, m_stopListPath); 
		BuildPostingsList(); 
		BuildWordList(); 
		TestInterface(); 
		break; 
	default: 
		break; 
	}
}

void CIndexBuilder::ReadParameters( CConfigFile _config )
{
	m_dataPath = _config.read<string>("DATA_PATH"); 
	m_stopListPath = _config.read<string>("STOPLIST_PATH"); 
	m_resultPath = _config.read<string>("RESULT_PATH"); 
	string str_workMode = _config.read<string>("WORK_MODE"); 

	if (str_workMode == "TEST")
	{
		m_workMode = WORKMODE_TEST; 
	}
	else if (str_workMode == "BUILD")
	{
		m_workMode = WORKMODE_BUILD; 
	}
}

void CIndexBuilder::LoadData( string _dataPath, string _stopListPath )
{
	m_numDocus = utility::CUtility::GetFilesInFolder(_dataPath, m_docuNameVec); 

	FOR (i, m_numDocus)
	{
		ifstream fin(_dataPath + m_docuNameVec[i]);
		int location = 0; 

		while (!fin.eof())
		{
			CTerm term; 
			fin >> term.m_word; 
			term.m_fileIdx = i; 
			term.m_locIdx = location; 
			location++; 
			m_termSet.push_back(term); 
		}

		fin.close(); 
	}

	m_numTerms = (int)m_termSet.size(); 

	ifstream stopFin(_stopListPath); 
	
	while (!stopFin.eof())
	{
		string stopWord; 
		stopFin >> stopWord; 
		m_stopWordList.push_back(stopWord); 
	}

	stopFin.close(); 
	m_numStopWords = (int)m_stopWordList.size(); 
}

void CIndexBuilder::BuildWordList()
{
	printf("Building Word list.\n"); 
	FOR (i, m_numTerms)
	{
		CTerm term = m_termSet[i]; 

		if (m_wordList.find(term.m_word) == m_wordList.end())
		{
			m_wordList.insert(make_pair(term.m_word, 1)); 
		}
	}

	FOR (i, m_numStopWords)
	{
		m_wordList.erase(m_stopWordList[i]); 
	}

	for (map<string, int>::iterator pos = m_wordList.begin(); pos != m_wordList.end(); pos++)
	{
		CPosting posting = m_postingList[pos->first]; 
		pos->second = (int)posting.m_docuList->size(); 
	}

	m_numWords = (int)m_wordList.size(); 
}

void CIndexBuilder::BuildPostingsList()
{
	printf("Building Postings list.\n"); 
	FOR (i, m_numTerms)
	{
		CTerm term = m_termSet[i]; 

		if (m_postingList.find(term.m_word) == m_postingList.end())
		{
			CPosting posting; 
			posting.m_postingList = new vector<vector<int>*>(); 
			posting.m_docuList = new map<int, int>(); 
			posting.m_docuList->insert(make_pair(term.m_fileIdx, 0));
			vector<int>* postinFile = new vector<int>(); 
			postinFile->push_back(term.m_locIdx); 
			posting.m_postingList->push_back(postinFile); 
			m_postingList.insert(make_pair(term.m_word, posting)); 
		}
		else
		{
			CPosting posting = m_postingList[term.m_word]; 

			if (posting.m_docuList->find(term.m_fileIdx) == posting.m_docuList->end())
			{
				posting.m_docuList->insert(make_pair(term.m_fileIdx, (int)posting.m_docuList->size())); 
				vector<int>* postinFile = new vector<int>(); 
				postinFile->push_back(term.m_locIdx); 
				posting.m_postingList->push_back(postinFile); 
			}
			else 
			{
				int mapIdx = (*posting.m_docuList)[term.m_fileIdx]; 
				(*posting.m_postingList)[mapIdx]->push_back(term.m_locIdx); 
			}
		}
	}

	FOR (i, m_numStopWords)
	{
		m_postingList.erase(m_stopWordList[i]); 
	}
}

void CIndexBuilder::SaveList( string _savePath )
{
	ofstream wordFout(_savePath+"wordlist.txt"); 
	ofstream postFout(_savePath+"postingsfile.txt"); 

	for (map<string, int>::iterator pos = m_wordList.begin(); pos != m_wordList.end(); pos++)
	{
		wordFout << pos->first << " " << pos->second << endl; 
	}

	wordFout.close(); 
	printf("Save Word list to %s\n", (_savePath+"wordlist.txt").c_str()); 

	for (map<string, CPosting>::iterator pos = m_postingList.begin(); pos != m_postingList.end(); pos++)
	{
		postFout << pos->first << endl; 
		CPosting posting = pos->second; 
		int numFiles = posting.m_docuList->size(); 
		int freq = 0; 

		FOR (i, numFiles)
		{
			freq += (*posting.m_postingList)[i]->size(); 
		}

		postFout << numFiles << " " << freq << endl; 
		int i = 0; 

		for (map<int, int>::iterator post_pos = posting.m_docuList->begin(); post_pos != posting.m_docuList->end(); post_pos++, i++)
		{
			int freqEachFile = (int)(*posting.m_postingList)[i]->size(); 
			postFout << "  " << m_docuNameVec[post_pos->first] << " [" << freqEachFile << "]:"; 
			FOR (j, freqEachFile)
			{
				postFout << " " << (*(*posting.m_postingList)[i])[j]; 
			}
			postFout << endl; 
		}
	}

	postFout.close(); 
	printf("Save Postings file to %s\n", (_savePath+"postingsfile.txt").c_str()); 

}

void CIndexBuilder::TestInterface()
{
	printf("Start test interface.\n"); 

	while (true)
	{
		printf("Please enter query word:\n"); 
		string line; 
		getline(cin, line); 

		if (line == "ZZZ")
		{
			printf("exit the program.\n"); 
			exit(0); 
		}

		if (line.empty())
		{
			printf("Empty input!!!"); 
			continue;
		}

		vectorString queryList; 
		
		while (!line.empty())
		{
			if (line.find(" ") != string::npos)
			{
				string term = line.substr(0, line.find(" ")); 
				line = line.substr(line.find(" ")+1); 
				queryList.push_back(term); 
			}
			else
			{
				queryList.push_back(line); 
				break; 
			}
		}
		
		if (queryList.size() == 1)
		{
			string query = queryList[0]; 

			if (m_postingList.find(query) == m_postingList.end())
			{// none of the terms are in the word list
				printf("Find nothing!!!"); 
			}
			else
			{
				CPosting posting = m_postingList[query]; 
				int numFiles = posting.m_docuList->size(); 
				map<int, int>::iterator pos = posting.m_docuList->begin(); 

				FOR (i, numFiles)
				{
					float tf = 1 + log10((float)(*posting.m_postingList)[i]->size()); 
					float idf = 1 + log10(m_numDocus / (float)m_wordList[query]); 
					float tfidf = tf * idf;

					printf("[posting %d]:\n", i); 
					printf("%f, %f, %f\n", tf, idf, tfidf); 
					printf("%d", pos->first); 
					
					FOR (j, (int)(*posting.m_postingList)[i]->size())
					{
						printf(", %d", (*(*posting.m_postingList)[i])[j]); 
					}

					printf("\n"); 

					int location  = (*(*posting.m_postingList)[i])[0]; 
					ifstream fin(m_dataPath+m_docuNameVec[pos->first]); 
					int idx = 0; 
					string word;

					while (!fin.eof() && idx < max(1, location-5))
					{
						fin >> word; 
						idx++; 
					}

					FOR (i, 11)
					{
						fin >> word; 
						if (i != location - max(1, location-5))
						{
							printf("%s ", word.c_str()); 
						}
						else 
						{
							printf("[%s] ", word.c_str()); 
						}
						idx++; 
					}

					fin.close(); 
					pos++; 
					printf("\n\n"); 
				}
			}
		}
		else 
		{
			vector<int> isAll(m_numDocus, 0);
			vector<float> tfidfAll(m_numDocus, 0.0); 
			
			FOR (i, (int)queryList.size())
			{
				string query = queryList[i]; 
				CPosting posting = m_postingList[query]; 
				int numFiles = posting.m_docuList->size(); 
				map<int, int>::iterator pos = posting.m_docuList->begin(); 

				FOR (j, numFiles)
				{
					float tf = 1 + log10((float)(*posting.m_postingList)[j]->size()); 
					float idf = 1 + log10(m_numDocus / (float)m_wordList[query]); 
					float tfidf = tf * idf;
					int fileIdx = pos->first; 

					tfidfAll[fileIdx] += tfidf; 
					isAll[fileIdx]++; 
					pos++; 
				}
			}

			vector<SortableElement<float>> sortTFIDF; 
			
			FOR (i, m_numDocus)
			{
				if (isAll[i] == (int)queryList.size())
				{
					sortTFIDF.push_back(SortableElement<float>(tfidfAll[i], i)); 
				}
			}

			vector<int> order; 
			sortOrder(sortTFIDF, order); 

			if (sortTFIDF.empty())
			{
				printf("Find nothing!!!"); 
			}
			else
			{
				for (int i = sortTFIDF.size() - 1; i >= 0; i--)
				{
					printf("%d, %f\n", sortTFIDF[i]._ind, sortTFIDF[i]._val); 
				}
			}
		}

		queryList.clear(); 
		printf("\n"); 
	}
}
