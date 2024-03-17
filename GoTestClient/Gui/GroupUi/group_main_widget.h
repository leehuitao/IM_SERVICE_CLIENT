#ifndef GROUP_MAIN_WIDGET_H
#define GROUP_MAIN_WIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "Network/packet_define.h"

namespace Ui {
class GroupMainWidget;
}

class GroupMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupMainWidget(QWidget *parent = nullptr);
    ~GroupMainWidget();

    void init();

    void updateIcon(int userId);


private:
    Ui::GroupMainWidget *ui;

    QList<int>                  m_allGroupList;

    QMap<int,int>               m_allUserStatus;
};

#endif // GROUP_MAIN_WIDGET_H
