//#include "AddOrderDialog.h"

//#include <QComboBox>
//#include <QLabel>
//#include <QVBoxLayout>

//class DropDownBox
//{
//    private:
//        QComboBox* _comboBox;
//        QLabel* _name;
//        QVBoxLayout* _layout;

//    public:
//        DropDownBox(QWidget* parent = 0);
//        DropDownBox(const QString&, const QStringList&, QWidget* parent=0);
//        ~DropDownBox();

//    public:
//        void addLabel(const QString& label)
//        {
//            _name->text(label);
//        }

//        void addComboItems(const QStringList& list)
//        {
//            _comboBox->addItems(list);
//        }

//        void addComboItem(const QString& item)
//        {
//            _comboBox->addItem(item);
//        }
//};

//DropDownBox::DropDownBox(QWidget *parent)
//{
//    _layout = new QVBoxLayout(parent);
//    _comboBox = new QComboBox(_layout);
//    _name = new QLabel(_layout);
//    _layout->addWidget(_name);
//    _layout->addWidget(_comboBox);
//}

//DropDownBox::DropDownBox(const QString& label, const QStringList& comboItems, QWidget* parent)
//{
//    _layout = new QVBoxLayout(parent);
//    _comboBox = new QComboBox(_layout);
//    _name = new QLabel(label,_layout);
//    _comboBox->addItems(comboItems);
//    _layout->addWidget(_name);
//    _layout->addWidget(_comboBox);
//}

//DropDownBox::~DropDownBox()
//{
//    delete _layout;
//    delete _name;
//    delete _comboBox;
//}


//OrderEntryDialog::OrderEntryDialog()
//{
//}
