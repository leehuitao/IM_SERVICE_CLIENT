#include "message_item.h"

#include "Logic/app_cache.h"

MessageItem::MessageItem(QWidget *parent): QTextEdit(parent)
{
    // 取消水平滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 取消垂直滚动条
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 设置背景为透明，以便自定义绘制
    setAttribute(Qt::WA_TranslucentBackground);
//    setAttribute(Qt::WA_OpaquePaintEvent, false);
    // 获取 MessageItem 中的 QTextEdit
}

void MessageItem::initMsg(const MsgBody &msg)
{
    m_msgBody = msg;
    //解析文字
    parseMsg(msg.Msg);
}

void MessageItem::initGroupMsg(const GroupBody &msg)
{
    m_groupMsgBody = msg;
    //解析文字
    parseMsg(msg.Msg);
}

void MessageItem::initMsg(const QString &imgText)
{
    updateBackground = 0;
    // 将图像插入到文本编辑框
    QTextImageFormat fragment;
    auto imgPath = imgText.contains("/") ? imgText: AppCache::Instance()->m_fileSavePath + imgText;

    QImage temp(imgPath);
    if(temp.width() > 200){
        temp = temp.scaledToWidth(200,Qt::SmoothTransformation);
    }
    if(temp.height()>200){
        temp = temp.scaledToHeight(200,Qt::SmoothTransformation);
    }
    fragment.setName(imgPath);
    fragment.setWidth(temp.width()); // 设置图像宽度
    fragment.setHeight(temp.height()); // 设置图像高度
    textCursor().insertImage(fragment);
}

//void MessageItem::paintEvent(QPaintEvent *event)
//{
//    //    if(updateBackground){
//    //        QPainter painter(viewport());
//    //        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//消锯齿
//    //        // 绘制背景气泡
//    //        QPainterPath path;
//    //        QRectF rect = viewport()->rect();
//    //        //框
//    //        QColor col_Kuang(75,164,242);
//    //        painter.setBrush(QBrush(col_Kuang));
//    //        painter.drawRoundedRect(rect,4,4);
//    //    }
//    QTextEdit::paintEvent(event);

//}

void MessageItem::resizeEvent(QResizeEvent *event)
{
    //    initMsg(m_msgBody);
    update();
}


void MessageItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    // 获取光标的位置
    QTextCursor cursor = cursorForPosition(event->pos());

    // 获取光标所在的文本块
    QTextBlock block = cursor.block();

    // 获取光标在文本块中的位置
    int positionInBlock = cursor.positionInBlock();

    // 检查文本块是否包含图片
    if (block.isValid() && block.layout() && positionInBlock < block.length()) {
        // 获取选中文本的格式
        QTextCharFormat format = cursor.charFormat();
        if (format.isImageFormat()) {
            QTextImageFormat imageFormat = format.toImageFormat();

            // 检查是否是图片
            if (!imageFormat.name().isEmpty()) {
                // 在这里处理图片点击事件
                QString imageUrl = imageFormat.name();
                qDebug() << "Image clicked: " << imageUrl;
                signImageClicked(imageUrl);
                // 例如，可以打开图片或执行其他操作
                // ...
                QTextEdit::mousePressEvent(event);
                return;
            }
        }
    }

    // 如果未点击图片，则默认处理点击事件
    QTextEdit::mousePressEvent(event);
}

void MessageItem::parseMsg(const QString &source)
{
    QFontMetricsF fm(this->font());
    m_lineHeight = fm.lineSpacing();
    //    this->clear();
    auto len = source.size();
    for (int i = 0; i < len; i++) {
        auto temp = source.mid(i);
        if (temp.startsWith(EmjHeader)) {
            // 处理表情标记
            int emjEndPos = temp.indexOf(EmjEnd);
            if (emjEndPos != -1) {
                QString emjText = temp.mid(10 , emjEndPos - 10); // 获取emj名
                //                qDebug()<<"emjText = "<<emjText;
                // 在这里处理表情文本 emjText
                i += emjText.size() + 10 + 7; // Move i to the position after the "%$emjend"
                // 将图像插入到文本编辑框
                addAnimation(EmojiPath[emjText.toInt() - 1],EmojiPath[emjText.toInt() - 1]);
            } else {
                // 没有找到匹配的"%$emjend"标记
                // 处理错误情况
            }
        } else if (temp.startsWith(ImageHeader)) {
            // 处理图片标记
            int imgEndPos = temp.indexOf(ImageEnd);
            if (imgEndPos != -1) {
                QString imgText = temp.mid(10, imgEndPos - 10); //获取图片名称
                // 在这里处理图片文本 imgText
                i += imgText.size()+ 10 + 7; // 移动到 "%$imgend" 后面
                // 将图像插入到文本编辑框
                QTextImageFormat fragment;
                auto imgPath = imgText.contains("/") ? imgText: AppCache::Instance()->m_fileSavePath + imgText;

                QImage temp(imgPath);
                if(temp.width() > 100){
                    temp = temp.scaledToWidth(100,Qt::SmoothTransformation);
                }
                if(temp.height()>100){
                    temp = temp.scaledToHeight(100,Qt::SmoothTransformation);
                }
                fragment.setName(imgPath);
                fragment.setWidth(temp.width()); // 设置图像宽度
                fragment.setHeight(temp.height()); // 设置图像高度
                textCursor().insertImage(fragment);
            } else {
                // 没有找到匹配的"%$imgend"标记
                // 处理错误情况
            }
        } else {
            // 普通文本字符
            this->insertPlainText(source.at(i));
        }
    }

}
void MessageItem::addAnimation(const QString& url, const QString& fileName)
{
    //插入Html描述的图片
    this->insertHtml("<img src='" + fileName + "'/>");

    //判断是否是相同的图片
    if (m_lstUrls.contains(url))
    {
        return;
    }
    else
    {
        m_lstUrls.append(url);
    }

    //创建QMovie以显示Gif
    QMovie* movie = new QMovie(this);
    movie->setFileName(fileName);
    movie->setCacheMode(QMovie::CacheNone);

    m_hasUrls.insert(movie, url);

    //绑定帧切换信号槽
    connect(movie, SIGNAL(frameChanged(int)), this, SLOT(subAnimate(int)));
    movie->start();
}

void MessageItem::focusOutEvent(QFocusEvent *e)
{
    if (e->lostFocus()) {
        // 将光标移动到文本末尾
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.clearSelection(); // 清除选中文本
        this->setTextCursor(cursor);

        // 设置文本编辑器为只读状态
        this->setReadOnly(true);

        // 更新编辑器外观
        this->viewport()->update();
    }
//    moveCursor(QTextCursor::End);
//    unsetCursor();
//    this->setReadOnly(true); // 设置为只读以隐藏光标
    update();
    QWidget::focusOutEvent(e);
}
void MessageItem::subAnimate(int a)
{
    //使用QMovie中的当前帧替换掉富文本中的图片元素
    QMovie* movie = qobject_cast<QMovie*>(sender());
    this->document()->addResource(QTextDocument::ImageResource,
                                  "qrc"+ m_hasUrls.value(movie), movie->currentPixmap());
    this->setLineWrapColumnOrWidth(this->lineWrapColumnOrWidth());
}
