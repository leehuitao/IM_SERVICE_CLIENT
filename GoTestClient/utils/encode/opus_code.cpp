#include "opus_code.h"
#include "Logic/global_center.h"

#define MAX_FRAME_SIZE 6*960

#define MAX_PACKET_SIZE 3*1276
OpusCode::OpusCode(CodeType type) : m_type(type)
{
    initEncoder();
    initDecoder();
}

void OpusCode::initEncoder()
{
    int err;
    int applications[3] = {OPUS_APPLICATION_AUDIO, OPUS_APPLICATION_VOIP, OPUS_APPLICATION_RESTRICTED_LOWDELAY};

    m_encoder = opus_encoder_create(m_sampleRate, m_channelNum, applications[1], &err);

    if(err != OPUS_OK || m_encoder == NULL) {
        printf("打开opus 编码器失败\n");
    }

    opus_encoder_ctl(m_encoder, OPUS_SET_VBR(0));//0:CBR, 1:VBR
    opus_encoder_ctl(m_encoder, OPUS_SET_VBR_CONSTRAINT(true));
    opus_encoder_ctl(m_encoder, OPUS_SET_BITRATE(m_sampleRate));
    opus_encoder_ctl(m_encoder, OPUS_SET_COMPLEXITY(8));//8    0~10
    opus_encoder_ctl(m_encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
    opus_encoder_ctl(m_encoder, OPUS_SET_LSB_DEPTH(16));//每个采样16个bit，2个byte
    opus_encoder_ctl(m_encoder, OPUS_SET_DTX(0));
    opus_encoder_ctl(m_encoder, OPUS_SET_INBAND_FEC(0));

}

void OpusCode::initDecoder()
{
    int err;
    m_decoder = opus_decoder_create(m_sampleRate, m_channelNum, &err);
    opus_decoder_ctl(m_decoder, OPUS_SET_LSB_DEPTH(16));
    if(err < 0 || m_decoder == NULL)
    {
        printf("创建解码器失败\n");
        return;
    }
}

//48000 采样率，48个样本/ms * 20ms * 2 channel = 1920
void OpusCode::encode(QByteArray &arr)
{

    const int input_len = arr.size();
    const int frame_size = 48*20;//960
    opus_int16 input_data[frame_size ] = {0};
    unsigned char out_data[MAX_PACKET_SIZE] = {0};
    memcpy(input_data, arr.data(),input_len);
    auto ret = opus_encode(m_encoder, input_data, frame_size, out_data, MAX_PACKET_SIZE);
    memcpy(arr.data(),out_data,ret);
    if(ret < 0) {
        printf("opus编码失败, %d\n", ret);
        return;
    }
}

void OpusCode::decode(QByteArray &arr)
{
//    auto len = arr.size() ;
//    arr.resize(len*2);
//    opus_int16 out[len];
//    auto frame_size = opus_decode(m_decoder, (const unsigned char *)arr.data(), len, out, len, 0);

//    if (frame_size < 0)
//    {
//        printf("解码失败\n");
//        return;
//    }

//    for (auto i = 0; i < m_channelNum * frame_size; i++)
//    {
//        arr[2 * i] = out[i] & 0xFF;
//        arr[2 * i + 1] = (out[i] >> 8) & 0xFF;
//    }
}

void OpusCode::handleData(QByteArray arr, int type)
{
    if(m_type == CodeType::EncodeType){
        encode(arr);
        signEncodeData(arr);
    }else if(m_type == CodeType::DecodeType){
        decode(arr);
        signDecodeData(arr);
    }else{
        QLOG_ERROR()<<"OpusCode unkown type";
    }
}
