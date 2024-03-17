#include "send_text_edit.h"
#include <QDebug>
#include <QTextBlock>
SendTextEdit::SendTextEdit(QWidget *parent):QTextEdit(parent)
{
    connect(this, &QTextEdit::textChanged, this, &SendTextEdit::handleTextChanged);

}

void SendTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        signSendMsg(getMsg());
    }else {
        if (event->matches(QKeySequence::Paste))
        {
            const QClipboard *clipboard = QApplication::clipboard();
            QByteArray textData = clipboard->mimeData()->data("text/plain");
            QString text = QString::fromUtf8(textData);
            if (text.contains("file://"))
            {
                // 将图像插入到文本编辑框
                QTextImageFormat fragment;
                QImage temp(text.mid(8));
                if(temp.width() > 100){
                    temp = temp.scaledToWidth(100,Qt::SmoothTransformation);
                }
                if(temp.height()>100){
                    temp = temp.scaledToHeight(100,Qt::SmoothTransformation);
                }
                fragment.setName(text);
                fragment.setWidth(temp.width()); // 设置图像宽度
                fragment.setHeight(temp.height()); // 设置图像高度
                textCursor().insertImage(fragment);
                return;
            }
        }
        // 正常字符输入
        QTextEdit::keyPressEvent(event);
    }
}
void SendTextEdit::addAnimation(const QString& url, const QString& fileName)
{
    //插入Html描述的图片
    this->insertHtml("<img src='" + url + "'/>");

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
    movie->setCacheMode(QMovie::CacheAll);

    m_hasUrls.insert(movie, fileName);

    //绑定帧切换信号槽
    connect(movie, SIGNAL(frameChanged(int)), this, SLOT(subAnimate(int)));
    movie->start();

}
void SendTextEdit::subAnimate(int a)
{
    //使用QMovie中的当前帧替换掉富文本中的图片元素
    QMovie* movie = qobject_cast<QMovie*>(sender());
    auto url = "qrc"+m_hasUrls.value(movie);
    auto pix = movie->currentPixmap();
    this->document()->addResource(QTextDocument::ImageResource,url,pix);
    // 通过设置HTML内容来刷新文档，以便图片更新生效
//    this->setHtml(this->toHtml());
    this->setLineWrapColumnOrWidth(this->lineWrapColumnOrWidth());

}

void SendTextEdit::insertEmoji(int index)
{
    // 将图像插入到文本编辑框
    //    QTextImageFormat fragment;
    //    fragment.setName(EmojiPath[index]);
    //    textCursor().insertImage(fragment);

    addAnimation(EmojiPath[index],EmojiPath[index]);
}

QString SendTextEdit::getMsg()
{
    auto sss = this->toPlainText();
    QString targetStr;
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    // 最后插入一个空格字符防止有图片没有读取到
    cursor.insertText(" ");
    cursor.movePosition(QTextCursor::Start);
    // 获取当前位置的字符
    auto currentblock = cursor.block();
    auto str = currentblock.text();
    int currentChangeLen = 0;
    targetStr = str;
    while (!cursor.atEnd()) {
        //cursor.select(QTextCursor::BlockUnderCursor);

        // 获取选中文本的格式
        QTextCharFormat charFormat = cursor.charFormat();

        // 检查是否有图片格式
        if (charFormat.isImageFormat()) {
            QTextImageFormat imageFormat = charFormat.toImageFormat();

            // 获取图片路径或标识
            QString imagePath = imageFormat.name();
            if(this->toolTip() == "hasImag")
            {
                imagePath = "file:///" + imagePath;
                this->setToolTip("");
            }

            // 如果图片路径以 "file://" 开头，表示它是本地文件路径
            if (imagePath.startsWith("file://")) {
                // 去除 "file://" 前缀，得到实际文件路径
                imagePath = imagePath.mid(8); // 去掉前7个字符
                // 在 imagePath 中就是图片的实际存储位置
                QString imgPath = ImageHeader+imagePath+ImageEnd;
                auto pos = cursor.position();
                if(pos == 0){
                    targetStr.remove(0,1);
                    targetStr.insert(0,imgPath);
                    currentChangeLen += imgPath.length();
                    cursor.movePosition(QTextCursor::NextCharacter);
                }else{
                    targetStr.remove(pos - 1 + currentChangeLen,1);
                    targetStr.insert(pos - 1 + currentChangeLen,imgPath);
                    currentChangeLen += imgPath.length() -1;
                }
                //cursor.movePosition(QTextCursor::NextCharacter);
            }
            //这是表情路径
            if (imagePath.startsWith(":/resource") || imagePath.startsWith("qrc:/resource")) {
                auto emojiname = imagePath.split("/").last();
                emojiname = emojiname.split(".").first();
                QString emjPath = EmjHeader+emojiname+EmjEnd;
                auto pos = cursor.position();
                if(pos == 0){
                    targetStr.remove(0,1);
                    targetStr.insert(0,emjPath);
                    currentChangeLen += emjPath.length();
                    cursor.movePosition(QTextCursor::NextCharacter);
                }else{
                    targetStr.remove(pos - 1 + currentChangeLen,1);
                    targetStr.insert(pos - 1 + currentChangeLen,emjPath);
                    currentChangeLen += emjPath.length() -1;
                }
                //cursor.movePosition(QTextCursor::NextCharacter);
            }
        }

        cursor.movePosition(QTextCursor::NextCharacter);
    }
    // 移动光标到最后一个字符的位置
    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);

    // 删除最后一个字符
    cursor.removeSelectedText();
    return targetStr;
}

void SendTextEdit::handleTextChanged() {

}

void SendTextEdit::mouseDoubleClickEvent(QMouseEvent *event)
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

                // 例如，可以打开图片或执行其他操作
                // ...

                // 不要忘记调用基类的方法以确保文本编辑功能正常工作
                QTextEdit::mousePressEvent(event);
                return;
            }
        }
    }

    // 如果未点击图片，则默认处理点击事件
    QTextEdit::mousePressEvent(event);
}

QString SendTextEdit::replaceEmojisImgs()
{
    return "";
}
