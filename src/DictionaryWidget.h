#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <mutex>

class QLabel;
class QLineEdit;
class QCheckBox;

class DictionaryWidget : public QWidget
{
    Q_OBJECT

public:
    DictionaryWidget();

public slots:
    void checking_function();
    void updating_function(bool new_seq);
    void print_results();

private:
    // константа, которая указывает как много слов можно выводить в окне:
    const int max_nmbr_mtch = 10;

    QLineEdit* pattern_edit;
    QLabel* nmbr_mtch_label;
    QLabel* output_label;
    QLabel* and_more;
    QCheckBox* check_box;

    bool in_progress = false;
    bool is_seq = false;

    std::string previous_pattern;
    std::string current_pattern;

    std::vector<std::string> * previous_ptr;
    std::vector<std::string> * current_ptr;

    std::mutex m;
};
