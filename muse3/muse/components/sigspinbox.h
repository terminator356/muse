#ifndef __SIGSPINBOX_H__
#define __SIGSPINBOX_H__

#include <QSpinBox>

class SigSpinBox : public QSpinBox
{
    Q_OBJECT
    bool _denominator;
protected:
    virtual void keyPressEvent(QKeyEvent*);
    virtual void stepBy(int);
public:
    explicit SigSpinBox(QWidget *parent = 0);
    void setDenominator();
    QSize sizeHint() const;

signals:
    void returnPressed();
    void escapePressed();
    void moveFocus();

public slots:

};

#endif // __SIGSPINBOX_H__
