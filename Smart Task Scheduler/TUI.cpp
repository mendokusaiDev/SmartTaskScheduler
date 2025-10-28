#include "TUI.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>
#include <ctime>
#include <limits>

using namespace std;
using scheduler::Task;

namespace scheduler {

    // ---- helpers ----
    static int ymd(int y, int m, int d) { return y * 10000 + m * 100 + d; }

    static void splitTime(long long ymdhm, int& y, int& m, int& d, int& hh, int& mm) {
        if (ymdhm < 0) { y = m = d = hh = mm = -1; return; }
        y = int(ymdhm / 100000000); ymdhm %= 100000000;
        m = int(ymdhm / 1000000);   ymdhm %= 1000000;
        d = int(ymdhm / 10000);     ymdhm %= 10000;
        hh = int(ymdhm / 100);      mm = int(ymdhm % 100);
    }

    static string hhmm(long long ymdhm) {
        int y, m, d, h, mm; splitTime(ymdhm, y, m, d, h, mm);
        if (h < 0) return "시간 미정";
        ostringstream os; os << setw(2) << setfill('0') << h << ":" << setw(2) << mm;
        return os.str();
    }

    static int weekdayOf(int y, int m, int d) { // 1=Mon..7=Sun
        std::tm tm{}; tm.tm_year = y - 1900; tm.tm_mon = m - 1; tm.tm_mday = d;
        std::mktime(&tm); int w = tm.tm_wday; if (w == 0) w = 7; return w;
    }

    static int lastDayOfMonth(int y, int m) {
        static int md[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
        if (m != 2) return md[m];
        if ((y % 400 == 0) || (y % 4 == 0 && y % 100 != 0)) return 29; return 28;
    }

    static void printTaskLine(Task* t) {
        long long s, e; t->getTime(s, e);
        cout << "  [" << t->getTaskNum() << "] "
            << (t->isfinished() ? "(완료) " : "")
            << hhmm(s) << " - " << hhmm(e)
            << "  | type=" << t->getType()
            << "  | due=" << t->getEnddate() << "\n";
    }

    // ---- TUI methods ----

    void TUI::mainMenu() {
        if (!c) c = new Calender();
        while (true) {
            cout << "\n===== SCHEDULER (TUI) =====\n"
                << "1) 월 보기   2) 주 보기   3) 일 보기\n"
                << "4) 작업 추가 5) 작업 수정 6) 작업 삭제\n"
                << "7) 통계      8) 간격변경   0) 종료\n> ";
            int cmd; if (!(cin >> cmd)) return;
            if (cmd == 0) break;
            switch (cmd) {
            case 1: showMonth(); break;
            case 2: showWeek();  break;
            case 3: showDay();   break;
            case 4: addTask();   break;
            case 5: editTask();  break;
            case 6: deleteTask(); break;
            case 7: showStatistics(); break;
            case 8: changeInterval(); break;
            default: cout << "메뉴를 다시 선택하세요.\n";
            }
        }
    }

    // 월 뷰: YYYY MM 입력 → 월 달력 + 일자별 작업수
    void TUI::showMonth() {
        int y, m; cout << "연도 월 입력 (예: 2025 10): ";
        cin >> y >> m;
        vector<Task*> tasks;
        int dummyDay = 1;
        if (!c->get_Month(tasks, y, m, dummyDay)) {
            cout << "해당 월 데이터가 없습니다.\n"; return;
        }
        // 일자별 개수 집계
        map<int, int> cnt;
        for (Task* t : tasks) {
            long long s, e; t->getTime(s, e);
            int yy, MM, dd, hh, mm; splitTime(s, yy, MM, dd, hh, mm);
            int date = (s >= 0) ? (yy * 10000 + MM * 100 + dd) : t->getEnddate();
            int d = date % 100; cnt[d]++;
        }

        int firstW = weekdayOf(y, m, 1); // 1..7
        int lastD = lastDayOfMonth(y, m);

        cout << "\n=== " << y << "." << m << " ===\n";
        cout << " Mo Tu We Th Fr Sa Su\n";
        int col = 1;
        for (int i = 1; i < firstW; i++) { cout << "   "; col++; }
        for (int d = 1; d <= lastD; ++d) {
            cout << setw(2) << d;
            if (cnt[d]) cout << "*"; else cout << " ";
            if (col % 7 == 0) cout << "\n"; else cout << " ";
            col++;
        }
        cout << "\n* 표시는 해당 일에 작업이 존재함을 의미\n";
    }

    // 주 뷰: 기준 YYYY MM DD 입력 → 그 주(월~일) 각 일자와 작업 목록
    void TUI::showWeek() {
        int y, m, d; cout << "연도 월 일 입력 (예: 2025 10 28): ";
        cin >> y >> m >> d;
        vector<Task*> tasks;
        c->get_Week(tasks, y, m, d);
        // 날짜별 그룹핑
        map<int, vector<Task*> > byDate;
        for (Task* t : tasks) {
            long long s, e; t->getTime(s, e);
            int yy, MM, dd, hh, mm;
            int key;
            if (s >= 0) { splitTime(s, yy, MM, dd, hh, mm); key = ymd(yy, MM, dd); }
            else { key = t->getEnddate(); }
            byDate[key].push_back(t);
        }
        for (map<int, vector<Task*> >::iterator it = byDate.begin(); it != byDate.end(); ++it) {
            int dateKey = it->first;
            vector<Task*>& vec = it->second;
            cout << "\n[" << dateKey << "]\n";
            for (Task* t : vec) printTaskLine(t);
        }
        if (byDate.empty()) cout << "이번 주 일정 없음\n";
    }

    // 일 뷰: YYYY MM DD 입력 → 해당 일 작업 출력
    void TUI::showDay() {
        int y, m, d; cout << "연도 월 일 입력 (예: 2025 10 28): ";
        cin >> y >> m >> d;
        vector<Task*> tasks;
        if (!c->get_Day(tasks, y, m, d)) {
            cout << "해당 일 일정 없음\n"; return;
        }
        cout << "\n[" << y << "-" << m << "-" << d << "] 일정\n";
        for (Task* t : tasks) printTaskLine(t);
        if (tasks.empty()) cout << "일정 없음\n";
    }

    // 작업 추가
    void TUI::addTask() {
        string name; int dur, due, type;
        cout << "이름: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name);
        cout << "예상소요(분): "; cin >> dur;
        cout << "마감일(YYYYMMDD): "; cin >> due;
        cout << "타입(정수): "; cin >> type;
        if (c->addTask(name, dur, due, type))
            cout << "추가 완료\n";
        else cout << "추가 실패\n";
    }

    // 작업 수정
    void TUI::editTask() {
        int num; cout << "수정할 TaskNum: "; cin >> num;
        string name; int dur, due, type;
        cout << "새 이름: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name);
        cout << "새 소요(분): "; cin >> dur;
        cout << "새 마감(YYYYMMDD): "; cin >> due;
        cout << "새 타입: "; cin >> type;
        if (c->editTask(num, name, dur, due, type))
            cout << "수정 완료\n";
        else cout << "수정 실패\n";
    }

    // 작업 삭제
    void TUI::deleteTask() {
        int num; cout << "삭제할 TaskNum: "; cin >> num;
        if (c->deleteTask(num))
            cout << "삭제 완료\n";
        else cout << "삭제 실패\n";
    }

    // 통계 보기
    void TUI::showStatistics() {
        auto st = c->getStatistics();
        cout << "완료:" << st.finished_count << " / 미완료:" << st.unfinished_count
            << " / 남은 작업:" << st.to_do_count << "\n";
    }

    // 스케줄 간격 변경
    void TUI::changeInterval() {
        int k; cout << "새 interval(분): "; cin >> k;
        vector<Task*> q, f;
        if (c->changeInterval(k, q, f))
            cout << "간격 변경 및 재스케줄 완료. queued=" << q.size() << ", failed=" << f.size() << "\n";
        else cout << "변경 실패\n";
    }

} // namespace scheduler

/*
사용 예)
int main(){
    scheduler::TUI app;
    app.mainMenu();
}
*/
