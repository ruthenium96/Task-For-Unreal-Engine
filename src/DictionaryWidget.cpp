#include "DictionaryWidget.h"
#include "dictionary.h"

#include <QLayout>
#include <QTimer>
#include <thread>

DictionaryWidget::DictionaryWidget() {
    // инициализируем и выделяем максимально необходимую память:
    previous_ptr = new std::vector<std::string>;
    current_ptr = new std::vector<std::string>;
    previous_ptr->reserve(466550);
    current_ptr->reserve(466550);

    auto* ver_layout = new QVBoxLayout;
    {
        QHBoxLayout* hor_layout;

        // поле ввода и check_box для значения Sequential:
        hor_layout = new QHBoxLayout;
        {
            hor_layout->addWidget(new QLabel("Enter the pattern: "));
            hor_layout->addWidget(pattern_edit = new QLineEdit);
            hor_layout->addStretch(2);
            hor_layout->addWidget(check_box = new QCheckBox("Sequential"));
        }
        ver_layout->addLayout(hor_layout);

        // количество совпадений:
        hor_layout = new QHBoxLayout;
        {
            hor_layout->addWidget(nmbr_mtch_label = new QLabel);
            nmbr_mtch_label->setText(QString("0"));
            hor_layout->addWidget(new QLabel("matches:"));
            hor_layout->addStretch(2);
        }
        ver_layout->addLayout(hor_layout);

        // вывод слов:
        hor_layout = new QHBoxLayout;
        {
            hor_layout->addWidget(output_label = new QLabel);
            hor_layout->addStretch(2);
        }
        ver_layout->addLayout(hor_layout);

        // строка, говорящая о большем числе найденных слов:
        hor_layout = new QHBoxLayout;
        {
            hor_layout->addWidget(and_more = new QLabel);
            hor_layout->addStretch(2);
        }
        ver_layout->addLayout(hor_layout);

        // for ui flexibility
        ver_layout->addStretch(2);
    }
    setLayout(ver_layout);

    // run |checking_function| once per 0.25 second
    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DictionaryWidget::checking_function);
    timer->start(250);

    setWindowTitle(tr("Dictionary"));
    resize(350, 300);
}

// основная проверяющая функция:
void DictionaryWidget::checking_function() {
    // если слова ещё ищутся, то пропускаем этот вызов:
    if (in_progress) {
        return;
    }

    // считываем значения из полей GUI:
    bool new_seq = check_box->isChecked();
    previous_pattern = current_pattern;
    current_pattern = pattern_edit->text().toUtf8().constData();

    // если ничего не изменилось или имеем пустое поле -- ничего не делаем
    if ((previous_pattern == current_pattern && new_seq == is_seq) || current_pattern.empty()) {
        return;
    }
    // если дошли сюда, что-то нужно изменить:
    updating_function(new_seq);
}

// функция, вызывающая поток-вычислитель и вызывающая функцию, пишущую в поля GUI
void DictionaryWidget::updating_function(bool new_seq) {
    // уточняем предыдущие результаты, если a) старый паттерн не пуст, b) не сменился is_seq
    // c) новый паттерн лежит в старом (в соответвующем is_seq смысле),
    // иначе ищем в файле-словаре:
    bool from_previous = !previous_pattern.empty() && new_seq == is_seq
                         && is_string(current_pattern, previous_pattern, new_seq);

    std::thread t([&]() {
        m.lock();
        in_progress = true;
        auto tmp_ptr = previous_ptr;
        previous_ptr = current_ptr;
        current_ptr = tmp_ptr;
        is_seq = new_seq;
        m.unlock();

        find_in(*previous_ptr, *current_ptr, current_pattern, is_seq, from_previous);

        m.lock();
        in_progress = false;
        print_results();
        m.unlock();
    });
    t.detach();
}

// функция, которая пишет в поля GUI
void DictionaryWidget::print_results() {
    int nmbr_mtch = current_ptr->size();
    nmbr_mtch_label->setText(QString::number(nmbr_mtch));

    std::string output;
    for (int i = 0; i < std::min(max_nmbr_mtch, nmbr_mtch); ++i) {
        output += current_ptr->at(i) + "\n";
    }
    output_label->setText(QString(output.c_str()));

    if (nmbr_mtch > max_nmbr_mtch) {
        and_more->setText(QString("and more..."));
    } else {
        and_more->setText(QString(""));
    }
}