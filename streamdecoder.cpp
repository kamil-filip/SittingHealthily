#include "streamdecoder.h"
#include <thread>
#include <chrono>

StreamDecoder::StreamDecoder()
{

}

StreamDecoder::StreamDecoder(QByteArray const &data)
{
    readLine = QString(data).toStdString();
}

StreamDecoder::StreamDecoder(std::string const &data)
{
    readLine = data;
}

StreamDecoder::StreamDecoder(QString const &data)
{
    readLine = data.toStdString();
}

std::vector<std::string> StreamDecoder::explode(std::string const& s, char const & c)
{
    std::string buff{""};
    std::vector<std::string> v;

    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);

    v.pop_back();
    return v;
}

float StreamDecoder::decodeFloat(std::string const &inString)
{
   /* std::vector<unsigned char> binData = unhex(inString);

    int intbits = (binData[3] << 24) | ((binData[2] & 0xff) << 16) | ((binData[1] & 0xff) << 8) | (binData[0] & 0xff);

    u.i = intbits;
    return u.f;
*/
    static unsigned char H[256], L[256];
      if (L['1'] == 0)
      {
          for (int i=0; i<10; i++)
          {
              H['0'+i] = (i<<4);
              L['0'+i] = i;
          }
          for (int i=0; i<6; i++)
          {
              H['a'+i] = H['A'+i] = ((10+i)<<4);
              L['a'+i] = L['A'+i] = (10+i);
          }
      }
      std::vector<unsigned char> result(inString.size()>>1);
      for (int i=0,n=inString.size(); i<n-1; i+=2)
        result[i>>1] = H[inString[i]]+L[inString[i+1]];




    int intbits = (result[3] << 24) | ((result[2] & 0xff) << 16) | ((result[1] & 0xff) << 8) | (result[0] & 0xff);


     u.i = intbits;
     return u.f;


}



std::vector<unsigned char> StreamDecoder::unhex(std::string const &inString)
{
    unsigned char H[256], L[256];
    if (L['1'] == 0)
    {
        for (int i=0; i<10; i++)
        {
            H['0'+i] = (i<<4);
            L['0'+i] = i;
        }
        for (int i=0; i<6; i++)
        {
            H['a'+i] = H['A'+i] = ((10+i)<<4);
            L['a'+i] = L['A'+i] = (10+i);
        }
    }

    std::vector<unsigned char> result(inString.size()>>1);

    for (int i=0,n=inString.size(); i<n-1; i+=2)
        result[i>>1] = H[inString[i]]+L[inString[i+1]];

    return result;
}

std::vector<float> StreamDecoder::getQuaternions()
{
    std::vector<float> uncodedValues { 0.0, 0.0, 0.0, 0.0 };
    std::vector<std::string> vecOfIn = explode(readLine, ',');
    auto uncodedItarator = uncodedValues.begin();


    for (auto& it : vecOfIn)
        if ( it.length() == 8 )
            *uncodedItarator++ = decodeFloat(it);

    return uncodedValues;
}

void StreamDecoder::setStreamLine(QByteArray const &data)
{
    readLine = QString(data).toStdString();
}

void StreamDecoder::setStreamLine(std::string const &data)
{
    readLine = data;
}

void StreamDecoder::setStreamLine(QString const &data)
{
    readLine = data.toStdString();
}
