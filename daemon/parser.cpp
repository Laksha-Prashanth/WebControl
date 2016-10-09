#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <curl/curl.h>
#include "json/json.h"
#include "json/json-forwards.h"

using namespace std;

#define REFRESH 2

string currentKey;

size_t getData(void *buffer, size_t size, size_t nmemb, void *userp)
{

	Json::Value* root = (Json::Value *)userp;
	Json::Reader reader;

	reader.parse((char*)buffer,(*root));
	Json::Value::iterator a;
	for(a = (*root).begin();a != (*root).end(); a++)
	{
		string code = (*a).get("code","").asString();;
		currentKey = a.key().asString();
		cout<<code<<endl;
		system("");
	}

	return size*nmemb;
}

CURLcode writeData(CURL* curl,Json::Value root)
{
	Json::FastWriter writer;
	string value = writer.write(root);
	cout<<value<<endl;
	string url = "https://testproject-9ac78.firebaseio.com/testData/read.json";
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());  
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value.c_str());
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
	//while(true)
	{
		time(&current);

		//if(current - start < REFRESH)
			//continue;

		Json::Value root;
		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, "https://testproject-9ac78.firebaseio.com/testData/unread.json?orderBy=\"i\"&limitToFirst=1");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&root);

		res = curl_easy_perform(curl);
		cout<<root.begin().key().asString()<<endl;

		if(res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			return 1;
		}

		res = writeData(curl,root);

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
