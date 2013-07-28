//===================================================================================================================================================================================
#include "PEExplorer.h"

using namespace std;

//===================================================================================================================================================================================
void PEExplorer::LogEvent(const char *cError)
{
	//获取系统时间
	time_t t; 
	tm *lt;
	char cDateTime[20];

	t=time(0); 
	lt=localtime(&t);
	sprintf(cDateTime,"%04d-%02d-%02d	%02d:%02d:%02d",lt->tm_year+1900,lt->tm_mon+1,lt->tm_mday,lt->tm_hour,lt->tm_min,lt->tm_sec);
	cDateTime[19]='\0';

	char buffer[MAX_PATH];
	if(0!=GetModuleFileName(NULL,buffer,MAX_PATH))
	{
		char *ch=strrchr(buffer,'\\');
		if(ch!=NULL)
		{
			*(ch+1)='\0';
			if((strlen(buffer)+strlen("Dump.log"))<MAX_PATH)
			{
				strcat(buffer,"Dump.log");
				ofstream mystream;
				mystream.open(buffer,ios::app);
				mystream.seekp(ios::end);
				if(!mystream.bad())
				{
					mystream<<cDateTime<<"	"<<cError<<endl;
				}
			}
		}
	}
}

//===================================================================================================================================================================================
int PEExplorer::GetExeBit(const char *ExeFile)
{
	int result = EXE_BIT_UNKONWN;

	FILE * peFile = NULL;  
	fopen_s(&peFile, ExeFile, "rb");  
	if (peFile == NULL)  
	{  
		fclose(peFile);  
		result = EXE_BIT_UNKONWN;
	}  
	else
	{
		IMAGE_DOS_HEADER imageDosHeader;  
		fread(&imageDosHeader, sizeof(IMAGE_DOS_HEADER), 1, peFile);  
		if (imageDosHeader.e_magic != IMAGE_DOS_SIGNATURE)  
		{  
			fclose(peFile);  
			result = EXE_BIT_UNKONWN; 
		}  
		else
		{
			IMAGE_NT_HEADERS imageNtHeaders;  
			fseek(peFile, imageDosHeader.e_lfanew, SEEK_SET);  
			fread(&imageNtHeaders, sizeof(IMAGE_NT_HEADERS), 1, peFile);  
			fclose(peFile);  
			if (imageNtHeaders.Signature != IMAGE_NT_SIGNATURE)  
			{  
				result = EXE_BIT_UNKONWN;
			}  
			else
			{
				if (imageNtHeaders.FileHeader.Machine == IMAGE_FILE_MACHINE_I386)  
				{  
					result = EXE_BIT_32; 
				}
				else if (imageNtHeaders.FileHeader.Machine == IMAGE_FILE_MACHINE_IA64 ||  
					imageNtHeaders.FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)  
				{  
					result = EXE_BIT_64;  
				}
			}
		}
	}

	return result;
}

//===================================================================================================================================================================================
