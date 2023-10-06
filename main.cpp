#include<iostream>
#include "include/tal_asr_api.h"
#include "include/tal_asr_define.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <algorithm>

void * asr_resource = nullptr;
void * GetDecModule()
{
    void * dec;
    int ret = -1;
    
    if((ret = TALASRInstanceCreate(asr_resource, &dec)) || !dec)
    {
        std::cout << "asr create failed:" << ret << std::endl;
        return nullptr;
    }
    if((ret = TalASRInstanceStart(dec, ""))){
        std::cout << "asr started failed:" << ret << std::endl;
        return nullptr;
    }
    return dec;
}
void Init() 
{
    const char *mod_dir = "../../../model/ETE-Chn-0-GEN";
    // 资源初始化状态, 0表示初始化成功, 其它表示异常
    int ret = TalASRResourceImport(mod_dir, &asr_resource);
    if(ret != 0 || !asr_resource) 
    {
        std::cout << "failed to load alg mod ret: " << ret << std::endl;
        exit(1);
    }
    
}


void getResult(std::string voicePath,std::string &result_list)
{
void *dec = GetDecModule();
    if(!dec)
    {
        std::cout << "got empty instance" << std::endl;
        return;
    }
    int ret = -1;

    std::ifstream audioFileStream(voicePath, std::ios::binary);
    if (!audioFileStream) 
    {
        std::cerr << "Failed to open audio file." << std::endl;
        return;
    }
    //std::string result = "";
    std::string audioData((std::istreambuf_iterator<char>(audioFileStream)),std::istreambuf_iterator<char>());
    int step=3200;
    int len=audioData.length();
    for(int i=0;i<len;i+=step)
    {
        int real_len = i + step >= len ? len - i : step;
        TalASRInstanceRecognize(dec, const_cast<char*>(audioData.c_str()) + i, real_len, result_list);
    }
    int res_stat = TalASRInstanceFinalRecognize(dec, nullptr, 0, result_list);
    //std::cout<<result<<std::endl;

}
std::string extractResult(const std::string& input) 
{
    std::string resultField = "\"result\":\"";
    size_t startPos = input.find(resultField);
    if (startPos == std::string::npos) 
    {
        return "";  
    }
    startPos += resultField.length();
    size_t endPos = input.find("\"", startPos);
    if (endPos == std::string::npos) 
    {
        return "";  
    }

    return input.substr(startPos, endPos - startPos);
}
std::string generateFormattedJSON(const std::string& input) {
    std::string output = "[";
    size_t startPos = input.find_first_of('[') + 1;
    size_t endPos = input.find_last_of(']');

    if (startPos == std::string::npos || endPos == std::string::npos) {
        return "";
    }

    std::string jsonArray = input.substr(startPos, endPos - startPos);
    size_t pos = 0;
    while (pos < jsonArray.length()) {
        size_t nextBegin = jsonArray.find("{\"beginTime\":", pos);
        if (nextBegin == std::string::npos) {
            break;
        }
        size_t nextEnd = jsonArray.find("}", nextBegin);
        if (nextEnd == std::string::npos) {
            break;
        }
        std::string entry = jsonArray.substr(nextBegin, nextEnd - nextBegin + 1);
        output += entry + ",\n";
        pos = nextEnd + 1;
    }

    output.erase(output.end() - 2, output.end()); // Remove the last comma and newline
    output += "],\n";

    size_t nbestPos = input.find("\"nbest\":[");
    if (nbestPos != std::string::npos) {
        size_t nbestEndPos = input.find_first_of(']', nbestPos);
        if (nbestEndPos != std::string::npos) {
            std::string nbestArray = input.substr(nbestPos, nbestEndPos - nbestPos + 1);
            output += nbestArray + ",\n";
        }
    }
    size_t resultPos = input.find("\"result\":");
    if (resultPos != std::string::npos) {
        size_t resultEndPos = input.find_first_of(',', resultPos);
        if (resultEndPos != std::string::npos) {
            std::string resultEntry = input.substr(resultPos, resultEndPos - resultPos);
            output += resultEntry + "}";
        }
    }
    //output += "}";
    return output;
}
int main()
{
    std::string path = "../../../cs.wav";
    std::string result;

    Init();
    getResult(path, result);
    result = generateFormattedJSON(result);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    std::cout << result << std::endl;
    return 0;
}