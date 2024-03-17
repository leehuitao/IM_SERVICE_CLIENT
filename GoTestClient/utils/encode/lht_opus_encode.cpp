#include "lht_opus_encode.h"

LhtOpusEncode::LhtOpusEncode(EncodeCallback *callback, LhtCodeType codeType, QObject *parent):
    EncodeBasic(callback,codeType,parent)
{

}

void LhtOpusEncode::initEncoder()
{
    int32_t err = 0;
    if(m_codeType == LhtEncode){
        m_encoder = opus_encoder_create(m_sample * m_channel , 1, OPUS_APPLICATION_AUDIO, &err);
        if (err != OPUS_OK) {
            fprintf(stderr, "failed to create encoder: %s\n", opus_strerror(err));
            return;
        }
    }else if(m_codeType == LhtDecode){
        // 创建 Opus 解码器
        m_decoder = opus_decoder_create(m_sample * m_channel, 1, &err);
        if (err != OPUS_OK) {
            fprintf(stderr, "failed to create decoder: %s\n", opus_strerror(err));
            return;
        }
    }


}

QByteArray LhtOpusEncode::encode(char *data, int len)
{
    if(!m_encoder){
        return QByteArray();
    }
    // 进行编码
    int ret = opus_encode(m_encoder, (const opus_int16 *)data, len / 2, m_encodeData, sizeof(m_encodeData));
    return QByteArray((char *)m_encodeData,ret);
}

QByteArray LhtOpusEncode::decode(char *data, int len)
{
    if(!m_decoder){
        return QByteArray();
    }
    // 解码数据
    UdpPack pack({data,len},1);
    int ret = opus_decode(m_decoder, (const unsigned char *)pack.data.data(), pack.data.length(), m_decodeData, m_frameSize, 0);
    if (ret < 0) {
        qDebug()<< "decoder error: "<< opus_strerror(ret);
        return QByteArray();
    }
    return  QByteArray((char *)m_decodeData,ret * 2);
}
