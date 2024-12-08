#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QFileInfo>

class ImageDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ImageDelegate(const QString &baseDir, QObject *parent = nullptr)
        : QStyledItemDelegate(parent), m_baseDir(baseDir) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QString imagePath = index.model()->data(index, Qt::DisplayRole).toString();

        QFileInfo fileInfo(imagePath);
        if (fileInfo.isRelative()) {
            imagePath = m_baseDir + "/" + imagePath;
        }

        QPixmap pixmap(imagePath);

        if (!pixmap.isNull()) {
            painter->drawPixmap(option.rect, pixmap.scaled(option.rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        return QSize(100, 200);
    }

private:
    QString m_baseDir;
};

#endif // IMAGEDELEGATE_H
