#include <iostream>
#include "curl/curl.h"
#include "json/json.h"

using namespace std;

void usage()
{
	cout << "Usage: translate word_you_want_to_translate" << endl;
}

int writer(char *data, size_t size, size_t nmemb, string *writerData)
{
	if (writerData == NULL)
		return 0;
	int len = size*nmemb;
	writerData->append(data, len); 
	return len;
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		usage();
		exit(0);
	}
	string buffer;
	string translate_url = "http://fanyi.youdao.com/openapi.do?keyfrom=xxxxxx&key=xxxxxx&type=data&doctype=json&version=1.1&q=";
	CURL * curl;
	CURLcode res;
	curl = curl_easy_init();

	int type;

	if ((*argv[1]&0x80) != 0)
	{
		type = 0;
		char *encode_word = curl_easy_escape(curl, argv[1], 0);
		translate_url += encode_word;
	} else {
		type = 1;
		translate_url += argv[1];
	}

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, translate_url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	if (buffer.empty())
	{
		cout << "The server return NULL!" << endl;
		exit(0);
	}
	
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(buffer, root);
	
	if (!parsingSuccessful)
	{
		cout << "Failed to parse the data!" << endl;
		exit(0);
	}

	if (root["errorCode"] != 0)
	{
		cout << "errorCode: " << root["errorCode"] << endl;
		exit(0);
	}
	const Json::Value basic = root["basic"];
	const Json::Value phonetic = basic["phonetic"];
	const Json::Value explains = basic["explains"];
	cout << "Provided by Youdao dictionary!" << endl;
	cout << "-----------------------------" << endl;
	const char *prefix = type == 0 ? "[" : "英[";
	cout << argv[1] << "\t" << prefix << phonetic.asString()  << "]" << endl;
	
	for(int i = 0; i < explains.size(); ++i)
		cout << explains[i].asString() << endl;

	return 0;
}
