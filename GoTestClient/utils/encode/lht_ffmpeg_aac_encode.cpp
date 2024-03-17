#include "lht_ffmpeg_aac_encode.h"
#include <fstream>

LhtFFmpegAacEncode::LhtFFmpegAacEncode(EncodeCallback *callback, LhtCodeType codeType, QObject *parent)
    :EncodeBasic(callback,codeType,parent)
{

}

void LhtFFmpegAacEncode::initEncoder()
{
    if(m_codeType == LhtEncode){
        m_inputFrame = av_frame_alloc();
        m_inputFrame->sample_rate = 44100;
        m_inputFrame->ch_layout.order = AV_CHANNEL_ORDER_NATIVE;
        m_inputFrame->ch_layout.nb_channels = 2;
        m_inputFrame->ch_layout.u.mask = AV_CH_LAYOUT_STEREO;
        m_inputFrame->format = AV_SAMPLE_FMT_S16;  // libfdk_aac默认的format
        m_inputFrame->nb_samples = 1024;           // LC
        int ret = av_frame_get_buffer(m_inputFrame, 0);      // 填充frame
        if(ret < 0){
            qDebug()<< "av_frame_get_buffer failed:"<< ret;
            return; // 这里应该使用goto end 这样的去后面释放之前的context等
        }

        QString encoderName = "libfdk_aac";
        const AVCodec* encoder = avcodec_find_encoder_by_name(encoderName.toUtf8());
        if(encoder == NULL){
            av_log(NULL, AV_LOG_ERROR, "avcodec_find_encoder_by_name %s failed\n", encoderName.toUtf8().data());
            return;
        }

        m_encoderCtx = avcodec_alloc_context3(encoder);
        if(m_encoderCtx == NULL){
            av_log(NULL, AV_LOG_ERROR, "avcodec_alloc_context3 failed\n");
            return;
        }

        // 设置编码参数
        m_encoderCtx->sample_fmt = (AVSampleFormat)m_inputFrame->format;
        m_encoderCtx->sample_rate = m_inputFrame->sample_rate;
        m_encoderCtx->ch_layout = m_inputFrame->ch_layout;
        //encoderCtx->bit_rate = 32000;  // 比特率
        m_encoderCtx->profile = FF_PROFILE_AAC_LOW; // 规格


        // 打开编码器
        ret = avcodec_open2(m_encoderCtx, encoder, NULL);
        if(ret < 0){
            qDebug()<< "avcodec_open2 failed: "<< ret;
            return; // 这里应该使用goto end 这样的去后面释放之前的context等
        }
        m_encodePacket = av_packet_alloc();
        // 打开要写入的二进制文件（覆盖已存在的文件）
        outFile = new std::ofstream(QString("./output.aac").toUtf8(), std::ios::binary);
        if(!outFile->is_open()){
            av_log(NULL, AV_LOG_ERROR, "Can not open or create output file");
            return ;
        }
    }else{
        QString encoderName = "libfdk_aac";
        const AVCodec *decoder = avcodec_find_decoder_by_name(encoderName.toUtf8());
        if(decoder == nullptr){
            qDebug ()<< "avcodec_find_decoder failed: %s, codec_id: AV_CODEC_ID_AAC";
            return ;
        }
        m_decodecCtx = avcodec_alloc_context3(decoder);
        if(m_decodecCtx == nullptr){
            qDebug ()<<"avcodec_alloc_context3 failed:";
            return ;
        }
        m_inputFrame = av_frame_alloc();
        m_inputFrame->sample_rate = 44100;
        m_inputFrame->ch_layout.order = AV_CHANNEL_ORDER_NATIVE;
        m_inputFrame->ch_layout.nb_channels = 2;
        m_inputFrame->ch_layout.u.mask = AV_CH_LAYOUT_STEREO;
        m_inputFrame->format = AV_SAMPLE_FMT_S16;  // libfdk_aac默认的format
        m_inputFrame->nb_samples = 1024;           // LC
        // 设置编码参数
        m_decodecCtx->sample_fmt = (AVSampleFormat)m_inputFrame->format;
        m_decodecCtx->sample_rate = m_inputFrame->sample_rate;
        m_decodecCtx->ch_layout = m_inputFrame->ch_layout;
        //encoderCtx->bit_rate = 32000;  // 比特率
        m_decodecCtx->profile = FF_PROFILE_AAC_LOW; // 规格
        auto ret = avcodec_open2(m_decodecCtx, decoder, nullptr);
        if(ret < 0){
            qDebug ()<<  "avcodec_open2 failed: %s\n"<<ret;
            return ;
        }
        m_decodePacket = av_packet_alloc();

        m_decodeFrame = av_frame_alloc();
        int frameSize = av_samples_get_buffer_size(NULL, m_decodecCtx->ch_layout.nb_channels, 1024, m_decodecCtx->sample_fmt, 1);
        uint8_t *frameBuffer = (uint8_t*)av_malloc(frameSize);
        avcodec_fill_audio_frame(m_decodeFrame, m_decodecCtx->ch_layout.nb_channels, m_decodecCtx->sample_fmt, frameBuffer, frameSize, 1);
        outDeFile = new std::ofstream(QString("./output.pcm").toUtf8(), std::ios::binary);
        if(!outDeFile->is_open()){
            av_log(NULL, AV_LOG_ERROR, "Can not open or create output file");
            return ;
        }
    }
}

QByteArray LhtFFmpegAacEncode::encode(char *data, int len)
{
    if(len != m_inputFrame->linesize[0])
    {
        qDebug()<<"input size error";
        return QByteArray();
    }
    memcpy(m_inputFrame->data[0],data,len);
    return  encodeAudioFrame();
}

QByteArray LhtFFmpegAacEncode::encodeAudioFrame()
{
    int ret = avcodec_send_frame(m_encoderCtx, m_inputFrame);
    if(ret < 0){
        qDebug()<< "avcodec_send_frame failed: "<< ret;
        return QByteArray();
    }


    while (ret >= 0) {
        ret = avcodec_receive_packet(m_encoderCtx, m_encodePacket);
        if (ret == AVERROR_EOF){
            av_log(NULL, AV_LOG_INFO, "EOF in avcodec_receive_packet \n");
        } else if(ret < 0) {
            qDebug()<< "avcodec_receive_packet error: "<< ret;
            av_packet_unref(m_encodePacket);
            break;
        }

        av_log(NULL, AV_LOG_INFO, "avcodec_receive_packet success, write to file \n");
        auto outArr =  QByteArray(reinterpret_cast<const char*>(m_encodePacket->data), m_encodePacket->size);
        outFile->write(reinterpret_cast<const char*>(m_encodePacket->data), m_encodePacket->size);
        av_packet_unref(m_encodePacket);
        return outArr;
    }
    return QByteArray();
}


QByteArray LhtFFmpegAacEncode::decode(char *data, int len)
{
    // 解码数据
    UdpPack pack({data,len},1);
    av_packet_from_data(m_decodePacket,(uint8_t*)pack.data.data(),pack.data.length());
    auto ret = avcodec_send_packet(m_decodecCtx, m_decodePacket);
    if(ret < 0){
        //        av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet failed: %s\n", Utils::avError2string(ret).c_str());
        av_packet_unref(m_decodePacket);
        return QByteArray();
    }
    while(avcodec_receive_frame(m_decodecCtx, m_decodeFrame) == 0){

        int dataSize = av_get_bytes_per_sample(m_decodecCtx->sample_fmt);   // 每个采样点的数据大小
        if(dataSize < 0){
            //            av_log(NULL, AV_LOG_ERROR, "av_get_bytes_per_sample failed: %s\n", Utils::avError2string(ret).c_str());
            av_packet_unref(m_decodePacket);
            return QByteArray();
        }

        auto outArr =  QByteArray(reinterpret_cast<const char*>(m_decodeFrame->data[0]), m_decodeFrame->linesize[0]);
        //        av_packet_unref(m_encodePacket);
        return outArr;
    }
    return QByteArray();
    av_packet_unref(m_decodePacket);
}

