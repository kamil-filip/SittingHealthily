#ifndef STREAMDECODER_H
#define STREAMDECODER_H

#include <QByteArray>
#include <QString>
#include <string>
#include <vector>

class StreamDecoder
{
    union {
        float f;  // assuming 32-bit IEEE 754 single-precision
        int i;    // assuming 32-bit 2's complement int
     } u;

public:
    StreamDecoder();
    StreamDecoder(QByteArray const &data);
    StreamDecoder(std::string const &data);
    StreamDecoder(QString const &data);

    std::vector<float> getQuaternions();
    std::vector<float> getEurlers();
    void updateEurlers(std::vector<float> &eurlerAngles);

    void setStreamLine(QByteArray const &data);
    void setStreamLine(std::string const &data);
    void setStreamLine(QString const &data);

private:
    std::string readLine;

    std::vector<unsigned char> unhex(std::string const &inString);
    std::vector<std::string> explode(std::string const &inString, char const & curChar);
    float decodeFloat(std::string const &inString);
    std::vector<float> quaternionToEuler(std::vector<float> const  &q);




};

#endif // STREAMDECODER_H
