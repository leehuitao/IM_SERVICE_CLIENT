#include "x264_decode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>
#include <QMutexLocker>
extern "C"{
#include <libavcodec/avcodec.h>
}



X264Decode::X264Decode(QObject *parent) : QObject(parent)
{

}

void X264Decode::initDecoder()
{
    enum AVCodecID audio_codec_id = AV_CODEC_ID_H264;
    const AVCodec *codec = avcodec_find_decoder(audio_codec_id);
//    const AVCodec *codec = avcodec_find_decoder_by_name("libx264");
    if (!codec) {
        qDebug()<<stderr<< "Codec not found\n";
        return;
    }
    m_parser = av_parser_init(codec->id);
    if (!m_parser) {
        qDebug()<<stderr<<  "Parser not found\n";
        return;
    }
    // 分配codec上下文
    m_c = avcodec_alloc_context3(codec);
    if(!m_c) {
        qDebug()<<stderr<<  "Could not allocate audio codec context\n";
        return;
    }

    //将解码器和解码器上下文进行关联
    int ret = avcodec_open2(m_c, codec, NULL);
    if(ret < 0) {
        qDebug()<<stderr<< "Could not open codec\n";
        return;
    }

    m_c->width = 640;
    m_c->height = 480;
    m_frame = av_frame_alloc();
    if(!m_frame) {
        qDebug()<<stderr<<  "Could not allocate audio frame\n";
        return;
    }
    m_mutex = new QMutex;
}

AVFrame * X264Decode::decode(AVPacket *pkt)
{
    QMutexLocker lock(m_mutex);
    AVPacket * ptk = av_packet_alloc();
    ptk->data = new uint8_t[pkt->size];
    memcpy(ptk->data,pkt->data,pkt->size);
    ptk->size = pkt->size;
    int ret = avcodec_send_packet(m_c, ptk);
    qDebug()<<"pkt->size"<<pkt->size;
    if(ret == AVERROR(EAGAIN))
    {
        qDebug()<<stderr<< "Receive_frame and send_packet both returned EAGAIN, which is an API violation.\n";
    }
    else if (ret < 0)
    {
        qDebug()<<stderr<< "Error submitting the packet to the decoder, err:%s, pkt_size:%d\n";
        return nullptr;
    }
    while (ret >= 0) {
        ret = avcodec_receive_frame(m_c, m_frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return nullptr;
        else if (ret < 0)
        {
            qDebug()<<stderr<<"Error during decoding\n";
            return nullptr;
        }
        return m_frame;

    }
}

AVFrame *X264Decode::decode(const QByteArray &encodeData)
{
//    QMutexLocker lock(m_mutex);
    AVPacket * pkt = av_packet_alloc();
    pkt->data = new uint8_t[encodeData.size()];
    memcpy(pkt->data,encodeData.data(),encodeData.size());
    pkt->size = encodeData.size();
    int ret = avcodec_send_packet(m_c, pkt);
//    qDebug()<<"pkt->size"<<encodeData.size();
    if(ret == AVERROR(EAGAIN))
    {
        qDebug()<<stderr<< "Receive_frame and send_packet both returned EAGAIN, which is an API violation.\n";
    }
    else if (ret < 0)
    {
        qDebug()<<stderr<< "Error submitting the packet to the decoder, err:%s, pkt_size:%d\n";
        return nullptr;
    }
    while (ret >= 0) {
        ret = avcodec_receive_frame(m_c, m_frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return nullptr;
        else if (ret < 0)
        {
            qDebug()<<stderr<<"Error during decoding\n";
            return nullptr;
        }
        return m_frame;

    }
}
