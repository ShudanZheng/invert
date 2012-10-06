#include "IndexBuilder.h"
#include "Timer.h"

int main()
{
	CTimer timer("Building an Index"); 
	CIndexBuilder* builder = new CIndexBuilder(); 
	builder->Initialize("invert.config"); 
	builder->Run(); 
	DELETE_OBJECT(builder); 
	return 0; 
}